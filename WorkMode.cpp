//---------------------------------------------------------------------------

#pragma hdrstop

#include "IniFiles.hpp"

#include "WorkMode.h"
#include "Main.h"
#include "Compute.h"
#include "PrepareSG.h"
#include "DebugMess.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

	int centralizerSleep = 500;		// ����� �������� ����� ������������� ������
// �������
	int SleepTemp=750;

//-----------------------------------------------------------------------------
__fastcall WorkThreadClass::WorkThreadClass(bool CreateSuspended, 		// ��������� ����� ������������
					LCard791 &lpp,							// ��������� �� ����� ���������� ��������
			   //		LCard791 &lpr,							// ��������� �� ����� ���������� ��������
					Result &_respop,						// ��������� �� ��������� ����������
					Result &_resprod,						// ��������� �� ��������� ����������
					TStatusBar *St,							// ��������� �� ���������, ������������ ������� ��������
					bool _Thick,							// ���������� �� �������������
					bool _Linear,                           // ���������� �� ����������)
					AlarmThreadClass &alth, 				// ����� ����� ������
					bool IsPrepare)							// ������ ���� (����� �� �������� � ������ �������)
	: TThread(CreateSuspended)
{
	St1 = St;
	lpop = &lpp;
  //	lprod = &lpr;
	respop = &_respop;
	resprod =  &_resprod,
	Thick = _Thick;
	Linear = _Linear;
	lat = &alth;
	Prepare = IsPrepare;

	// ������� ����� ������� ��� ������� ��������
	TIniFile * ini = new TIniFile(Globals::IniFileName);
	cross_len = ini->ReadInteger("Size","CrossModuleLength",1000);
	linear_len = ini->ReadInteger("Size","LinearModuleLength",1000);
	zone_len = ini->ReadInteger("Size","ZoneLength",250);
	st_len = ini->ReadInteger("Size","St1St4",250);
	delete ini;
}

//-----------------------------------------------------------------------------

void __fastcall WorkThreadClass::Execute()
{
	NameThreadForDebugging("WorkThread");
	transit = false;

	stext1 = "����� \"������\"";
	stext2 = "������� � ��������� ���������";
	ProtocolForm->SendToProtocol("-----------");
	ProtocolForm->SendToProtocol(stext1);
	Synchronize( &UpdateMainForm );

	if (Prepare)
	{
		UnicodeString prepare_result = PrepareForWork( );
		if ( prepare_result != "ok" )
		{
			stext1 = "����� \"������\" �� ��������!";
			stext2 = prepare_result;
			ProtocolForm->SendToProtocol("������: " + stext2);
			Synchronize( &UpdateMainForm );
			digital->ResetOutSignal("������ ����");
			digital->ResetOutSignal("����� ����");
			cool = false;
			return;
		}
	}

	ProtocolForm->SendToProtocol("������: ���������� ������ �������" );
	SetReturnValue( WorkMode() );
	cool = ReturnValue;
	Terminate();
	return;
}
//-----------------------------------------------------------------------------

void __fastcall WorkThreadClass::UpdateMainForm()
{
	St1->Panels->Items[1]->Text = stext1;
	St1->Panels->Items[2]->Text = stext2;
	St1->Refresh();

	Application->ProcessMessages();
	SignalsStateForm->Refresh();
}

//-----------------------------------------------------------------------------
//-------------------------------------------------------------------------------
UnicodeString WorkThreadClass::PrepareForWork( )
{
// ���������, �� �������� �� ����� � �������
	if ( digital->CheckInSignal("���� ����") ||
			digital->CheckInSignal("���� �����") ||
			digital->CheckInSignal("���� ����")  ||
			digital->CheckInSignal("���� �����") ||
			digital->CheckInSignal("����� ����") ||
			digital->CheckInSignal("����� �����") )
		return "����� � �������!";

// ���������, �� ��������� �� ���������
	if ( lpop->isOkTemp("1") || lpop->isOkTemp("2") )
		return "��������� ���������!";

// ���������, ������� �� ������ ������ ���������
	if (digital->SignalExists("�� �������") && !digital->CheckInSignal("�� �������"))
		return "�������� ������ ���������!";

// ���������� ����� ������ �� ���������� �����
	digital->SetOutSignal("������ ����");
	if ( !digital->WaitForInSignal("������ ������", 5000 , TThread::CurrentThread) )
		return "������ ����������� �����!";

// ���������� ����� ������ �� ���������� �������
	digital->SetOutSignal("����� ����");
	if ( !digital->WaitForInSignal("����� ������", 3000 , TThread::CurrentThread) )
		return "������ ����������� �������!";
	if (Thick && Linear)
	{
			String result = ModulePosition::SetAllModulesInWork("�����", "����", "����" );
			if ( result != "ok" )
				return result;
	}
	else
	{
// ������������� ���������� � ������ ��������� (������ � ������)
		if ( !digital->CheckInSignal("����� ��") )
		{
			String result = ModulePosition::SetModuleInWork( "�����" );
			if ( result != "ok" )
				return result;
		}

	// ������������� ������� � ������ ���������
		if ( Thick )
		{
			String result = ModulePosition::SetModuleInWork( "����");
			if ( result != "ok" )
				return result;
		}
		else
		{
			String result = ModulePosition::SetModuleInService( "����");
			if ( result != "ok" )
				return result;
		}

	// ������������� ���������� � ������ ���������
		if ( Linear )
		{
			String result = ModulePosition::SetModuleInWork( "����");
			if ( result != "ok" )
				return result;
		}
		else
		{
			String result = 	ModulePosition::SetModuleInService( "����");
			if ( result != "ok" )
				return result;
		}
	}

// ��������� ������ � ������ �������
/*
	if ( Thick && !digital->CheckInSignal("���� ����� ���") )
	{
		digital->BlinkOutSignal("���� ����� �������");
		if ( !digital->WaitForInSignal("���� ����� ��������",SleepTemp , TThread::CurrentThread) )
			return "������ ������ �����������!";
	}

	if ( Linear && !digital->CheckInSignal("���� ����� ���") )
	{
		digital->BlinkOutSignal("���� ����� �������");
		if ( !digital->WaitForInSignal("���� ����� ��������",SleepTemp , TThread::CurrentThread) )
			return "������ ������ ����������� ������!";
	}
*/
	return "ok";
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
#if 0
bool WorkThreadClass::WorkMode()
{
	stext2 = "�������� ������";
	Synchronize( &UpdateMainForm );
	double lin_v;
	double cr_v;
	double st_v;

// ��������� �������� �������� �� ini
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	String Sect = "Type_" + MainForm->cbTypeSize->Text;
	delete ini;
// ��������� ��������� ����, ������� ���� 1 ���
	bool result = true;
	while ( result )
	{
// ���� ����� ����� �������� (������ ��1)
		stext2 = "���� ����� ����� �������� ����������";
		ProtocolForm->SendToProtocol("������: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForInSignal("��� ��1", 0, TThread::CurrentThread) )
		{
			stext2 = "�� ��������� ����� ����� ��������! (������� ��1)";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
		digital->ResetOutSignal("��� ���������");
		digital->ResetOutSignal("��� �������");

// �������� ��������� ����, ��������� �������
		digital->SetOutSignal("�������� ������");
		digital->SetOutSignal("�������� �������");
		TThread::CurrentThread->Sleep(500);
		if ( lpop->isOkTemp("MG") )
		{
			stext2 = "��� ���������� ����!";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
// ���������, ������� �������� ������������
	ProtocolForm->SendToProtocol("������: ���������� " + IntToStr(Globals::current_diameter));
	respop->sensors = (Globals::current_diameter == 60) ? (10) : (12);
	resprod->sensors = 4;
		//LCard �����!!!
		lpop->InitADC(16,0,"PP",0);
		lpop->Start();
		TThread::CurrentThread->Sleep(200);
		lpop->Stop();


// ��������������� ���� ������ � LCard
		ProtocolForm->SendToProtocol("������: �������������� ���� ������ � LCard");
		int init=0;
		if(SystemConst::SolidGroupSource == 0)
			init=2;		  //���� �� ����� ����� �� �������������� ����� � ��

		lpop->InitADC( 16,0, "PP", init);
		/* todo zzzzzz
		if ( Linear )
			lprod->InitADC(resprod->sensors,0, "PR", 0);
        */
// ���� ������� ���������� �� �����������
		digital->SetOutSignal("������");
		if ( Thick )
		{
			stext2="���� ���������� �����������";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForInSignal("����������",30000, TThread::CurrentThread))
			{
				stext2 = "�� ��������� ���������� �����������";
				ProtocolForm->SendToProtocol("������: " + stext2);
				result = false;
				break;
			}
		}
// ������ ����������
		if ( Linear )
		{
			digital->SetOutSignal("���� ��� ��");
			digital->SetOutSignal("���� STF");
			for (UINT s = 0; s < Globals::speeds.inSpeed.size(); s++)
				digital->SetOutSignal("���� �������� " + IntToStr(Globals::speeds.inSpeed[s]));
			if ( !digital->WaitForDropInSignal("���� �� RUN", 7000 , TThread::CurrentThread) )
			{
				stext2 = "�� ���������� �������� �������� ����������� ������!";
				ProtocolForm->SendToProtocol("������: " + stext2);
				result = false;
				break;
			}
		}
			if ( Terminated )
				break;


// ���� �������� �����������
		digital->SetOutSignal("����");
		if ( Thick )
		{
			stext2="���� �������� �����������";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForInSignal("��������",10000, TThread::CurrentThread))
			{
				stext2 = "�� ��������� �������� �����������";
				ProtocolForm->SendToProtocol("������: " + stext2);
				result = false;
				break;
			}
			lat->SetControl();
		}

// �������� �������� ����� ������
		digital->SetOutSignal("��: ��� ��");
		for (UINT s = 0; s < Globals::speeds.workSpeed.size(); s++)
			digital->SetOutSignal("��: �������� " + IntToStr(Globals::speeds.workSpeed[s]));
		digital->SetOutSignal("��: STF");
		if ( !digital->WaitForDropInSignal("��: �� RUN",3000, TThread::CurrentThread) )
		{
			stext2 = "�� ���������� �������� �������� ����������";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
		if ( Terminated )
			break;

// ���������� ������� � ���, ����� �������
		digital->SetOutSignal("��� ������");
		digital->SetOutSignal("��� ��������");

//************************����� ����********************************************
// ���� ����� �� ������ ������
		stext2 = "���� ����� �� ������ 1";
		ProtocolForm->SendToProtocol("������: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForInSignal("������ 1", 130000, TThread::CurrentThread ) )
		{
			stext2 = "�� ��������� ����� �� ������ 1!";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
		st1 = GetTickCount();
		if ( Terminated )
			break;
// ���� ����� �� ����� �����������
		stext2 = "���� ����� �� ����� �����������";
		Synchronize( &UpdateMainForm );
		if ( digital->WaitForInSignal("����� ����",30000, TThread::CurrentThread) )
		{
			TThread::CurrentThread->Sleep(900);
			lpop->Start();
			cross_t1 = GetTickCount();
		}
		else
		{
			stext2 = "�� ��������� ����� �� ����� �����������!";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
		if ( Terminated )
			break;
/*
	// ���� ����� �� ������ �����������
		stext2 = "���� ����� �� ������ �����������";
		Synchronize( &UpdateMainForm );
		if ( digital->WaitForInSignal("����� �����",4000, TThread::CurrentThread) )
			cross_t2 = GetTickCount();
		else
		{
			stext2 = "�� ��������� ����� �� ������ �����������!";
			result = false;
			break;
		}
*/
/*
// ���� ����� �� ����� �����������, ���� �����
		if ( Thick )
		{
			stext2 = "���� ����� �� ����� �����������";
			Synchronize( &UpdateMainForm );
			if ( digital->WaitForInSignal("���� ����",5000, TThread::CurrentThread) )
			{
//				TThread::CurrentThread->Sleep( centralizerSleep );
//				digital->BlinkOutSignal("���� ����� on");
			}
			else
			{
				stext2 = "�� ��������� ����� �� ����� �����������!";
				result = false;
				break;
			}
		}
		if ( Terminated )
			break;
*/
// ���� ����� �� ����� �����������, ���� �����
		if ( Linear )
		{
			stext2 = "���� ����� �� ����� �����������";
			Synchronize( &UpdateMainForm );
			digital->SetOutSignal("���� ��� ��");
			if ( digital->WaitForInSignal("���� ����",15000, TThread::CurrentThread) )
			{
				TThread::CurrentThread->Sleep(900);
  //toto zzzzz				lprod->Start();
				lin_t1 = GetTickCount();
//				TThread::CurrentThread->Sleep( centralizerSleep );
//				digital->BlinkOutSignal("���� ����� on");
			}
			else
			{
				stext2 = "�� ��������� ����� �� ����� �����������!";
				ProtocolForm->SendToProtocol("������: " + stext2);
				result = false;
				break;
			}
		}
/*
	pp.CalculateDeadZone(ini->ReadInteger("Size","ZoneLength",2));
	pr.CalculateDeadZone(ini->ReadInteger("Size","ZoneLength",2));
	// ���� ����� �� ������ �����������
	if ( Linear )
		{
			stext2 = "���� ����� �� ������ �����������";
			Synchronize( &UpdateMainForm );
			if ( digital->WaitForInSignal("���� �����",4000, TThread::CurrentThread) )
				lin_t2 = GetTickCount();
			else
			{
				stext2 = "�� ��������� ����� �� ������ �����������!";
				result = false;
				break;
			}
			lin_v = linear_len / (lin_t2 - lin_t1);		// �������� �/�
		}
		if ( Terminated )
			break;
*/
// ���� ����� �� ������ 4
		stext2 = "���� ����� �� ������ 4";
		ProtocolForm->SendToProtocol("������: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForInSignal("������ 4",15000, TThread::CurrentThread) )
		{
			stext2 = "�� ��������� ����� �� ������ 4!";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
		st4 = GetTickCount();
		digital->SetOutSignal("����");
		//�� ����� �����
		if ( digital->CheckInSignal("���� �����") )
		{
			digital->ResetOutSignal("���� �������� 1");
			digital->ResetOutSignal("���� �������� 2");
			digital->ResetOutSignal("���� �������� 3");
			for (UINT s = 0; s < Globals::speeds.linearSpeed.size(); s++)
				digital->SetOutSignal("���� �������� " + IntToStr(Globals::speeds.linearSpeed[s]));
		//	ProtocolForm->SendToProtocol("������� �������� �������� �����������");
		}

// ���� ����� �� ����� � ��
		stext2 = "���� ����� �� ����� � ������ ��";
		ProtocolForm->SendToProtocol("������: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForInSignal("�� ����",2000, TThread::CurrentThread) )
		{
			stext2 = "�� ��������� ����� ����� � ������ ��!";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
		if(SystemConst::SolidGroupSource == 0)      //���� �� ����� LCard
		{
			TThread::CurrentThread->Sleep(200);
			lpop->SetMeasInSG();
		}

// ���� ����� ����� �� ������ 1
		stext2 = "���� ����� ����� �� ������ 1";
		ProtocolForm->SendToProtocol("������: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForDropInSignal( "������ 1" , 100000 , TThread::CurrentThread) )
		{
			stext2 = "�� ��������� ������ ����� �� ������ 1!";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
		st1_out = GetTickCount();
// ���� ����� ����� � �������� ������
		if ( !digital->WaitForDropInSignal( "����� ����" , 4000 , TThread::CurrentThread) )
		{
			stext2 = "�� ��������� ����� ����� �� ����� �����������!";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
		cross_t3 = GetTickCount();
		TThread::CurrentThread->Sleep(1000);
		MeasOutCrossModule=lpop->Num;  //������� ������ ���� �������� �� ������ ������ �� ����� ������

		//lpop->Stop();

		stext2 = "���� ������ ����� �� �����������";
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForDropInSignal( "����� �����" , 4000 , TThread::CurrentThread) )
		{
			stext2 = "�� ��������� ������ ����� �� �����������!";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
		digital->ResetOutSignal("�������� �������");
//		lpop->Stop();
		st_v = st_len / (st4 - st1);
		double st_tube = st_v * ( st1_out - st1);
		short cr_zones = (short) Math::Ceil( st_tube / zone_len);
		if (cr_zones > Globals::max_zones)
			cr_zones = Globals::max_zones;
		respop->zones = cr_zones;
		sms->length_in_zones = cr_zones;

// ���� ������ �� �����������
		if (Thick)
		{
			stext2 = "���� ������ ����� �� �����������";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForDropInSignal( "���� ����" , 8000 , TThread::CurrentThread) )
			{
				stext2 = "�� ��������� ����� ����� �� ����� �����������!";
				ProtocolForm->SendToProtocol("������: " + stext2);
				result = false;
				break;
			}
			lat->ResetControl();
		}
		if (Linear)
		{
			digital->ResetOutSignal("���� �������� 1");
			digital->ResetOutSignal("���� �������� 2");
			digital->ResetOutSignal("���� �������� 3");
			for (UINT s = 0; s < Globals::speeds.outSpeed.size(); s++)
				digital->SetOutSignal("���� �������� " + IntToStr(Globals::speeds.outSpeed[s]));
			ProtocolForm->SendToProtocol("�������� �������� �������� �����������");
		}

// ���� ����� ����� � �������� �����������
		if ( Linear )
		{
			if ( !digital->WaitForDropInSignal( "���� ����" , 10000 , TThread::CurrentThread) )
			{
				stext2 = "�� ��������� ����� ����� �� ����� �����������!";
				ProtocolForm->SendToProtocol("������: " + stext2);
				result = false;
				break;
			}
			lin_t3 = GetTickCount();
			TThread::CurrentThread->Sleep(1200);
   //todo zzzzz			lprod->Stop();

			stext2 = "���� ������ ����� �� �����������";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForDropInSignal( "���� �����" , 25000 , TThread::CurrentThread) )
			{
				stext2 = "�� ��������� ������ ����� �� �����������!";
				result = false;
				break;
			}
//			lprod->Stop();
			digital->ResetOutSignal("���� STF");
		}

// ����� ������, ����� ������ ���������
	stext2 = "���� ����� ����� �� ������ 4";
	ProtocolForm->SendToProtocol("������: " + stext2);
	Synchronize( &UpdateMainForm );
	if ( !digital->WaitForDropInSignal( "������ 4" , 30000 , TThread::CurrentThread) )
	{
		stext2 = "�� ��������� ����� ����� �� ������ 4!";
		ProtocolForm->SendToProtocol("������: " + stext2);
		result = false;
		break;
	}
	digital->ResetOutSignal("��: STF");
	digital->ResetOutSignal("����");

	stext2 = "���� ������ ����� �� ���������";
	ProtocolForm->SendToProtocol("������: " + stext2);
	Synchronize( &UpdateMainForm );
	if( !digital->WaitForDropInSignal( "�� �����" , 5000 , TThread::CurrentThread) )
	{
		stext2 = "�� ��������� ������ ����� �� ���������!";
		ProtocolForm->SendToProtocol("������: " + stext2);
		result = false;
		break;
	}
	lpop->Stop();
	lpop->SetMeasOutSG();
	ProtocolForm->SendToProtocol("������: ���������� ���� lpop" );

	//st_v = st_len / (st4 - st1);
	//double st_tube = st_v * ( st1_out - st1);
// ������������ ������ �����������
/*todo 1605
	if ( Linear )
	{
		short lin_zones = (short) Math::Ceil( st_tube / zone_len );
		if (lin_zones > Globals::max_zones)
			lin_zones = Globals::max_zones;
		resprod->zones = lin_zones;
 //todo zzzzz		resprod->PrepareData( lprod , resprod->sensors, resprod->sensors );
		resprod->ComputeZoneData();
		resprod->CalculateDeadZone(zone_len );
		ProtocolForm->SendToProtocol("������: ���������� ������ �����������" );
	}
*/
	long trash=0;
	if(SystemConst::SolidGroupSource == 0)      //���� �� ����� LCard
		//��������� ������� ������ ���� �������� ����� ���� ��� ����� ����� �� �����������
		trash = (lpop->Num-MeasOutCrossModule)*lpop->part;
	else
		trash=0;

	ProtocolForm->SendToProtocol("������: ���������� trash" );

 // ������������� ������ �����������
	//short cr_zones = (short) Math::Ceil( st_tube / zone_len);
	//if (cr_zones > Globals::max_zones)
	//	cr_zones = Globals::max_zones;


	//respop->zones = cr_zones;
	//todo 1605
	respop->PrepareData( lpop ,16, respop->sensors,trash );
	//respop->PrepareData( lpop ,respop->data, resprod->data);
	respop->ComputeZoneData();
	respop->CalculateDeadZone(zone_len );

	ProtocolForm->SendToProtocol("������: ���������� ������ �����������" );


	if ( Linear )
	{
		short lin_zones = (short) Math::Ceil( st_tube / zone_len );
		if (lin_zones > Globals::max_zones)
			lin_zones = Globals::max_zones;
		resprod->zones = lin_zones;
 //todo zzzzz		resprod->PrepareData( lprod , resprod->sensors, resprod->sensors );
		resprod->ComputeZoneData();
		resprod->CalculateDeadZone(zone_len );
		ProtocolForm->SendToProtocol("������: ���������� ������ �����������" );
	}

	digital->ResetOutSignal("�������� ������");
	digital->ResetOutSignal("�������� �������");
	digital->ResetOutSignal("������");

// ����, ����� ����� ������� � �����
		stext2 = "���� ����� � ����� ����� (��2)";
		ProtocolForm->SendToProtocol("������: " + stext2);
		Synchronize( &UpdateMainForm );
	  /*	while (true)
		{
			if ( Terminated )
				break;
			if ( digital->WaitForInSignal( "��� ��2" , 50000 , TThread::CurrentThread) )
			{
				if ( digital->CheckInBouncelessSignal("��� ��2",true) )
				{
					ProtocolForm->SendToProtocol("������: �������� ��2");
					break;
				}
				else
					continue;
			}
			else
			{
				stext2 = "�� ��������� ����� � ����� �����! (��2)";
				ProtocolForm->SendToProtocol("������: " + stext2);
				result = false;
				break;
			}
		}
		*/
		break;
	}
	// ������� ����������� �������
	digital->ResetOutSignal("�������� ������");
	digital->ResetOutSignal("�������� �������");
//	digital->ResetOutSignal("��: ��� ��");
	digital->ResetOutSignal("��: �������� 1");
	digital->ResetOutSignal("��: �������� 2");
	digital->ResetOutSignal("��: �������� 3");
	digital->ResetOutSignal("��: STF");

	digital->ResetOutSignal("������ ����");
	digital->ResetOutSignal("����� ����");

	digital->ResetOutSignal("������");
	digital->ResetOutSignal("����");
	digital->ResetOutSignal("����");

	digital->ResetOutSignal("��� ������");
	digital->ResetOutSignal("��� ��������");

	if ( Linear )
	{
//		digital->ResetOutSignal("���� ��� ��");
		digital->ResetOutSignal("���� STF");
		digital->ResetOutSignal("���� �������� 1");
		digital->ResetOutSignal("���� �������� 2");
		digital->ResetOutSignal("���� �������� 3");
		digital->ResetOutSignal("���� ��� ��");
	}

	if ( Terminated )
	{
		lpop->Stop();
	   //	lpop->SetMeasOutSG();
	  //todo zzzzz	if ( Linear )lprod->Stop();

		ProtocolForm->SendToProtocol("������: ������ �� ����� �� Terminate");
		ReturnValue = 0;
		return false;
	}
	if ( result)
	{
		stext1 = "����� \"������\" �������� �������";
		ProtocolForm->SendToProtocol(stext1);
		stext2 = "";
		Synchronize( &UpdateMainForm );
		return true;
	}
	else
	{
		lat->ResetControl();
		lpop->Stop();

//todo zzzzz		if ( Linear )lprod->Stop();
		stext1 = "����� \"������\" �� ��������!";
		ProtocolForm->SendToProtocol(stext1);
		Synchronize( &UpdateMainForm );
		return false;
	}
}
#else
bool WorkThreadClass::WorkMode()
{
#if 1
	stext2 = "�������� ������";
	Synchronize( &UpdateMainForm );
	double lin_v;
	double cr_v;
	double st_v;

// ��������� �������� �������� �� ini
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	String Sect = "Type_" + MainForm->cbTypeSize->Text;
	delete ini;
// ��������� ��������� ����, ������� ���� 1 ���
	bool result = true;
	while ( result )
	{
// ���� ����� ����� �������� (������ ��1)
		stext2 = "���� ����� ����� �������� ����������";
		ProtocolForm->SendToProtocol("������: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForInSignal("��� ��1", 0, TThread::CurrentThread) )
		{
			stext2 = "�� ��������� ����� ����� ��������! (������� ��1)";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
		digital->ResetOutSignal("��� ���������");
		digital->ResetOutSignal("��� �������");

// �������� ��������� ����, ��������� �������
		digital->SetOutSignal("�������� ������");
		digital->SetOutSignal("�������� �������");
		TThread::CurrentThread->Sleep(500);
  		if ( lpop->isOkTemp("MG") )
		{
			stext2 = "��� ���������� ����!";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
// ���������, ������� �������� ������������
	ProtocolForm->SendToProtocol("������: ���������� " + IntToStr(Globals::current_diameter));
	respop->sensors = (Globals::current_diameter == 60) ? (10) : (12);
	resprod->sensors = 4;
		//LCard �����!!!
		lpop->InitADC(16,0,"PP",0);
		lpop->Start();
		TThread::CurrentThread->Sleep(200);
		lpop->Stop();


// ��������������� ���� ������ � LCard
		ProtocolForm->SendToProtocol("������: �������������� ���� ������ � LCard");
		int init=0;
		if(SystemConst::SolidGroupSource == 0)
			init=2;		  //���� �� ����� ����� �� �������������� ����� � ��

		lpop->InitADC( 16,0, "PP", init);
		/* todo zzzzzz
		if ( Linear )
			lprod->InitADC(resprod->sensors,0, "PR", 0);
        */
// ���� ������� ���������� �� �����������
		digital->SetOutSignal("������");
		if ( Thick )
		{
			stext2="���� ���������� �����������";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForInSignal("����������",30000, TThread::CurrentThread))
			{
				stext2 = "�� ��������� ���������� �����������";
				ProtocolForm->SendToProtocol("������: " + stext2);
				result = false;
				break;
			}
		}
// ������ ����������
		if ( Linear )
		{
			digital->SetOutSignal("���� ��� ��");
			digital->SetOutSignal("���� STF");
			for (UINT s = 0; s < Globals::speeds.inSpeed.size(); s++)
				digital->SetOutSignal("���� �������� " + IntToStr(Globals::speeds.inSpeed[s]));
#if 0
			if ( !digital->WaitForDropInSignal("���� �� RUN", 13000 , TThread::CurrentThread) )
			{
				stext2 = "�� ���������� �������� �������� ����������� ������!";
				ProtocolForm->SendToProtocol("������: " + stext2);
				result = false;
				break;
			}
#else
		   dprint("�������� �������� �������� ���������� ������\n");
#endif
		}
			if ( Terminated )
				break;

// ���� �������� �����������
		digital->SetOutSignal("����");
		if ( Thick )
		{
			stext2="���� �������� �����������";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForInSignal("��������",10000, TThread::CurrentThread))
			{
				stext2 = "�� ��������� �������� �����������";
				ProtocolForm->SendToProtocol("������: " + stext2);
				result = false;
				break;
			}
			lat->SetControl();
		}
// �������� �������� ����� ������
		digital->SetOutSignal("��: ��� ��");
		for (UINT s = 0; s < Globals::speeds.workSpeed.size(); s++)
			digital->SetOutSignal("��: �������� " + IntToStr(Globals::speeds.workSpeed[s]));
		digital->SetOutSignal("��: STF");
		if ( !digital->WaitForDropInSignal("��: �� RUN",3000, TThread::CurrentThread) )
		{
			stext2 = "�� ���������� �������� �������� ����������";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
		if ( Terminated )
			break;
// ���������� ������� � ���, ����� �������
		digital->SetOutSignal("��� ������");
		digital->SetOutSignal("��� ��������");



//************************����� ����********************************************
// ���� ����� �� ������ ������
		stext2 = "���� ����� �� ������ 1";
		ProtocolForm->SendToProtocol("������: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForInSignal("������ 1", 10000, TThread::CurrentThread ) )
		{
			stext2 = "�� ��������� ����� �� ������ 1!";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
		st1 = GetTickCount();
		if ( Terminated )
			break;
// ���� ����� �� ����� �����������
		stext2 = "���� ����� �� ����� �����������";
		Synchronize( &UpdateMainForm );
		if ( digital->WaitForInSignal("����� ����",10000, TThread::CurrentThread) )
		{
			TThread::CurrentThread->Sleep(900);
			lpop->Start();
			Lir::Instance().Start(Thick, Linear);
			cross_t1 = GetTickCount();
		}
		else
		{
			stext2 = "�� ��������� ����� �� ����� �����������!";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
		if ( Terminated )
			break;
/*
	// ���� ����� �� ������ �����������
		stext2 = "���� ����� �� ������ �����������";
		Synchronize( &UpdateMainForm );
		if ( digital->WaitForInSignal("����� �����",4000, TThread::CurrentThread) )
			cross_t2 = GetTickCount();
		else
		{
			stext2 = "�� ��������� ����� �� ������ �����������!";
			result = false;
			break;
		}
*/
/*
// ���� ����� �� ����� �����������, ���� �����
		if ( Thick )
		{
			stext2 = "���� ����� �� ����� �����������";
			Synchronize( &UpdateMainForm );
			if ( digital->WaitForInSignal("���� ����",5000, TThread::CurrentThread) )
			{
//				TThread::CurrentThread->Sleep( centralizerSleep );
//				digital->BlinkOutSignal("���� ����� on");
			}
			else
			{
				stext2 = "�� ��������� ����� �� ����� �����������!";
				result = false;
				break;
			}
		}
		if ( Terminated )
			break;
*/
// ���� ����� �� ����� �����������, ���� �����
		if ( Linear )
		{
			stext2 = "���� ����� �� ����� �����������";
			Synchronize( &UpdateMainForm );
			digital->SetOutSignal("���� ��� ��");
			if ( digital->WaitForInSignal("���� ����",15000, TThread::CurrentThread) )
			{
				TThread::CurrentThread->Sleep(900);
  //toto zzzzz				lprod->Start();
				lin_t1 = GetTickCount();
//				TThread::CurrentThread->Sleep( centralizerSleep );
//				digital->BlinkOutSignal("���� ����� on");
			}
			else
			{
				stext2 = "�� ��������� ����� �� ����� �����������!";
				ProtocolForm->SendToProtocol("������: " + stext2);
				result = false;
				break;
			}
		}
/*
	pp.CalculateDeadZone(ini->ReadInteger("Size","ZoneLength",2));
	pr.CalculateDeadZone(ini->ReadInteger("Size","ZoneLength",2));
	// ���� ����� �� ������ �����������
	if ( Linear )
		{
			stext2 = "���� ����� �� ������ �����������";
			Synchronize( &UpdateMainForm );
			if ( digital->WaitForInSignal("���� �����",4000, TThread::CurrentThread) )
				lin_t2 = GetTickCount();
			else
			{
				stext2 = "�� ��������� ����� �� ������ �����������!";
				result = false;
				break;
			}
			lin_v = linear_len / (lin_t2 - lin_t1);		// �������� �/�
		}
		if ( Terminated )
			break;
*/
// ���� ����� �� ������ 4
		stext2 = "���� ����� �� ������ 4";
		ProtocolForm->SendToProtocol("������: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForInSignal("������ 4",15000, TThread::CurrentThread) )
		{
			stext2 = "�� ��������� ����� �� ������ 4!";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
		st4 = GetTickCount();
		digital->SetOutSignal("����");
		//�� ����� �����
		if ( digital->CheckInSignal("���� �����") )
		{
			digital->ResetOutSignal("���� �������� 1");
			digital->ResetOutSignal("���� �������� 2");
			digital->ResetOutSignal("���� �������� 3");
			for (UINT s = 0; s < Globals::speeds.linearSpeed.size(); s++)
				digital->SetOutSignal("���� �������� " + IntToStr(Globals::speeds.linearSpeed[s]));
		//	ProtocolForm->SendToProtocol("������� �������� �������� �����������");
		}

// ���� ����� �� ����� � ��
		stext2 = "���� ����� �� ����� � ������ ��";
		ProtocolForm->SendToProtocol("������: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForInSignal("�� ����",2000, TThread::CurrentThread) )
		{
			stext2 = "�� ��������� ����� ����� � ������ ��!";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
		if(SystemConst::SolidGroupSource == 0)      //���� �� ����� LCard
		{
			TThread::CurrentThread->Sleep(200);
			lpop->SetMeasInSG();
		}
		PrepareSG::Instance().timeStart = timeGetTime();
// ���� ����� ����� �� ������ 1
		stext2 = "���� ����� ����� �� ������ 1";
		ProtocolForm->SendToProtocol("������: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForDropInSignal( "������ 1" , 100000 , TThread::CurrentThread) )
		{
			stext2 = "�� ��������� ������ ����� �� ������ 1!";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
		st1_out = GetTickCount();

// ���� ����� ����� � �������� ������
		if ( !digital->WaitForDropInSignal( "����� ����" , 4000 , TThread::CurrentThread) )
		{
			stext2 = "�� ��������� ����� ����� �� ����� �����������!";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
		cross_t3 = GetTickCount();
		TThread::CurrentThread->Sleep(1000);
		MeasOutCrossModule=lpop->Num;  //������� ������ ���� �������� �� ������ ������ �� ����� ������

		//lpop->Stop();

		stext2 = "���� ������ ����� �� �����������";
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForDropInSignal( "����� �����" , 4000 , TThread::CurrentThread) )
		{
			stext2 = "�� ��������� ������ ����� �� �����������!";
			ProtocolForm->SendToProtocol("������: " + stext2);
			result = false;
			break;
		}
		digital->ResetOutSignal("�������� �������");
//		lpop->Stop();
		st_v = st_len / (st4 - st1);
		double st_tube = st_v * ( st1_out - st1);
		short cr_zones = (short) Math::Ceil( st_tube / zone_len);
		if (cr_zones > Globals::max_zones)
			cr_zones = Globals::max_zones;
		respop->zones = cr_zones;
		sms->length_in_zones = cr_zones;

// ���� ������ �� �����������
		if (Thick)
		{
			stext2 = "���� ������ ����� �� �����������";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForDropInSignal( "���� ����" , 8000 , TThread::CurrentThread) )
			{
				stext2 = "�� ��������� ����� ����� �� ����� �����������!";
				ProtocolForm->SendToProtocol("������: " + stext2);
				result = false;
				break;
			}
			lat->ResetControl();
		}
		if (Linear)
		{
			digital->ResetOutSignal("���� �������� 1");
			digital->ResetOutSignal("���� �������� 2");
			digital->ResetOutSignal("���� �������� 3");
			for (UINT s = 0; s < Globals::speeds.outSpeed.size(); s++)
				digital->SetOutSignal("���� �������� " + IntToStr(Globals::speeds.outSpeed[s]));
			ProtocolForm->SendToProtocol("�������� �������� �������� �����������");
		}

// ���� ����� ����� � �������� �����������
		if ( Linear )
		{
			if ( !digital->WaitForDropInSignal( "���� ����" , 10000 , TThread::CurrentThread) )
			{
				stext2 = "�� ��������� ����� ����� �� ����� �����������!";
				ProtocolForm->SendToProtocol("������: " + stext2);
				result = false;
				break;
			}
			lin_t3 = GetTickCount();
			TThread::CurrentThread->Sleep(1200);
   //todo zzzzz			lprod->Stop();

			stext2 = "���� ������ ����� �� �����������";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForDropInSignal( "���� �����" , 25000 , TThread::CurrentThread) )
			{
				stext2 = "�� ��������� ������ ����� �� �����������!";
				result = false;
				break;
			}
//			lprod->Stop();
			digital->ResetOutSignal("���� STF");
		}

// ����� ������, ����� ������ ���������
	stext2 = "���� ����� ����� �� ������ 4";
	ProtocolForm->SendToProtocol("������: " + stext2);
	Synchronize( &UpdateMainForm );
	if ( !digital->WaitForDropInSignal( "������ 4" , 30000 , TThread::CurrentThread) )
	{
		stext2 = "�� ��������� ����� ����� �� ������ 4!";
		ProtocolForm->SendToProtocol("������: " + stext2);
		result = false;
		break;
	}
	digital->ResetOutSignal("��: STF");
	digital->ResetOutSignal("����");

	stext2 = "���� ������ ����� �� ���������";
	ProtocolForm->SendToProtocol("������: " + stext2);
	Synchronize( &UpdateMainForm );
	if( !digital->WaitForDropInSignal( "�� �����" , 5000 , TThread::CurrentThread) )
	{
		stext2 = "�� ��������� ������ ����� �� ���������!";
		ProtocolForm->SendToProtocol("������: " + stext2);
		result = false;
		break;
	}
	PrepareSG::Instance().timeStop = timeGetTime();
	lpop->Stop();
	lpop->SetMeasOutSG();
	ProtocolForm->SendToProtocol("������: ���������� ���� lpop" );

	//st_v = st_len / (st4 - st1);
	//double st_tube = st_v * ( st1_out - st1);
// ������������ ������ �����������
/*todo 1605
	if ( Linear )
	{
		short lin_zones = (short) Math::Ceil( st_tube / zone_len );
		if (lin_zones > Globals::max_zones)
			lin_zones = Globals::max_zones;
		resprod->zones = lin_zones;
 //todo zzzzz		resprod->PrepareData( lprod , resprod->sensors, resprod->sensors );
		resprod->ComputeZoneData();
		resprod->CalculateDeadZone(zone_len );
		ProtocolForm->SendToProtocol("������: ���������� ������ �����������" );
	}
*/
	long trash=0;
	if(SystemConst::SolidGroupSource == 0)      //���� �� ����� LCard
		//��������� ������� ������ ���� �������� ����� ���� ��� ����� ����� �� �����������
		trash = (lpop->Num-MeasOutCrossModule)*lpop->part;
	else
		trash=0;

	ProtocolForm->SendToProtocol("������: ���������� trash" );

 // ������������� ������ �����������
	//short cr_zones = (short) Math::Ceil( st_tube / zone_len);
	//if (cr_zones > Globals::max_zones)
	//	cr_zones = Globals::max_zones;

 /* 1805      ������� ��  ������ ����
	//respop->zones = cr_zones;
	//todo 1605
   //	respop->PrepareData( lpop ,16, respop->sensors,trash );
   //	respop->PrepareData( lpop ,respop->data, resprod->data);
	respop->ComputeZoneData();
	respop->CalculateDeadZone(zone_len );

	ProtocolForm->SendToProtocol("������: ���������� ������ �����������" );


	if ( Linear )
	{
		short lin_zones = (short) Math::Ceil( st_tube / zone_len );
		if (lin_zones > Globals::max_zones)
			lin_zones = Globals::max_zones;
		resprod->zones = lin_zones;
 //todo zzzzz		resprod->PrepareData( lprod , resprod->sensors, resprod->sensors );
		resprod->ComputeZoneData();
		resprod->CalculateDeadZone(zone_len );
		ProtocolForm->SendToProtocol("������: ���������� ������ �����������" );
	}
 */
	if(Linear)
	{
		Compute(lpop, *respop, *resprod);
		ProtocolForm->SendToProtocol("������: ���������� ������ �����������" );
	}
	else
	{
		 Compute(lpop, *respop);
	}
	ProtocolForm->SendToProtocol("������: ���������� ������ �����������" );
	digital->ResetOutSignal("�������� ������");
	digital->ResetOutSignal("�������� �������");
	digital->ResetOutSignal("������");

// ����, ����� ����� ������� � �����
		stext2 = "���� ����� � ����� ����� (��2)";
		ProtocolForm->SendToProtocol("������: " + stext2);
		Synchronize( &UpdateMainForm );
	  /*	while (true)
		{
			if ( Terminated )
				break;
			if ( digital->WaitForInSignal( "��� ��2" , 50000 , TThread::CurrentThread) )
			{
				if ( digital->CheckInBouncelessSignal("��� ��2",true) )
				{
					ProtocolForm->SendToProtocol("������: �������� ��2");
					break;
				}
				else
					continue;
			}
			else
			{
				stext2 = "�� ��������� ����� � ����� �����! (��2)";
				ProtocolForm->SendToProtocol("������: " + stext2);
				result = false;
				break;
			}
		}
		*/
		break;
	}
	// ������� ����������� �������
	digital->ResetOutSignal("�������� ������");
	digital->ResetOutSignal("�������� �������");
//	digital->ResetOutSignal("��: ��� ��");
	digital->ResetOutSignal("��: �������� 1");
	digital->ResetOutSignal("��: �������� 2");
	digital->ResetOutSignal("��: �������� 3");
	digital->ResetOutSignal("��: STF");

	digital->ResetOutSignal("������ ����");
	digital->ResetOutSignal("����� ����");

	digital->ResetOutSignal("������");
	digital->ResetOutSignal("����");
	digital->ResetOutSignal("����");

	digital->ResetOutSignal("��� ������");
	digital->ResetOutSignal("��� ��������");

	if ( Linear )
	{
//		digital->ResetOutSignal("���� ��� ��");
		digital->ResetOutSignal("���� STF");
		digital->ResetOutSignal("���� �������� 1");
		digital->ResetOutSignal("���� �������� 2");
		digital->ResetOutSignal("���� �������� 3");
		digital->ResetOutSignal("���� ��� ��");
	}

	if ( Terminated )
	{
		lpop->Stop();
	   //	lpop->SetMeasOutSG();
	  //todo zzzzz	if ( Linear )lprod->Stop();

		ProtocolForm->SendToProtocol("������: ������ �� ����� �� Terminate");
		ReturnValue = 0;
		return false;
	}
	if ( result)
	{
		stext1 = "����� \"������\" �������� �������";
		ProtocolForm->SendToProtocol(stext1);
		stext2 = "";
		Synchronize( &UpdateMainForm );
		return true;
	}
	else
	{
		lat->ResetControl();
		lpop->Stop();

//todo zzzzz		if ( Linear )lprod->Stop();
		stext1 = "����� \"������\" �� ��������!";
		ProtocolForm->SendToProtocol(stext1);
		Synchronize( &UpdateMainForm );
		return false;
	}
	#endif
}
#endif

//-----------------------------------------------------------------------------

