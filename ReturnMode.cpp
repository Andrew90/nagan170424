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

	stext1 = "Режим \"Возврат\"";
	stext2 = "Начинаем работу";
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
// проверяем, не застряла ли труба в модулях
		if ( digital->CheckInSignal("прод вход") ||
				digital->CheckInSignal("пРод выход") ||
				digital->CheckInSignal("ТолЩ Вход")  ||
				digital->CheckInSignal("ТолЩ Выход") ||
				digital->CheckInSignal("Попер Вход") ||
				digital->CheckInSignal("ПопеР Выход") )
		{
			stext2="Труба в модулях!";
			result=false;
			break;
		}
        digital->ResetOutSignal("Стойка Транзит");
		digital->ResetOutSignal("Стойка Авто");
		dprint("%s %d\n", __FUNCTION__, 1);
// проверяем, есть ли труба в конце линии (ТР2)
/*		if ( !digital->CheckInBouncelessSignal("Цех тр2",true) )
		{
			stext2="Нет трубы в конце линии!";
			result=false;
			break;
		}
*/
/*
// проверяем, нужно ли отводить устройства : ПОПЕРЕЧНЫЙ
		stext2="Убираем поперечный модуль";
		Synchronize( &UpdateMainForm );
		ModuleResult = ModulePosition::SetModuleInService("Попер");
		if ( ModuleResult != "ok")
		{
			stext2 = ModuleResult;
			result = false;
			break;
		}
// проверяем, нужно ли отводить устройства : ТОЛЩИНА
		stext2="Убираем толщиномер";
		Synchronize( &UpdateMainForm );
		ModuleResult = ModulePosition::SetModuleInService("Толщ");
		if ( ModuleResult != "ok")
		{
			stext2 = ModuleResult;
			result = false;
			break;
		}
// проверяем, нужно ли отводить устройства : ПРОДОЛЬНЫЙ
		stext2="Убираем продольный модуль";
		Synchronize( &UpdateMainForm );
		ModuleResult = ModulePosition::SetModuleInService("Прод");
		if ( ModuleResult != "ok")
		{
			stext2 = ModuleResult;
			result = false;
			break;
		}
*/

// отводим сразу все модули
		stext2="Убираем все модули ";
		Synchronize( &UpdateMainForm );
		ModuleResult = ModulePosition::SetAllModulesInService("Попер", "Толщ", "Прод");
		if ( ModuleResult != "ok")
		{
			stext2 = ModuleResult;
			result = false;
			break;
		}
		dprint("%s %d\n", __FUNCTION__, 2);
// устанавливаем режим работы контроллера СТОЕК

		if ( !digital->WaitForDropInSignal("Стойка Работа",30000, TThread::CurrentThread) )
		{
			stext2="Ошибка контроллера стоек!";
			result=false;
			break;
		}
		Sleep(1500);
		dprint("%s %d\n", __FUNCTION__, 3);
		digital->SetOutSignal("Стойка Транзит");
		stext2="Ждем контроллер стоек";
		Synchronize( &UpdateMainForm );
  /*
		if ( !digital->WaitForInSignal("Стойка Прогон",30000, TThread::CurrentThread) )
		{
			stext2="Ошибка контроллера стоек!";
			result=false;
			break;
		}
		dprint("%s %d\n", __FUNCTION__, 4);
         */
// устанавливаем режим работы контроллера СХВАТОВ
		digital->ResetOutSignal("Схват Транзит");
		digital->ResetOutSignal("Схват Авто");
		if ( !digital->WaitForDropInSignal("Схват Работа",30000, TThread::CurrentThread) )
		{
			stext2="Ошибка контроллера схватов!";
			result=false;
			break;
		}
		dprint("%s %d\n", __FUNCTION__, 5);
		digital->SetOutSignal("Схват Транзит");
		stext2="Ждем контроллер схватов";
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForInSignal("Схват Прогон",30000, TThread::CurrentThread) )
		{
			stext2="Ошибка контроллера схватов!";
			result=false;
			break;
		}
		 dprint("%s %d\n", __FUNCTION__, 6);
// устанавливаем выходы на частотник транспорта, труба едет назад
		stext2="Труба едет назад";
		Synchronize( &UpdateMainForm );

		digital->SetOutSignal("ТС: Пит ПЧ");

			for (UINT s = 0; s < Globals::speeds.workSpeed.size(); s++)
		digital->SetOutSignal("ТС: Скорость " + IntToStr(Globals::speeds.returnSpeed[s] ) );

		digital->SetOutSignal("ТС: STR");
		Sleep(1500);
		digital->SetOutSignal("Цех назад");
		digital->SetOutSignal("Цех движение");

		stext2="Ждем появления трубы в начале линии (ТР1)";
		Synchronize( &UpdateMainForm );
		/*if ( digital->CheckInBouncelessSignal("Цех Реверс" , true) )
		{   */
			stext2="Ждем трубу на стойке 1";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForInSignal("Стойка 1",55000, TThread::CurrentThread) )
			{
				stext2="Не дождались трубу на стойке 1!";
				result=false;
				break;
			}
			dprint("%s %d\n", __FUNCTION__, 7);
			stext2="Ждем схода трубы со стойки 1";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForDropInSignal("Стойка 1",55000, TThread::CurrentThread) )
			{
				stext2="Не дождались схода трубы со стойки 1!";
				result=false;
				break;
			}
			   dprint("%s %d\n", __FUNCTION__, 8);
	 /*	}
		else
		{
			while (true)
			{
				if ( digital->WaitForInSignal("цех тр1", 180000 , TThread::CurrentThread) )
				{
					if ( digital->CheckInBouncelessSignal("цех тр1",true) )
					{
						break;
					}
					else
						continue;
				}
				else
				{
					stext2 = "Не дождались трубу в начале линии!";
					result = false;
					break;
				}
			}
		}   */
// цикл идет только 1 раз
		break;
	}

// сбрасываем управляющие сигналы
//	digital->ResetOutSignal("ТС: Пит ПЧ");
	digital->ResetOutSignal("ТС: скорость 2");
	Sleep(1500);
	digital->ResetOutSignal("ТС: STR");

	digital->ResetOutSignal("Стойка транзит");
	digital->ResetOutSignal("Схват Транзит");
	digital->ResetOutSignal("Цех назад");
	digital->ResetOutSignal("Цех движение");


	if (Terminated)
	{
    	return false;
	}
	dprint("%s %d\n", __FUNCTION__, 9);
	if ( result)
	{
		stext2="";
		stext1="Режим Возврат успешно завершен";
		ProtocolForm->SendToProtocol("Возврат: " + stext1);
		Synchronize( &UpdateMainForm );
		dprint("%s %d\n", __FUNCTION__, 10);
		return true;
	}
	else
	{
		stext1="Режим Возврат не завершен";
		ProtocolForm->SendToProtocol("Выгон: " + stext1);
		Synchronize( &UpdateMainForm );
		dprint("%s %d\n", __FUNCTION__, 11);
		return false;
	}
	dprint("%s %d\n", __FUNCTION__, 12);
}




