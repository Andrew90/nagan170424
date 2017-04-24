// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Main.h"
#include "clustering.h"
//#include "About.h"
#include "ColorSettings.h"
#include "ReportsView.h"
#include "ViewEtalons.h"
#include "SMS.h"
#include "BigArray.h"
#include "PrepareSG.h"
#include "DebugMess.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "TeeGDIPlus"
#pragma link "CommPort"
#pragma resource "*.dfm"

using System::TThreadFunc;

TMainForm *MainForm;

// ---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner)
{
	ini = new TIniFile(Globals::IniFileName);
	// Коннектимся к базе данных, настройки подключения дергаем из .Ini
	int a=ini->ReadInteger("PP", "SizeBuf", 10);
	int b=ini->ReadInteger("PR", "SizeBuf", 10);
	l1 = new LCard791( 32 * 1024 * 1024 / sizeof(ULONG));
  //todo	l2 = new LCard791( b* 1024 * 1024 / sizeof(ULONG));

	digital = new AllSignals();
	digital->FindCards();

	MainForm->Tag = 0; // запуск произведен 1й раз очистка буфера не требуется
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	DragAcceptFiles(Handle, true); // Разрешаем перетаскивание файлов

	bool RHKret = RegisterHotKey(MainForm->Handle,
		// Handle окна, которому отправлять сообщения WM_HOTKEY
		0x00E, // УСЛОВНЫЙ идентификатор горячего ключа
		0, // модификатор
		VK_F1); // код клавиши

	bool RHKret1 = RegisterHotKey(MainForm->Handle,
		// Handle окна, которому отправлять сообщения WM_HOTKEY
		0x00A, // УСЛОВНЫЙ идентификатор горячего ключа
		MOD_ALT+MOD_CONTROL,// модификатор
		VK_ADD); // код клавиши

	MainFormInit(Sender);
	KeyPreview = true;
	l1->FindCard(); // Ищем платы в системе

	String s1                 = ini->ReadString("CardNames", "LCard0", "");
	String s2                 = ini->ReadString("CardNames", "LCard1", "");
	Globals::current_typesize = ini->ReadString("Default", "TypeSize", "1");
	String str                = Globals::current_typesize;

	//Считаем пороги из ini файла
	Globals::tubeSG.ReadThresholds();

	bool ll1 = ((s1 != "") ? l1->Initilalize(s1, false) : false);
  //todo	bool ll2 = ((s2 != "") ? l2->Initilalize(s2, true) : false);
 //todo	if (ll1 && ll2)
   //todo		StatusBarBottom->Panels->Items[2]->Text = "2 платы L791 загружены";
  //todo	else
		if (ll1)//todo || ll2)
			StatusBarBottom->Panels->Items[2]->Text = "1 плата L791 загружена";
		else
			StatusBarBottom->Panels->Items[2]->Text = "ПЛАТЫ L791 НЕ ЗАГРУЖЕНЫ";

	short num_of_cards                      = digital->InitializeCards(false);
	StatusBarBottom->Panels->Items[3]->Text = "Загружено плат Advantech1730: 2";// + IntToStr(num_of_cards);

	UpdateComboBox();
	LoadSettings(Sender);
	ClearCharts(Sender);

	// откроем COM порт для работы
	MyCom = new MyComPort();

	// инициализируем передачу данных между модулями по TCP/IP
	sms = new SMS (MainForm);
	sms->StartServer();

	// установи настройки групп прочности
	// SetSolidGroup(Sender);

	// смотрим, какие модули работают по умолчанию (в последний раз)
	this->cbLinear->Checked        = ini->ReadBool("Default", "IsLinear", true);
	this->cbThickness->Checked     = ini->ReadBool("Default", "IsThickness", true);
	this->cbInterruptView->Checked = ini->ReadBool("Default", "IsInterruptView", false);
	this->cbNoMarking->Checked	   = ini->ReadBool("LIR" , "NoMarking" , false);

	SystemConst::SetSystemConstants();
	SetProjectSpecialty();

	TThread::CurrentThread->NameThreadForDebugging("Main ");

	stats = new Statistics();
	UpdateStats();
	digital->SetOutSignal("ТС: Пит ПЧ");
	digital->SetOutSignal("Прод Пит ПЧ");

	Lir::Instance().Init(digital->AdvCards[0]);
   //	Lir::Instance().Start();

	if(SystemConst::ExternalTransport)
	{
		digital->SetOutSignal("Питание ПЧ ЛВ");
		ExtTransport::resultPrevious = ini->ReadInteger("SignalResult", "Previous", 1);
		ExtTransport::resultCurrent = ini->ReadInteger("SignalResult", "Current", 1);

		if(digital->CheckInSignal("Сброс") )
		{
			digital->SetOutSignal("Цех Признак");
			StatusBarBottom->Panels->Items[0]->Text = "Труба на линии выгона: Годно";
			if( ExtTransport::resultPrevious == 0)
			{
				digital->SetOutSignal("Цех Результат");
				StatusBarBottom->Panels->Items[0]->Text = "Труба на линии выгона: Брак";
			}
			StatusBarBottom->Refresh();
		}
		if(digital->CheckInSignal("ТР конец линии") )
		{
			if( ExtTransport::resultCurrent == 1)
				StatusBarBottom->Panels->Items[0]->Text += "Труба в конце линии: Годно";
			else
				StatusBarBottom->Panels->Items[0]->Text += "Труба в конце линии: Брак";
			StatusBarBottom->Refresh();
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
	DragAcceptFiles(Handle, false); // Запрещаем перетаскивание файлов
	delete stats;
	if(SystemConst::ExternalTransport && digital->CheckInSignal("ТР конец линии") )
		ini->WriteInteger("SignalResult", "Current", ExtTransport::resultCurrent);

	if(SystemConst::ExternalTransport && digital->CheckInSignal("Сброс") )
		ini->WriteInteger("SignalResult", "Previous", ExtTransport::resultPrevious);

	if (!digital->CloseCards())
		Application->MessageBoxW(L"Платы Advantech закрыты некорректно", L"Ошибка", MB_ICONERROR);

	if (l1 != NULL)
	{
		l1->Close();
		delete l1;
	}
	/*todo
	if (l2 != NULL)
	{
		l2->Close();
		delete l2;
	}
	 */
	// запись дефолтного всего
	ini->WriteString("Default", "TypeSize", cbTypeSize->Text);
	ini->WriteBool("Default", "IsLinear", cbLinear->Checked);
	ini->WriteBool("Default", "IsThickness", cbThickness->Checked);
	ini->WriteBool("Default", "IsInterruptView", cbInterruptView->Checked);
	ini->WriteBool("LIR" , "NoMarking" , cbNoMarking->Checked);
	delete ini;
	// закрытие COM-порта
    delete MyCom;

	UnregisterHotKey(MainForm->Handle, // Handle окна
		0x00E); // наш идентификатор горячего ключа
	UnregisterHotKey(MainForm->Handle, // Handle окна
		0x00A); // наш идентификатор горячего ключа
		delete digital;
}

// ---------------------------------------------------------------------------
void TMainForm::SetProjectSpecialty()
{
	cbLinear->Visible = !SystemConst::HideInGain;
	cbThickness->Visible = !SystemConst::HideInGain;
	bGoodTube->Visible = ini->ReadBool("hide" , "hide" , false);//true;!SystemConst::HideInGain;
	menuTestASUConnection->Visible = SystemConst::ComWithASU;
	cbNoMarking->Visible = SystemConst::IsOnline;
	if (SystemConst::IsOnline)
	{
		MixingTimer->Interval = ini->ReadInteger("LIR", "PauseMixing" , 300000);
		MixingTimer->Enabled = true;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TMainForm::MainFormInit(TObject *Sender)
{
	int w  = MainForm->Width;
	int ht = (StatusBarTop->Height) + 80;
	int h  = (MainForm->Height) - (StatusBarBottom->Height * 3) - ht;
	int wm = 20, hm = 50;
	int hp = 5;

	pStatistics->Left = w - pStatistics->Width - 30;

	eCrossDeadZoneStart->Left      = w - 380;
	eCrossDeadZoneFinish->Left     = w - 120;
	Label10->Left                  = w - 500;
	Label11->Left                  = w - 240;
	eLinearDeadZoneStart->Left     = w - 380;
	eLinearDeadZoneFinish->Left    = w - 120;
	Label5->Left                   = w - 500;
	Label13->Left                  = w - 240;

	MainForm->gbCrossDef->Width        = w - wm * 2;
	MainForm->gbCrossDef->Left         = wm / 2;
	MainForm->gbCrossDef->Height       = h / hp;
	MainForm->gbCrossDef->Top          = ht;
	MainForm->CrossDefectChart->Height = h / hp - hm;

	MainForm->gbLinearDef->Width        = w - wm * 2;
	MainForm->gbLinearDef->Left         = wm / 2;
	MainForm->gbLinearDef->Height       = h / hp;
	MainForm->gbLinearDef->Top          = ht + h / hp;
	MainForm->LinearDefectChart->Height = h / hp - hm;

	MainForm->gbThickness->Width     = w - wm * 2;
	MainForm->gbThickness->Left      = wm / 2;
	MainForm->gbThickness->Height    = h / hp * 2;
	MainForm->gbThickness->Top       = ht + h / hp * 2;
	MainForm->ThicknessChart->Height = h / hp * 2 - hm;

	MainForm->gbSummary->Width     = w - wm * 2;
	MainForm->gbSummary->Left      = wm / 2;
	MainForm->gbSummary->Height    = h / hp;
	MainForm->gbSummary->Top       = ht + h / hp * 4;
	MainForm->SummaryChart->Height = h / hp - hm;

	// this->LinearDefectChart->View3D=true;
	this->LinearDefectChart->Title->Visible        = false;
	this->LinearDefectChart->BottomAxis->Visible   = true;
	this->LinearDefectChart->BottomAxis->Automatic = false;
	this->LinearDefectChart->BottomAxis->Minimum   = 0;
	this->LinearDefectChart->BottomAxis->Maximum   = Globals::max_zones;
	this->LinearDefectChart->LeftAxis->Visible     = true;
	this->LinearDefectChart->LeftAxis->Automatic   = false;
	this->LinearDefectChart->LeftAxis->Minimum     = 0;
	this->LinearDefectChart->LeftAxis->Maximum     = Globals::LinSensors;
	this->LinearDefectChart->Zoom->Allow           = false;
	this->LinearDefectChart->AllowPanning          = TPanningMode::pmNone;
	for (int i = 0; i < Globals::LinSensors; i++)
	{
		LinearDefectChart->Series[i]->ColorEachPoint=true;
		LinearDefectChart->Series[i]->Marks->Visible=false;
		((TBarSeries*) LinearDefectChart->Series[i])->BarWidthPercent=100;
		((TBarSeries*) LinearDefectChart->Series[i])->MultiBar = mbStacked;//TMultiBar(3);
		((TBarSeries*) LinearDefectChart->Series[i])->SideMargins=false;
		((TBarSeries*) LinearDefectChart->Series[i])->OffsetPercent=50;
	}

	// this->CrossDefectChart->View3D=true;
	this->CrossDefectChart->Title               = LinearDefectChart->Title;
	this->CrossDefectChart->BottomAxis          = LinearDefectChart->BottomAxis;
	this->CrossDefectChart->LeftAxis->Visible   = true;
	this->CrossDefectChart->LeftAxis->Automatic = false;
	this->CrossDefectChart->LeftAxis->Minimum   = 0;
	this->CrossDefectChart->LeftAxis->Maximum   = 12;
	this->CrossDefectChart->Zoom->Allow         = false;
	this->CrossDefectChart->AllowPanning        = TPanningMode::pmNone;
	for (int i =0; i < 12; i++)
	{
		CrossDefectChart->Series[i]->ColorEachPoint=true;
		CrossDefectChart->Series[i]->Marks->Visible=false;
		((TBarSeries*) CrossDefectChart->Series[i])->BarWidthPercent=100;
		((TBarSeries*) CrossDefectChart->Series[i])->MultiBar = mbStacked;//TMultiBar(3);
		((TBarSeries*) CrossDefectChart->Series[i])->SideMargins=false;
		((TBarSeries*) CrossDefectChart->Series[i])->OffsetPercent=50;
	}

//	this->ThicknessChart->View3D              = true;
	this->ThicknessChart->Title->Visible      = false;
	this->ThicknessChart->LeftAxis->Automatic = false;
	this->ThicknessChart->LeftAxis->Minimum   = 0;
	this->ThicknessChart->LeftAxis->Maximum   = 8;
	this->ThicknessChart->BottomAxis          = LinearDefectChart->BottomAxis;
	this->ThicknessChart->Zoom->Allow         = false;
	this->ThicknessChart->AllowPanning        = TPanningMode::pmNone;
	ThicknessChart->Series[0]->ColorEachPoint=true;
	ThicknessChart->Series[0]->Marks->Visible=false;
	((TBarSeries*) ThicknessChart->Series[0])->BarWidthPercent=100;
	((TBarSeries*) ThicknessChart->Series[0])->OffsetPercent=50;
	((TBarSeries*) ThicknessChart->Series[0])->SideMargins=true;


	this->SummaryChart->View3D              = true;
	this->SummaryChart->Title->Visible      = false;
	this->SummaryChart->BottomAxis          = LinearDefectChart->BottomAxis;
	this->SummaryChart->LeftAxis->Automatic = false;
	this->SummaryChart->LeftAxis->Minimum = 0;
	this->SummaryChart->LeftAxis->Maximum = 1;
	this->SummaryChart->Zoom->Allow         = False;
	this->SummaryChart->AllowPanning        = TPanningMode::pmNone;
	SummaryChart->Series[0]->ColorEachPoint=true;
	SummaryChart->Series[0]->Marks->Visible=false;
	((TBarSeries*) SummaryChart->Series[0])->BarWidthPercent=100;
	((TBarSeries*) SummaryChart->Series[0])->OffsetPercent=50;
	((TBarSeries*) SummaryChart->Series[0])->SideMargins=true;


	StatusBarBottom->Panels->Items[0]->Width = w / 4;
	StatusBarBottom->Panels->Items[1]->Width = w / 4;
	StatusBarBottom->Panels->Items[2]->Width = w / 4;
	StatusBarBottom->Panels->Items[3]->Width = w / 4;

	StatusBarTop->Panels->Items[0]->Width = w / 4;
	StatusBarTop->Panels->Items[1]->Width = w * 3 / 8;
	StatusBarTop->Panels->Items[2]->Width = w * 3 / 8;
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::ClearCharts(TObject *Sender)
{
	pp.DeleteData();
	pr.DeleteData();
	th.DeleteData();
	sum.DeleteData();

	for (int i=0; i < 12; i++)
		CrossDefectChart->Series[i]->Clear();
	for (int i = 0; i < Globals::LinSensors; i++)
		LinearDefectChart->Series[i]->Clear();
	ThicknessChart->Series[0]->Clear();
	SummaryChart->Series[0]->Clear();

	this->lResult->Caption     = "Результат";
	this->lCut1->Caption       = "";
	this->lCut2->Caption       = "";
	this->pSolidGroup->Caption = "ГП";
	this->pSolidGroup->Color   = clDkGray;

	MainForm->Refresh();
	MainFormInit(Sender);
	Globals::isView = false;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::FormActivate(TObject *Sender)
{
	// при активации формы загружаем настройки
	UpdateComboBox();
	LoadSettings(Sender);
	// если были добавлены эталоны по группе прочности - пересчитываем центры
	if (SolidGroupForm->Tag == 1)
	{
		SolidGroupForm->Tag = 0;
	  	SetSolidGroup(Sender);
	}
}
// ---------------------------------------------------------------------------

void TMainForm::LoadSettings(TObject *Sender)
{
	String sect = "Type_" + Globals::current_typesize;
	// прочитаем используемый диаметр (типоразмер) и прогрузим эталоны по группе прочности
	int temp_diam             = Globals::current_diameter;
	Globals::current_diameter = ini->ReadInteger(sect, "Diameter", 1);
	if (temp_diam != Globals::current_diameter)
		SetSolidGroup(Sender);
	Globals::speeds.setSpeeds(Globals::current_typesize);
	// читаем параметры типоразмера - пороги отбраковки, мертвые зоны, минимальный гнодный участок
	double p1, r1, t1;
	pp.borders[0]       = ini->ReadFloat(sect, "CrossBorder1", 0.0);
	pr.borders[0]       = ini->ReadFloat(sect, "LinearBorder1", 0.0);
	th.borders[0]       = ini->ReadFloat(sect, "ThicknessBorder1", 0.0);
	sum.min_good_length = ini->ReadInteger(sect, "Min_Good_Length", 0);
	sum.end_cut			= ini->ReadInteger(sect, "EndCut" ,0);

	p1 = ini->ReadFloat(sect, "CrossBorder2", 0.0);
	r1 = ini->ReadFloat(sect, "LinearBorder2", 0.0);
	t1 = ini->ReadFloat(sect, "ThicknessBorder2", 0.0);


	pp.dead_zone_start=ini->ReadInteger(sect, "CrossDeadZoneStart", 0);
	pp.dead_zone_finish = ini->ReadInteger(sect, "CrossDeadZoneFinish", 0);

	pr.dead_zone_start  = ini->ReadInteger(sect, "LinearDeadZoneStart", 0);
	pr.dead_zone_finish = ini->ReadInteger(sect, "LinearDeadZoneFinish", 0);

	this->eCrossBorder1->Text     = pp.borders[0];
	this->eLinearBorder1->Text    = pr.borders[0];
	this->eThicknessBorder1->Text = th.borders[0];
	this->eCrossBorder2->Text     = p1;
	this->eLinearBorder2->Text    = r1;
	this->eThicknessBorder2->Text = t1;
	this->eMin_Good_Length->Text  = sum.min_good_length;

	this->eCrossDeadZoneStart->Text      = IntToStr( pp.dead_zone_start );
	this->eCrossDeadZoneFinish->Text     = IntToStr( pp.dead_zone_finish );
	this->eLinearDeadZoneStart->Text     = pr.dead_zone_start;
	this->eLinearDeadZoneFinish->Text    = pr.dead_zone_finish;

	if (p1 == 0)
		pp.borders.set_length(1);
	else
	{
		pp.borders.set_length(2);
		pp.borders[1] = p1;
	}

	if (r1 == 0)
		pr.borders.set_length(1);
	else
	{
		pr.borders.set_length(2);
		pr.borders[1] = r1;
	}

	if (t1 == 0)
		th.borders.set_length(1);
	else
	{
		th.borders.set_length(2);
		th.borders[1] = t1;
	}

}
// ---------------------------------------------------------------------------

void TMainForm::UpdateComboBox()
{
	cbTypeSize->Items->Clear();
	TStringList * sections = new TStringList();
	ini->ReadSections(sections);
	for (int i = 0; i < sections->Count; i++)
		if (sections->Strings[i].SubString(1, 5) == "Type_")
			cbTypeSize->AddItem(sections->Strings[i].SubString(6, 20), NULL);

	cbTypeSize->ItemIndex = cbTypeSize->Items->IndexOf(Globals::current_typesize);
	sections->~TStringList();
	if (cbTypeSize->Items->Count == 0)
		Application->MessageBoxW(L"Ни одного типоразмера не найдено! Создайте их, нажав клавишу F4", L"Предупреждение!", MB_ICONWARNING);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::cbTypeSizeSelect(TObject *Sender)
{
	Globals::current_typesize = cbTypeSize->Text;
	LoadSettings(Sender);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::eSettingsChange(TObject *Sender)
{
	ini->WriteString("Type_" + cbTypeSize->Text, ((TEdit *) Sender)->Name.SubString(2, 29),	((TEdit *) Sender)->Text);
	LoadSettings(Sender);
}

// ---------------------------------------------------------------------------
void TMainForm::SetSolidGroup(TObject *Sender)
{
	using namespace SolidGroups;
	int ts = Globals::current_diameter; // текущий типоразмер (диаметр)
	vector<Tube>etalons; // вектор эталонов из БД
  	if( DM->Connect )
		etalons = DM->ReadEtalon(ts); // читаем эталоны конкретного диаметра из базы
	if (etalons.empty() || etalons.size() < 3 )
	{
		StatusBarBottom->Panels->Items[1]->Text = "ЭТАЛОНЫ ГРУППЫ ПРОЧНОСТИ НЕ ОБНАРУЖЕНЫ";
		StatusBarBottom->Refresh();
	}
	else
	{
		anal.initEtalons(etalons);
		StatusBarBottom->Panels->Items[1]->Text = "Эталоны группы прочности загружены";
		StatusBarBottom->Refresh();

		// memo---------------------------------------
		Memo1->Lines->Add("-------------------------------------------------------------");
		for (int i = 0; i < etalons.size(); i++)
		{
			vector<double>buf;
			buf                = etalons[i].Coordinates();
			UnicodeString tube = (String) etalons[i].solidGroup() + " ";
			for (int j = 0; j < 8; j++)
				tube += UnicodeString(buf[j]) + "\t";
			Memo1->Lines->Add(tube);
		}

		vector<Tube>centers;
		centers = anal.etalonCenters();
		Memo1->Lines->Add("-------------------------------------------------------------");
		ProtocolForm->SendToProtocol("Загружены эталоны группы прочности");
		for (int i = 0; i < centers.size(); i++)
		{
			vector<double>buf;
			buf                = centers[i].Coordinates();
			UnicodeString tube = (String) centers[i].solidGroup() + " ";
			for (int j = 0; j < 8; j++)
				tube += UnicodeString(Math::SimpleRoundTo(buf[j], 0)) + "\t";
			Memo1->Lines->Add(tube);
			ProtocolForm->SendToProtocol(tube);
		}
		// memo ---------------------------------------------------
	}
}
// ---------------------------------------------------------------------------

void TMainForm::IdentifySolidGroup()
{
	// *********считаем группу прочности*************************
	if(!SystemConst::isSolidGroupMS_DOS)     //если считаем по старой схеме
	{
		using namespace SolidGroups;
		map<Group, double>mp;
		map<Group, double>::iterator it;

		mp = anal.checkTube(tubeSG);

		Memo1->Lines->Add("******* ГП **********");
		for (it = mp.begin(); it != mp.end(); ++it)
			Memo1->Lines->Add(" " + (String) it->first + " " + FloatToStrF(it->second , ffFixed, 4,4));

		double maxValue = 1.0e-8;
		Group group;

		for (it = mp.begin(); it != mp.end(); ++it)
		{
			ProtocolForm->SendToProtocol("Группа " +(String)it->first + " вероятность " + Math::SimpleRoundTo(it->second,-3)*100.0 + " %");
			if (it->second > maxValue)
			{
				maxValue = it->second;
				group    = it->first;
			}
		}
		double tempThreshold=ini->ReadFloat("OtherSettings", "SgThreshold",0.5);

		ProtocolForm->SendToProtocol("Порог вероятности "+FloatToStr(tempThreshold));

		if (maxValue > tempThreshold)
		{
			pSolidGroup->Caption = (String)group;

			switch (group.value())
			{
			case Group::Types::sgD:
				pSolidGroup->Color = clYellow;
				break;
			case Group::Types::sgE:
					pSolidGroup->Color = clBlue;
					break;
			case Group::Types::sgK:
				pSolidGroup->Color = clGreen;
				break;
			}
			tubeSG.setSolidGroup(group);
			manual_num = group.GroupToNumber();		// костыль
		}
		else		// НЕ ОПРЕДЕЛЕНО !!!! ГП
		{
			pSolidGroup->Caption = "Нет";		// manual_name;		// костыль
			pSolidGroup->Color   = clGray;
			tubeSG.setSolidGroup(Group::Types::sgUnknown);
		}
		pSolidGroup->Refresh();
	}
	else	//если считаем через коэффициенты корреляции (c помощью проги MS-DOS)
	{
		using namespace SolidGroups;
//		String result = Globals::tubeSG.calcSolidGroup(tubeSG.Coordinates());
		String result = tubeSG.calcSolidGroup(tubeSG.Coordinates());
		result = tubeSG.calcSolidGroup(tubeSG.Coordinates());
		pSolidGroup->Caption = result;
		if(result == "K")
			{
				pSolidGroup->Color = clGreen;
//				Globals::tubeSG.setSolidGroup(Group::Types::sgK);
				tubeSG.setSolidGroup(Group::Types::sgK);
			}
		else
		if(result == "E")
			{
				pSolidGroup->Color = clBlue;
//				Globals::tubeSG.setSolidGroup(Group::Types::sgE);
				tubeSG.setSolidGroup(Group::Types::sgE);
			}
		else
			{
				pSolidGroup->Color = clYellow;
//				Globals::tubeSG.setSolidGroup(Group::Types::sgD);
				tubeSG.setSolidGroup(Group::Types::sgD);
			}
	}
}

// -----------------------------------------------------------------------------
#if 0
void __fastcall TMainForm::bTestClick(TObject *Sender)
{
	int channels=16;
	digital->SetOutSignal("Прод Пит СУ");

	l1->InitADC( 16,
				 ini->ReadFloat("PP", "Kadr", 0.0),
				 "PP", 0);
   /*todo   defect
	l2->InitADC( Globals::LinSensors,
				 ini->ReadFloat("PR","Kadr",0.0),
				 "PR", 0);     // 1-ГП, 0- Дефектоскопия
   */
	// ------------------------Сбор данных с LCARD-----------------------------------
	l1->ClearBuffer();
  //todo	l2->ClearBuffer();
	ClearCharts(Sender);
	Globals::isView = false;

	StatusBarTop->Panels->Items[1]->Text = L"Идет сбор данных с АЦП";
	StatusBarTop->Refresh();

	l1->InitDAC(true);
  //todo	l2->InitDAC(true);

	l1->Start();
 //todo	l2->Start();
	SetAbleButtons(false); // Блокируем главную форму от кривых рук

	for (DWORD tt = GetTickCount(); GetTickCount() - tt < 5000; Sleep(10))
		Application->ProcessMessages();

	SetAbleButtons(true); // Разблокировка формы
	l1->Stop();
  //todo	l2->Stop();

	StatusBarTop->Panels->Items[1]->Text = L"Сбор данных с АЦП завершен";
	StatusBarTop->Refresh();
	// ------------------Тестовое заполнение результата и вывод на экран-------------
	// pp.CreateTestData(1);
	int DefSensors=(Globals::current_diameter == 60) ? (10) : (12);
	pp.zones = 50;
 //todo 1605
 	pp.PrepareData(l1, channels, DefSensors);
 //pp.PrepareData(l1, pp.data, pr.data);
	pp.meas_per_sensor.set_length(pp.zones);
	for (int i = 0; i < pp.zones; i++)
		pp.meas_per_sensor[i] = pp.meas_per_zone;
	pp.ComputeZoneData();
	pp.CalculateDeadZone(ini->ReadInteger("Size","ZoneLength",2));

	pr.zones = 50;
	//pr.CreateTestData(2);
 //todo l2	pr.PrepareData(l2, Globals::LinSensors, Globals::LinSensors);
	pr.meas_per_sensor.set_length(pr.zones);
	for (int i = 0; i < pr.zones; i++)
		pr.meas_per_sensor[i] = pr.meas_per_zone;
	pr.ComputeZoneData();
	pr.CalculateDeadZone(ini->ReadInteger("Size","ZoneLength",2));

	th.ComputeZoneData();
	sum.ComputeZonesData(pp, pr, th);

	pp.PutResultOnChart(CrossDefectChart, MainForm);
	pr.PutResultOnChart(LinearDefectChart, MainForm);
	th.PutResultOnChart(ThicknessChart, MainForm);
	sum.PutResultOnChart(SummaryChart, MainForm);

	sum.MakeDecision();
	this->lResult->Caption    = sum.decision;
	this->lCut1->Caption      = sum.cut1;
	this->lCut2->Caption      = sum.cut2;
	Globals::isView           = true;

	digital->ResetOutSignal("Прод Пит СУ");
}
#else
void __fastcall TMainForm::bTestClick(TObject *Sender)
{
#if 1
	int channels=16;
	digital->SetOutSignal("Прод Пит СУ");

	l1->InitADC( 0xDEADBEEF,
				 ini->ReadFloat("PP", "Kadr", 0.0),
				 "PP", 0);
   /*todo   defect
	l2->InitADC( Globals::LinSensors,
				 ini->ReadFloat("PR","Kadr",0.0),
				 "PR", 0);     // 1-ГП, 0- Дефектоскопия
   */
	// ------------------------Сбор данных с LCARD-----------------------------------
	l1->ClearBuffer();
  //todo	l2->ClearBuffer();
	ClearCharts(Sender);
	Globals::isView = false;

	StatusBarTop->Panels->Items[1]->Text = L"Идет сбор данных с АЦП";
	StatusBarTop->Refresh();

	l1->InitDAC(true);
  //todo	l2->InitDAC(true);

	l1->Start();
 //todo	l2->Start();
	SetAbleButtons(false); // Блокируем главную форму от кривых рук

	for (DWORD tt = GetTickCount(); GetTickCount() - tt < 5000; Sleep(10))
		Application->ProcessMessages();

	SetAbleButtons(true); // Разблокировка формы
	l1->Stop();
  //todo	l2->Stop();

	StatusBarTop->Panels->Items[1]->Text = L"Сбор данных с АЦП завершен";
	StatusBarTop->Refresh();
	// ------------------Тестовое заполнение результата и вывод на экран-------------
	// pp.CreateTestData(1);
	int DefSensors =(Globals::current_diameter == 60) ? (10) : (12);
  //	pp.zones = 50;
 //todo 1605
  //	pp.PrepareData(l1, channels, DefSensors);
 //pp.PrepareData(l1, pp.data, pr.data);
	vector<vector<int> > offsets(2);
	int lengthZone = l1->Index / BigArray::countSensors / 50;
	for(int i = 0; i < 50; ++i)
	{
	   offsets[0].push_back(i * lengthZone);
	   offsets[1].push_back(i * lengthZone);
	}
//---------------------------------------------------------------
/*
//проверка медианного фильтра прошла успешно
for(int i = 0, k = 5000; i < l1->Index; ++i, ++k)
{
	 int t = int(0.01 * (1 + (i % 18))  * Globals::level100);
	 l1->Base[i] = t;
	 if((i / 18) % 18 == 0) l1->Base[i] = 10* Globals::level100;
	 if((i / 18) % 18 == 1) l1->Base[i] = 10* Globals::level100;
   //	 if((i / 18) % 18 == 2) l1->Base[i] = 10* Globals::level100;
  //	 if((i / 18) % 18 == 3) l1->Base[i] = 10* Globals::level100;
	// if((i / 18) % 18 == 4) l1->Base[i] = 15* Globals::level100;
	 if(k > 10000) k = 0;
}
*/
//-----------------------------------------------------------------
	BigArray::Prepare(
		DefSensors
		, l1->Base
		, l1->Index
		, offsets
		, pp.data
		, pr.data
	);
  //	pp.meas_per_sensor.set_length(pp.zones);
  //	for (int i = 0; i < pp.zones; i++)
   //		pp.meas_per_sensor[i] = pp.meas_per_zone;
	pp.ComputeZoneData();
	String section = "Type_" + Globals::current_typesize;
	int len = GetPrivateProfileInt(section.c_str()
	   , L"CrossDeadZoneStart", 600
	   , Globals::IniFileName.c_str()
	   );
	pp.CalculateDeadFront(len);
	len = GetPrivateProfileInt(section.c_str()
	   , L"CrossDeadZoneFinish", 250
	   , Globals::IniFileName.c_str()
	   );
	pp.CalculateDeadBack(len);

  //	pr.zones = 50;
	//pr.CreateTestData(2);
 //todo l2	pr.PrepareData(l2, Globals::LinSensors, Globals::LinSensors);
   //	pr.meas_per_sensor.set_length(pr.zones);
   //	for (int i = 0; i < pr.zones; i++)
   //		pr.meas_per_sensor[i] = pr.meas_per_zone;
	pr.ComputeZoneData();
	pr.CalculateDeadFront(len);
		pr.CalculateDeadBack(len);

	//todo pr.CalculateDeadZone(ini->ReadInteger("Size","ZoneLength",2));

	th.ComputeZoneData();
	sum.ComputeZonesData(pp, pr, th, true, true);

	pp.PutResultOnChart(CrossDefectChart, MainForm);
	pr.PutResultOnChart(LinearDefectChart, MainForm);
	th.PutResultOnChart(ThicknessChart, MainForm);
	sum.PutResultOnChart(SummaryChart, MainForm);

	sum.MakeDecision();
	this->lResult->Caption    = sum.decision;
	this->lCut1->Caption      = sum.cut1;
	this->lCut2->Caption      = sum.cut2;
	Globals::isView           = true;

	digital->ResetOutSignal("Прод Пит СУ");
	#endif
}
#endif
// -----------------------------------------------------------------------------

void __fastcall TMainForm::bViewClick(TObject *Sender)
{
	if (Globals::isView)
	{
		Result *r1, *r2;
		r1 = &pp;
		r2 = &pr;
		ViewForm->SendResults(*r1, *r2);
		ViewForm->Show();
	}
}
// -----------------------------------------------------------------------------

void __fastcall TMainForm::bWorkClick(TObject *Sender)
{
	SetAbleButtons(false);
	if ( workonline != NULL ) // && !workonline->CheckTerminated() )
	{
//		workonline->Terminate();
		workonline = NULL;
		ProtocolForm->SendToProtocol(L"Старый тред убит" );
	}
	// режим РАБОТЫ!!!
	if ( bWork->Caption == "F5 Продолжить" )
	{
		// посылаем результат в АСУ (пока не получится или юзер не откажется)
		if (SystemConst::ComWithASU)
		{
			bool sent = false;
			do
			{
				sent = MyCom->SendResultToASU(pp, pr, th, sum , manual_num);
				ProtocolForm->SendToProtocol(L"Результат в АСУSeries: " + (String)((sent)?(L"Да"):(L"Нет")));
				if ( !sent )
					if ( Application->MessageBoxW(L"Ошибка при передаче результатов в АСУ. Повторить?",
								L"Внимание",MB_RETRYCANCEL) == 2 )
						sent = true;
			}
			while (!sent);
		}
		// выставляем сигналы (перекладка 2 = признак)
		if ( sum.decision == "Годно" || sum.decision == "Класс 2" || GoodTube )
		{
			if(!SystemConst::ExternalTransport)
			{
				digital->SetOutSignal("Цех Результат");
				Sleep(500);
				digital->SetOutSignal("Цех Признак");
			}
			else
			{
				ExtTransport::resultCurrent = 1;
				ini->WriteInteger("SignalResult", "Current", 1);
			}
			stats->AddTube("Г");
		}
		else if (BadTube)					// костыли нягань
		{
			if(!SystemConst::ExternalTransport)
				digital->SetOutSignal("Цех Признак");
			else
			{
				ExtTransport::resultCurrent = 0;
				ini->WriteInteger("SignalResult", "Current", 0);
			}
			stats->AddTube("Б");
		}
		else if (sum.decision == "Брак")		// костыли нягань
		{
			if(!SystemConst::ExternalTransport)
			{
				digital->SetOutSignal("Цех Результат");
				Sleep(500);
				digital->SetOutSignal("Цех Признак");
			}
			stats->AddTube("Б");
		}

	}
	UpdateStats();
//	ClearCharts(Sender);
	GoodTube = false;
	BadTube = false;
	ProtocolForm->ClearProtocol();

	alarmthread = new AlarmThreadClass(true);
	bWork->Caption = "F5 Продолжить";
   /*   todo
	if (SystemConst::IsOnline)
	{
		Counter = new Advantech1784(false);
		workonline = new ThreadOnLine(false,Counter, *l1, *l2, pp, pr , th ,sum , StatusBarTop,
								cbThickness->Checked, cbLinear->Checked, *alarmthread );

		workonline->OnTerminate = CompleteWork;
		workonline->FreeOnTerminate  = true;
		Counter->Reset();
	}
	else
	*/
	{
	///* xxxxxx     вернутся позже
		workthread  = new WorkThreadClass(true, *l1, /*l2,*/ pp, pr, StatusBarTop ,
						cbThickness->Checked, cbLinear->Checked, *alarmthread , 1);
		workthread->OnTerminate = CompleteWork;
		workthread->FreeOnTerminate  = true;
		workthread->Start();
	   //	*/
	}

	alarmthread->OnTerminate     = AlarmFinished;
	alarmthread->FreeOnTerminate = true;
	alarmthread->Start();
	tempSGbegin = l1->getTemperatureForSG();
	ProtocolForm->SendToProtocol("Температура перед циклом= " + IntToStr(tempSGbegin) );
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::bCancelWorkClick(TObject *Sender)
{
	if ( workthread != NULL && !workthread->Finished )
	{
		workthread->Terminate();
		ProtocolForm->SendToProtocol("User: Сбросили рабочий поток");
	}
	if ( workonline != NULL && !workonline->Finished)
	{
		workonline->Terminate();
		ProtocolForm->SendToProtocol("User: Сбросили Онлайн работу");
	}
	if ( alarmthread != NULL && !alarmthread->Finished)
	{
		alarmthread->Terminate();
		ProtocolForm->SendToProtocol("User: Сбросили аварийный поток");
	}
	if ( returnthread != NULL && !returnthread->Finished)
	{
		returnthread->Terminate();
		ProtocolForm->SendToProtocol("User: Сбросили поток возврата");
	}
	if ( cfthread != NULL && !cfthread->Finished)
	{
		cfthread->Terminate();
		ProtocolForm->SendToProtocol("User: Сбросили поток выгона");
	}
	if ( Counter != NULL && !Counter->Finished)
	{
		Counter->Terminate();
		ProtocolForm->SendToProtocol("User: Сбросили поток счетчика");
	}

	StatusBarTop->Panels->Items[1]->Text = "Режим \"Работа\" не завершен!";
	StatusBarTop->Panels->Items[2]->Text = "Прервано пользователем!";
	ProtocolForm->SendToProtocol("Прервано пользователем!");

	digital->ResetOutSignal("Стойка Транзит");
	digital->ResetOutSignal("Стойка Авто");
	if(SystemConst::CentralizersCard)
		digital->ResetOutSignal("Схват Транзит");

	bWork->Caption = "F5 Работа";
	SetAbleButtons(true);
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::CompleteWork(TObject *Sender)
{
	ProtocolForm->SendToProtocol("Зашли в Complete Work");
	pp.Clear(CrossDefectChart);
	pr.Clear(LinearDefectChart);
	th.Clear(ThicknessChart);
	if ( alarmthread != NULL && !alarmthread->Finished)
	{
		alarmthread->Terminate();
		ProtocolForm->SendToProtocol("Work: Завершили аварийный тред");
	}
	if ( Counter != NULL && !Counter->Finished)
	{
		Counter->Terminate();
		ProtocolForm->SendToProtocol("Work: Завершили тред счетчиков");
	}

	if ( (workthread!=NULL && workthread->transit) ||
		( workonline!=NULL && workonline->transit) )
	{
		ProtocolForm->SendToProtocol("CompleteWork. Идем в транзит");
    	bCrashForwardClick(Sender);
	}
	else if (workthread!=NULL && workthread->cool) //||
//		(  workonline->cool) )	 && workonline!=NULL
	{
		pp.CalculateDeadZone(200);
		pr.CalculateDeadZone(200);
		pp.PutResultOnChart(CrossDefectChart, MainForm);
		pr.PutResultOnChart(LinearDefectChart, MainForm);
		CrossDefectChart->Refresh();
		LinearDefectChart->Refresh();

// ждем получения группы прочности
		ProtocolForm->SendToProtocol("Ждем получения группы прочности");
		/*
		if (SystemConst::SolidGroupSource == sgsCOMPort)
		{
			tubeSG = MyCom->GetSolidGroup();
		}
		else
		if( SystemConst::SolidGroupSource == sgsLCard ) // ждем получения группы прочности с LCard
		*/
		{
			using namespace SolidGroups;
		  //	vector<int>buffer = PrepareSG::Instance().Do(l1->Base, l1->Index, l1->timeOffs);                      //Берем вектор
		  	vector<int>buffer = l1->PrepareSGData(false);		//пихаем в него полученный сигнал
			int temp = l1->getTemperatureForSG();
			ProtocolForm->SendToProtocol("Main: temp = " + IntToStr(temp));
//			tubeSG.setTemperature( l1->getTemperatureForSG() );
//			tubeSG.setTemperature( temp );
			tubeSG.setTemperature( tempSGbegin );
			tubeSG = Tube(buffer, TubeType(Globals::current_diameter));    //Считаем сигнал
			Globals::tubeSG = tubeSG;
		}
dprint("WARNING \n");
///* todo Пока забил для отладки
		//выведем в статус бар
		/* хуй догадаться чё такое
		this->StatusBarBottom->Panels->Items[0]->Text = " ";
		for( unsigned int i = 0; i < 8; i++ )
		{
			short ff = tubeSG.Coordinates()[i];
			StatusBarBottom->Panels->Items[0]->Text += UnicodeString(ff)+" ";
		}
		this->StatusBarTop->Refresh();
		*/
		IdentifySolidGroup();        //пытаемся опеределить ГП
//*/
// ждем результата от толщины, если она работала
		if ( cbThickness->Checked )
		{
			dprint("wait result from data base\n");
			if(digital->WaitForInSignal("Результат",60000) )
			{
				dprint("wait result from data base2\n");
				if (!SystemConst::IsOnline)
				{
					ProtocolForm->SendToProtocol("Мы перед результатом толщины");
						dprint("connecting to base\n");
					if(DM->Connect)
					{
						long ID_Tube=0;
						this->StatusBarTop->Panels->Items[2]->Text=L"Ждем результат толщины";
						ProtocolForm->SendToProtocol("Есть результат делаем запрос к бд");
						th.zones=DM->GetThick(th.zone_data,ID_Tube);
						th.PutResultOnChart(ThicknessChart, MainForm);
						this->StatusBarTop->Panels->Items[2]->Text=L"";
						dprint("data base ok\n");
					}
				}
			}
			else
				this->StatusBarTop->Panels->Items[2]->Text=L"Не дождались результат толщины";
		}
		sum.ComputeZonesData(pp, pr, th, cbLinear->Checked, cbThickness->Checked);
		sum.PutResultOnChart(SummaryChart, MainForm);
		sum.MakeDecision();
		this->lResult->Caption = sum.decision;
		this->lCut1->Caption   = sum.cut1;
		this->lCut2->Caption   = sum.cut2;
		Globals::isView        = true;
		StatusBarTop->Panels->Items[2]->Text = " ";

// записываем полный результат в базу
if(!MainForm->cbEtalon->Checked)
	 DM->SaveTube(pp.zone_data,
					 pr.zone_data,
					 sum.zones,
					 sum.decision.SubString(1, 1),	//конечный результат по трубе
					 (String)tubeSG.solidGroup(),	//группа прочности
					 Globals::current_diameter,		//Типоразмер
					 MainForm->cbEtalon->Checked );
// смотрим, что делать дальше
		if ( cbInterruptView->Checked && !cbEtalon->Checked )
		{
			SetAbleButtons(true);
		}
		else if ( cbEtalon->Checked )
		{
			SetAbleButtons(true , bWork , menuWork);
		}
		else
		{
//			workonline = NULL;
//			delete workonline;
			Sleep(2000);
			SetAbleButtons(true);
			bWorkClick(Sender);
		}
	}
	else
	{
		bWork->Caption = "F5 Работа";
		SetAbleButtons(true);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::AlarmFinished(TObject *Sender)
{
	if (alarmthread->is_alarm)
	{
		ProtocolForm->SendToProtocol("Зашли в alarmFinished - is_alarm");
		if ( workthread != NULL && !workthread->Finished )
		{
			workthread->Terminate();
			ProtocolForm->SendToProtocol("Alarm: Сбросили обычную работу");
		}
		if ( returnthread != NULL && !returnthread->Finished)
		{
			returnthread->Terminate();
			ProtocolForm->SendToProtocol("Alarm: Сбросили Возврат");
		}
		if ( cfthread != NULL && !cfthread->Finished)
		{
			cfthread->Terminate();
			ProtocolForm->SendToProtocol("Alarm: Сбросили Выгон");
		}
		if ( workonline != NULL && !workonline->Finished)
		{
			workonline->Terminate();
			ProtocolForm->SendToProtocol("Alarm: Сбросили Онлайн работу");
		}
		if ( Counter != NULL && !Counter->Finished)
		{
			Counter->Terminate();
			ProtocolForm->SendToProtocol("Alarm: Сбросили поток Счетчика");
		}
		ProtocolForm->SendToProtocol("Alarm: Сбросили все потоки");
		String str = "Цикл прерван! Авария: " + alarmthread->reason;
		Application->MessageBoxW(str.w_str(), L"Авария", MB_ICONERROR);
		StatusBarTop->Panels->Items[1]->Text = "Режим \"Работа\" не завершен!";
		StatusBarTop->Panels->Items[2]->Text = "Вышли по аварии: " + alarmthread->reason;
		bWork->Caption = "F5 Работа";
		SetAbleButtons(true);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TMainForm::ReturnFinished(TObject *Sender)
{
	if ( alarmthread != NULL && !alarmthread->Finished)
	{
		alarmthread->Terminate();
		ProtocolForm->SendToProtocol("Return: Сбросили аварийный поток");
	}
	bWork->Caption = "F5 Работа";
	SetAbleButtons(true);
//	delete alarmthread;
//	delete returnthread;
//	delete cfthread;
}
// ---------------------------------------------------------------------------
void __fastcall TMainForm::TransitFinished(TObject *Sender)
{
	if ( alarmthread != NULL && !alarmthread->Finished)
	{
		alarmthread->Terminate();
		ProtocolForm->SendToProtocol("Transit: Сбросили аварийный поток");
	}
	bWork->Caption = "F5 Работа";
	if (cfthread->cool && cfthread->isTransit)	// если был завершен транзит - переходим в работу (пыть-ях)
	{
	   //	bWorkClick(Sender);
	}
	else
	{
		SetAbleButtons(true);
	//	delete alarmthread;
	//	delete returnthread;
	//	delete cfthread;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::bCrashForwardClick(TObject *Sender)
{
	// режим аварийного выгона застрявшей трубы
	SetAbleButtons(false);

	alarmthread = new AlarmThreadClass(true);
	cfthread  = new CrashForwardThread(true, StatusBarTop , *alarmthread );

	cfthread->OnTerminate = TransitFinished;
	alarmthread->OnTerminate = AlarmFinished;
	cfthread->FreeOnTerminate  = true;
	alarmthread->FreeOnTerminate = true;
	cfthread->Start();
	alarmthread->Start();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::bReturnModeClick(TObject *Sender)
{
	// режим возврата трубы
	SetAbleButtons(false);

	alarmthread = new AlarmThreadClass(true);
	returnthread  = new ReturnThread(true, StatusBarTop , *alarmthread );

	returnthread->OnTerminate = ReturnFinished;
	alarmthread->OnTerminate     = AlarmFinished;
	returnthread->FreeOnTerminate  = true;
	alarmthread->FreeOnTerminate = true;

	returnthread->Start();
	alarmthread->Start();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::bManageClick(TObject *Sender)
{
 //	if (digital->cards_needed > digital->cards_exist)
  //		Application->MessageBoxW
  //			(L"Модуль не доступен, так как не хватает плат Advantech1730. \n Ошибка 001", L"Ошибка!", MB_ICONERROR);
  //	else
	{
		ManageForm->SendHandleToSignals(*l1);
		ManageForm->Show();
	}
}

// ---------------------------------------------------------------------------

void __fastcall TMainForm::bGoodTubeClick(TObject *Sender)
{
	ProtocolForm->SendToProtocol("Оператор сменил результат на Годно");
	StatusBarBottom->Panels->Items[0]->Text = "Оператор сменил результат на Годно";
	StatusBarBottom->Refresh();
	GoodTube = true;
	sum.decision = "Годно";

	if(SystemConst::ExternalTransport)
	{
		ExtTransport::resultCurrent = 1;
		ini->WriteInteger("SignalResult", "Current", 1);
	}
	bWorkClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::bBadTubeClick(TObject *Sender)
{
	ProtocolForm->SendToProtocol("Оператор сменил результат на Брак");
	StatusBarBottom->Panels->Items[0]->Text = "Оператор сменил результат на Брак";
	StatusBarBottom->Refresh();
	BadTube = true;
	sum.decision = "Брак";

	if(SystemConst::ExternalTransport)
	{
		ExtTransport::resultCurrent = 0;
		ini->WriteInteger("SignalResult", "Current", 0);
	}
	bWorkClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	if (Key == 112)
		WinExec("hh Help.chm", SW_RESTORE);
	if (Key == 27)
		MainForm->Close();
	if (Key == '`' || Key == L'ё')
	{
		cbTypeSize->ItemIndex = (cbTypeSize->ItemIndex < cbTypeSize->Items->Count - 1) ? (cbTypeSize->ItemIndex + 1) : (0);
		cbTypeSizeSelect(Sender);
	}
}
// -----------------------------------------------------------------------------

void __fastcall TMainForm::ChartClick(TObject *Sender)
{
	if (Globals::isView)
	{
	 //	pp.CalculateDeadZone(200);
	//	pr.CalculateDeadZone(200);
		pp.PutResultOnChart(CrossDefectChart, MainForm);
		pr.PutResultOnChart(LinearDefectChart, MainForm);
		sum.ComputeZonesData(pp,pr,th, cbLinear->Checked, cbThickness->Checked);
		sum.MakeDecision();
		this->lResult->Caption = sum.decision;
		this->lCut1->Caption   = sum.cut1;
		this->lCut2->Caption   = sum.cut2;
		sum.PutResultOnChart(SummaryChart, MainForm);
	}
}
// -----------------------------------------------------------------------------
void __fastcall TMainForm::menuTypeSizeClick(TObject *Sender)
{
	// показать окно с настройками типоразмера
	SettingsForm->Show();
}
// -----------------------------------------------------------------------------
void __fastcall TMainForm::menuMeasuresClick(TObject *Sender)
{
	// показать окно с настройками измерений
	ADCSettForm->Show();
}
// -----------------------------------------------------------------------------
void __fastcall TMainForm::menuQuitClick(TObject *Sender)
{
	// выход из программы
	MainForm->Close();
}
// -----------------------------------------------------------------------------
void __fastcall TMainForm::menuSignalsStateClick(TObject *Sender)
{
	// отображает форму с входами и выходами без возможности редактирования
	if (digital->cards_needed > digital->cards_exist)
		Application->MessageBoxW
			(L"Модуль не доступен, так как не хватает плат Advantech1730. \n Ошибка 001", L"Ошибка!", MB_ICONERROR);
	else
		SignalsStateForm->Show();
}
// -----------------------------------------------------------------------------
void __fastcall TMainForm::menuSaveTubeClick(TObject *Sender)
{
	if (SaveToFileDialog->Execute())
	{
		// Создаем пустой файл, чтобы записать в него собранные данные
		FILE *file;
		file = fopen(AnsiString(SaveToFileDialog->FileName).c_str(), "w");
		// fprintf(file,"%s \t %s\n","Zones","Sensors");
		fclose(file);

		pp.SaveTubeToFile(SaveToFileDialog->FileName); // Поперечные данные

		pr.SaveTubeToFile(SaveToFileDialog->FileName); // Продольные данные

		th.SaveTubeToFile(SaveToFileDialog->FileName); // Толщинометрия (только zone_data)
	}
}

//------------------------------------------------------------------------------
void __fastcall TMainForm::menuLoadTubeClick(TObject *Sender)
{
	if (OpenDialogFromFile->Execute())
	{
		ClearCharts(Sender);
		ReadFromFile((OpenDialogFromFile->FileName).c_str());
	}
}
// ---------------------------------------------------------------------------
void TMainForm::ReadFromFile(UnicodeString path)
{
	FILE *file;
	file = fopen(AnsiString(path).c_str(), "r+") ;

	if (file == NULL)
		Application->MessageBoxW(L"Не удалось открыть файл для чтения", L"Ошибка",MB_ICONERROR | MB_OK);
	pp.LoadTubeFromFile(file);
	pp.ComputeZoneData();
   //	pp.meas_per_sensor.set_length(pp.zones);
   //	for (int i = 0; i < pp.zones; i++)
	//	pp.meas_per_sensor[i] = pp.meas_per_zone;
	pp.CalculateDeadZone(ini->ReadInteger("Size","ZoneLength",2));

	pr.LoadTubeFromFile(file);
	pr.ComputeZoneData();
   //	pr.meas_per_sensor.set_length(pr.zones);
	//for (int i = 0; i < pr.zones; i++)
	 //	pr.meas_per_sensor[i] = pr.meas_per_zone;
	pr.CalculateDeadZone(ini->ReadInteger("Size","ZoneLength",2));

	th.LoadTubeFromFile(file);
	fclose(file); // закрыли

	pp.PutResultOnChart(CrossDefectChart, MainForm);
	pr.PutResultOnChart(LinearDefectChart, MainForm);
	th.PutResultOnChart(ThicknessChart , MainForm);

	sum.ComputeZonesData(pp, pr, th, true, true);
	sum.PutResultOnChart(SummaryChart, MainForm);
	sum.MakeDecision();
	this->lResult->Caption = sum.decision;
	this->lCut1->Caption   = sum.cut1;
	this->lCut2->Caption   = sum.cut2;
	Globals::isView        = true;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::menuTestLcardClick(TObject *Sender)
{
	// осциллограф - сканирует 1 датчик в реальном времени
	//	MainForm->Close();
    //ShellExecute(0, L"open", L"Project1.exe", 0, 0, SW_SHOWNORMAL);
}
// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuSolidGroupClick(TObject *Sender)
{
	// добавление эталонов по группе прочности
	SolidGroupForm->Send_Tube(tubeSG);
	SolidGroupForm->Show();
}
// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuTestAdvantechClick(TObject *Sender)
{
	// закрывает прогу и вызывает прогу с управлением Выходами платы Advantech
	ShellExecute(0, L"open", L"..\\..\\Settings\\IO_Management.exe", 0, 0, SW_SHOWNORMAL);
}
// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuF1Click(TObject *Sender)
{
	WinExec("hh ..\\..\\help\\Help.chm", SW_RESTORE);
}
// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuAboutClick(TObject *Sender)
{
//	AboutBox->Show();
}
// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuColorsClick(TObject *Sender)
{
	FormColor->Show();
}
// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuViewEtalonsClick(TObject *Sender)
{
	FormViewEtalons->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::menuProtocolClick(TObject *Sender)
{
	ProtocolForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ReportViewClick(TObject *Sender)
{
	FormReport->Show();
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::menuSGTestClick(TObject *Sender)
{
	SGForm->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::menuTestASUConnectionClick(TObject *Sender)
{
	MyCom->TestConnection();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::menuGraphicsSGClick(TObject *Sender)
{
	SGGraphForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ApplicationEventsMessage(tagMSG &Msg, bool &Handled)
{
	if (Msg.message == WM_HOTKEY)
	{ // сообщение наше
		if (Msg.wParam == 0x00E) // идентификатор наш
				WinExec("hh ..\\..\\help\\Help.chm", SW_RESTORE);
		else
		if (Msg.wParam == 0x00A) // идентификатор наш
			{
				if (SystemConst::HideInGain)
				{
					cbLinear->Visible = !cbLinear->Visible;
					cbThickness->Visible = !cbThickness->Visible;
					bGoodTube->Visible = ! bGoodTube->Visible;
				}
			}
	}
}
// ---------------------------------------------------------------------------
// блокировка/разблокировка клавиш
void TMainForm::SetAbleButtons(bool state , TButton *exc1 , TMenuItem *exc2 ,TButton *exc3, TButton *exc4 )
{
	this->cbLinear->Enabled    = state;
	this->cbThickness->Enabled = state;
	this->cbTypeSize->Enabled  = state;
	this->cbNoMarking->Enabled = state;

	this->bTest->Enabled         = state;
	this->bView->Enabled         = state;
	this->bWork->Enabled         = state;
	this->bCrashForward->Enabled = state;
	this->bReturnMode->Enabled   = state;
	this->bManage->Enabled       = state;
	this->bGoodTube->Enabled	 = state;
	this->bBadTube->Enabled	 = state;

	this->eCrossBorder1->Enabled            = state;
	this->eCrossBorder2->Enabled            = state;
	this->eLinearBorder1->Enabled           = state;
	this->eLinearBorder2->Enabled           = state;
	this->eMin_Good_Length->Enabled         = state;
	this->eThicknessBorder1->Enabled        = state;
	this->eThicknessBorder2->Enabled        = state;
	this->eCrossDeadZoneStart->Enabled      = state;
	this->eCrossDeadZoneFinish->Enabled     = state;
	this->eLinearDeadZoneStart->Enabled     = state;
	this->eLinearDeadZoneFinish->Enabled    = state;

	this->menuTube->Enabled         	 = state;
	this->menuSettings->Enabled     	 = state;
	this->menuTest->Enabled         	 = state;
	this->menuTestAdvantech->Enabled	 = state;
 //	this->menuTestLcard->Enabled     	 = state;
	this->menuSGTest->Enabled			 = state;
	this->menuTestASUConnection->Enabled = state;
	this->menuWork->Enabled          	 = state;
	this->menuCrashForward->Enabled  	 = state;
	this->menuReturnMode->Enabled    	 = state;
	this->menuManagement->Enabled    	 = state;
	this->menuQuit->Enabled          	 = state;

	if (exc1 != NULL)
		exc1->Enabled = !state;
	if (exc2 != NULL)
		exc2->Enabled = !state;
	if (exc3 != NULL)
		exc3->Enabled = !state;
	if (exc4 != NULL)
		exc4->Enabled = !state;
}
//----------------------------------------------------------------------------

void __fastcall TMainForm::cbEtalonClick(TObject *Sender)
{
	if(cbEtalon->Checked)
	{
		SetAbleButtons(false , bWork,NULL);
		bView->Enabled=true;
		bReturnMode->Enabled=true;
		cbInterruptView->Checked = true;
		cbInterruptView->Enabled = false;
	}
	else
	{
		cbInterruptView->Enabled=true;
		SetAbleButtons(true);
		bWork->Enabled=true;
	}
}
//----------------------------------------------------------------------------
void __fastcall TMainForm::cbNoMarkingClick(TObject *Sender)
{
	ini->WriteBool("LIR" , "NoMarking" , cbNoMarking->Checked);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::bStatsToNullClick(TObject *Sender)
{
	stats->StatsToNull();
	UpdateStats();
}
//---------------------------------------------------------------------------
void TMainForm::UpdateStats()
{
	eBrack->Text = stats->GetBrack();
	eValid->Text = stats->GetSecondClass() + stats->GetValid();
	eBrackInDaStreet->Text = stats->GetBrackInDaStreet();
	eBrack->Refresh();
	eValid->Refresh();
	eBrackInDaStreet->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::eBrackInDaStreetChange(TObject *Sender)
{
	ini->WriteInteger("Statistics","BrackInDaStreet", eBrackInDaStreet->Text.ToInt());
	stats->SetBrackInDaStreet(eBrackInDaStreet->Text.ToInt());
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MixingTimerTimer(TObject *Sender)
{
	if(digital->SignalExists("марк воздух") )
	{
		MixingTimer->Enabled = false;
		if ( digital->CheckInBouncelessSignal("Марк воздух",true) )
		{
			int mixTime = ini->ReadInteger("LIR" , "TimeMixing", 1000);
			digital->SetOutSignal("Бак перемешать");
			for (DWORD tt = GetTickCount(); GetTickCount() - tt < mixTime; Sleep(10))
				Application->ProcessMessages();
			digital->ResetOutSignal("Бак перемешать");
			MixingTimer->Interval = ini->ReadInteger("LIR" , "PauseMixing" , 300000);
			cbNoMarking->Enabled = true;
			cbNoMarking->Checked = false;
			cbNoMarking->Hint = "Работа без маркировки дефектов" ;
		}
		else
		{
			cbNoMarking->Enabled = false;
			cbNoMarking->Checked = true;
			cbNoMarking->Hint = "Нет воздуха в системе маркировки!" ;
			MixingTimer->Interval = 60000;
		}
		MixingTimer->Enabled = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Button1Click(TObject *Sender)
{
/*
	vector<double> test;
	test.resize(8);
	test[0] = 1770;
	test[1] = 1768;
	test[2] = 1765;
	test[3] = 1760;
	test[4] = 1780;
	test[5] = 1880;
	test[6] = 1762;
	test[7] = 1300;
*/
	tubeSG.calcSolidGroup( tubeSG.Coordinates() );

/*	// контрольная сумма по 2-ому алгоритму
	unsigned char test[5] = { 0x5, 2, 0x1 ,0,0 };	// длина посылки 5, абонент, функция обмена 1
	unsigned short rock = MyCom->countCRC16_2(test, 3);
	Edit1->Text = String::IntToHex(rock,4);
*/

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button2Click(TObject *Sender)
{
/*
	// тест посылки результата
	SolidGroups::Group sggg;
	sggg = SolidGroups::Group::sgE;
	bool pzd = MyCom->SendResultToASU(pp, pr, th, sum , sggg);
	if (pzd)
		StatusBarTop->Panels->Items[2]->Text="Посылка отправлена успешно";
	else
		StatusBarTop->Panels->Items[2]->Text="Передача неудача";
*/
	// тест посылки с номером трубы
/*	long shit = MyCom->GetTubeNumber();
	StatusBarTop->Panels->Items[2]->Text="Номер трубы: " + IntToStr((__int64)shit);
	int fuck = MyCom->GetPlantCut();
	StatusBarTop->Panels->Items[2]->Text += ", рез: " + IntToStr(fuck);

 */
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::bManualSGClick(TObject *Sender)
{
	TForm * fsg = new TForm(MainForm);
	TLabel * cap = new TLabel(fsg);
	TLabel * lnum = new TLabel(fsg);
	TLabel * lnam = new TLabel(fsg);
	TEdit * number = new TEdit(fsg);
	TEdit * namesg = new TEdit(fsg);
	fsg->Width = 400;
	fsg->Left = 700;
	fsg->Top = 80;
	fsg->Height = 150;
	fsg->Caption = "Группа прочности принудительно";
	cap->Parent = fsg;
	cap->Caption = "Введите группу прочности, которая будет замещаться вместо неопределенной";
	cap->WordWrap = true;
	cap->SetBounds(20,20,300,50);
	lnum->Parent = fsg;
	lnam->Parent = fsg;
	lnum->SetBounds(20,60,100,20);
	number->Text = "4";
	namesg->Text = "N80";
	lnam->SetBounds(20,80,100,20);
	lnum->Caption = "Номер в АСУ: ";
	lnam->Caption = "Имя группы: ";
	number->SetBounds(140,60,60,40);
	number->NumbersOnly = true;
	number->Parent = fsg;
	namesg->Parent = fsg;
	namesg->SetBounds(140,80,60,40);
	fsg->ShowModal();
	manual_num = number->Text.ToInt();
	manual_name = namesg->Text;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::WmDropFiles(TWMDropFiles& Message)
{
   HDROP drop_handle = (HDROP)Message.Drop;
   wchar_t fName[MAXPATH];
   int filenum = DragQueryFile(drop_handle, -1, NULL, NULL);
   for(int i = 0; i < filenum; i++){
	  DragQueryFile(drop_handle, i, fName, MAXPATH);
	  ReadFile(fName);
	 }
   DragFinish(drop_handle);
}
//---------------------------------------------------------------------------------
// После перетаскивания, вычисляем расширения и открываем файл -----
void __fastcall TMainForm::ReadFile(AnsiString FileName)
{
   AnsiString str = ExtractFileExt(FileName);
   if(str == ".sg")
   {
		SGForm->Show();
		SGForm->LoadFromFile(FileName);
   }
   else if(str == ".dkb")
		ReadFromFile(FileName);
}
//---------------------------------------------------------------------------------

void __fastcall TMainForm::N1Click(TObject *Sender)
{
	// about
	AboutBox1->Show();
}
//---------------------------------------------------------------------------

