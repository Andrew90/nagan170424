//---------------------------------------------------------------------------


#pragma hdrstop

#include "ThreadOnLine.h"
#include "Math.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

int SleepTemp=750;		// ����� �������� ������� (�� � ���� �������)
UINT Counter1Delay = 1000;		// �������� ��� �������� � 0 �� 1 �������
UINT SynchroTime = 110;			// ����� �� ����������� ������� ������ � ����������

//---------------------------------------------------------------------------
//----����������� - ��������� ������� ���������� �� ����������---------------
__fastcall ThreadOnLine::ThreadOnLine(bool CreateSuspended,
							Advantech1784 *_Counter,	// ����� Advantech1784
							LCard791 &_l1,				//���������� �����
							LCard791 &_l2,				//���������� �����
							Result &_respop,			// ��������� �� ��������� ����������
							Result &_resprod,			// ��������� �� ��������� ����������
							ThicknessResult &_resth,	// ��������� �� ��������� �������
							SummaryResult &_ressum,		// ��������� �� ��������� ���������
							TStatusBar *St,				// ��������� �� ���������, ������������ ������� ��������
							bool _Thick,				// ���������� �� �������������
							bool _Linear,				// ���������� �� ����������)
							AlarmThreadClass &alth)		// ����� ����� ������

{
	CalcOffsets();		// ������� �������
	ppFirst=0;
	ppSecond=0;
	prFirst=0;
	prSecond=0;

	ppOld = 0;
	prOld = 0;
	thOld = -1;					// ��� ������� ����� �����������
	markOld = -1;				// ��� ���������� 1-�� ����
	current_counter = 0;		// �������� � 1-�� ��������
	long Counter0Fin = 0;
	long Counter1Fin = 0;
	Collect = true;
	transit = false;

	Counter = _Counter; 	//���
	lpop = &_l1;			//���������� �����
	lprod = &_l2;			//���������� �����
	respop = &_respop;
	resprod =  &_resprod;
	resth = &_resth;
	ressum = &_ressum;
	StB = St;
	Thick = _Thick;
	Linear = _Linear;
	lat = &alth;
	Counter->Reset(current_counter);
}
//---------------------------------------------------------------------------
//-----������ ������ ������--------------------------------------------------
void __fastcall ThreadOnLine::Execute()
{
	NameThreadForDebugging("WorkOnlineThread");
	stext1 = "����� \"������ (�)\"";
	stext2 = "������� � ��������� ���������";
	ProtocolForm->SendToProtocol("-----------");
	ProtocolForm->SendToProtocol(stext1);
	Synchronize( &UpdateStatus );

	UnicodeString prepare_result = PrepareForWork( );
	if ( prepare_result != "ok" )
	{
		stext1 = "����� \"������ (�)\" �� ��������!";
		stext2 = prepare_result;
		ProtocolForm->SendToProtocol("������ (�): " + stext2);
		Synchronize( &UpdateStatus );
		digital->ResetOutSignal("������ ����");
		digital->ResetOutSignal("����� ����");
		Finally();
		cool = false;
		if (prepare_result == "�������")
			transit = true;
		return;
	}

	ProtocolForm->SendToProtocol("������ (�): ���������� ������ �������" );
	SetReturnValue( OnlineCycle() );
	cool = ReturnValue;
	Finally();
	Terminate();
	return;
}
//---------------------------------------------------------------------------
//-----���������� � ������ �� ������ ������ �� �������� �����----------------
UnicodeString ThreadOnLine::PrepareForWork( )
{
// ���������� ��� ������� � ���
	if (digital->SignalExists("��� �����"))
		digital->ResetOutSignal("��� �����");
	digital->ResetOutSignal("��� ������");
	digital->ResetOutSignal("��� ���������");
	digital->ResetOutSignal("��� ��");
//	digital->ResetOutSignal("��� �������");
	digital->ResetOutSignal("������ �������");
	digital->ResetOutSignal("����� �������");

// ���������, �� �������� �� ����� � �������
	if ( digital->CheckInSignal("���� ����") ||
			digital->CheckInSignal("���� �����") ||
			digital->CheckInSignal("���� ����")  ||
			digital->CheckInSignal("���� �����") ||
			digital->CheckInSignal("����� ����") ||
			digital->CheckInSignal("����� �����") )
		return "����� � �������!";

// ���������, �� ������� �� ������ �� �����
	if ( digital->CheckInSignal("������ 1") ||
			digital->CheckInSignal("������ 2") ||
			digital->CheckInSignal("������ 3")  ||
			digital->CheckInSignal("������ 4") ||
			digital->CheckInSignal("�� �����") ||
			digital->CheckInSignal("���� ����") )
		return "����� �� �����!";

// ���������, �� ��������� �� ���������
	if ( lpop->isOkTemp("1") || lpop->isOkTemp("2") )
		return "��������� ���������!";

// ���������, ������� �� ������ ��������������� � ������ ���������
	if ( !digital->CheckInBouncelessSignal("�� �������" , false) )
		return "�������� ������ ���������������!";

// �������� ������ ��� �����
	ProtocolForm->SendToProtocol("������ (�): �������� ������ ��� �����" );
	try
	{
		//��� ��� ������������� �����, ���� ����� ���������� ������ �� LCard,�
		ULONG Size=(5*1024*1024)/sizeof(ULONG);
		Buffer=new ULONG[Size];
	}
	catch (...)
	{
		String Err = "�� ������� �������� ������ ��� ������";
		Application->MessageBox(L"�� ������� �������� ������ ��� ������",L"������",MB_OK);
		ProtocolForm->SendToProtocol(Err);
		return Err;
	}

// ���� ��� ����
	stext2="���� ������� \"��� ����\"";
	Synchronize( &UpdateStatus );
	if ( !digital->WaitForInSignal("��� ����",0, TThread::CurrentThread) )
		return "�� ��������� ������� ��� ����!";
	lat->SetPlantCycle();

// ���� ����� ����� � ��������� ���������
/*	stext2 = "���� ����� ����� � ��������� ���������";
	ProtocolForm->SendToProtocol("������ (�): " + stext2);
	Synchronize( &UpdateStatus );
	if ( !digital->WaitForDropInSignal("��� ��2", 0, TThread::CurrentThread) )
		return "�� ��������� ����� ����� � ����� �����! (������� ��2)";
	digital->ResetOutSignal("��� �������");
*/
// ���� ����� ����� �������� (������ ��1)
	digital->SetOutSignal("��� ����������");
	stext2 = "���� ����� ����� �������� ����������";
	ProtocolForm->SendToProtocol("������ (�): " + stext2);
	Synchronize( &UpdateStatus );
	if ( !digital->WaitForInSignal("��� ��1", 0, TThread::CurrentThread) )
		return "�� ��������� ����� ����� ��������! (������� ��1)";

	digital->ResetOutSignal("��� ���������");
	digital->ResetOutSignal("��� ����������");
	digital->ResetOutSignal("��� �������");
	MainForm->ClearCharts(this);

// ���������, ��� �� ���� �������������� (��� �������)
	if ( digital->CheckInSignal("��� �������" )	)
		return "�������";

// ���������� ����� ������ �� ���������� �����
	stext2 = "���� ���������� �����";
	ProtocolForm->SendToProtocol("������ (�): " + stext2);
	Synchronize( &UpdateStatus );
	digital->ResetOutSignal("������ �������");
	if ( !digital->WaitForDropInSignal("������ ������", 8000 , TThread::CurrentThread) )
		return "������ ����������� �����!";
	TThread::CurrentThread->Sleep(500);
	digital->SetOutSignal("������ ����");
	if ( !digital->WaitForInSignal("������ ������", 4000 , TThread::CurrentThread) )
		return "������ ����������� �����!";
	lat->SetStand("������");

// ���������� ����� ������ �� ���������� �������
	stext2 = "���� ���������� �������";
	ProtocolForm->SendToProtocol("������ (�): " + stext2);
	Synchronize( &UpdateStatus );
	digital->ResetOutSignal("����� �������");
	if ( !digital->WaitForDropInSignal("����� ������", 6000 , TThread::CurrentThread) )
		return "������ ����������� �������!";
	TThread::CurrentThread->Sleep(500);
	digital->SetOutSignal("����� ����");
	if ( !digital->WaitForInSignal("����� ������", 4000 , TThread::CurrentThread) )
		return "������ ����������� �������!";

	if (Thick && Linear)
	{
		String result = ModulePosition::SetAllModulesInWork("����" , "����", "�����");
		if (result != "ok")
			return result;
	}
	else
	{
// ������������� ������� � ������ ���������
		stext2 = "������ ������ � ������ ���������";
		ProtocolForm->SendToProtocol("������ (�): " + stext2);
		Synchronize( &UpdateStatus );
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

// ������������� ���������� � ������ ��������� (������ � ������)
		if ( !digital->CheckInSignal("����� ��") )
		{
			String result = ModulePosition::SetModuleInWork( "�����" );
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
	if ( Thick && SystemConst::ThickCentralizer && !digital->CheckInSignal("���� ����� ���") )
	{
		digital->BlinkOutSignal("���� ����� �������");
		if ( !digital->WaitForInSignal("���� ����� ���",SleepTemp , TThread::CurrentThread) )
			return "������ ������ �����������!";
	}

	if ( Linear && SystemConst::LinearCentralizer && !digital->CheckInSignal("���� ����� ���") )
	{
		digital->BlinkOutSignal("���� ����� �������");
		if ( !digital->WaitForInSignal("���� ����� ���",SleepTemp , TThread::CurrentThread) )
			return "������ ������ ����������� ������!";
	}

// �������� ����� ����� �� ���
	if (SystemConst::ComWithASU)
	{
		long tube_num;
		do
		{
			tube_num = MyCom->GetTubeNumber();
			if (tube_num == -1)
			{
				if ( Application->MessageBoxW(L"������ ��� ��������� ������ ����� �� ���. ���������?",
							L"��������",MB_RETRYCANCEL) == 2 )
					tube_num = 11;
			}
		}
		while (tube_num == -1);
		Globals::tube_number = tube_num;
		plantCut = MyCom->GetPlantCut();
		String s = (tube_num == 55555555) ? ((String)"������") : ( IntToStr((__int64) tube_num) );
		stext1 = "������ (�): ����� " + s;
		stext2 = "������ (�): ��� �� �������������� " + IntToStr(plantCut);
		ProtocolForm->SendToProtocol(stext1);
		ProtocolForm->SendToProtocol(stext2);
		if (plantCut > 0)
			stext1 += ", ��� ������ " + IntToStr(plantCut);
		Synchronize( &UpdateStatus );
	}

// �������� ��������� ����, ��������� �������
	digital->SetOutSignal("�������� ������");
	digital->SetOutSignal("�������� �������");
	TThread::CurrentThread->Sleep(500);
	if ( lpop->isOkTemp("MG") )
		return "��� ���������� ����!";

// ���������, ������� �������� ������������
	ProtocolForm->SendToProtocol("������ (�): ���������� " + IntToStr(Globals::current_diameter));
	respop->sensors = (Globals::current_diameter == 60) ? (10) : (12);
	resprod->sensors = Globals::LinSensors;
	//LCard �����!!!
	lpop->InitADC(16 , 0.0 , "PP" , 0);
	lpop->Start();
	TThread::CurrentThread->Sleep(50);
	lpop->Stop();

// ��������������� ���� ������ � LCard
	ProtocolForm->SendToProtocol("������ (�): �������������� ���� ������ � LCard");
	lpop->InitADC(16 , 0.0 , "PP" , 2);			// Mode: 0-�������������, 1 - ��, 2 - �Ѩ
	if ( Linear )
		lprod->InitADC(resprod->sensors,0.0, "PR", 0);

// ���� ������� ���������� �� �����������
	digital->SetOutSignal("������");
	if ( Thick )
	{
		stext2="���� ���������� �����������";
		Synchronize( &UpdateStatus );           // ������������!!!
		if ( !digital->WaitForDropInSignal("����������",30000, TThread::CurrentThread))
			return "�� ��������� ���������� �����������";
	}

// ������ ����������
	if ( Linear )
	{
		digital->SetOutSignal("���� ��� ��");
		digital->SetOutSignal("���� STF");
		for (UINT s = 0; s < Globals::speeds.inSpeed.size(); s++)
			digital->SetOutSignal("���� �������� " + IntToStr(Globals::speeds.inSpeed[s]));
		if ( !digital->WaitForDropInSignal("���� �� RUN", 3000 , TThread::CurrentThread) )
			return "�� ���������� �������� �������� ����������� ������!";
	}

// ���� �������� �����������
	digital->SetOutSignal("����");
	if ( Thick )
	{
		stext2="���� �������� �����������";
		Synchronize( &UpdateStatus );		// ������������ !!!
		if ( !digital->WaitForDropInSignal("��������",10000, TThread::CurrentThread))
			return "�� ��������� �������� �����������";
		lat->SetControl();
	}

// ���������������� �������� �����������������
	digital->BlinkOutSignal("���������������" , 50);

// �������� �������� ����� ������
	digital->SetOutSignal("��: ��� ��");
	for (UINT s = 0; s < Globals::speeds.workSpeed.size(); s++)
		digital->SetOutSignal("��: �������� " + IntToStr(Globals::speeds.workSpeed[s]));
	digital->SetOutSignal("��: STF");
	if ( !digital->WaitForDropInSignal("��: �� RUN",3000, TThread::CurrentThread) )
		return "�� ���������� �������� �������� ����������";

// ���������� ������� � ���, ����� �������
	stext2 = "����� ���� ������";
	ProtocolForm->SendToProtocol("������ (�): " + stext2);
	Synchronize( &UpdateStatus );
	digital->SetOutSignal("��� ������");
	if (digital->SignalExists("��� ��������"))
		digital->SetOutSignal("��� ��������");

	return "ok";
}

//-------------------------------------------------------------------------------
//----������ ����, ���������� ���������� � ����������� ��� �������---------------
bool ThreadOnLine::OnlineCycle()
{
	// ����� ������� �� ����� ��������� �����
	bool LinWorkSpeed = false;		// ����� �� ������� ��������
	bool LinOutSpeed = false;		// ����� �� �������� ��������
	bool InSG = false;				// ����� ����� � ���������������
	bool EndCross = false;			// ����� ����� �� �����������
	bool ThickFinished = false;		// ����� ����� �� �����������
	bool TubeQuit = false;			// ����� ����� � 4 ������
	bool result;					// ��������� ����� �����
	bool Poper = false;				// ������� ����� ���� (���������)
	bool Prod = false;				// ������� ���� ���� (���������)
	bool Tolsh = false;				// ������� ���� ���� (������ ������)
	DWORD C1Start = 0;				// ����� ������� �� ����� �������� 1
	int plantMark = -10;			// ���� ���� �� �������������� � ��������� �� ���� �����
	int trash=16-( (Globals::current_diameter == 60) ? (10) : (12) );
	short ppChannel = 16;			//���-�� ������� ��
	short prChannel = Globals::LinSensors;	//���-�� ������� ���
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	unsigned markTime = ini->ReadInteger( "LIR" , "MarkerTime" , 100 );		// ����� ��������������
	bool mark = ! (ini->ReadBool( "LIR" , "NoMarking" , 0));		 	// �� �����������!
	delete ini;
	PoperStart =  200000;				// ������ ����� �������, ����� �� �������� �� �����
	PoperFinish = 200000;				// ������ ����� �������, ����� ������� ppOnline
	ProdStart =   200000;               // ������ ����� �������, ����� �� �������� �� �����
	ProdFinish =  200000;               // ������ ����� �������, ����� ������� prOnline
	TolshStart =  200000;               // ������ ����� �������, ����� �� �������� ������ ������ �������
	TolshFinish = 200000;               // ������ ����� �������, ����� ������� thOnline
	MarkStart =   200000;				// ������ ����� �������, ����� �� ����������� ������ �������
	MarkFinish =  200000;               // ������ ����� �������, ����� ������� markOnline
	bool poperLast = false;				// ���������� ��������� (��������) ���� �� �����������
	bool prodLast = false;				// ���������� ��������� (��������) ���� �� �����������
	bool tolshLast = false;				// ���������� ��������� (��������) ���� �� �����������
	long startScan = 0;					// ������ ����� (�������� ����� �����)
	long endScan = 0;                   // ����� �����
	long scanTime = 0;                  // ����� �����
	long scans = 0;                     // ����� ������ �� ����
	long biggestScan = 0;				// ����� ������ ����
	St2in = 0;
	St4in = 0;
	St1out = 0;
	St3out = 0;
	St4out = 0;
	GPout = 0;
	DWORD syncSetTime = 0;				// ����� ����������� �������� ������� ������
	bool syncSet = false;				// ������ ������ ���������
	DWORD markSetTime = 0;				// ����� ����������� �������� ������� ���������������
	bool markSet = false;				// ������ ��������������� ���������
	int drebezgGP = 0;					// ��������� ������ �� �����, ����
	ProtocolForm->SendToProtocol("������� � �������� ����");

	while(Collect)
	{
        startScan = GetTickCount();
		CalcOnlineZones();
// ����, ����� ���������� ���� ������
		if ( ppOnline == 0 && !lpop->getStarted() && digital->CheckInSignal("����� ����") && Poper)
		{
			lpop->Start();
			ProtocolForm->SendToProtocol("������ (�): ����� ����� �����������");
		}
		if ( prOnline == 0 && !lprod->getStarted() && digital->CheckInSignal("���� ����") && Linear && Prod)
		{
			digital->SetOutSignal("���� ��� ��");
			lprod->Start();
			ProtocolForm->SendToProtocol("������ (�): ����� ����� �����������");
		}
//		TThread::CurrentThread->Sleep(1);

//**************************************************************************
//**************����������� ���������� �����********************************
		if( ppOnline > ppOld && (digital->CheckInSignal("����� ����") || digital->CheckInSignal("����� �����")) )
		{
			ppOld = ppOnline;
			int Part = lpop->part;                             //������� ��������� � ����� �����
			ppNumber = lpop->GetNum();					    	 //���-�� ��������� ������
			ppSecond = ppNumber * Part;                          //����� �������� ������ �� ������ ������
			meas_per_sensor = (ppSecond-ppFirst)/ppChannel;      //���-�� ��������� � ���� �� ������ �������
			memcpy(&Buffer[0],&lpop->Base[ppFirst],sizeof(ULONG)*(ppSecond-ppFirst));   //��������  �� ������ LCard � ��� ������������� �����
			int p=0;
			ppData.set_length(ppChannel-trash);                                       //���-�� �������� �������������
			for(int i=0; i < ppData.Length; i++)
				ppData[i].set_length(meas_per_sensor);

			for( UINT i=0; i < ppSecond - ppFirst; i+=ppChannel )         //����� �������
			{
				for(int j=0; j < ppChannel-trash; j++)                    //������� �� �������������
				{
					if (p >= ppData[j].Length)
						break;

					ppData[ j ][ p ] = short ( Buffer[i+j] ) ;
				}

				p++;
			}
			Synchronize(DrawResultPP);
			ProtocolForm->SendToProtocol("������ (�): ���� =�����=" + IntToStr(ppOnline) );
			ppFirst = ppSecond;
			CalcOnlineZones();
		 }
//**************��������� ���� ����������� (��������)********************************
		 if (TotalCounter >= PoperFinish && !poperLast)
		 {
			poperLast = true;
			int Part = lpop->part;                             //������� ��������� � ����� �����
			ppNumber = lpop->GetNum();					    	 //���-�� ��������� ������
			ppSecond = ppNumber * Part;                          //����� �������� ������ �� ������ ������
			memcpy(&Buffer[0],&lpop->Base[ppFirst],sizeof(ULONG)*(ppSecond-ppFirst));   //��������  �� ������ LCard � ��� ������������� �����
			ppData.set_length(ppChannel-trash);                                       //���-�� �������� �������������
			meas_per_sensor = respop->total_meas / respop->zones;	// ����� ��������� ������� ��� ������� �� ���� �����
			meas_per_sensor -= meas_per_sensor % ppChannel;			// ����� �������� �� 16
			unsigned meas = meas_per_sensor * ppChannel;			// ���� ������ ������, � ��� ����� ���������
			int good = (ppSecond-ppFirst)/ppChannel;    			// ����� ������� ���������
			int p = 0;
			for(int i=0; i < ppData.Length; i++)
				ppData[i].set_length(meas_per_sensor);

			for( UINT i = 0; i < meas; i += ppChannel )         	//  ppSecond - ppFirst
			{                                                       // ����� ������� = ppChannel
				for(int j = 0; j < ppChannel-trash; j++)            //������� �� �������������
				{
					if (p >= good )						// ���� ��������� ��� ���������� ��������
						ppData[ j ][ p ] = 0 ;
					else
						ppData[ j ][ p ] = short ( Buffer[i+j] ) ;
				}
				p++;
			}
			Synchronize(DrawResultPP);
			ProtocolForm->SendToProtocol("������ (�): ��������� ���� =�����=" );
		 }
//		TThread::CurrentThread->Sleep(1);

//******************************************************************************
//*****************����������� ���������� �����*********************************
		if( prOnline == (prOld + 1) && (digital->CheckInSignal("���� ����") || digital->CheckInSignal("���� �����")) )
		{
			prOld = prOnline;
			int prPart = lprod->part;                          //���-�� �������� � ����� �����
			prNumber = lprod->GetNum();                        //������� ������� ������ �� ������ ������
			prSecond = prNumber*prPart;                       //����� �������� ������ �� ������ ������
			meas_per_sensor=(prSecond-prFirst)/prChannel;	//���-�� ��������� � ���� �� ������ �������
			//��������  �� ������ LCard � ��� ������������� �����
			memcpy(&Buffer[0],&lprod->Base[prFirst],sizeof(ULONG)*(prSecond-prFirst));
			int p=0;
			prData.set_length(prChannel);
			for(int i=0; i < prData.Length; i++)                                //���-�� �������� �������������
				prData[i].set_length(meas_per_sensor);

			for(UINT i=0; i < prSecond-prFirst; i+=prChannel)                   //����� �������
			{
				for(int j=0; j<prChannel; j++)                                  //������� �� �������������
				{
					if(p >= prData[j].Length)
						break;
					prData[ j ][ p ] = short(Buffer[i+j] );
				}
				p++;
			}
			//����� ��������� � ������� ����������
			Synchronize(DrawResultPR);
			ProtocolForm->SendToProtocol("������ (�): ���� =����="  + IntToStr(prOnline) );
			//���������� ���-�� ��������, ��� ������ ��������� ����
			prFirst =prSecond;
			CalcOnlineZones();
		}
//*****************��������� ���� ����������� (��������)*********************************
		if (TotalCounter >= ProdFinish && !prodLast)
		{
        	prodLast = true;
			int prPart = lprod->part;                          //���-�� �������� � ����� �����
			prNumber = lprod->GetNum();                        //������� ������� ������ �� ������ ������
			prSecond = prNumber*prPart;                       //����� �������� ������ �� ������ ������
			memcpy(&Buffer[0],&lprod->Base[prFirst],sizeof(ULONG)*(prSecond-prFirst));
			int p=0;
			prData.set_length(prChannel);
			meas_per_sensor = resprod->total_meas / resprod->zones;	// ����� ��������� ������� ��� ������� �� ���� �����
			meas_per_sensor -= meas_per_sensor % prChannel;			// ����� �������� �� 8
			unsigned meas = meas_per_sensor * prChannel;			// ���� ������ ������, � ��� ����� ���������
			int good = (prSecond-prFirst)/prChannel;	//���-�� ��������� � ���� �� ������ �������

			for(int i=0; i < prData.Length; i++)                                //���-�� �������� �������������
				prData[i].set_length(meas_per_sensor);

			for(UINT i = 0; i < meas; i += prChannel)                   //����� �������
			{
				for(int j=0; j < prChannel; j++)                                  //������� �� �������������
				{
					if(p >= good)
						prData[ j ][ p ] = 0;
					else
						prData[ j ][ p ] = short(Buffer[i+j] );
				}
				p++;
			}
			Synchronize(DrawResultPR);
			ProtocolForm->SendToProtocol("������ (�): ��������� ���� =����=");
		}
//		TThread::CurrentThread->Sleep(1);

//******************************************************************************
//***** ������ ������ ������ � ����������, ���� ���������� �� ����**************
		if ( thOnline > thOld  && (digital->CheckInSignal("���� ����") || digital->CheckInSignal("���� �����")) )
		{
			thOld = thOnline;
			digital->SetOutSignal("������");
			syncSetTime = GetTickCount();
			syncSet = true;
			ProtocolForm->SendToProtocol("������ (�): ���� =����= " + IntToStr(thOnline));
			CalcOnlineZones();
		}
		if (TotalCounter >= TolshFinish && !tolshLast)
		{
			tolshLast = true;
			digital->SetOutSignal("������");
			syncSetTime = GetTickCount();
			syncSet = true;
			ProtocolForm->SendToProtocol("������ (�): ��������� ���� =����= ");
		}
		DWORD sync = GetTickCount();
		if ( (sync - syncSetTime > SynchroTime) && syncSet)
		{
			digital->ResetOutSignal("������");
			syncSet = false;
		}

//******************************************************************************
//***** �������������� ����������� ��������*************************************
//		CalcOnlineZones();
		if ( (markOnline == markOld + 1) && (digital->CheckInSignal("�� �����") || digital->CheckInSignal("���� ����")))
		{
			markOld = markOnline;
			if ( ressum->zone_data[markOnline] == 0 && markOnline < ressum->zones && mark )
			{
				digital->SetOutSignal("���������������" );
				markSetTime = GetTickCount();
				markSet = true;
				ProtocolForm->SendToProtocol("�������������� ���� " + IntToStr(markOnline));
			}
			else if (markOnline == plantMark && plantCut > 0)
			{
				digital->BlinkOutSignal("���������������" , 3*markTime);
				ProtocolForm->SendToProtocol("������������� ��� ������� " + IntToStr(markOnline));
			}
		}
		DWORD markLast = GetTickCount();
		if ( (markLast - markSetTime > markTime ) && markSet )
		{
			digital->ResetOutSignal("���������������");
			markSet = false;
		}
		TThread::CurrentThread->Sleep(1);

// ����� ����� � �����, �������� ������ ���� - ������� ����� �������
		if ( !Poper && digital->CheckInSignal("����� ����")  )
		{
			PoperStart = TotalCounter;
			Poper = true;
			ProtocolForm->SendToProtocol("������� ����� ����");
		}

// ����� ����� �� ����� � �����, �������� ������ ����-������� ����� ������
		if ( Poper && current_counter >=1 && !digital->CheckInSignal("����� ����") )
		{
			PoperFinish = TotalCounter + ppOffset[current_counter];
			Poper = false;
			ProtocolForm->SendToProtocol("��������� ����� ����");
		}

// ����� ����� � ����, �������� ������ ���� - ������� ����� �������
		if ( !Prod && digital->CheckInSignal("���� ����")  )
		{
			ProdStart = TotalCounter;
			Prod = true;
			ProtocolForm->SendToProtocol("������� ���� ����");
			digital->SetOutSignal("���� ��� ��");
		}

// ����� ����� �� ����� � ����, �������� ������ ����-������� ����� ������
		if ( Prod && current_counter >=1 && !digital->CheckInSignal("���� ����") )
		{
			ProdFinish = TotalCounter + prOffset[current_counter];
			Prod = false;
			ProtocolForm->SendToProtocol("��������� ���� ����");
		}

// ����� ����� � ����, �������� ������ ���� - ������� ����� �������
		if ( !Tolsh && digital->CheckInSignal("���� ����" ) )
		{
			TolshStart = TotalCounter;
			Tolsh = true;
			ProtocolForm->SendToProtocol("������� ���� ����");
		}

// ����� ����� �� ����� � ����, �������� ������ ����-������� ����� ������
		if ( Tolsh && current_counter >=1 && !digital->CheckInSignal("���� ����") )
		{
			TolshFinish = TotalCounter + thOffset[current_counter];
			Tolsh = false;
			ProtocolForm->SendToProtocol("��������� ���� ����");
		}

// ������� ������� ����� � ��
// ����� ����� � �������������, �������� ������ ���� - ������� ����� �������
		if ( !InSG && digital->CheckInSignal("�� �����") )
		{
			drebezgGP++;
		}
		else
			drebezgGP = 0;

		if (drebezgGP == 5)
		{
			MarkStart = TotalCounter;
			lpop->SetMeasInSG();
			respop->CalculateDeadFront(200);
			resprod->CalculateDeadFront(200);
		  //	ressum->ComputeZonesData(*respop,*resprod,*resth);
			InSG = true;
			ProtocolForm->SendToProtocol("������� �� ����� (��� �������������)");
//			ProtocolForm->SendToProtocol("����� ����� � ���������������");
		}

// ����� ����� �� ��������������, �������� ������ ����-������� ����� ������
		if ( InSG && current_counter == 2 && !digital->CheckInSignal("�� �����") )
		{
				// ������� ��� ������� - ��� ������� �� 1 �����
			MarkFinish = TotalCounter + markOffset[current_counter] + 3*imp_per_zone[current_counter];
			InSG = false;
			GPout = Counter->GetCounter(2);
			ProtocolForm->SendToProtocol("��������� �� ����� (��� ��������������)");
		}

// ��������� ����� ����� � ������� ������� ��� ����, ������������ ��� ��� �� �������
		if ( plantCut > 0 && plantMark < 0 && !digital->CheckInSignal("��� ����") )
		{
			double tube_len = ( 6330.0 / 200.0 ) + (float)Counter->GetCounter(0) / (float)imp_per_zone[0];
			int tube_length_zone = (int)(tube_len);
			plantMark = tube_length_zone - plantCut;
			ProtocolForm->SendToProtocol("���� ���� �� �������: " + IntToStr(plantMark));
			ProtocolForm->SendToProtocol("����� ����� � �����: " + IntToStr(tube_length_zone));
			if (plantMark < 0)		// �����
				plantMark = 100;
		}

// �������, ��� ������ �� ����� �������������� ������ �������� �� �����������
		if ( !ThickFinished && current_counter >= 1 && !digital->CheckInSignal("���� �����") )
		{
			lat->ResetControl();
			ProtocolForm->SendToProtocol("����� ����� �� �����������");
			ThickFinished = true;
		}

// �������, ��� ����� ����� �� �����������
/*		if ( !EndCross && current_counter >= 1 && !digital->CheckInSignal("����� �����") )
		{
			MeasOutCrossModule = lpop->Num;  //������� ������ ���� �������� �� ������ ������ �� ����� ������
			digital->ResetOutSignal("�������� �������");
			ProtocolForm->SendToProtocol("����� ����� �� �����������");
			EndCross = true;
		}
*/
// �������, ��� ���� ������ ������� �������� �������� � �����������
		if ( !LinWorkSpeed && digital->CheckInSignal("���� �����") )
		{
			digital->ResetOutSignal("���� �������� 1");
			digital->ResetOutSignal("���� �������� 2");
			digital->ResetOutSignal("���� �������� 3");
			for (UINT s = 0; s < Globals::speeds.linearSpeed.size(); s++)
				digital->SetOutSignal("���� �������� " + IntToStr(Globals::speeds.linearSpeed[s]));
			ProtocolForm->SendToProtocol("������� �������� �������� �����������");
			LinWorkSpeed = true;
		}

// �������, ��� ���� ������ �������� �������� �������� � �����������
		if ( !LinOutSpeed && Linear && current_counter >= 1 && !digital->CheckInSignal("������ 3") )
		{
			St3out = Counter->GetCounter(1);
			digital->ResetOutSignal("���� �������� 1");
			digital->ResetOutSignal("���� �������� 2");
			digital->ResetOutSignal("���� �������� 3");
			for (UINT s = 0; s < Globals::speeds.outSpeed.size(); s++)
				digital->SetOutSignal("���� �������� " + IntToStr(Globals::speeds.outSpeed[s]));
			ProtocolForm->SendToProtocol("�������� �������� �������� �����������");
			LinOutSpeed = true;
		}

// �������, ��� ���� ���������� ������� �� 1
		if ( C1Start == 0 && current_counter == 0 && digital->CheckInSignal("������ 4")  )
		{
			C1Start = GetTickCount();
			digital->SetOutSignal("����");
			St4in = Counter->GetCounter(0);
			ProtocolForm->SendToProtocol("������ ������ �������� 1");
		}
		DWORD now = GetTickCount();
		if ( current_counter == 0 && ((now - C1Start) > Counter1Delay) && digital->CheckInSignal("������ 4") )
		{
			Counter0Fin = Counter->GetCounter(0);
			Counter->Reset(1);
			current_counter = 1;
//			CalcOnlineZones();
			ProtocolForm->SendToProtocol("������� 1 �������");
		}

// �������, ��� ���� ���������� ������� �� 2
		if ( current_counter == 1 && !digital->CheckInSignal("������ 3") && digital->CheckInSignal("���� ����"))
		{
			Counter1Fin = Counter->GetCounter(1);
			Counter->Reset(2);
			current_counter = 2;
//			CalcOnlineZones();
			ProtocolForm->SendToProtocol("������� 2 �������");
		}

// ��������� �����
		if ( current_counter == 2 && !digital->CheckInSignal("�� �����") && lpop->getStarted() )
		{
			lpop->Stop();
			ProtocolForm->SendToProtocol("������ (�): ���� ����� �����������");
		}
		if ( current_counter == 2 && !digital->CheckInSignal("���� ����") && !digital->CheckInSignal("���� �����") && lprod->getStarted() )
		{
			lprod->Stop();
			digital->ResetOutSignal("���� ��� ��");
			ProtocolForm->SendToProtocol("������ (�): ���� ����� �����������");
		}

// �������, ��� ����� ����� �� ��������� - �������� ������ �� � ����������
		if ( !TubeQuit && current_counter == 2 && !digital->CheckInSignal("������ 4") )
		{
			St4out = Counter->GetCounter(2);
			digital->ResetOutSignal("�������� ������");
			digital->ResetOutSignal("�������� �������");
			digital->ResetOutSignal("��: STF");
			digital->ResetOutSignal("���� STF");
			digital->ResetOutSignal("���� ��� ��");
//			digital->ResetOutSignal("������");
			digital->ResetOutSignal("����");
			ProtocolForm->SendToProtocol("����� ����� �� ������ 4");
			respop->CalculateDeadBack(200);
			resprod->CalculateDeadBack(200);
		 	ressum->ComputeZonesData(*respop,*resprod,*resth, true, true);
			Synchronize(RedrawGraphics);
			TubeQuit = true;
		}

// ���������� ����� - ���� ������ 2 � ���������� ������ 1
		if ( St2in == 0 && digital->CheckInSignal("������ 2") )
		{
        	St2in = Counter->GetCounter(0);
		}
		if( St1out == 0 && current_counter == 1 && !digital->CheckInSignal("������ 1") )
		{
        	St1out = Counter->GetCounter(1);
		}

// ������� ��������� �����, ��� ������ �������
		if ( current_counter == 2 && !digital->CheckInSignal("���� ����") )
		{
			Collect = false;
			result = true;
			ProtocolForm->SendToProtocol("������ (�): ��������� ����");
		}

// �������, �� ���� �� ������
		if (Terminated)
		{
			Collect = false;
			result = false;
			ProtocolForm->SendToProtocol("������ (�): ����� � Terminated");
		}
//		Synchronize(OnlineZones);	// ���������, ��� ���
		endScan = GetTickCount();
		scanTime += (endScan - startScan);
		if (endScan - startScan > biggestScan)
			biggestScan = endScan - startScan;
		scans++;
	}
	double average = (double)scanTime / (double) scans;
	ProtocolForm->SendToProtocol("������� ����� �����: " + FloatToStrF(average , ffFixed , 3,2));
	ProtocolForm->SendToProtocol("���������� ����� �����: " + FloatToStrF(biggestScan , ffFixed , 5,4));
	if ( result )
		Calibrate();
	return result;
}
//---------------------------------------------------------------------------
void ThreadOnLine::Finally()
{
	if ( !Terminated )		// ���� ������ �� ����� ����, � �� �� ������ ��� ������� �����
	{
		lat->ResetPlantCycle();
		lat->ResetControl();
		lat->ResetStand();
	}
	// ������� ����������� �������
	digital->ResetOutSignal("�������� ������");
	digital->ResetOutSignal("�������� �������");
	digital->ResetOutSignal("��: �������� 1");
	digital->ResetOutSignal("��: �������� 2");
	digital->ResetOutSignal("��: �������� 3");
	digital->ResetOutSignal("��: STF");
//	digital->ResetOutSignal("������ ����");
//	digital->ResetOutSignal("����� ����");
	digital->ResetOutSignal("������");
	digital->ResetOutSignal("����");
	digital->ResetOutSignal("����");

	digital->ResetOutSignal("��� ������");
	if (digital->SignalExists("��� ��������"))
		digital->ResetOutSignal("��� ��������");

	if ( Linear )
	{
		digital->ResetOutSignal("���� STF");
		digital->ResetOutSignal("���� �������� 1");
		digital->ResetOutSignal("���� �������� 2");
		digital->ResetOutSignal("���� �������� 3");
		digital->ResetOutSignal("���� ��� ��");
	}
	if (Buffer != NULL)
		delete Buffer;
	lpop->Stop();
	lprod->Stop();
}
//---------------------------------------------------------------------------
void __fastcall ThreadOnLine::OnlineZones()
{
	MainForm->Memo1->Lines->Clear();
	long CounterValue = Counter->GetCounter(0);
	long Counter2 = Counter->GetCounter(1);
	long Counter3 = Counter->GetCounter(2);

//	MainForm->Memo1->Lines->Add("������ �������: " + IntToStr((__int64)thOffset[current_counter]));
//	MainForm->Memo1->Lines->Add("������ ������: " + IntToStr((__int64)ppOffset[current_counter]));
//	MainForm->Memo1->Lines->Add("������ �����: " + IntToStr((__int64)prOffset[current_counter]));
//	MainForm->Memo1->Lines->Add("������ ������: " + IntToStr((__int64)markOffset[current_counter]));
//	MainForm->Memo1->Lines->Add("��������� � ����: " + IntToStr((__int64)imp_per_zone[current_counter]));
//	MainForm->Memo1->Lines->Add("�������1: " + IntToStr((__int64)CounterValue));
//	MainForm->Memo1->Lines->Add("�������2: " + IntToStr((__int64)Counter2));
//	MainForm->Memo1->Lines->Add("�������3: " + IntToStr((__int64)Counter3));
	MainForm->Memo1->Lines->Add("���� �������: " + IntToStr(thOnline));
	MainForm->Memo1->Lines->Add("���� ������: " + IntToStr(ppOnline));
	MainForm->Memo1->Lines->Add("���� �����: " + IntToStr(prOnline));
	MainForm->Memo1->Lines->Add("���� ������: " + IntToStr(markOnline));
	MainForm->Memo1->Lines->Add("������� : " + IntToStr(current_counter));
	MainForm->Memo1->Lines->Add("TotalCounter : " + IntToStr((__int64)TotalCounter));
	MainForm->Memo1->Lines->Add("******************* " );
	MainForm->Memo1->Lines->Add("TolshStart : " + IntToStr((__int64)TolshStart));
	MainForm->Memo1->Lines->Add("TolshFinish : " + IntToStr((__int64)TolshFinish));
	MainForm->Memo1->Lines->Add("PoperStart : " + IntToStr((__int64)PoperStart));
	MainForm->Memo1->Lines->Add("PoperFinish : " + IntToStr((__int64)PoperFinish));
	MainForm->Memo1->Lines->Add("ProdStart : " + IntToStr((__int64)ProdStart));
	MainForm->Memo1->Lines->Add("ProdFinish : " + IntToStr((__int64)ProdFinish));
	MainForm->Memo1->Lines->Add("MarkStart : " + IntToStr((__int64)MarkStart));
	MainForm->Memo1->Lines->Add("MarkFinish : " + IntToStr((__int64)MarkFinish));

	MainForm->Memo1->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall ThreadOnLine::UpdateStatus()
{
	StB->Panels->Items[1]->Text = stext1;
	StB->Panels->Items[2]->Text = stext2;
	StB->Refresh();

	Application->ProcessMessages();
	SignalsStateForm->Refresh();
}
//-----------------------------------------------------------------------------

void __fastcall ThreadOnLine::DrawResultPP()
{
	//������ � ����� ������ ��
	respop->AddZone(ppData);
	respop->PutResultOnChart(MainForm->CrossDefectChart, MainForm);
	if ( !Linear && !Thick )
	{
		ressum->AddZone(*respop, *resprod, *resth);
		ressum->ComputeZonesData(*respop , *resprod , *resth, true, true);
		ressum->PutResultOnChart(MainForm->SummaryChart, MainForm);
	}
}
//---------------------------------------------------------------------------
void __fastcall ThreadOnLine::DrawResultPR()
{
	//������ � ����� ������ ���
	resprod->AddZone(prData);
	resprod->PutResultOnChart(MainForm->LinearDefectChart, MainForm);
	// ������ ������ � ����� ���������� ����������
	if (Linear && !Thick)
	{
		ressum->AddZone(*respop, *resprod, *resth);
		ressum->PutResultOnChart(MainForm->SummaryChart, MainForm);
	}
}

//---------------------------------------------------------------------------
void __fastcall ThreadOnLine::RedrawGraphics()
{
	respop->PutResultOnChart(MainForm->CrossDefectChart , MainForm);
	resprod->PutResultOnChart(MainForm->LinearDefectChart , MainForm);
	ressum->PutResultOnChart(MainForm->SummaryChart, MainForm);
}
//---------------------------------------------------------------------------
void ThreadOnLine::CalcOnlineZones()
{
	double ppO = 0.0;		// ��������� ���������� ��� ������� ������ ����
	double prO = 0.0;
	double thO = 0.0;
	double markO = 0.0;
	// � ����-���, � ������ �������� ������� ����� ����!!!
	TotalCounter = Counter->GetCounter(current_counter);
	if ( current_counter > 0 )
		TotalCounter += Counter0Fin;
	if ( current_counter > 1 )
		TotalCounter += Counter1Fin;

	if ( TotalCounter - PoperFinish < 0 )	// imp_per_zone[current_counter]
		ppO = (double)( TotalCounter - PoperStart - ppOffset[0] ) / (double)imp_per_zone[0];
	if ( TotalCounter - ProdFinish < 0 )	// imp_per_zone[current_counter]
		prO = (double)( TotalCounter - ProdStart - prOffset[0] ) / (double)imp_per_zone[0];
	if ( TotalCounter - TolshFinish < 0 )	// imp_per_zone[current_counter]
		thO = (double)( TotalCounter - TolshStart - thOffset[0] ) / (double)imp_per_zone[0];
	if ( TotalCounter - MarkFinish < imp_per_zone[current_counter] )
		markO = (double)( TotalCounter - MarkStart - markOffset[current_counter] )
				 / (double)imp_per_zone[current_counter];

	if (current_counter == 2)
	{
		long Counter2 = Counter->GetCounter(current_counter);
		if ( TotalCounter - ProdFinish < 0 )		// imp_per_zone[current_counter]
			prO = (double)Counter2 / (double)imp_per_zone[current_counter] +
			(double)(Counter0Fin + Counter1Fin - ProdStart - prOffset[0]) / (double)imp_per_zone[0];
		if ( TotalCounter - MarkFinish < imp_per_zone[current_counter] )
			markO = (double)Counter2 / (double)imp_per_zone[current_counter] +
			(double)(Counter0Fin + Counter1Fin - MarkStart - markOffset[0]) / (double)imp_per_zone[0];
	}
	ppOnline = Math::Floor(ppO);
	prOnline = Math::Floor(prO);
	thOnline = Math::Floor(thO);
	markOnline = Math::Floor(markO);
}
//---------------------------------------------------------------------------
void ThreadOnLine::CalcOffsets()
{
	TIniFile *ini = new TIniFile(Globals::IniFileName);

	double impulsePerRound = ini->ReadInteger("LIR","ImpulsePerRound", 50 );
	double zone_len = ini->ReadInteger("Size","ZoneLength",250);
	double ppOffset_mm = ini->ReadInteger("LIR","LirToCross",250);
	double prOffset_mm = ini->ReadInteger("LIR","LirToLinear",250);
	double thOffset_mm = ini->ReadInteger("LIR","LirToThickness",250);
	double markOffset_mm = ini->ReadInteger("LIR","LirToMarker",250);

	for (int i=0; i < Globals::Counters; i++)
	{
		perimeters[i] = ini->ReadFloat("LIR","Perimeter_"+IntToStr(i), 100 );
		imp_per_zone[i] = (long) ( impulsePerRound * zone_len / perimeters[i] );
		ppOffset[i] = 	  (long) ( ppOffset_mm * impulsePerRound / perimeters[i] );
		prOffset[i] = 	  (long) ( prOffset_mm * impulsePerRound / perimeters[i] );
		thOffset[i] = 	  (long) ( thOffset_mm * impulsePerRound / perimeters[i] );
		markOffset[i] =   (long) ( markOffset_mm * impulsePerRound / perimeters[i] );
	}
	delete ini;
}
//---------------------------------------------------------------------------
void ThreadOnLine::Calibrate()
{
	long lir1 = St4in - St2in;
	long lir2 = St3out - St1out;
	long lir3 = GPout - St4out;

	TIniFile *ini = new TIniFile(Globals::IniFileName);
	long St4St2_mm = ini->ReadInteger("Size" , "St4_St2" , 500);
	long St3St1_mm = ini->ReadInteger("Size" , "St3_St1" , 500);
	long GPSt4_mm = ini->ReadInteger("Size" , "GP_St4" , 500);

	double lir1_len = (double) St4St2_mm / ( (double) lir1 / 1000.0);
	double lir2_len = (double) St3St1_mm / ( (double) lir2 / 1000.0);
	double lir3_len = (double) GPSt4_mm /  ( (double) lir3 / 1000.0);
	ProtocolForm->SendToProtocol("����������: ���1 - " + FloatToStrF(lir1_len , ffFixed , 6,3)+
									 " ���2 - " + FloatToStrF(lir2_len , ffFixed , 6, 3)+
									 " ���3 - " + FloatToStrF(lir3_len , ffFixed , 6, 3)	);

	bool cal = ini->ReadBool("LIR" , "IsCalibrate" , 0 );
	if (cal)
	{
		if ( lir1_len > 175 && lir1_len < 195 )
			ini->WriteFloat("LIR","Perimeter_0" , Math::RoundTo(lir1_len , -2) );
		if ( lir2_len > 175 && lir2_len < 195 )
			ini->WriteFloat("LIR","Perimeter_1" , Math::RoundTo(lir2_len , -2) );
		if ( lir3_len > 315 && lir3_len < 330 )
			ini->WriteFloat("LIR","Perimeter_2" , Math::RoundTo(lir3_len , -2) );
	}
	delete ini;
}
//---------------------------------------------------------------------------

