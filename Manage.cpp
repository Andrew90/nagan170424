//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

//using Microsoft::ManagementConsole::Advanced;


#include "Manage.h"
#include "Global.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TManageForm *ManageForm;
//---------------------------------------------------------------------------
__fastcall TManageForm::TManageForm(TComponent* Owner)
	: TForm(Owner)
{
// задаем имена для кнопок и состояний, картинки и цвета
	centr_on = "Сжат";
	centr_off = "Разжат";
	centr_close = "Сжать";
	centr_open = "Разжать";
	centr_cl = new Graphics::TBitmap();
	centr_op = new Graphics::TBitmap();

	KeyPreview = true;

	mod_on = "В работе";
	mod_off = "Обслуживание";
	mod_mov = "Перемещается";
	mod_work = "Включить";
	mod_service = "Отключить";
	mod_dsbl = "Недоступно";

	if (SystemConst::ExternalTransport) // GAN *********************
	{
		load_Up = "Вверху";
		load_Down = "Внизу";
		load_mow = "Перемещается";
		load_st = "Перекладчик ";

		load_work = "Недоступно";

		load_onon = "запрещено!";
		load_offoff = "разрешено!";

		load_UP = new Graphics::TBitmap();
		load_Dwn = new Graphics::TBitmap();
		load_mvg = new Graphics::TBitmap();
	}

	mod_serv = new Graphics::TBitmap();
	mod_wrk = new Graphics::TBitmap();
	mod_mvg = new Graphics::TBitmap();
	try
	{
		mod_serv ->LoadFromFile(  "..\\..\\Pix\\Module-Service.bmp");
		mod_wrk ->LoadFromFile(  "..\\..\\Pix\\Module-Work.bmp");
		mod_mvg ->LoadFromFile(  "..\\..\\Pix\\Module-Move.bmp");
		centr_cl ->LoadFromFile(  "..\\..\\Pix\\Centralizer-close.bmp");
		centr_op ->LoadFromFile(  "..\\..\\Pix\\Centralizer-open.bmp");
		is_pix_ok = true;
	}
	catch (...)
	{
		is_pix_ok=false;
		Application->MessageBoxW(L"Изображения для динамических кнопок не найдены. \n Ошибка 500",L"Ошибка",MB_ICONERROR);
	}


// смотрим, какие устройства доступны для управления
		this->lThicknessState->Visible = SystemConst::ThickModuleManage;
		this->pThicknessState->Visible = SystemConst::ThickModuleManage;
		this->bThicknessState->Visible = SystemConst::ThickModuleManage;

		this->lThicknessCentralizer->Visible = SystemConst::ThickCentralizer;
		this->pThicknessCentralizer->Visible = SystemConst::ThickCentralizer;
		this->bThicknessCentralizer->Visible = SystemConst::ThickCentralizer;

		this->lLinearCentralizer->Visible = SystemConst::LinearCentralizer;
		this->pLinearCentralizer->Visible = SystemConst::LinearCentralizer;
		this->bLinearCentralizer->Visible = SystemConst::LinearCentralizer;

		this->gbTransport->Visible = SystemConst::TransportManage;
		this->gbCounters->Visible = SystemConst::IsOnline;

		this->GroupPerekl->Visible = SystemConst::ExternalTransport;
}
//---------------------------------------------------------------------------
void TManageForm::SendHandleToSignals(LCard791 &lcard)
{
	lt = &lcard;
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::FormShow(TObject *Sender)
{
// читаем цвета из файла
	TIniFile *ini = new TIniFile(Globals::IniFileName);

	on = ini->ReadInteger( "Color" , "ActiveIn" , 0 );
	off = ini->ReadInteger( "Color" , "NotActiveIn" , 0 );
	move = ini->ReadInteger( "Color" , "NotActiveOut" , 0 );

	delete ini;

// включаем таймер, отслеживающий состояние входов и выходов
	this->InputTimer->Enabled=true;
	butt_enabled = true;
	this->StatusBarBottom->Panels->Items[0]->Text="";
	this->StatusBarBottom->Refresh();
	TemperatureTimer->Enabled = true;

}
//---------------------------------------------------------------------------
void __fastcall TManageForm::FormClose(TObject *Sender, TCloseAction &Action)
{
// выключаем таймер, отслеживающий состояние входов и выходов
	InputTimer->Enabled = false;
	TemperatureTimer->Enabled = false;
// сбрасываем сигналы, если пользователь забыл об этом
	digital -> ResetOutSignal("Соленоид Охлажд");
	digital -> ResetOutSignal("Соленоид Питание");
//	digital -> ResetOutSignal("прод пит пч");
	digital -> ResetOutSignal("прод скорость 1");
	digital -> ResetOutSignal("прод скорость 2");
	digital -> ResetOutSignal("прод скорость 3");
	digital -> ResetOutSignal("прод STF");
	bStopClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TManageForm::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	if (Key == 27)
		ManageForm->Close();
}
//---------------------------------------------------------------------------
void TManageForm::ShowCentralizerOpened(TBitBtn *btn, TPanel *pan)
{
// схват разжат, задаем соответствующие параметры объектам, связанным с ним
	pan->Color = off;
	pan->Caption = centr_off;
	btn->Caption = centr_close;
	btn->Glyph = centr_cl;
}
//---------------------------------------------------------------------------

void TManageForm::ShowCentralizerClosed(TBitBtn *btn, TPanel *pan)
{
// схват сжат, задаем соответствующие параметры объектам, связанным с ним
	pan->Color = on;
	pan->Caption = centr_on;
	btn->Caption = centr_open;
	btn->Glyph = centr_op;
}
//---------------------------------------------------------------------------
void TManageForm::ShowModuleinWork(TBitBtn *btn, TPanel *pan)
{
// модуль в работе , задаем соответствующие параметры объектам, связанным с ним
	pan->Color = on;
	pan->Caption = mod_on;
	btn->Caption = mod_service;
	btn->Glyph = mod_serv;

	if ( butt_enabled )
		btn->Enabled = true;
	else
		btn->Enabled = false;
}
//---------------------------------------------------------------------------
void TManageForm::ShowModuleinService(TBitBtn *btn, TPanel *pan)
{
// модуль в положении обслуживания , задаем соответствующие параметры объектам, связанным с ним
	pan->Color = off;
	pan->Caption = mod_off;
	btn->Caption = mod_work;
	btn->Glyph = mod_wrk;

	if ( butt_enabled )
		btn->Enabled = true;
	else
		btn->Enabled = false;
}
//---------------------------------------------------------------------------

void TManageForm::ShowModuleMoving(TBitBtn *btn, TPanel *pan)
{
// модуль перемещается из одного положения в другое , задаем соответствующие параметры объектам, связанным с ним
	pan->Color = move;
	pan->Caption = mod_mov;
	btn->Enabled = false;
	btn->Caption = mod_dsbl;
	btn->Glyph = mod_mvg;
}
//---------------------------------------------------------------------------
void TManageForm::ShowModuleState( byte state, TBitBtn *btn, TPanel *pan )
{
	switch (state)
	{
		case 0:
			ShowModuleMoving(btn, pan);
			break;
		case 1:
			ShowModuleinWork(btn, pan);
			break;
		case 2:
			ShowModuleinService(btn, pan);
			break;
	}
}

//---------------------------------------------------------------------------
void TManageForm::ShowLoaderUp(TBitBtn *btn, TPanel *pan)
{
	pan->Color = off;                      //  GAN ****************
	pan->Caption = load_Up;
	btn->Glyph = load_UP;
	btn->Caption = load_st + IntToStr(btn->Tag)+" работа";
}
//---------------------------------------------------------------------------
void TManageForm::ShowLoaderDown(TBitBtn *btn, TPanel *pan)
{
	pan->Color = on;                      //  GAN ****************
	pan->Caption = load_Down;
	btn->Glyph = load_Dwn;
	btn->Caption = load_st + IntToStr(btn->Tag)+" работа";
}
//---------------------------------------------------------------------------
void TManageForm::ShowLoaderMoving(TBitBtn *btn, TPanel *pan)
{
	pan->Color = move;                // GAN *********************
	pan->Caption = load_mow;
	btn->Caption = load_work;
	btn->Glyph = mod_mvg;
}
//---------------------------------------------------------------------------
void TManageForm::ShowLoaderState( byte state, TBitBtn *btn, TPanel *pan )
{
	switch (state)
	{                      				// GAN *********************
		case 0:
			ShowLoaderMoving(btn, pan);
			break;
		case 1:
			ShowLoaderDown(btn, pan);
			break;
		case 2:
			ShowLoaderUp(btn, pan);
			break;
	}
}
//---------------------------------------------------------------------------
void TManageForm::ShowLoadPosAllowed(TPanel *pan1, TPanel *pan2)
{
	pan1->Color = on;                // GAN *********************
	pan2->Color = on;
	pan2->Caption = load_offoff;
	pan1->Caption = "Перемещение";
}
//---------------------------------------------------------------------------
void TManageForm::ShowLoadPosDenied(TPanel *pan1, TPanel *pan2)
{
	pan1->Color = move;                // GAN *********************
	pan2->Color = move;
	pan2->Caption = load_onon;
	pan1->Caption = "Перемещение";
}
//---------------------------------------------------------------------------
void TManageForm::ShowLoadPosition( byte position, TPanel *pan1, TPanel *pan2 )
{
	switch (position)
	{                      // GAN *********************
     	case 0,1:
			ShowLoadPosAllowed(pan1, pan2);
			break;
		case 2:
			ShowLoadPosDenied(pan1, pan2);
			break;
		case 3:
			ShowLoadPosDenied(pan1, pan2);
			break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::InputTimerTimer(TObject *Sender)
{
// смотрим состояние схватов
	if (SystemConst::LinearCentralizer)
	{
		if ( digital->CheckInSignal("Прод схват исх") )
			ShowCentralizerOpened(bLinearCentralizer,pLinearCentralizer);
		else ShowCentralizerClosed(bLinearCentralizer,pLinearCentralizer);
	}
	if (SystemConst::ThickCentralizer)
	{
		if ( digital->CheckInSignal("Толщ схват исх") )
			ShowCentralizerOpened(bThicknessCentralizer,pThicknessCentralizer);
		else ShowCentralizerClosed(bThicknessCentralizer,pThicknessCentralizer);
	}
// смотрим состояние модулей и перекладчиков по более сложной схеме
	byte pp_state, pr_state, th_state, load1_state, load2_state, load1_position, load2_position; // GAN *****************
	pr_state = digital->CheckInSignal("Прод РП")  + ( ( digital->CheckInSignal("Прод ПО") ) <<1 );
	pp_state = digital->CheckInSignal("Попер РП") + ( ( digital->CheckInSignal("Попер ПО") )<<1 );

	ShowModuleState ( pr_state , bLinearState	, pLinearState		);
	ShowModuleState ( pp_state , bCrossState	, pCrossState		);

	if (SystemConst::ThickModuleManage)
	{
		th_state = digital->CheckInSignal("Толщ РП") + ( ( digital->CheckInSignal("Толщ ПО") ) <<1 );
		ShowModuleState ( th_state , bThicknessState, pThicknessState	);
	}

	if (SystemConst::ExternalTransport)   // GAN *****************
	{
		load1_state = digital->CheckInSignal("ТР перекл1 внизу") + ((digital->CheckInSignal("ТР перекл1 вверху")) <<1);  // GAN *****************
		load2_state = digital->CheckInSignal("ТР перекл2 внизу") + ((digital->CheckInSignal("ТР перекл2 вверху")) <<1);  // GAN *****************
		load1_position = digital->CheckInSignal("ТР стеллаж") + ((digital->CheckInSignal("ТР начало линии")) <<1);
		load2_position = digital->CheckInSignal("ТР конец линии") + ((digital->CheckInSignal("ТР выгружена")) <<1);
		ShowLoaderState ( load1_state , bLoader1, pLoader1 ); // GAN *****************
		ShowLoaderState ( load2_state , bLoader2, pLoader2 ); // GAN *****************
		ShowLoadPosition ( load1_position, pStillage, pBegin); // GAN *****************
		ShowLoadPosition ( load2_position, pEndLines, pNextLines); // GAN *****************
	}

// смотрим, включали мы питание соленоидов или нет
	if ( !( digital->CheckOutSignal("Соленоид Питание") ) )
		bPowerSolenoid->Kind = bkCancel;
	else
		bPowerSolenoid->Kind = bkOK;
	bPowerSolenoid->Cancel = false;
	bPowerSolenoid->Caption = "Питание";

// смотрим, включали мы охлаждение соленоидов или нет
	if ( !( digital->CheckOutSignal("Соленоид Охлажд")) )
		bCoolerSolenoid->Kind = bkCancel;
	else
		bCoolerSolenoid->Kind = bkOK;
	bCoolerSolenoid->Cancel = false;
	bCoolerSolenoid->Caption = "Охлаждение";

// смотрим, включали мы вращение продольного или нет
	if ( digital->CheckOutSignal("Прод STF"))
	{
		bRotation->Glyph = mod_mvg;
		cbRL->Enabled = false;
		cbRM->Enabled = false;
		cbRH->Enabled = false;
	}
	else
	{
		bRotation->Glyph = mod_wrk;
		cbRL->Enabled = true;
		cbRM->Enabled = true;
		cbRH->Enabled = true;
	}
// смотрим на цепи управления, если есть вращение продольного или транспорта
	if ( !digital->CheckInSignal("Цепи управления") && digital->CheckOutSignal("Прод STF"))
	{
//		digital->ResetOutSignal("Питание ПЧ");
		digital->ResetOutSignal("Прод STF");
		digital->ResetOutSignal("Прод скорость 1");
		digital->ResetOutSignal("Прод скорость 2");
		digital->ResetOutSignal("Прод скорость 3");
		StatusBarBottom->Panels->Items[0]->Text="Модуль остановлен аварийно";
	}
	if ( !digital->CheckInSignal("Цепи управления")
		&& ( digital->CheckOutSignal("ТС: STF") || digital->CheckOutSignal("ТС: STR") ) )
	{
		bStopClick(Sender);
		StatusBarBottom->Panels->Items[0]->Text="Транспорт остановлен аварийно";
	}
}
//---------------------------------------------------------------------------
void __fastcall TManageForm::bAnyCentralizerClick(TObject *Sender)
{
	TBitBtn *Item = dynamic_cast<TBitBtn *>(Sender);
	String SignalName;

	if ( Item->Name == bThicknessCentralizer->Name )
		SignalName = "Толщ Схват";	// если схват толщины
	else if ( Item->Name == bLinearCentralizer->Name)
		SignalName = "Прод Схват";	// если схват продольного

	if (Item->Caption==centr_open)
		SignalName += " разжать";		// "разжать" центратор (схват)
	else if (Item->Caption==centr_close)
		SignalName += " сжать";		// "сжать" центратор (схват)

	digital->BlinkOutSignal(SignalName);
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bAnyStateClick(TObject *Sender)
{
	TBitBtn *Item = dynamic_cast<TBitBtn *>(Sender);
	String Module;				// название модуля, с которым работаем

	if ( Item->Name == bThicknessState->Name)
		Module = "Толщ";				// если двигаем толщину

	else if ( Item->Name == bLinearState->Name)
		Module = "Прод";				// если двигаем продольный

	else if ( Item->Name == bCrossState->Name)
		Module = "Попер";				// если двигаем поперечный

	String job = Item->Caption;				// надпись на кнопке указывает на работу, которую надо проделать с модулем

	StatusBarBottom->Panels->Items[0]->Text=" ";
	StatusBarBottom->Refresh();

	// проверяем, нет ли трубы на входе и выходе из модуля
	if  ( !( (digital->CheckInBouncelessSignal(Module + " Вход",false))
			|| (digital->CheckInBouncelessSignal(Module + " Выход",false) )))
	{
		if ( job == mod_service )
			StatusBarBottom->Panels->Items[0]->Text = ModulePosition::SetModuleInService(Module);
		else if ( job == mod_work )
			StatusBarBottom->Panels->Items[0]->Text = ModulePosition::SetModuleInWork ( Module );
	}
	else
		StatusBarBottom->Panels->Items[0]->Text="Труба в модуле - действие запрещено!";
}
//---------------------------------------------------------------------------
void __fastcall TManageForm::bForwardClick(TObject *Sender)
{
// user хочет двигать трубу вперед, проверяем блокировки

	if (  ( digital->CheckInSignal("прод рп") && !digital->CheckInSignal("прод схват исх") ) ||
				( digital->CheckInSignal("толщ рп") && !digital->CheckInSignal("толщ схват исх") ) ) {
			StatusBarBottom->Panels->Items[0]->Text="Схваты не в исходном!";
	}

	if ( ( !digital->CheckInSignal("прод рп") && !digital->CheckInSignal("прод по") ) ||
				( !digital->CheckInSignal("толщ рп") && !digital->CheckInSignal("толщ по") ) ||
				( !digital->CheckInSignal("попер рп") && !digital->CheckInSignal("попер по") )  ) {
			StatusBarBottom->Panels->Items[0]->Text="Модули двигаются!";
		 }
	else
	{
		digital->SetOutSignal("Стойка Транзит");
		if ( digital->WaitForInSignal("Стойка Прогон",5000) )
		{
			digital->SetOutSignal("ТС: Пит ПЧ");
			for (int s = 0; s < Globals::speeds.transitSpeed.size(); s++)
				digital->SetOutSignal("ТС: Скорость " + IntToStr(Globals::speeds.transitSpeed[s]));
			digital->SetOutSignal("ТС: STF");
			if(digital->SignalExists("Цех вперед") )
				digital->SetOutSignal("Цех вперед");
			bBackward->Enabled = false;
			butt_enabled = false;

			if (SystemConst::ExternalTransport)     //GAN************
			{
				digital->SetOutSignal("ТР Питание ПЧ"); //GAN************
				for (int t = 0; t< Globals::speeds.transitSpeed.size(); t++)   //GAN************
					digital->SetOutSignal("ТР Скорость " + IntToStr(Globals::speeds.transitSpeed[t]));
				digital->SetOutSignal("ТР вращение вперед");   //GAN************
			}
			StatusBarBottom->Panels->Items[0]->Text="Труба движется вперед";
		}
		else
			StatusBarBottom->Panels->Items[0]->Text="Ошибка контроллера стоек!";
	}

	StatusBarBottom->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bBackwardClick(TObject *Sender)
{
// user хочет трубу назад, проверяем все блокировки
	if ( !( digital->CheckInSignal("Прод ПО")) ||
			!( digital->CheckInSignal("Толщ ПО" )) ||
			!( digital->CheckInSignal("Попер ПО" ) ) ) {
		StatusBarBottom->Panels->Items[0]->Text="Модули не в положении обслуживания!";
	}
	else if ( digital->CheckInSignal("прод вход") ||
				digital->CheckInSignal("пРод выход") ||
				digital->CheckInSignal("ТолЩ Вход")  ||
				digital->CheckInSignal("ТолЩ Выход") ||
				digital->CheckInSignal("Попер Вход") ||
				digital->CheckInSignal("ПопеР Выход") ) {
		StatusBarBottom->Panels->Items[0]->Text="Труба в модулях!";
	}
// блокировки прошли, устанавливаем режим работы контроллера стоек
	else
	{
		digital->SetOutSignal("Стойка Транзит");
		if ( digital->WaitForInSignal("Стойка Прогон",5000) )
		{
			digital->SetOutSignal("ТС: Пит ПЧ");
			for (int s = 0; s < Globals::speeds.returnSpeed.size(); s++)
				digital->SetOutSignal("ТС: Скорость " + IntToStr(Globals::speeds.returnSpeed[s]));
			digital->SetOutSignal("ТС: STR");
			if( digital->SignalExists("Цех ОП") )
				digital->SetOutSignal("Цех ОП");
			bForward->Enabled = false;
			butt_enabled = false;

			if (SystemConst::ExternalTransport)     //GAN************
			{
				digital->SetOutSignal("ТР Питание ПЧ");  //GAN************
				for (int t = 0; t < Globals::speeds.returnSpeed.size(); t++)
					digital->SetOutSignal("ТР Скорость " + IntToStr(Globals::speeds.returnSpeed[t]));
				digital->SetOutSignal("ТР вращение назад");  //GAN************
			}

			StatusBarBottom->Panels->Items[0]->Text="Труба движется назад";
		}
		else
			StatusBarBottom->Panels->Items[0]->Text="Ошибка контроллера стоек!";
	}
	StatusBarBottom->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bStopClick(TObject *Sender)
{
// сбрасываем сигналы с преобразователя частоты
//	digital->ResetOutSignal("ТС: Пит ПЧ");
	digital->ResetOutSignal("ТС: Скорость 1");
	digital->ResetOutSignal("ТС: Скорость 2");
	digital->ResetOutSignal("ТС: Скорость 3");

    if (SystemConst::ExternalTransport)
	{
		digital->ResetOutSignal("ТР Скорость 1");
		digital->ResetOutSignal("ТР Скорость 2");
		digital->ResetOutSignal("ТР Скорость 3");
		digital->ResetOutSignal("ТР вращение вперед");
		digital->ResetOutSignal("ТР вращение назад");
	}
	else
	{
		if( digital->SignalExists("Цех ОП") )
		digital->ResetOutSignal("Цех ОП");
	}

	digital->ResetOutSignal("ТС: STR");
	digital->ResetOutSignal("ТС: STF");
//сбрасываем сигналы с контроллера стоек
	digital->ResetOutSignal("Стойка Авто");
	digital->ResetOutSignal("Стойка Транзит");
	if (SystemConst::CentralizersCard)
	{
		digital->ResetOutSignal("Схват Авто");
		digital->ResetOutSignal("Схват Транзит");
	}
	bForward->Enabled = true;
	bBackward->Enabled = true;
	butt_enabled = true;

	StatusBarBottom->Panels->Items[0]->Text="Движение трубы остановлено";
	StatusBarBottom->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bPowerSolenoidClick(TObject *Sender)
{
	String s;
	if ( lt->isOkTemp("1") || lt->isOkTemp("2") )
	   //	StatusBarBottom->Panels->Items[0]->Text="Соленоиды перегреты!";
	   s = "Соленоиды перегреты!  ";

 //	else
	{
		if ( !( digital->CheckOutSignal("Соленоид Питание") ) )
		{
			digital -> SetOutSignal("Соленоид Питание");
			s += " Соленоид Включён";
		}
		else //if ( digital->CheckOutSignal("Соленоид Питание") )
		{
			digital -> ResetOutSignal("Соленоид Питание");
			s += " Соленоид ОТключён";
        }
	  	StatusBarBottom->Panels->Items[0]->Text= s;
	}
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bCoolerSolenoidClick(TObject *Sender)
{
	if ( !( digital->CheckOutSignal("Соленоид Охлажд")) )
		digital -> SetOutSignal("Соленоид Охлажд");
	else if ( digital->CheckOutSignal("Соленоид Охлажд"))
		digital -> ResetOutSignal("Соленоид Охлажд");
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::FormDestroy(TObject *Sender)
{
	delete centr_cl;
	delete centr_op;
	delete mod_serv;
	delete mod_wrk;
	delete mod_mvg;
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bRotationClick(TObject *Sender)
{
// user хочет вращать модуль продольный, проверяем блокировки
	if ( !digital->CheckInSignal("Прод РП") )
		StatusBarBottom->Panels->Items[0]->Text="Модуль не в рабочем положении!";

	else if ( !cbRL->Checked && !cbRM->Checked && !cbRH->Checked )
		StatusBarBottom->Panels->Items[0]->Text="Скорость вращения не выбрана!";

	else
	{
		if ( !digital->CheckOutSignal("Прод STF") )
		{
			digital->SetOutSignal("Прод Пит ПЧ");
			digital->SetOutSignal("Прод STF");
			if ( cbRL->Checked )
				digital->SetOutSignal("Прод скорость 1");
			if ( cbRM->Checked )
				digital->SetOutSignal("Прод скорость 2");
			if ( cbRH->Checked )
				digital->SetOutSignal("Прод скорость 3");
			StatusBarBottom->Panels->Items[0]->Text="Модуль вращается";
		}
		else if ( digital->CheckOutSignal("Прод STF") )
		{
//			digital->ResetOutSignal("Прод Пит ПЧ");
			digital->ResetOutSignal("Прод STF");
			digital->ResetOutSignal("Прод скорость 1");
			digital->ResetOutSignal("Прод скорость 2");
			digital->ResetOutSignal("Прод скорость 3");
			StatusBarBottom->Panels->Items[0]->Text="Модуль остановлен";
		}
	}
	StatusBarBottom->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::TemperatureTimerTimer(TObject *Sender)
{
	UnicodeString a;
	double buf   = lt->GetTemperature(0);
	EditT1->Text = a.sprintf(L"%0.1f", buf);
	a            = "";
	buf          = lt->GetTemperature(1);
	EditT2->Text = a.sprintf(L"%0.1f", buf);
	a            = "";
	buf          = lt->GetMagnetic();
	EditMg->Text = a.sprintf(L"%0.1f", buf);
}
//---------------------------------------------------------------------------


void __fastcall TManageForm::bStartLIRClick(TObject *Sender)
{
	lCounter = new Advantech1784(false);
	CounterTimer->Enabled = true;
	StatusBarBottom->Panels->Items[0]->Text="Счетчики запущены";
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bResetLIRClick(TObject *Sender)
{
	lCounter->Reset();
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bStopLIRClick(TObject *Sender)
{
	CounterTimer->Enabled = false;
	lCounter->Terminate();
	bool tr = lCounter->WaitFor();
	if (tr)
		StatusBarBottom->Panels->Items[0]->Text="Счетчики остановлены без ошибок";
	else if (!tr)
		StatusBarBottom->Panels->Items[0]->Text="Счтчики остановлены c ошибками";

}
//---------------------------------------------------------------------------

void __fastcall TManageForm::CounterTimerTimer(TObject *Sender)
{
	memoCounters->Clear();
	memoCounters->Lines->Add("Счетчик 1: " + IntToStr((__int64)lCounter->GetCounter(0)) );
	memoCounters->Lines->Add("Счетчик 2: " + IntToStr((__int64)lCounter->GetCounter(1)));
	memoCounters->Lines->Add("Счетчик 3: " + IntToStr((__int64)lCounter->GetCounter(2)));
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bLoader1Click(TObject *Sender)
{
//GAN************
	bLoader1->Enabled=false;
	bLoader2->Enabled=false;
	if  (pBegin->Caption=="разрешено!")
	{
		String res = ExtTransport::Load();
		if (res == "ok")
			StatusBarBottom->Panels->Items[0]->Text="Перекладчик 1 отработал верно";
		else
			StatusBarBottom->Panels->Items[0]->Text="Ошибка работы перекладчика в начале линии!";
	}
	else
		StatusBarBottom->Panels->Items[0]->Text="Ошибка работы перекладчика в начале линии!";
		StatusBarBottom->Refresh();
	bLoader1->Enabled=true;
	bLoader2->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bLoader2Click(TObject *Sender)
{
//GAN************
	bLoader1->Enabled=false;
	bLoader2->Enabled=false;
	if  (pNextLines->Caption=="разрешено!")
	{
		String res = ExtTransport::UnloadTubeOutOfLine();
		if (res == "ok")
			StatusBarBottom->Panels->Items[0]->Text="Перекладчик 2 отработал верно";
		else
			StatusBarBottom->Panels->Items[0]->Text="Ошибка работы перекладчика 2!";

	}
	StatusBarBottom->Panels->Items[0]->Text="Ошибка работы перекладчика в конце линии!";
	StatusBarBottom->Refresh();
	bLoader1->Enabled=true;
	bLoader2->Enabled=true;
}
//---------------------------------------------------------------------------

