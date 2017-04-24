//---------------------------------------------------------------------------


#pragma hdrstop

#include "ThreadOnLine.h"
#include "Math.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

int SleepTemp=750;		// время возврата схватов (не в этом проекте)
UINT Counter1Delay = 1000;		// задержка при переходе с 0 на 1 счетчик
UINT SynchroTime = 110;			// время по выставлению сигнала Синхро в Толщиномер

//---------------------------------------------------------------------------
//----конструктор - переносим внешние переменные на внутренние---------------
__fastcall ThreadOnLine::ThreadOnLine(bool CreateSuspended,
							Advantech1784 *_Counter,	// плата Advantech1784
							LCard791 &_l1,				//Поперечная плата
							LCard791 &_l2,				//Продольная плата
							Result &_respop,			// указатель на результат поперечный
							Result &_resprod,			// указатель на результат продольный
							ThicknessResult &_resth,	// указатель на результат толщины
							SummaryResult &_ressum,		// указатель на результат суммарный
							TStatusBar *St,				// указатель на статусбар, показывающий течение процесса
							bool _Thick,				// используем ли толщинометрию
							bool _Linear,				// используем ли продольный)
							AlarmThreadClass &alth)		// адрес треда аварии

{
	CalcOffsets();		// считаем отступы
	ppFirst=0;
	ppSecond=0;
	prFirst=0;
	prSecond=0;

	ppOld = 0;
	prOld = 0;
	thOld = -1;					// для сигнала СТАРТ толщиномеру
	markOld = -1;				// для маркировки 1-ой зоны
	current_counter = 0;		// начинаем с 1-го счетчика
	long Counter0Fin = 0;
	long Counter1Fin = 0;
	Collect = true;
	transit = false;

	Counter = _Counter; 	//ЛИР
	lpop = &_l1;			//Поперечная плата
	lprod = &_l2;			//Продольная плата
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
//-----запуск потока работы--------------------------------------------------
void __fastcall ThreadOnLine::Execute()
{
	NameThreadForDebugging("WorkOnlineThread");
	stext1 = "Режим \"Работа (Д)\"";
	stext2 = "Готовим к исходному положению";
	ProtocolForm->SendToProtocol("-----------");
	ProtocolForm->SendToProtocol(stext1);
	Synchronize( &UpdateStatus );

	UnicodeString prepare_result = PrepareForWork( );
	if ( prepare_result != "ok" )
	{
		stext1 = "Режим \"Работа (Д)\" не завершен!";
		stext2 = prepare_result;
		ProtocolForm->SendToProtocol("Работа (Д): " + stext2);
		Synchronize( &UpdateStatus );
		digital->ResetOutSignal("Стойка Авто");
		digital->ResetOutSignal("Схват Авто");
		Finally();
		cool = false;
		if (prepare_result == "Транзит")
			transit = true;
		return;
	}

	ProtocolForm->SendToProtocol("Работа (Д): Подготовка прошла успешно" );
	SetReturnValue( OnlineCycle() );
	cool = ReturnValue;
	Finally();
	Terminate();
	return;
}
//---------------------------------------------------------------------------
//-----подготовка к работе от самого начала до движения трубы----------------
UnicodeString ThreadOnLine::PrepareForWork( )
{
// сбрасываем все сигналы в Цех
	if (digital->SignalExists("Цех назад"))
		digital->ResetOutSignal("Цех назад");
	digital->ResetOutSignal("Цех вперед");
	digital->ResetOutSignal("Цех Результат");
	digital->ResetOutSignal("Цех ОП");
//	digital->ResetOutSignal("Цех Признак");
	digital->ResetOutSignal("Стойка Транзит");
	digital->ResetOutSignal("Схват Транзит");

// проверяем, не застряла ли труба в модулях
	if ( digital->CheckInSignal("прод вход") ||
			digital->CheckInSignal("пРод выход") ||
			digital->CheckInSignal("ТолЩ Вход")  ||
			digital->CheckInSignal("ТолЩ Выход") ||
			digital->CheckInSignal("Попер Вход") ||
			digital->CheckInSignal("ПопеР Выход") )
		return "Труба в модулях!";

// проверяем, не замкнут ли датчик на линии
	if ( digital->CheckInSignal("Стойка 1") ||
			digital->CheckInSignal("Стойка 2") ||
			digital->CheckInSignal("Стойка 3")  ||
			digital->CheckInSignal("Стойка 4") ||
			digital->CheckInSignal("ГП труба") ||
			digital->CheckInSignal("Марк Вход") )
		return "Труба на линии!";

// проверяем, не перегреты ли соленоиды
	if ( lpop->isOkTemp("1") || lpop->isOkTemp("2") )
		return "Соленоиды перегреты!";

// проверяем, включен ли модуль размагничивания и группы прочности
	if ( !digital->CheckInBouncelessSignal("ГП включен" , false) )
		return "Включите модуль размагничивания!";

// выделяем память под буфер
	ProtocolForm->SendToProtocol("Работа (Д): Выделяем память под буфер" );
	try
	{
		//Это наш промежуточный буфер, сюда будем копировать данные из LCard,а
		ULONG Size=(5*1024*1024)/sizeof(ULONG);
		Buffer=new ULONG[Size];
	}
	catch (...)
	{
		String Err = "Не удалось выделить память под буффер";
		Application->MessageBox(L"Не удалось выделить память под буффер",L"Ошибка",MB_OK);
		ProtocolForm->SendToProtocol(Err);
		return Err;
	}

// ждем цех цикл
	stext2="Ждем сигнала \"Цех Цикл\"";
	Synchronize( &UpdateStatus );
	if ( !digital->WaitForInSignal("Цех цикл",0, TThread::CurrentThread) )
		return "Не дождались сигнала Цех Цикл!";
	lat->SetPlantCycle();

// ждем ухода трубы с выходного рольганга
/*	stext2 = "Ждем ухода трубы с выходного рольганга";
	ProtocolForm->SendToProtocol("Работа (Д): " + stext2);
	Synchronize( &UpdateStatus );
	if ( !digital->WaitForDropInSignal("Цех тр2", 0, TThread::CurrentThread) )
		return "Не дождались ухода трубы с конца линии! (сигнала тр2)";
	digital->ResetOutSignal("Цех Признак");
*/
// ждем трубу перед позицией (сигнал ТР1)
	digital->SetOutSignal("Цех перекладка");
	stext2 = "Ждем трубу перед позицией бесконечно";
	ProtocolForm->SendToProtocol("Работа (Д): " + stext2);
	Synchronize( &UpdateStatus );
	if ( !digital->WaitForInSignal("Цех тр1", 0, TThread::CurrentThread) )
		return "Не дождались трубу перед позицией! (сигнала тр1)";

	digital->ResetOutSignal("Цех Результат");
	digital->ResetOutSignal("Цех перекладка");
	digital->ResetOutSignal("Цех Признак");
	MainForm->ClearCharts(this);

// проверяем, был ли брак шаблонирования (цех транзит)
	if ( digital->CheckInSignal("Цех транзит" )	)
		return "Транзит";

// выставляем режим работы на контроллер стоек
	stext2 = "Ждем контроллер стоек";
	ProtocolForm->SendToProtocol("Работа (Д): " + stext2);
	Synchronize( &UpdateStatus );
	digital->ResetOutSignal("Стойка Транзит");
	if ( !digital->WaitForDropInSignal("Стойка Прогон", 8000 , TThread::CurrentThread) )
		return "Ошибка контроллера стоек!";
	TThread::CurrentThread->Sleep(500);
	digital->SetOutSignal("Стойка Авто");
	if ( !digital->WaitForInSignal("Стойка Работа", 4000 , TThread::CurrentThread) )
		return "Ошибка контроллера стоек!";
	lat->SetStand("Работа");

// выставляем режим работы на контроллер схватов
	stext2 = "Ждем контроллер схватов";
	ProtocolForm->SendToProtocol("Работа (Д): " + stext2);
	Synchronize( &UpdateStatus );
	digital->ResetOutSignal("Схват Транзит");
	if ( !digital->WaitForDropInSignal("Схват Прогон", 6000 , TThread::CurrentThread) )
		return "Ошибка контроллера схватов!";
	TThread::CurrentThread->Sleep(500);
	digital->SetOutSignal("Схват Авто");
	if ( !digital->WaitForInSignal("Схват Работа", 4000 , TThread::CurrentThread) )
		return "Ошибка контроллера схватов!";

	if (Thick && Linear)
	{
		String result = ModulePosition::SetAllModulesInWork("Толщ" , "Прод", "Попер");
		if (result != "ok")
			return result;
	}
	else
	{
// устанавливаем толщину в нужное положение
		stext2 = "Ставим модули в нужное положение";
		ProtocolForm->SendToProtocol("Работа (Д): " + stext2);
		Synchronize( &UpdateStatus );
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

// устанавливаем поперечный в нужное положение (всегда в работе)
		if ( !digital->CheckInSignal("Попер РП") )
		{
			String result = ModulePosition::SetModuleInWork( "Попер" );
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
	if ( Thick && SystemConst::ThickCentralizer && !digital->CheckInSignal("Толщ схват исх") )
	{
		digital->BlinkOutSignal("Толщ схват разжать");
		if ( !digital->WaitForInSignal("Толщ схват исх",SleepTemp , TThread::CurrentThread) )
			return "Ошибка схвата толщиномера!";
	}

	if ( Linear && SystemConst::LinearCentralizer && !digital->CheckInSignal("Прод схват исх") )
	{
		digital->BlinkOutSignal("Прод схват разжать");
		if ( !digital->WaitForInSignal("Прод схват исх",SleepTemp , TThread::CurrentThread) )
			return "Ошибка схвата продольного модуля!";
	}

// получаем номер трубы из АСУ
	if (SystemConst::ComWithASU)
	{
		long tube_num;
		do
		{
			tube_num = MyCom->GetTubeNumber();
			if (tube_num == -1)
			{
				if ( Application->MessageBoxW(L"Ошибка при получении номера трубы из АСУ. Повторить?",
							L"Внимание",MB_RETRYCANCEL) == 2 )
					tube_num = 11;
			}
		}
		while (tube_num == -1);
		Globals::tube_number = tube_num;
		plantCut = MyCom->GetPlantCut();
		String s = (tube_num == 55555555) ? ((String)"ЭТАЛОН") : ( IntToStr((__int64) tube_num) );
		stext1 = "Работа (Д): труба " + s;
		stext2 = "Работа (Д): рез по шаблонированию " + IntToStr(plantCut);
		ProtocolForm->SendToProtocol(stext1);
		ProtocolForm->SendToProtocol(stext2);
		if (plantCut > 0)
			stext1 += ", рез шаблон " + IntToStr(plantCut);
		Synchronize( &UpdateStatus );
	}

// включаем магнитное поле, проверяем наличие
	digital->SetOutSignal("Соленоид Охлажд");
	digital->SetOutSignal("Соленоид питание");
	TThread::CurrentThread->Sleep(500);
	if ( lpop->isOkTemp("MG") )
		return "Нет магнитного поля!";

// посмотрим, сколько датчиков используется
	ProtocolForm->SendToProtocol("Работа (Д): типоразмер " + IntToStr(Globals::current_diameter));
	respop->sensors = (Globals::current_diameter == 60) ? (10) : (12);
	resprod->sensors = Globals::LinSensors;
	//LCard Козлы!!!
	lpop->InitADC(16 , 0.0 , "PP" , 0);
	lpop->Start();
	TThread::CurrentThread->Sleep(50);
	lpop->Stop();

// иниициализируем сбор данных с LCard
	ProtocolForm->SendToProtocol("Работа (Д): инициализируем сбор данных с LCard");
	lpop->InitADC(16 , 0.0 , "PP" , 2);			// Mode: 0-дефектоскопия, 1 - ГП, 2 - ВСЁ
	if ( Linear )
		lprod->InitADC(resprod->sensors,0.0, "PR", 0);

// ждем сигнала Готовность от толщиномера
	digital->SetOutSignal("Работа");
	if ( Thick )
	{
		stext2="Ждем готовности толщиномера";
		Synchronize( &UpdateStatus );           // ИНВЕРТИРОВАН!!!
		if ( !digital->WaitForDropInSignal("Готовность",30000, TThread::CurrentThread))
			return "Не дождались готовности толщиномера";
	}

// крутим продольный
	if ( Linear )
	{
		digital->SetOutSignal("Прод Пит ПЧ");
		digital->SetOutSignal("Прод STF");
		for (UINT s = 0; s < Globals::speeds.inSpeed.size(); s++)
			digital->SetOutSignal("Прод Скорость " + IntToStr(Globals::speeds.inSpeed[s]));
		if ( !digital->WaitForDropInSignal("Прод ПЧ RUN", 3000 , TThread::CurrentThread) )
			return "Не достигнута скорость вращения продольного модуля!";
	}

// ждем вращения толщиномера
	digital->SetOutSignal("Цикл");
	if ( Thick )
	{
		stext2="Ждем вращения толщиномера";
		Synchronize( &UpdateStatus );		// Инвертирован !!!
		if ( !digital->WaitForDropInSignal("Контроль",10000, TThread::CurrentThread))
			return "Не дождались вращения толщиномера";
		lat->SetControl();
	}

// профилактическая продувка дефектоотметчиков
	digital->BlinkOutSignal("Дефектоотметчик" , 50);

// включаем движение трубы вперед
	digital->SetOutSignal("ТС: Пит ПЧ");
	for (UINT s = 0; s < Globals::speeds.workSpeed.size(); s++)
		digital->SetOutSignal("ТС: Скорость " + IntToStr(Globals::speeds.workSpeed[s]));
	digital->SetOutSignal("ТС: STF");
	if ( !digital->WaitForDropInSignal("ТС: ПЧ RUN",3000, TThread::CurrentThread) )
		return "Не достигнута скорость вращения транспорта";

// выставляем сигналы в цех, труба поехала
	stext2 = "Труба едет вперед";
	ProtocolForm->SendToProtocol("Работа (Д): " + stext2);
	Synchronize( &UpdateStatus );
	digital->SetOutSignal("Цех вперед");
	if (digital->SignalExists("Цех движение"))
		digital->SetOutSignal("Цех движение");

	return "ok";
}

//-------------------------------------------------------------------------------
//----онлайн цикл, крутящийся бесконечно и проверяющий все события---------------
bool ThreadOnLine::OnlineCycle()
{
	// флаги событий во время кругового цикла
	bool LinWorkSpeed = false;		// смена на рабочую скорость
	bool LinOutSpeed = false;		// смена на выходную скорость
	bool InSG = false;				// труба зашла в размагничивание
	bool EndCross = false;			// труба вышла из поперечного
	bool ThickFinished = false;		// труба вышла из толщиномера
	bool TubeQuit = false;			// труба вышла с 4 стойки
	bool result;					// результат всего цикла
	bool Poper = false;				// замкнут Попер Вход (измерения)
	bool Prod = false;				// замкнут Прод вход (измерения)
	bool Tolsh = false;				// замкнут Толщ вход (выдача Синхро)
	DWORD C1Start = 0;				// старт таймера по смене счетчика 1
	int plantMark = -10;			// зона реза по шаблонированию в пересчете на нашу трубу
	int trash=16-( (Globals::current_diameter == 60) ? (10) : (12) );
	short ppChannel = 16;			//кол-во каналов ПП
	short prChannel = Globals::LinSensors;	//Кол-во каналов ПрП
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	unsigned markTime = ini->ReadInteger( "LIR" , "MarkerTime" , 100 );		// время дефектоотметки
	bool mark = ! (ini->ReadBool( "LIR" , "NoMarking" , 0));		 	// не маркировать!
	delete ini;
	PoperStart =  200000;				// делаем старт большим, чтобы не собирать до трубы
	PoperFinish = 200000;				// делаем финиш большим, чтобы считать ppOnline
	ProdStart =   200000;               // делаем старт большим, чтобы не собирать до трубы
	ProdFinish =  200000;               // делаем финиш большим, чтобы считать prOnline
	TolshStart =  200000;               // делаем старт большим, чтобы не выдавать Синхро раньше времени
	TolshFinish = 200000;               // делаем финиш большим, чтобы считать thOnline
	MarkStart =   200000;				// делаем старт большим, чтобы не маркировать раньше времени
	MarkFinish =  200000;               // делаем финиш большим, чтобы считать markOnline
	bool poperLast = false;				// добавление послденей (неполной) зоны по поперечному
	bool prodLast = false;				// добавление полсденей (неполной) зоны по продольному
	bool tolshLast = false;				// добавление последней (неполной) зоны по толщиномеру
	long startScan = 0;					// начало скана (пробежки всего цикла)
	long endScan = 0;                   // конец скана
	long scanTime = 0;                  // время скана
	long scans = 0;                     // число сканов за цикл
	long biggestScan = 0;				// самый долгий скан
	St2in = 0;
	St4in = 0;
	St1out = 0;
	St3out = 0;
	St4out = 0;
	GPout = 0;
	DWORD syncSetTime = 0;				// время выставления текущего сигнала Синхро
	bool syncSet = false;				// сигнал Синхро выставлен
	DWORD markSetTime = 0;				// время выставления текущего сигнала Дефектоотметчик
	bool markSet = false;				// сигнал Дефектоотметчик выставлен
	int drebezgGP = 0;					// дребезжит сигнал ГП труба, сука
	ProtocolForm->SendToProtocol("Заходим в круговой цикл");

	while(Collect)
	{
        startScan = GetTickCount();
		CalcOnlineZones();
// ждем, когда стартануть сбор данных
		if ( ppOnline == 0 && !lpop->getStarted() && digital->CheckInSignal("Попер вход") && Poper)
		{
			lpop->Start();
			ProtocolForm->SendToProtocol("Работа (Д): Старт сбора Поперечного");
		}
		if ( prOnline == 0 && !lprod->getStarted() && digital->CheckInSignal("Прод вход") && Linear && Prod)
		{
			digital->SetOutSignal("Прод пит су");
			lprod->Start();
			ProtocolForm->SendToProtocol("Работа (Д): Старт сбора Продольного");
		}
//		TThread::CurrentThread->Sleep(1);

//**************************************************************************
//**************Обсчитываем поперечную плату********************************
		if( ppOnline > ppOld && (digital->CheckInSignal("Попер вход") || digital->CheckInSignal("Попер выход")) )
		{
			ppOld = ppOnline;
			int Part = lpop->part;                             //столько измерений в одной части
			ppNumber = lpop->GetNum();					    	 //кол-во собранных частей
			ppSecond = ppNumber * Part;                          //всего собранно данных на данный момент
			meas_per_sensor = (ppSecond-ppFirst)/ppChannel;      //кол-во измерений в зоне по одному датчику
			memcpy(&Buffer[0],&lpop->Base[ppFirst],sizeof(ULONG)*(ppSecond-ppFirst));   //копируем  из буфера LCard в наш промежуточный буфер
			int p=0;
			ppData.set_length(ppChannel-trash);                                       //кол-во датчиков дефектоскопии
			for(int i=0; i < ppData.Length; i++)
				ppData[i].set_length(meas_per_sensor);

			for( UINT i=0; i < ppSecond - ppFirst; i+=ppChannel )         //всего каналов
			{
				for(int j=0; j < ppChannel-trash; j++)                    //каналов по дефектоскопии
				{
					if (p >= ppData[j].Length)
						break;

					ppData[ j ][ p ] = short ( Buffer[i+j] ) ;
				}

				p++;
			}
			Synchronize(DrawResultPP);
			ProtocolForm->SendToProtocol("Работа (Д): зона =Попер=" + IntToStr(ppOnline) );
			ppFirst = ppSecond;
			CalcOnlineZones();
		 }
//**************Последняя зона поперечного (неполная)********************************
		 if (TotalCounter >= PoperFinish && !poperLast)
		 {
			poperLast = true;
			int Part = lpop->part;                             //столько измерений в одной части
			ppNumber = lpop->GetNum();					    	 //кол-во собранных частей
			ppSecond = ppNumber * Part;                          //всего собранно данных на данный момент
			memcpy(&Buffer[0],&lpop->Base[ppFirst],sizeof(ULONG)*(ppSecond-ppFirst));   //копируем  из буфера LCard в наш промежуточный буфер
			ppData.set_length(ppChannel-trash);                                       //кол-во датчиков дефектоскопии
			meas_per_sensor = respop->total_meas / respop->zones;	// число измерений берется как среднее по всей трубе
			meas_per_sensor -= meas_per_sensor % ppChannel;			// чтобы делилось на 16
			unsigned meas = meas_per_sensor * ppChannel;			// весь массив данных, в том числе фантомных
			int good = (ppSecond-ppFirst)/ppChannel;    			// число хороших измерений
			int p = 0;
			for(int i=0; i < ppData.Length; i++)
				ppData[i].set_length(meas_per_sensor);

			for( UINT i = 0; i < meas; i += ppChannel )         	//  ppSecond - ppFirst
			{                                                       // всего каналов = ppChannel
				for(int j = 0; j < ppChannel-trash; j++)            //каналов по дефектоскопии
				{
					if (p >= good )						// если заполнили все нормальные значения
						ppData[ j ][ p ] = 0 ;
					else
						ppData[ j ][ p ] = short ( Buffer[i+j] ) ;
				}
				p++;
			}
			Synchronize(DrawResultPP);
			ProtocolForm->SendToProtocol("Работа (Д): последняя зона =Попер=" );
		 }
//		TThread::CurrentThread->Sleep(1);

//******************************************************************************
//*****************Обсчитываем продольную плату*********************************
		if( prOnline == (prOld + 1) && (digital->CheckInSignal("Прод вход") || digital->CheckInSignal("Прод выход")) )
		{
			prOld = prOnline;
			int prPart = lprod->part;                          //Кол-во отсчетов в одной части
			prNumber = lprod->GetNum();                        //Сколько собрали кусков на данный момент
			prSecond = prNumber*prPart;                       //всего собранно данных на данный момент
			meas_per_sensor=(prSecond-prFirst)/prChannel;	//кол-во измерений в зоне по одному датчику
			//копируем  из буфера LCard в наш промежуточный буфер
			memcpy(&Buffer[0],&lprod->Base[prFirst],sizeof(ULONG)*(prSecond-prFirst));
			int p=0;
			prData.set_length(prChannel);
			for(int i=0; i < prData.Length; i++)                                //кол-во датчиков дефектоскопии
				prData[i].set_length(meas_per_sensor);

			for(UINT i=0; i < prSecond-prFirst; i+=prChannel)                   //всего каналов
			{
				for(int j=0; j<prChannel; j++)                                  //каналов по дефектоскопии
				{
					if(p >= prData[j].Length)
						break;
					prData[ j ][ p ] = short(Buffer[i+j] );
				}
				p++;
			}
			//здесь отрисовка и подсчет результата
			Synchronize(DrawResultPR);
			ProtocolForm->SendToProtocol("Работа (Д): зона =Прод="  + IntToStr(prOnline) );
			//запоминаем кол-во отсчетов, как начало следующей зоны
			prFirst =prSecond;
			CalcOnlineZones();
		}
//*****************Последняя зона продольного (неполная)*********************************
		if (TotalCounter >= ProdFinish && !prodLast)
		{
        	prodLast = true;
			int prPart = lprod->part;                          //Кол-во отсчетов в одной части
			prNumber = lprod->GetNum();                        //Сколько собрали кусков на данный момент
			prSecond = prNumber*prPart;                       //всего собранно данных на данный момент
			memcpy(&Buffer[0],&lprod->Base[prFirst],sizeof(ULONG)*(prSecond-prFirst));
			int p=0;
			prData.set_length(prChannel);
			meas_per_sensor = resprod->total_meas / resprod->zones;	// число измерений берется как среднее по всей трубе
			meas_per_sensor -= meas_per_sensor % prChannel;			// чтобы делилось на 8
			unsigned meas = meas_per_sensor * prChannel;			// весь массив данных, в том числе фантомных
			int good = (prSecond-prFirst)/prChannel;	//кол-во измерений в зоне по одному датчику

			for(int i=0; i < prData.Length; i++)                                //кол-во датчиков дефектоскопии
				prData[i].set_length(meas_per_sensor);

			for(UINT i = 0; i < meas; i += prChannel)                   //всего каналов
			{
				for(int j=0; j < prChannel; j++)                                  //каналов по дефектоскопии
				{
					if(p >= good)
						prData[ j ][ p ] = 0;
					else
						prData[ j ][ p ] = short(Buffer[i+j] );
				}
				p++;
			}
			Synchronize(DrawResultPR);
			ProtocolForm->SendToProtocol("Работа (Д): последняя зона =Прод=");
		}
//		TThread::CurrentThread->Sleep(1);

//******************************************************************************
//***** Выдаем сигнал Синхро в Толщиномер, ждем результата от него**************
		if ( thOnline > thOld  && (digital->CheckInSignal("Толщ вход") || digital->CheckInSignal("Толщ выход")) )
		{
			thOld = thOnline;
			digital->SetOutSignal("Синхро");
			syncSetTime = GetTickCount();
			syncSet = true;
			ProtocolForm->SendToProtocol("Работа (Д): зона =Толщ= " + IntToStr(thOnline));
			CalcOnlineZones();
		}
		if (TotalCounter >= TolshFinish && !tolshLast)
		{
			tolshLast = true;
			digital->SetOutSignal("Синхро");
			syncSetTime = GetTickCount();
			syncSet = true;
			ProtocolForm->SendToProtocol("Работа (Д): последняя зона =Толщ= ");
		}
		DWORD sync = GetTickCount();
		if ( (sync - syncSetTime > SynchroTime) && syncSet)
		{
			digital->ResetOutSignal("Синхро");
			syncSet = false;
		}

//******************************************************************************
//***** Дефектоотметка бракованных участков*************************************
//		CalcOnlineZones();
		if ( (markOnline == markOld + 1) && (digital->CheckInSignal("гп труба") || digital->CheckInSignal("Марк вход")))
		{
			markOld = markOnline;
			if ( ressum->zone_data[markOnline] == 0 && markOnline < ressum->zones && mark )
			{
				digital->SetOutSignal("Дефектоотметчик" );
				markSetTime = GetTickCount();
				markSet = true;
				ProtocolForm->SendToProtocol("Промаркирована зона " + IntToStr(markOnline));
			}
			else if (markOnline == plantMark && plantCut > 0)
			{
				digital->BlinkOutSignal("Дефектоотметчик" , 3*markTime);
				ProtocolForm->SendToProtocol("Промаркирован рез шаблона " + IntToStr(markOnline));
			}
		}
		DWORD markLast = GetTickCount();
		if ( (markLast - markSetTime > markTime ) && markSet )
		{
			digital->ResetOutSignal("Дефектоотметчик");
			markSet = false;
		}
		TThread::CurrentThread->Sleep(1);

// труба зашла в Попер, начинаем отсчет мини - отступа перед стартом
		if ( !Poper && digital->CheckInSignal("Попер вход")  )
		{
			PoperStart = TotalCounter;
			Poper = true;
			ProtocolForm->SendToProtocol("Замкнут Попер вход");
		}

// труба вышла из входа в Попер, начинаем отсчет мини-отступа перед стопом
		if ( Poper && current_counter >=1 && !digital->CheckInSignal("Попер вход") )
		{
			PoperFinish = TotalCounter + ppOffset[current_counter];
			Poper = false;
			ProtocolForm->SendToProtocol("Разомкнут Попер вход");
		}

// труба зашла в Прод, начинаем отсчет мини - отступа перед стартом
		if ( !Prod && digital->CheckInSignal("Прод вход")  )
		{
			ProdStart = TotalCounter;
			Prod = true;
			ProtocolForm->SendToProtocol("Замкнут Прод вход");
			digital->SetOutSignal("Прод пит су");
		}

// труба вышла из входа в Прод, начинаем отсчет мини-отступа перед стопом
		if ( Prod && current_counter >=1 && !digital->CheckInSignal("Прод вход") )
		{
			ProdFinish = TotalCounter + prOffset[current_counter];
			Prod = false;
			ProtocolForm->SendToProtocol("Разомкнут Прод вход");
		}

// труба зашла в Толщ, начинаем отсчет мини - отступа перед стартом
		if ( !Tolsh && digital->CheckInSignal("Толщ вход" ) )
		{
			TolshStart = TotalCounter;
			Tolsh = true;
			ProtocolForm->SendToProtocol("Замкнут Толщ вход");
		}

// труба вышла из входа в Толщ, начинаем отсчет мини-отступа перед стопом
		if ( Tolsh && current_counter >=1 && !digital->CheckInSignal("Толщ вход") )
		{
			TolshFinish = TotalCounter + thOffset[current_counter];
			Tolsh = false;
			ProtocolForm->SendToProtocol("Разомкнут Толщ вход");
		}

// смотрим наличие трубы в ГП
// труба зашла в Дефектотметку, начинаем отсчет мини - отступа перед стартом
		if ( !InSG && digital->CheckInSignal("гп труба") )
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
			ProtocolForm->SendToProtocol("Замкнут ГП труба (для дефектотметки)");
//			ProtocolForm->SendToProtocol("Труба зашла в размагничивание");
		}

// труба вышла из Дефектоотметки, начинаем отсчет мини-отступа перед стопом
		if ( InSG && current_counter == 2 && !digital->CheckInSignal("гп труба") )
		{
				// костыль для проекта - оба датчика на 1 входе
			MarkFinish = TotalCounter + markOffset[current_counter] + 3*imp_per_zone[current_counter];
			InSG = false;
			GPout = Counter->GetCounter(2);
			ProtocolForm->SendToProtocol("Разомкнут ГП труба (для дефектоотметки)");
		}

// вычисляем длину трубы с помощью сигнала Цех База, просчитываем наш рез по шаблону
		if ( plantCut > 0 && plantMark < 0 && !digital->CheckInSignal("Цех База") )
		{
			double tube_len = ( 6330.0 / 200.0 ) + (float)Counter->GetCounter(0) / (float)imp_per_zone[0];
			int tube_length_zone = (int)(tube_len);
			plantMark = tube_length_zone - plantCut;
			ProtocolForm->SendToProtocol("Зона реза по шаблону: " + IntToStr(plantMark));
			ProtocolForm->SendToProtocol("Длина трубы в зонах: " + IntToStr(tube_length_zone));
			if (plantMark < 0)		// тесты
				plantMark = 100;
		}

// смотрим, что больше не нужно контролировать сигнал Контроль из толщиномера
		if ( !ThickFinished && current_counter >= 1 && !digital->CheckInSignal("Толщ выход") )
		{
			lat->ResetControl();
			ProtocolForm->SendToProtocol("Труба вышла из толщиномера");
			ThickFinished = true;
		}

// смотрим, что труба вышла из поперечного
/*		if ( !EndCross && current_counter >= 1 && !digital->CheckInSignal("Попер Выход") )
		{
			MeasOutCrossModule = lpop->Num;  //Столько данных было собранно на момент выхода из попер модуля
			digital->ResetOutSignal("Соленоид питание");
			ProtocolForm->SendToProtocol("Труба вышла из поперечного");
			EndCross = true;
		}
*/
// смотрим, что надо задать рабочую скорость вращения у продольного
		if ( !LinWorkSpeed && digital->CheckInSignal("Прод выход") )
		{
			digital->ResetOutSignal("Прод скорость 1");
			digital->ResetOutSignal("Прод скорость 2");
			digital->ResetOutSignal("Прод скорость 3");
			for (UINT s = 0; s < Globals::speeds.linearSpeed.size(); s++)
				digital->SetOutSignal("Прод Скорость " + IntToStr(Globals::speeds.linearSpeed[s]));
			ProtocolForm->SendToProtocol("Рабочая скорость вращения продольного");
			LinWorkSpeed = true;
		}

// смотрим, что надо задать Выходную скорость вращения у продольного
		if ( !LinOutSpeed && Linear && current_counter >= 1 && !digital->CheckInSignal("Стойка 3") )
		{
			St3out = Counter->GetCounter(1);
			digital->ResetOutSignal("Прод скорость 1");
			digital->ResetOutSignal("Прод скорость 2");
			digital->ResetOutSignal("Прод скорость 3");
			for (UINT s = 0; s < Globals::speeds.outSpeed.size(); s++)
				digital->SetOutSignal("Прод Скорость " + IntToStr(Globals::speeds.outSpeed[s]));
			ProtocolForm->SendToProtocol("Выходная скорость вращения продольного");
			LinOutSpeed = true;
		}

// смотрим, что надо переменить счетчик на 1
		if ( C1Start == 0 && current_counter == 0 && digital->CheckInSignal("Стойка 4")  )
		{
			C1Start = GetTickCount();
			digital->SetOutSignal("База");
			St4in = Counter->GetCounter(0);
			ProtocolForm->SendToProtocol("Начали отсчет счетчика 1");
		}
		DWORD now = GetTickCount();
		if ( current_counter == 0 && ((now - C1Start) > Counter1Delay) && digital->CheckInSignal("Стойка 4") )
		{
			Counter0Fin = Counter->GetCounter(0);
			Counter->Reset(1);
			current_counter = 1;
//			CalcOnlineZones();
			ProtocolForm->SendToProtocol("Счетчик 1 активен");
		}

// смотрим, что надо переменить счетчик на 2
		if ( current_counter == 1 && !digital->CheckInSignal("Стойка 3") && digital->CheckInSignal("Марк вход"))
		{
			Counter1Fin = Counter->GetCounter(1);
			Counter->Reset(2);
			current_counter = 2;
//			CalcOnlineZones();
			ProtocolForm->SendToProtocol("Счетчик 2 активен");
		}

// остановим платы
		if ( current_counter == 2 && !digital->CheckInSignal("ГП труба") && lpop->getStarted() )
		{
			lpop->Stop();
			ProtocolForm->SendToProtocol("Работа (Д): Стоп сбора Поперечного");
		}
		if ( current_counter == 2 && !digital->CheckInSignal("Прод вход") && !digital->CheckInSignal("Прод выход") && lprod->getStarted() )
		{
			lprod->Stop();
			digital->ResetOutSignal("Прод пит су");
			ProtocolForm->SendToProtocol("Работа (Д): Стоп сбора Продольного");
		}

// смотрим, что труба вышла из установки - осталось только ГП и маркировка
		if ( !TubeQuit && current_counter == 2 && !digital->CheckInSignal("Стойка 4") )
		{
			St4out = Counter->GetCounter(2);
			digital->ResetOutSignal("соленоид охлажд");
			digital->ResetOutSignal("Соленоид питание");
			digital->ResetOutSignal("ТС: STF");
			digital->ResetOutSignal("Прод STF");
			digital->ResetOutSignal("Прод пит су");
//			digital->ResetOutSignal("Работа");
			digital->ResetOutSignal("База");
			ProtocolForm->SendToProtocol("Труба вышла со стойки 4");
			respop->CalculateDeadBack(200);
			resprod->CalculateDeadBack(200);
		 	ressum->ComputeZonesData(*respop,*resprod,*resth, true, true);
			Synchronize(RedrawGraphics);
			TubeQuit = true;
		}

// калибровка ЛИРов - ждем Стойка 2 и пропадания Стойка 1
		if ( St2in == 0 && digital->CheckInSignal("Стойка 2") )
		{
        	St2in = Counter->GetCounter(0);
		}
		if( St1out == 0 && current_counter == 1 && !digital->CheckInSignal("Стойка 1") )
		{
        	St1out = Counter->GetCounter(1);
		}

// хорошее окончание цикла, все прошло успешно
		if ( current_counter == 2 && !digital->CheckInSignal("Марк вход") )
		{
			Collect = false;
			result = true;
			ProtocolForm->SendToProtocol("Работа (Д): Закончили цикл");
		}

// смотрим, не было ли сброса
		if (Terminated)
		{
			Collect = false;
			result = false;
			ProtocolForm->SendToProtocol("Работа (Д): Зашли в Terminated");
		}
//		Synchronize(OnlineZones);	// посмотрим, что там
		endScan = GetTickCount();
		scanTime += (endScan - startScan);
		if (endScan - startScan > biggestScan)
			biggestScan = endScan - startScan;
		scans++;
	}
	double average = (double)scanTime / (double) scans;
	ProtocolForm->SendToProtocol("Среднее время скана: " + FloatToStrF(average , ffFixed , 3,2));
	ProtocolForm->SendToProtocol("Наибольшее время скана: " + FloatToStrF(biggestScan , ffFixed , 5,4));
	if ( result )
		Calibrate();
	return result;
}
//---------------------------------------------------------------------------
void ThreadOnLine::Finally()
{
	if ( !Terminated )		// если выпали из цикла сами, а не по аварии или желанию юзера
	{
		lat->ResetPlantCycle();
		lat->ResetControl();
		lat->ResetStand();
	}
	// сбросим управляющие сигналы
	digital->ResetOutSignal("соленоид охлажд");
	digital->ResetOutSignal("соленоид питание");
	digital->ResetOutSignal("ТС: Скорость 1");
	digital->ResetOutSignal("ТС: Скорость 2");
	digital->ResetOutSignal("ТС: Скорость 3");
	digital->ResetOutSignal("ТС: STF");
//	digital->ResetOutSignal("Стойка Авто");
//	digital->ResetOutSignal("Схват Авто");
	digital->ResetOutSignal("Работа");
	digital->ResetOutSignal("Цикл");
	digital->ResetOutSignal("База");

	digital->ResetOutSignal("Цех вперед");
	if (digital->SignalExists("Цех движение"))
		digital->ResetOutSignal("Цех движение");

	if ( Linear )
	{
		digital->ResetOutSignal("Прод STF");
		digital->ResetOutSignal("Прод Скорость 1");
		digital->ResetOutSignal("Прод Скорость 2");
		digital->ResetOutSignal("Прод Скорость 3");
		digital->ResetOutSignal("Прод пит су");
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

//	MainForm->Memo1->Lines->Add("Отступ Толщины: " + IntToStr((__int64)thOffset[current_counter]));
//	MainForm->Memo1->Lines->Add("Отступ Попера: " + IntToStr((__int64)ppOffset[current_counter]));
//	MainForm->Memo1->Lines->Add("Отступ Прода: " + IntToStr((__int64)prOffset[current_counter]));
//	MainForm->Memo1->Lines->Add("Отступ Дефера: " + IntToStr((__int64)markOffset[current_counter]));
//	MainForm->Memo1->Lines->Add("Импульсов в зоне: " + IntToStr((__int64)imp_per_zone[current_counter]));
//	MainForm->Memo1->Lines->Add("Счетчик1: " + IntToStr((__int64)CounterValue));
//	MainForm->Memo1->Lines->Add("Счетчик2: " + IntToStr((__int64)Counter2));
//	MainForm->Memo1->Lines->Add("Счетчик3: " + IntToStr((__int64)Counter3));
	MainForm->Memo1->Lines->Add("Зона Толщины: " + IntToStr(thOnline));
	MainForm->Memo1->Lines->Add("Зона Попера: " + IntToStr(ppOnline));
	MainForm->Memo1->Lines->Add("Зона Прода: " + IntToStr(prOnline));
	MainForm->Memo1->Lines->Add("Зона Дефера: " + IntToStr(markOnline));
	MainForm->Memo1->Lines->Add("Счетчик : " + IntToStr(current_counter));
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
	//Расчет и вывод данных ПП
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
	//Расчет и вывод данных ПрП
	resprod->AddZone(prData);
	resprod->PutResultOnChart(MainForm->LinearDefectChart, MainForm);
	// расчет данных и вывод суммарного результата
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
	double ppO = 0.0;		// временные переменные для рассчет онлайн зоны
	double prO = 0.0;
	double thO = 0.0;
	double markO = 0.0;
	// в пыть-яхе, в других проектах рассчет будет иной!!!
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
	ProtocolForm->SendToProtocol("Калибровка: ЛИР1 - " + FloatToStrF(lir1_len , ffFixed , 6,3)+
									 " ЛИР2 - " + FloatToStrF(lir2_len , ffFixed , 6, 3)+
									 " ЛИР3 - " + FloatToStrF(lir3_len , ffFixed , 6, 3)	);

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

