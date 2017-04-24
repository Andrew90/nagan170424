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

	int centralizerSleep = 500;		// время задержки перед срабатыванием схвата
// удалить
	int SleepTemp=750;

//-----------------------------------------------------------------------------
__fastcall WorkThreadClass::WorkThreadClass(bool CreateSuspended, 		// создавать поток замороженным
					LCard791 &lpp,							// указатель на плату поперечных дефектов
			   //		LCard791 &lpr,							// указатель на плату продольных дефектов
					Result &_respop,						// указатель на результат поперечный
					Result &_resprod,						// указатель на результат продольный
					TStatusBar *St,							// указатель на статусбар, показывающий течение процесса
					bool _Thick,							// используем ли толщинометрию
					bool _Linear,                           // используем ли продольный)
					AlarmThreadClass &alth, 				// адрес треда аварии
					bool IsPrepare)							// первый цикл (нужно ли готовить к работе систему)
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

	// запишем длины модулей для расчета скорости
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

	stext1 = "Режим \"Работа\"";
	stext2 = "Готовим к исходному положению";
	ProtocolForm->SendToProtocol("-----------");
	ProtocolForm->SendToProtocol(stext1);
	Synchronize( &UpdateMainForm );

	if (Prepare)
	{
		UnicodeString prepare_result = PrepareForWork( );
		if ( prepare_result != "ok" )
		{
			stext1 = "Режим \"Работа\" не завершен!";
			stext2 = prepare_result;
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			Synchronize( &UpdateMainForm );
			digital->ResetOutSignal("Стойка Авто");
			digital->ResetOutSignal("Схват Авто");
			cool = false;
			return;
		}
	}

	ProtocolForm->SendToProtocol("Работа: Подготовка прошла успешно" );
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
// проверяем, не застряла ли труба в модулях
	if ( digital->CheckInSignal("прод вход") ||
			digital->CheckInSignal("пРод выход") ||
			digital->CheckInSignal("ТолЩ Вход")  ||
			digital->CheckInSignal("ТолЩ Выход") ||
			digital->CheckInSignal("Попер Вход") ||
			digital->CheckInSignal("ПопеР Выход") )
		return "Труба в модулях!";

// проверяем, не перегреты ли соленоиды
	if ( lpop->isOkTemp("1") || lpop->isOkTemp("2") )
		return "Соленоиды перегреты!";

// проверяем, включен ли модуль группы прочности
	if (digital->SignalExists("ГП включен") && !digital->CheckInSignal("ГП включен"))
		return "Включите группу прочности!";

// выставляем режим работы на контроллер стоек
	digital->SetOutSignal("Стойка Авто");
	if ( !digital->WaitForInSignal("Стойка Работа", 5000 , TThread::CurrentThread) )
		return "Ошибка контроллера стоек!";

// выставляем режим работы на контроллер схватов
	digital->SetOutSignal("Схват Авто");
	if ( !digital->WaitForInSignal("Схват Работа", 3000 , TThread::CurrentThread) )
		return "Ошибка контроллера схватов!";
	if (Thick && Linear)
	{
			String result = ModulePosition::SetAllModulesInWork("Попер", "Толщ", "Прод" );
			if ( result != "ok" )
				return result;
	}
	else
	{
// устанавливаем поперечный в нужное положение (всегда в работе)
		if ( !digital->CheckInSignal("Попер РП") )
		{
			String result = ModulePosition::SetModuleInWork( "Попер" );
			if ( result != "ok" )
				return result;
		}

	// устанавливаем толщину в нужное положение
		if ( Thick )
		{
			String result = ModulePosition::SetModuleInWork( "Толщ");
			if ( result != "ok" )
				return result;
		}
		else
		{
			String result = ModulePosition::SetModuleInService( "Толщ");
			if ( result != "ok" )
				return result;
		}

	// устанавливаем продольный в нужное положение
		if ( Linear )
		{
			String result = ModulePosition::SetModuleInWork( "Прод");
			if ( result != "ok" )
				return result;
		}
		else
		{
			String result = 	ModulePosition::SetModuleInService( "Прод");
			if ( result != "ok" )
				return result;
		}
	}

// разжимаем схваты у нужных модулей
/*
	if ( Thick && !digital->CheckInSignal("Толщ схват исх") )
	{
		digital->BlinkOutSignal("Толщ схват разжать");
		if ( !digital->WaitForInSignal("Толщ схват исходное",SleepTemp , TThread::CurrentThread) )
			return "Ошибка схвата толщиномера!";
	}

	if ( Linear && !digital->CheckInSignal("Прод схват исх") )
	{
		digital->BlinkOutSignal("Прод схват разжать");
		if ( !digital->WaitForInSignal("Прод схват исходное",SleepTemp , TThread::CurrentThread) )
			return "Ошибка схвата продольного модуля!";
	}
*/
	return "ok";
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
#if 0
bool WorkThreadClass::WorkMode()
{
	stext2 = "Начинаем работу";
	Synchronize( &UpdateMainForm );
	double lin_v;
	double cr_v;
	double st_v;

// прочитаем скорость вращения из ini
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	String Sect = "Type_" + MainForm->cbTypeSize->Text;
	delete ini;
// открываем фиктивный цикл, который идет 1 раз
	bool result = true;
	while ( result )
	{
// ждем трубу перед позицией (сигнал ТР1)
		stext2 = "Ждем трубу перед позицией бесконечно";
		ProtocolForm->SendToProtocol("Работа: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForInSignal("Цех тр1", 0, TThread::CurrentThread) )
		{
			stext2 = "Не дождались трубу перед позицией! (сигнала тр1)";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
		digital->ResetOutSignal("Цех Результат");
		digital->ResetOutSignal("Цех Признак");

// включаем магнитное поле, проверяем наличие
		digital->SetOutSignal("Соленоид Охлажд");
		digital->SetOutSignal("Соленоид питание");
		TThread::CurrentThread->Sleep(500);
		if ( lpop->isOkTemp("MG") )
		{
			stext2 = "Нет магнитного поля!";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
// посмотрим, сколько датчиков используется
	ProtocolForm->SendToProtocol("Работа: типоразмер " + IntToStr(Globals::current_diameter));
	respop->sensors = (Globals::current_diameter == 60) ? (10) : (12);
	resprod->sensors = 4;
		//LCard Козлы!!!
		lpop->InitADC(16,0,"PP",0);
		lpop->Start();
		TThread::CurrentThread->Sleep(200);
		lpop->Stop();


// иниициализируем сбор данных с LCard
		ProtocolForm->SendToProtocol("Работа: инициализируем сбор данных с LCard");
		int init=0;
		if(SystemConst::SolidGroupSource == 0)
			init=2;		  //Если ГП через ЛКард то инициализируем плату с ГП

		lpop->InitADC( 16,0, "PP", init);
		/* todo zzzzzz
		if ( Linear )
			lprod->InitADC(resprod->sensors,0, "PR", 0);
        */
// ждем сигнала Готовность от толщиномера
		digital->SetOutSignal("Работа");
		if ( Thick )
		{
			stext2="Ждем готовности толщиномера";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForInSignal("Готовность",30000, TThread::CurrentThread))
			{
				stext2 = "Не дождались готовности толщиномера";
				ProtocolForm->SendToProtocol("Работа: " + stext2);
				result = false;
				break;
			}
		}
// крутим продольный
		if ( Linear )
		{
			digital->SetOutSignal("Прод Пит ПЧ");
			digital->SetOutSignal("Прод STF");
			for (UINT s = 0; s < Globals::speeds.inSpeed.size(); s++)
				digital->SetOutSignal("Прод Скорость " + IntToStr(Globals::speeds.inSpeed[s]));
			if ( !digital->WaitForDropInSignal("Прод ПЧ RUN", 7000 , TThread::CurrentThread) )
			{
				stext2 = "Не достигнута скорость вращения продольного модуля!";
				ProtocolForm->SendToProtocol("Работа: " + stext2);
				result = false;
				break;
			}
		}
			if ( Terminated )
				break;


// ждем вращения толщиномера
		digital->SetOutSignal("Цикл");
		if ( Thick )
		{
			stext2="Ждем вращения толщиномера";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForInSignal("Контроль",10000, TThread::CurrentThread))
			{
				stext2 = "Не дождались вращения толщиномера";
				ProtocolForm->SendToProtocol("Работа: " + stext2);
				result = false;
				break;
			}
			lat->SetControl();
		}

// включаем движение трубы вперед
		digital->SetOutSignal("ТС: Пит ПЧ");
		for (UINT s = 0; s < Globals::speeds.workSpeed.size(); s++)
			digital->SetOutSignal("ТС: Скорость " + IntToStr(Globals::speeds.workSpeed[s]));
		digital->SetOutSignal("ТС: STF");
		if ( !digital->WaitForDropInSignal("ТС: ПЧ RUN",3000, TThread::CurrentThread) )
		{
			stext2 = "Не достигнута скорость вращения транспорта";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
		if ( Terminated )
			break;

// выставляем сигналы в цех, труба поехала
		digital->SetOutSignal("Цех вперед");
		digital->SetOutSignal("Цех движение");

//************************труба едет********************************************
// ждем трубу на первой стойке
		stext2 = "Ждем трубу на стойке 1";
		ProtocolForm->SendToProtocol("Работа: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForInSignal("Стойка 1", 130000, TThread::CurrentThread ) )
		{
			stext2 = "Не дождались трубы на стойке 1!";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
		st1 = GetTickCount();
		if ( Terminated )
			break;
// ждем трубу на входе поперечного
		stext2 = "Ждем трубу на входе поперечного";
		Synchronize( &UpdateMainForm );
		if ( digital->WaitForInSignal("Попер вход",30000, TThread::CurrentThread) )
		{
			TThread::CurrentThread->Sleep(900);
			lpop->Start();
			cross_t1 = GetTickCount();
		}
		else
		{
			stext2 = "Не дождались трубы на входе поперечного!";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
		if ( Terminated )
			break;
/*
	// ждем трубу на выходе поперечного
		stext2 = "Ждем трубу на выходе поперечного";
		Synchronize( &UpdateMainForm );
		if ( digital->WaitForInSignal("Попер выход",4000, TThread::CurrentThread) )
			cross_t2 = GetTickCount();
		else
		{
			stext2 = "Не дождались трубы на выходе поперечного!";
			result = false;
			break;
		}
*/
/*
// ждем трубу на входе толщиномера, если нужно
		if ( Thick )
		{
			stext2 = "Ждем трубу на входе толщиномера";
			Synchronize( &UpdateMainForm );
			if ( digital->WaitForInSignal("Толщ вход",5000, TThread::CurrentThread) )
			{
//				TThread::CurrentThread->Sleep( centralizerSleep );
//				digital->BlinkOutSignal("Толщ схват on");
			}
			else
			{
				stext2 = "Не дождались трубы на входе толщиномера!";
				result = false;
				break;
			}
		}
		if ( Terminated )
			break;
*/
// ждем трубу на входе продольного, если нужно
		if ( Linear )
		{
			stext2 = "Ждем трубу на входе продольного";
			Synchronize( &UpdateMainForm );
			digital->SetOutSignal("Прод пит су");
			if ( digital->WaitForInSignal("Прод вход",15000, TThread::CurrentThread) )
			{
				TThread::CurrentThread->Sleep(900);
  //toto zzzzz				lprod->Start();
				lin_t1 = GetTickCount();
//				TThread::CurrentThread->Sleep( centralizerSleep );
//				digital->BlinkOutSignal("Прод схват on");
			}
			else
			{
				stext2 = "Не дождались трубы на входе продольного!";
				ProtocolForm->SendToProtocol("Работа: " + stext2);
				result = false;
				break;
			}
		}
/*
	pp.CalculateDeadZone(ini->ReadInteger("Size","ZoneLength",2));
	pr.CalculateDeadZone(ini->ReadInteger("Size","ZoneLength",2));
	// ждем трубу на выходе продольного
	if ( Linear )
		{
			stext2 = "Ждем трубу на выходе продольного";
			Synchronize( &UpdateMainForm );
			if ( digital->WaitForInSignal("Прод выход",4000, TThread::CurrentThread) )
				lin_t2 = GetTickCount();
			else
			{
				stext2 = "Не дождались трубы на выходе продольного!";
				result = false;
				break;
			}
			lin_v = linear_len / (lin_t2 - lin_t1);		// скорость м/с
		}
		if ( Terminated )
			break;
*/
// ждем трубу на стойке 4
		stext2 = "Ждем трубу на стойке 4";
		ProtocolForm->SendToProtocol("Работа: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForInSignal("Стойка 4",15000, TThread::CurrentThread) )
		{
			stext2 = "Не дождались трубы на стойке 4!";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
		st4 = GetTickCount();
		digital->SetOutSignal("База");
		//по новой схеме
		if ( digital->CheckInSignal("Прод выход") )
		{
			digital->ResetOutSignal("Прод скорость 1");
			digital->ResetOutSignal("Прод скорость 2");
			digital->ResetOutSignal("Прод скорость 3");
			for (UINT s = 0; s < Globals::speeds.linearSpeed.size(); s++)
				digital->SetOutSignal("Прод Скорость " + IntToStr(Globals::speeds.linearSpeed[s]));
		//	ProtocolForm->SendToProtocol("Рабочая скорость вращения продольного");
		}

// ждем трубу на входе в гп
		stext2 = "Ждем трубу на входе в модуль гп";
		ProtocolForm->SendToProtocol("Работа: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForInSignal("гп вход",2000, TThread::CurrentThread) )
		{
			stext2 = "Не дождались входа трубы в модуль гп!";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
		if(SystemConst::SolidGroupSource == 0)      //Если ГП через LCard
		{
			TThread::CurrentThread->Sleep(200);
			lpop->SetMeasInSG();
		}

// ждем схода трубы со стойки 1
		stext2 = "Ждем схода трубы со стойки 1";
		ProtocolForm->SendToProtocol("Работа: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForDropInSignal( "Стойка 1" , 100000 , TThread::CurrentThread) )
		{
			stext2 = "Не дождались выхода трубы со стойки 1!";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
		st1_out = GetTickCount();
// ждем схода трубы с датчиков попера
		if ( !digital->WaitForDropInSignal( "Попер вход" , 4000 , TThread::CurrentThread) )
		{
			stext2 = "Не дождались схода трубы со входа поперечного!";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
		cross_t3 = GetTickCount();
		TThread::CurrentThread->Sleep(1000);
		MeasOutCrossModule=lpop->Num;  //Столько данных было собранно на момент выхода из попер модуля

		//lpop->Stop();

		stext2 = "Ждем выхода трубы из поперечного";
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForDropInSignal( "Попер выход" , 4000 , TThread::CurrentThread) )
		{
			stext2 = "Не дождались выхода трубы из поперечного!";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
		digital->ResetOutSignal("соленоид питание");
//		lpop->Stop();
		st_v = st_len / (st4 - st1);
		double st_tube = st_v * ( st1_out - st1);
		short cr_zones = (short) Math::Ceil( st_tube / zone_len);
		if (cr_zones > Globals::max_zones)
			cr_zones = Globals::max_zones;
		respop->zones = cr_zones;
		sms->length_in_zones = cr_zones;

// ждем выхода из толщиномера
		if (Thick)
		{
			stext2 = "Ждем выхода трубы из толщиномера";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForDropInSignal( "Толщ вход" , 8000 , TThread::CurrentThread) )
			{
				stext2 = "Не дождались схода трубы со входа толщиномера!";
				ProtocolForm->SendToProtocol("Работа: " + stext2);
				result = false;
				break;
			}
			lat->ResetControl();
		}
		if (Linear)
		{
			digital->ResetOutSignal("Прод скорость 1");
			digital->ResetOutSignal("Прод скорость 2");
			digital->ResetOutSignal("Прод скорость 3");
			for (UINT s = 0; s < Globals::speeds.outSpeed.size(); s++)
				digital->SetOutSignal("Прод Скорость " + IntToStr(Globals::speeds.outSpeed[s]));
			ProtocolForm->SendToProtocol("Выходная скорость вращения продольного");
		}

// ждем схода трубы с датчиков продольного
		if ( Linear )
		{
			if ( !digital->WaitForDropInSignal( "Прод вход" , 10000 , TThread::CurrentThread) )
			{
				stext2 = "Не дождались схода трубы со входа продольного!";
				ProtocolForm->SendToProtocol("Работа: " + stext2);
				result = false;
				break;
			}
			lin_t3 = GetTickCount();
			TThread::CurrentThread->Sleep(1200);
   //todo zzzzz			lprod->Stop();

			stext2 = "Ждем выхода трубы из продольного";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForDropInSignal( "Прод выход" , 25000 , TThread::CurrentThread) )
			{
				stext2 = "Не дождались выхода трубы из продольного!";
				result = false;
				break;
			}
//			lprod->Stop();
			digital->ResetOutSignal("Прод STF");
		}

// труба уехала, можно многое отключить
	stext2 = "Ждем схода трубы со стойки 4";
	ProtocolForm->SendToProtocol("Работа: " + stext2);
	Synchronize( &UpdateMainForm );
	if ( !digital->WaitForDropInSignal( "Стойка 4" , 30000 , TThread::CurrentThread) )
	{
		stext2 = "Не дождались схода трубы со стойки 4!";
		ProtocolForm->SendToProtocol("Работа: " + stext2);
		result = false;
		break;
	}
	digital->ResetOutSignal("ТС: STF");
	digital->ResetOutSignal("База");

	stext2 = "Ждем выхода трубы из установки";
	ProtocolForm->SendToProtocol("Работа: " + stext2);
	Synchronize( &UpdateMainForm );
	if( !digital->WaitForDropInSignal( "гп выход" , 5000 , TThread::CurrentThread) )
	{
		stext2 = "Не дождались выхода трубы из установки!";
		ProtocolForm->SendToProtocol("Работа: " + stext2);
		result = false;
		break;
	}
	lpop->Stop();
	lpop->SetMeasOutSG();
	ProtocolForm->SendToProtocol("Работа: остановили сбор lpop" );

	//st_v = st_len / (st4 - st1);
	//double st_tube = st_v * ( st1_out - st1);
// просчитываем данные продольного
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
		ProtocolForm->SendToProtocol("Работа: рассчитали данные продольного" );
	}
*/
	long trash=0;
	if(SystemConst::SolidGroupSource == 0)      //Если ГП через LCard
		//посчитаем сколько мусора надо откинуть после того как труба вышла из поперечного
		trash = (lpop->Num-MeasOutCrossModule)*lpop->part;
	else
		trash=0;

	ProtocolForm->SendToProtocol("Работа: рассчитали trash" );

 // просчитывыаем данные поперечного
	//short cr_zones = (short) Math::Ceil( st_tube / zone_len);
	//if (cr_zones > Globals::max_zones)
	//	cr_zones = Globals::max_zones;


	//respop->zones = cr_zones;
	//todo 1605
	respop->PrepareData( lpop ,16, respop->sensors,trash );
	//respop->PrepareData( lpop ,respop->data, resprod->data);
	respop->ComputeZoneData();
	respop->CalculateDeadZone(zone_len );

	ProtocolForm->SendToProtocol("Работа: рассчитали данные поперечного" );


	if ( Linear )
	{
		short lin_zones = (short) Math::Ceil( st_tube / zone_len );
		if (lin_zones > Globals::max_zones)
			lin_zones = Globals::max_zones;
		resprod->zones = lin_zones;
 //todo zzzzz		resprod->PrepareData( lprod , resprod->sensors, resprod->sensors );
		resprod->ComputeZoneData();
		resprod->CalculateDeadZone(zone_len );
		ProtocolForm->SendToProtocol("Работа: рассчитали данные продольного" );
	}

	digital->ResetOutSignal("соленоид охлажд");
	digital->ResetOutSignal("соленоид питание");
	digital->ResetOutSignal("Работа");

// ждем, когда труба приедет в конец
		stext2 = "Ждем трубу в конце линии (ТР2)";
		ProtocolForm->SendToProtocol("Работа: " + stext2);
		Synchronize( &UpdateMainForm );
	  /*	while (true)
		{
			if ( Terminated )
				break;
			if ( digital->WaitForInSignal( "Цех ТР2" , 50000 , TThread::CurrentThread) )
			{
				if ( digital->CheckInBouncelessSignal("цех тр2",true) )
				{
					ProtocolForm->SendToProtocol("Работа: Получили ТР2");
					break;
				}
				else
					continue;
			}
			else
			{
				stext2 = "Не дождались трубу в конце линии! (ТР2)";
				ProtocolForm->SendToProtocol("Работа: " + stext2);
				result = false;
				break;
			}
		}
		*/
		break;
	}
	// сбросим управляющие сигналы
	digital->ResetOutSignal("соленоид охлажд");
	digital->ResetOutSignal("соленоид питание");
//	digital->ResetOutSignal("ТС: Пит ПЧ");
	digital->ResetOutSignal("ТС: скорость 1");
	digital->ResetOutSignal("ТС: скорость 2");
	digital->ResetOutSignal("ТС: скорость 3");
	digital->ResetOutSignal("ТС: STF");

	digital->ResetOutSignal("Стойка Авто");
	digital->ResetOutSignal("Схват Авто");

	digital->ResetOutSignal("Работа");
	digital->ResetOutSignal("Цикл");
	digital->ResetOutSignal("База");

	digital->ResetOutSignal("Цех вперед");
	digital->ResetOutSignal("Цех движение");

	if ( Linear )
	{
//		digital->ResetOutSignal("Прод Пит ПЧ");
		digital->ResetOutSignal("Прод STF");
		digital->ResetOutSignal("Прод Скорость 1");
		digital->ResetOutSignal("Прод Скорость 2");
		digital->ResetOutSignal("Прод Скорость 3");
		digital->ResetOutSignal("Прод пит су");
	}

	if ( Terminated )
	{
		lpop->Stop();
	   //	lpop->SetMeasOutSG();
	  //todo zzzzz	if ( Linear )lprod->Stop();

		ProtocolForm->SendToProtocol("Работа: Выпали из цикла по Terminate");
		ReturnValue = 0;
		return false;
	}
	if ( result)
	{
		stext1 = "Режим \"Работа\" завершен успешно";
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
		stext1 = "Режим \"Работа\" не завершен!";
		ProtocolForm->SendToProtocol(stext1);
		Synchronize( &UpdateMainForm );
		return false;
	}
}
#else
bool WorkThreadClass::WorkMode()
{
#if 1
	stext2 = "Начинаем работу";
	Synchronize( &UpdateMainForm );
	double lin_v;
	double cr_v;
	double st_v;

// прочитаем скорость вращения из ini
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	String Sect = "Type_" + MainForm->cbTypeSize->Text;
	delete ini;
// открываем фиктивный цикл, который идет 1 раз
	bool result = true;
	while ( result )
	{
// ждем трубу перед позицией (сигнал ТР1)
		stext2 = "Ждем трубу перед позицией бесконечно";
		ProtocolForm->SendToProtocol("Работа: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForInSignal("Цех тр1", 0, TThread::CurrentThread) )
		{
			stext2 = "Не дождались трубу перед позицией! (сигнала тр1)";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
		digital->ResetOutSignal("Цех Результат");
		digital->ResetOutSignal("Цех Признак");

// включаем магнитное поле, проверяем наличие
		digital->SetOutSignal("Соленоид Охлажд");
		digital->SetOutSignal("Соленоид питание");
		TThread::CurrentThread->Sleep(500);
  		if ( lpop->isOkTemp("MG") )
		{
			stext2 = "Нет магнитного поля!";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
// посмотрим, сколько датчиков используется
	ProtocolForm->SendToProtocol("Работа: типоразмер " + IntToStr(Globals::current_diameter));
	respop->sensors = (Globals::current_diameter == 60) ? (10) : (12);
	resprod->sensors = 4;
		//LCard Козлы!!!
		lpop->InitADC(16,0,"PP",0);
		lpop->Start();
		TThread::CurrentThread->Sleep(200);
		lpop->Stop();


// иниициализируем сбор данных с LCard
		ProtocolForm->SendToProtocol("Работа: инициализируем сбор данных с LCard");
		int init=0;
		if(SystemConst::SolidGroupSource == 0)
			init=2;		  //Если ГП через ЛКард то инициализируем плату с ГП

		lpop->InitADC( 16,0, "PP", init);
		/* todo zzzzzz
		if ( Linear )
			lprod->InitADC(resprod->sensors,0, "PR", 0);
        */
// ждем сигнала Готовность от толщиномера
		digital->SetOutSignal("Работа");
		if ( Thick )
		{
			stext2="Ждем готовности толщиномера";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForInSignal("Готовность",30000, TThread::CurrentThread))
			{
				stext2 = "Не дождались готовности толщиномера";
				ProtocolForm->SendToProtocol("Работа: " + stext2);
				result = false;
				break;
			}
		}
// крутим продольный
		if ( Linear )
		{
			digital->SetOutSignal("Прод Пит ПЧ");
			digital->SetOutSignal("Прод STF");
			for (UINT s = 0; s < Globals::speeds.inSpeed.size(); s++)
				digital->SetOutSignal("Прод Скорость " + IntToStr(Globals::speeds.inSpeed[s]));
#if 0
			if ( !digital->WaitForDropInSignal("Прод ПЧ RUN", 13000 , TThread::CurrentThread) )
			{
				stext2 = "Не достигнута скорость вращения продольного модуля!";
				ProtocolForm->SendToProtocol("Работа: " + stext2);
				result = false;
				break;
			}
#else
		   dprint("отключил проверку вращения продолного модуля\n");
#endif
		}
			if ( Terminated )
				break;

// ждем вращения толщиномера
		digital->SetOutSignal("Цикл");
		if ( Thick )
		{
			stext2="Ждем вращения толщиномера";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForInSignal("Контроль",10000, TThread::CurrentThread))
			{
				stext2 = "Не дождались вращения толщиномера";
				ProtocolForm->SendToProtocol("Работа: " + stext2);
				result = false;
				break;
			}
			lat->SetControl();
		}
// включаем движение трубы вперед
		digital->SetOutSignal("ТС: Пит ПЧ");
		for (UINT s = 0; s < Globals::speeds.workSpeed.size(); s++)
			digital->SetOutSignal("ТС: Скорость " + IntToStr(Globals::speeds.workSpeed[s]));
		digital->SetOutSignal("ТС: STF");
		if ( !digital->WaitForDropInSignal("ТС: ПЧ RUN",3000, TThread::CurrentThread) )
		{
			stext2 = "Не достигнута скорость вращения транспорта";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
		if ( Terminated )
			break;
// выставляем сигналы в цех, труба поехала
		digital->SetOutSignal("Цех вперед");
		digital->SetOutSignal("Цех движение");



//************************труба едет********************************************
// ждем трубу на первой стойке
		stext2 = "Ждем трубу на стойке 1";
		ProtocolForm->SendToProtocol("Работа: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForInSignal("Стойка 1", 10000, TThread::CurrentThread ) )
		{
			stext2 = "Не дождались трубы на стойке 1!";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
		st1 = GetTickCount();
		if ( Terminated )
			break;
// ждем трубу на входе поперечного
		stext2 = "Ждем трубу на входе поперечного";
		Synchronize( &UpdateMainForm );
		if ( digital->WaitForInSignal("Попер вход",10000, TThread::CurrentThread) )
		{
			TThread::CurrentThread->Sleep(900);
			lpop->Start();
			Lir::Instance().Start(Thick, Linear);
			cross_t1 = GetTickCount();
		}
		else
		{
			stext2 = "Не дождались трубы на входе поперечного!";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
		if ( Terminated )
			break;
/*
	// ждем трубу на выходе поперечного
		stext2 = "Ждем трубу на выходе поперечного";
		Synchronize( &UpdateMainForm );
		if ( digital->WaitForInSignal("Попер выход",4000, TThread::CurrentThread) )
			cross_t2 = GetTickCount();
		else
		{
			stext2 = "Не дождались трубы на выходе поперечного!";
			result = false;
			break;
		}
*/
/*
// ждем трубу на входе толщиномера, если нужно
		if ( Thick )
		{
			stext2 = "Ждем трубу на входе толщиномера";
			Synchronize( &UpdateMainForm );
			if ( digital->WaitForInSignal("Толщ вход",5000, TThread::CurrentThread) )
			{
//				TThread::CurrentThread->Sleep( centralizerSleep );
//				digital->BlinkOutSignal("Толщ схват on");
			}
			else
			{
				stext2 = "Не дождались трубы на входе толщиномера!";
				result = false;
				break;
			}
		}
		if ( Terminated )
			break;
*/
// ждем трубу на входе продольного, если нужно
		if ( Linear )
		{
			stext2 = "Ждем трубу на входе продольного";
			Synchronize( &UpdateMainForm );
			digital->SetOutSignal("Прод пит су");
			if ( digital->WaitForInSignal("Прод вход",15000, TThread::CurrentThread) )
			{
				TThread::CurrentThread->Sleep(900);
  //toto zzzzz				lprod->Start();
				lin_t1 = GetTickCount();
//				TThread::CurrentThread->Sleep( centralizerSleep );
//				digital->BlinkOutSignal("Прод схват on");
			}
			else
			{
				stext2 = "Не дождались трубы на входе продольного!";
				ProtocolForm->SendToProtocol("Работа: " + stext2);
				result = false;
				break;
			}
		}
/*
	pp.CalculateDeadZone(ini->ReadInteger("Size","ZoneLength",2));
	pr.CalculateDeadZone(ini->ReadInteger("Size","ZoneLength",2));
	// ждем трубу на выходе продольного
	if ( Linear )
		{
			stext2 = "Ждем трубу на выходе продольного";
			Synchronize( &UpdateMainForm );
			if ( digital->WaitForInSignal("Прод выход",4000, TThread::CurrentThread) )
				lin_t2 = GetTickCount();
			else
			{
				stext2 = "Не дождались трубы на выходе продольного!";
				result = false;
				break;
			}
			lin_v = linear_len / (lin_t2 - lin_t1);		// скорость м/с
		}
		if ( Terminated )
			break;
*/
// ждем трубу на стойке 4
		stext2 = "Ждем трубу на стойке 4";
		ProtocolForm->SendToProtocol("Работа: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForInSignal("Стойка 4",15000, TThread::CurrentThread) )
		{
			stext2 = "Не дождались трубы на стойке 4!";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
		st4 = GetTickCount();
		digital->SetOutSignal("База");
		//по новой схеме
		if ( digital->CheckInSignal("Прод выход") )
		{
			digital->ResetOutSignal("Прод скорость 1");
			digital->ResetOutSignal("Прод скорость 2");
			digital->ResetOutSignal("Прод скорость 3");
			for (UINT s = 0; s < Globals::speeds.linearSpeed.size(); s++)
				digital->SetOutSignal("Прод Скорость " + IntToStr(Globals::speeds.linearSpeed[s]));
		//	ProtocolForm->SendToProtocol("Рабочая скорость вращения продольного");
		}

// ждем трубу на входе в гп
		stext2 = "Ждем трубу на входе в модуль гп";
		ProtocolForm->SendToProtocol("Работа: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForInSignal("гп вход",2000, TThread::CurrentThread) )
		{
			stext2 = "Не дождались входа трубы в модуль гп!";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
		if(SystemConst::SolidGroupSource == 0)      //Если ГП через LCard
		{
			TThread::CurrentThread->Sleep(200);
			lpop->SetMeasInSG();
		}
		PrepareSG::Instance().timeStart = timeGetTime();
// ждем схода трубы со стойки 1
		stext2 = "Ждем схода трубы со стойки 1";
		ProtocolForm->SendToProtocol("Работа: " + stext2);
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForDropInSignal( "Стойка 1" , 100000 , TThread::CurrentThread) )
		{
			stext2 = "Не дождались выхода трубы со стойки 1!";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
		st1_out = GetTickCount();

// ждем схода трубы с датчиков попера
		if ( !digital->WaitForDropInSignal( "Попер вход" , 4000 , TThread::CurrentThread) )
		{
			stext2 = "Не дождались схода трубы со входа поперечного!";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
		cross_t3 = GetTickCount();
		TThread::CurrentThread->Sleep(1000);
		MeasOutCrossModule=lpop->Num;  //Столько данных было собранно на момент выхода из попер модуля

		//lpop->Stop();

		stext2 = "Ждем выхода трубы из поперечного";
		Synchronize( &UpdateMainForm );
		if ( !digital->WaitForDropInSignal( "Попер выход" , 4000 , TThread::CurrentThread) )
		{
			stext2 = "Не дождались выхода трубы из поперечного!";
			ProtocolForm->SendToProtocol("Работа: " + stext2);
			result = false;
			break;
		}
		digital->ResetOutSignal("соленоид питание");
//		lpop->Stop();
		st_v = st_len / (st4 - st1);
		double st_tube = st_v * ( st1_out - st1);
		short cr_zones = (short) Math::Ceil( st_tube / zone_len);
		if (cr_zones > Globals::max_zones)
			cr_zones = Globals::max_zones;
		respop->zones = cr_zones;
		sms->length_in_zones = cr_zones;

// ждем выхода из толщиномера
		if (Thick)
		{
			stext2 = "Ждем выхода трубы из толщиномера";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForDropInSignal( "Толщ вход" , 8000 , TThread::CurrentThread) )
			{
				stext2 = "Не дождались схода трубы со входа толщиномера!";
				ProtocolForm->SendToProtocol("Работа: " + stext2);
				result = false;
				break;
			}
			lat->ResetControl();
		}
		if (Linear)
		{
			digital->ResetOutSignal("Прод скорость 1");
			digital->ResetOutSignal("Прод скорость 2");
			digital->ResetOutSignal("Прод скорость 3");
			for (UINT s = 0; s < Globals::speeds.outSpeed.size(); s++)
				digital->SetOutSignal("Прод Скорость " + IntToStr(Globals::speeds.outSpeed[s]));
			ProtocolForm->SendToProtocol("Выходная скорость вращения продольного");
		}

// ждем схода трубы с датчиков продольного
		if ( Linear )
		{
			if ( !digital->WaitForDropInSignal( "Прод вход" , 10000 , TThread::CurrentThread) )
			{
				stext2 = "Не дождались схода трубы со входа продольного!";
				ProtocolForm->SendToProtocol("Работа: " + stext2);
				result = false;
				break;
			}
			lin_t3 = GetTickCount();
			TThread::CurrentThread->Sleep(1200);
   //todo zzzzz			lprod->Stop();

			stext2 = "Ждем выхода трубы из продольного";
			Synchronize( &UpdateMainForm );
			if ( !digital->WaitForDropInSignal( "Прод выход" , 25000 , TThread::CurrentThread) )
			{
				stext2 = "Не дождались выхода трубы из продольного!";
				result = false;
				break;
			}
//			lprod->Stop();
			digital->ResetOutSignal("Прод STF");
		}

// труба уехала, можно многое отключить
	stext2 = "Ждем схода трубы со стойки 4";
	ProtocolForm->SendToProtocol("Работа: " + stext2);
	Synchronize( &UpdateMainForm );
	if ( !digital->WaitForDropInSignal( "Стойка 4" , 30000 , TThread::CurrentThread) )
	{
		stext2 = "Не дождались схода трубы со стойки 4!";
		ProtocolForm->SendToProtocol("Работа: " + stext2);
		result = false;
		break;
	}
	digital->ResetOutSignal("ТС: STF");
	digital->ResetOutSignal("База");

	stext2 = "Ждем выхода трубы из установки";
	ProtocolForm->SendToProtocol("Работа: " + stext2);
	Synchronize( &UpdateMainForm );
	if( !digital->WaitForDropInSignal( "гп выход" , 5000 , TThread::CurrentThread) )
	{
		stext2 = "Не дождались выхода трубы из установки!";
		ProtocolForm->SendToProtocol("Работа: " + stext2);
		result = false;
		break;
	}
	PrepareSG::Instance().timeStop = timeGetTime();
	lpop->Stop();
	lpop->SetMeasOutSG();
	ProtocolForm->SendToProtocol("Работа: остановили сбор lpop" );

	//st_v = st_len / (st4 - st1);
	//double st_tube = st_v * ( st1_out - st1);
// просчитываем данные продольного
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
		ProtocolForm->SendToProtocol("Работа: рассчитали данные продольного" );
	}
*/
	long trash=0;
	if(SystemConst::SolidGroupSource == 0)      //Если ГП через LCard
		//посчитаем сколько мусора надо откинуть после того как труба вышла из поперечного
		trash = (lpop->Num-MeasOutCrossModule)*lpop->part;
	else
		trash=0;

	ProtocolForm->SendToProtocol("Работа: рассчитали trash" );

 // просчитывыаем данные поперечного
	//short cr_zones = (short) Math::Ceil( st_tube / zone_len);
	//if (cr_zones > Globals::max_zones)
	//	cr_zones = Globals::max_zones;

 /* 1805      поменял на  смотри ниже
	//respop->zones = cr_zones;
	//todo 1605
   //	respop->PrepareData( lpop ,16, respop->sensors,trash );
   //	respop->PrepareData( lpop ,respop->data, resprod->data);
	respop->ComputeZoneData();
	respop->CalculateDeadZone(zone_len );

	ProtocolForm->SendToProtocol("Работа: рассчитали данные поперечного" );


	if ( Linear )
	{
		short lin_zones = (short) Math::Ceil( st_tube / zone_len );
		if (lin_zones > Globals::max_zones)
			lin_zones = Globals::max_zones;
		resprod->zones = lin_zones;
 //todo zzzzz		resprod->PrepareData( lprod , resprod->sensors, resprod->sensors );
		resprod->ComputeZoneData();
		resprod->CalculateDeadZone(zone_len );
		ProtocolForm->SendToProtocol("Работа: рассчитали данные продольного" );
	}
 */
	if(Linear)
	{
		Compute(lpop, *respop, *resprod);
		ProtocolForm->SendToProtocol("Работа: рассчитали данные продольного" );
	}
	else
	{
		 Compute(lpop, *respop);
	}
	ProtocolForm->SendToProtocol("Работа: рассчитали данные поперечного" );
	digital->ResetOutSignal("соленоид охлажд");
	digital->ResetOutSignal("соленоид питание");
	digital->ResetOutSignal("Работа");

// ждем, когда труба приедет в конец
		stext2 = "Ждем трубу в конце линии (ТР2)";
		ProtocolForm->SendToProtocol("Работа: " + stext2);
		Synchronize( &UpdateMainForm );
	  /*	while (true)
		{
			if ( Terminated )
				break;
			if ( digital->WaitForInSignal( "Цех ТР2" , 50000 , TThread::CurrentThread) )
			{
				if ( digital->CheckInBouncelessSignal("цех тр2",true) )
				{
					ProtocolForm->SendToProtocol("Работа: Получили ТР2");
					break;
				}
				else
					continue;
			}
			else
			{
				stext2 = "Не дождались трубу в конце линии! (ТР2)";
				ProtocolForm->SendToProtocol("Работа: " + stext2);
				result = false;
				break;
			}
		}
		*/
		break;
	}
	// сбросим управляющие сигналы
	digital->ResetOutSignal("соленоид охлажд");
	digital->ResetOutSignal("соленоид питание");
//	digital->ResetOutSignal("ТС: Пит ПЧ");
	digital->ResetOutSignal("ТС: скорость 1");
	digital->ResetOutSignal("ТС: скорость 2");
	digital->ResetOutSignal("ТС: скорость 3");
	digital->ResetOutSignal("ТС: STF");

	digital->ResetOutSignal("Стойка Авто");
	digital->ResetOutSignal("Схват Авто");

	digital->ResetOutSignal("Работа");
	digital->ResetOutSignal("Цикл");
	digital->ResetOutSignal("База");

	digital->ResetOutSignal("Цех вперед");
	digital->ResetOutSignal("Цех движение");

	if ( Linear )
	{
//		digital->ResetOutSignal("Прод Пит ПЧ");
		digital->ResetOutSignal("Прод STF");
		digital->ResetOutSignal("Прод Скорость 1");
		digital->ResetOutSignal("Прод Скорость 2");
		digital->ResetOutSignal("Прод Скорость 3");
		digital->ResetOutSignal("Прод пит су");
	}

	if ( Terminated )
	{
		lpop->Stop();
	   //	lpop->SetMeasOutSG();
	  //todo zzzzz	if ( Linear )lprod->Stop();

		ProtocolForm->SendToProtocol("Работа: Выпали из цикла по Terminate");
		ReturnValue = 0;
		return false;
	}
	if ( result)
	{
		stext1 = "Режим \"Работа\" завершен успешно";
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
		stext1 = "Режим \"Работа\" не завершен!";
		ProtocolForm->SendToProtocol(stext1);
		Synchronize( &UpdateMainForm );
		return false;
	}
	#endif
}
#endif

//-----------------------------------------------------------------------------

