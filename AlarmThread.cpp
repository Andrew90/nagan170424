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
// контролируем аварийные ситуации
	NameThreadForDebugging("AlarmThread");
	bool cycle = true;
	is_alarm = false;
// бесконечно проверяем, есть ли необходимы для работы сигналы
	while ( cycle )
	{
	  /*
		if ( !digital->CheckInBouncelessSignal("Воздух", false))
//		if ( !digital->CheckInSignal("воздух" ))
		{
			cycle = false;
			reason = "Пропал воздух!";
			ProtocolForm->SendToProtocol("AlarmThread: " + reason);
		}
		*/
		TThread::CurrentThread->Sleep(40);
 //------------------------------------------------------------------------
		if ( !digital->CheckInBouncelessSignal("гп включен", false))
		{
			cycle = false;
			reason = "Не включен модуль группы прочности!";
			ProtocolForm->SendToProtocol("AlarmThread: " + reason);
		}
		TThread::CurrentThread->Sleep(40);
//-------------------------------------------------------------------------
		if ( !digital->CheckInBouncelessSignal("цепи управления", false))
//		if ( !digital->CheckInSignal("тс: цепи управления"))
		{
			cycle = false;
			reason = "Нет сигнала цепи управления!";
			ProtocolForm->SendToProtocol("AlarmThread: " + reason);
		}
		TThread::CurrentThread->Sleep(40);
	 /*
		if ( !digital->CheckInBouncelessSignal("защита двигателей" , false))
	//	if ( !digital->CheckInSignal("тс: защита двигателей" ))
		{
		  //	cycle = false;
			reason = "Сработала защита двигателей!";
			ProtocolForm->SendToProtocol("AlarmThread: " + reason);
			//dprint("EROR ENGINE\n");
		}
	  	*/
		TThread::CurrentThread->Sleep(40);

		readyM->Acquire();
		if (control)
		{
			if ( !digital->CheckInSignal("Контроль") )
			{
				cycle = false;
				reason = "Пропал сигнал \"Контроль\" от толщиномера!";
				ProtocolForm->SendToProtocol("AlarmThread: " + reason);
			}
		}
		readyM->Release();

		if (Terminated)
			break;

	}
// проверям, как вышли - по аварии или штатно, терминатнули из главного треда
	if ( !cycle )
	{
//		Synchronize(&UpdateCaption);
		is_alarm = true;
	}

}
//---------------------------------------------------------------------------

void __fastcall AlarmThreadClass::UpdateCaption()
{
	MainForm->Caption = "Все ебнулось";
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
//-------сигнал Цех Цикл от цеха---------------------------------------------
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
//------сигналы Работа, Прогон от контроллера стоек--------------------------
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


