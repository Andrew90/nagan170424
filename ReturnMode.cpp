//---------------------------------------------------------------------------


#pragma hdrstop

#include "ReturnMode.h"
#include "SignalsState.h"
#include "DebugMess.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall ReturnThread::ReturnThread(bool CreateSuspended , TStatusBar *St, AlarmThreadClass &_alth)
				: TThread(CreateSuspended)
{
	StatusBar = St;
}
//---------------------------------------------------------------------------
void __fastcall ReturnThread::UpdateMainForm()
{
	StatusBar->Panels->Items[1]->Text = stext1;
	StatusBar->Panels->Items[2]->Text = stext2;
	StatusBar->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall ReturnThread::Execute()
{
	NameThreadForDebugging("ReturnThread");

	stext1 = "����� \"�������\"";
	stext2 = "�������� ������";
	Synchronize( &UpdateMainForm );
	cool = ReturnMode();
	Terminate();
	return;
}
//---------------------------------------------------------------------------
bool ReturnThread::ReturnMode()
{
	bool result=true;
	String ModuleResult;

	while ( result )
	{
	  dprint("%s %d\n", __FUNCTION__, 0);
// ���������, �� �������� �� ����� � �������
		if ( digital->CheckInSignal("���� ����") ||
				digital->CheckInSignal("���� �����") ||
				digital->CheckInSignal("���� ����")  ||
				digital->CheckInSignal("���� �����") ||
				digital->CheckInSignal("����� ����") ||
				digital->CheckInSignal("����� �����") )
		{
			stext2="����� � �������!";
			result=false;
			break;
		}
        digital->ResetOutSignal("������ �������");
		digital->ResetOutSignal("������ ����");
		dprint("%s %d\n", __FUNCTION__, 1);
// ���������, ���� �� ����� � ����� ����� (��2)
/*		if ( !digital->CheckInBouncelessSignal("��� ��2",true) )
		{
			stext2="��� ����� � ����� �����!";
			result=false;
			break;
		}
*/
/*
// ���������, ����� �� �������� ���������� : ����������
		stext2="������� ���������� ������";
		Synchronize( &UpdateMainForm );
		ModuleResult = ModulePosition::SetModuleInService("�����");
		if ( ModuleResult != "ok")
		{
			stext2 = ModuleResult;
			result = false;
			break;
		}
// ���������, ����� �� �������� ���������� : �������
		stext2="������� ����������";
		Synchronize( &UpdateMainForm );
		ModuleResult = ModulePosition::SetModuleInService("����");
		if ( ModuleResult != "ok")
		{
			stext2 = ModuleResult;
			result = false;
			break;
		}
// ���������, ����� �� �������� ���������� : ����������
		stext2="������� ���������� ������";
		Synchronize( &UpdateMainForm );
		ModuleResult = ModulePosition::SetModuleInService("����");
		if ( ModuleResult != "ok")
		{
			stext2 = ModuleResult;
			result = false;
			break;
		}
*/

// ������� ����� ��� ������
		stext2="������� ��� ������ ";
		Synchronize( &UpdateMainForm );
		ModuleResult = ModulePosition::SetAllModulesInService("�����", "����", "����");
		if ( ModuleResult != "ok")
		{
			stext2 = ModuleResult;
			result = false;
			break;
		}
		dprint("%s %d\n", __FUNCTION__, 2);
// ������������� ����� ������ ����������� �����

		if ( !digital->WaitForDropInSignal("������ ������",30000, TThread::CurrentThread) )
		{
			stext2="������ ����������� �����!";
			result=false;
			break;
		}
		Sleep(1500);
		dprint("%s %d\n", __FUNCTION__, 3);
		digital->SetOutSignal("������ �������");
		stext2="���� ���������� �����";
		Synchronize( &UpdateMainForm );
  /*
		if ( !digital->WaitForInSignal("������ ������",30000, TThread::CurrentThread) )
		{
			stext2="������ ����������� �����!";
			result=false;
			break;
		}
		dprint("%s %d\n", __FUNCTION__, 4);
         */
// ������������� ����� ������ ����������� �������
		digital->ResetOutSignal("����� �������");
		digital->ResetOutSignal("����� ����");
		if ( !digital->WaitForDropInSignal("����� ������",30000, TThread::CurrentThread) )
		{
			stext2="������ ����������� �������!";
			result=false;
			break;
		}
		dprint("%s %d\n", __FUNCTION__, 5);
		digital->SetOutSignal("����� �������");
		stext2="���� ���������� �������";
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForInSignal("����� ������",30000, TThread::CurrentThread) )
		{
			stext2="������ ����������� �������!";
			result=false;
			break;
		}
		 dprint("%s %d\n", __FUNCTION__, 6);
// ������������� ������ �� ��������� ����������, ����� ���� �����
		stext2="����� ���� �����";
		Synchronize( &UpdateMainForm );

		digital->SetOutSignal("��: ��� ��");

			for (UINT s = 0; s < Globals::speeds.workSpeed.size(); s++)
		digital->SetOutSignal("��: �������� " + IntToStr(Globals::speeds.returnSpeed[s] ) );

		digital->SetOutSignal("��: STR");
		Sleep(1500);
		digital->SetOutSignal("��� �����");
		digital->SetOutSignal("��� ��������");

		stext2="���� ��������� ����� � ������ ����� (��1)";
		Synchronize( &UpdateMainForm );
		/*if ( digital->CheckInBouncelessSignal("��� ������" , true) )
		{   */
			stext2="���� ����� �� ������ 1";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForInSignal("������ 1",55000, TThread::CurrentThread) )
			{
				stext2="�� ��������� ����� �� ������ 1!";
				result=false;
				break;
			}
			dprint("%s %d\n", __FUNCTION__, 7);
			stext2="���� ����� ����� �� ������ 1";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForDropInSignal("������ 1",55000, TThread::CurrentThread) )
			{
				stext2="�� ��������� ����� ����� �� ������ 1!";
				result=false;
				break;
			}
			   dprint("%s %d\n", __FUNCTION__, 8);
	 /*	}
		else
		{
			while (true)
			{
				if ( digital->WaitForInSignal("��� ��1", 180000 , TThread::CurrentThread) )
				{
					if ( digital->CheckInBouncelessSignal("��� ��1",true) )
					{
						break;
					}
					else
						continue;
				}
				else
				{
					stext2 = "�� ��������� ����� � ������ �����!";
					result = false;
					break;
				}
			}
		}   */
// ���� ���� ������ 1 ���
		break;
	}

// ���������� ����������� �������
//	digital->ResetOutSignal("��: ��� ��");
	digital->ResetOutSignal("��: �������� 2");
	Sleep(1500);
	digital->ResetOutSignal("��: STR");

	digital->ResetOutSignal("������ �������");
	digital->ResetOutSignal("����� �������");
	digital->ResetOutSignal("��� �����");
	digital->ResetOutSignal("��� ��������");


	if (Terminated)
	{
    	return false;
	}
	dprint("%s %d\n", __FUNCTION__, 9);
	if ( result)
	{
		stext2="";
		stext1="����� ������� ������� ��������";
		ProtocolForm->SendToProtocol("�������: " + stext1);
		Synchronize( &UpdateMainForm );
		dprint("%s %d\n", __FUNCTION__, 10);
		return true;
	}
	else
	{
		stext1="����� ������� �� ��������";
		ProtocolForm->SendToProtocol("�����: " + stext1);
		Synchronize( &UpdateMainForm );
		dprint("%s %d\n", __FUNCTION__, 11);
		return false;
	}
	dprint("%s %d\n", __FUNCTION__, 12);
}




