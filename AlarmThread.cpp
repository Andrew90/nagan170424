//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AlarmThread.h"
#include "Main.h"
#include "DebugMess.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall AlarmThreadClass::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall AlarmThreadClass::AlarmThreadClass(bool CreateSuspended)
	: TThread(CreateSuspended)
{
	readyM = new TMutex(false);
	control = false;
}
//---------------------------------------------------------------------------
void __fastcall AlarmThreadClass::Execute()
{
// ������������ ��������� ��������
	NameThreadForDebugging("AlarmThread");
	bool cycle = true;
	is_alarm = false;
// ���������� ���������, ���� �� ���������� ��� ������ �������
	while ( cycle )
	{
	  /*
		if ( !digital->CheckInBouncelessSignal("������", false))
//		if ( !digital->CheckInSignal("������" ))
		{
			cycle = false;
			reason = "������ ������!";
			ProtocolForm->SendToProtocol("AlarmThread: " + reason);
		}
		*/
		TThread::CurrentThread->Sleep(40);
 //------------------------------------------------------------------------
		if ( !digital->CheckInBouncelessSignal("�� �������", false))
		{
			cycle = false;
			reason = "�� ������� ������ ������ ���������!";
			ProtocolForm->SendToProtocol("AlarmThread: " + reason);
		}
		TThread::CurrentThread->Sleep(40);
//-------------------------------------------------------------------------
		if ( !digital->CheckInBouncelessSignal("���� ����������", false))
//		if ( !digital->CheckInSignal("��: ���� ����������"))
		{
			cycle = false;
			reason = "��� ������� ���� ����������!";
			ProtocolForm->SendToProtocol("AlarmThread: " + reason);
		}
		TThread::CurrentThread->Sleep(40);
	 /*
		if ( !digital->CheckInBouncelessSignal("������ ����������" , false))
	//	if ( !digital->CheckInSignal("��: ������ ����������" ))
		{
		  //	cycle = false;
			reason = "��������� ������ ����������!";
			ProtocolForm->SendToProtocol("AlarmThread: " + reason);
			//dprint("EROR ENGINE\n");
		}
	  	*/
		TThread::CurrentThread->Sleep(40);

		readyM->Acquire();
		if (control)
		{
			if ( !digital->CheckInSignal("��������") )
			{
				cycle = false;
				reason = "������ ������ \"��������\" �� �����������!";
				ProtocolForm->SendToProtocol("AlarmThread: " + reason);
			}
		}
		readyM->Release();

		if (Terminated)
			break;

	}
// ��������, ��� ����� - �� ������ ��� ������, ������������ �� �������� �����
	if ( !cycle )
	{
//		Synchronize(&UpdateCaption);
		is_alarm = true;
	}

}
//---------------------------------------------------------------------------

void __fastcall AlarmThreadClass::UpdateCaption()
{
	MainForm->Caption = "��� ��������";
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void AlarmThreadClass::SetControl()
{
	readyM->Acquire();
	control = true;
	readyM->Release();
}

void AlarmThreadClass::ResetControl()
{
	readyM->Acquire();
	control = false;
	readyM->Release();
}
//---------------------------------------------------------------------------
//-------������ ��� ���� �� ����---------------------------------------------
void AlarmThreadClass::SetPlantCycle()
{
 //	plantM->Acquire();
 //	plantCycle = true;
 //	plantM->Release();
}
void AlarmThreadClass::ResetPlantCycle()
{
//	plantM->Acquire();
//	plantCycle = false;
//	plantM->Release();
}
//---------------------------------------------------------------------------
//------������� ������, ������ �� ����������� �����--------------------------
void AlarmThreadClass::SetStand(String st1, String st2)
{
//	standM->Acquire();
//	stand = true;
//	stand1 = st1;
//	stand2 = st2;
//	standM->Release();
}
void AlarmThreadClass::ResetStand()
{
//	standM->Acquire();
//	stand = false;
//	standM->Release();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


