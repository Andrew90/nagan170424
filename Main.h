//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Chart.hpp"
#include "TeEngine.hpp"
#include "TeeProcs.hpp"
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "Series.hpp"
#include <ComCtrls.hpp>
#include <windows.h>
#include "IniFiles.hpp"
#include <ImgList.hpp>
#include <Dialogs.hpp>
#include <AppEvnts.hpp>
#include "TeeGDIPlus.hpp"

#include "ADCSettings.h"
#include "Protocol.h"
#include "View.h"
//#include "Result.h"
#include "LCARD791.h"
#include "AdvantechSignals.h"
#include "Settings.h"
#include "Manage.h"
#include "SignalsState.h"
#include "ReturnMode.h"
#include "CrashForward.h"
#include "WorkMode.h"
#include "AlarmThread.h"
#include "Statistics.h"
#include "SolidGroup.h"
#include "SolidGroupTest.h"
#include "SolidGroupGraphics.h"
#include "ComPort.h"
#include <dir.h>
#include "Advantech1784.h"
#include "ThreadOnLine.h"
#include "ExtTransport.h"
#include "CommPort.h"
#include "ABOUT_NTC_NK_URAN.h"

class Advantech1784;
class ThreadOnLine;
//разгадана загадка 21го века!!!
//если в новом модуле с описанием класса в .h файл подключить main.h
//то main перестает видеть его тип, и класс придется явно объявлять как сделано выше.
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TButton *bTest;
	TButton *bWork;
	TButton *bView;
	TButton *bManage;
	TButton *bCrashForward;
	TButton *bReturnMode;
	TButton *bCancelWork;
	TButton *bStatsToNull;
	TMainMenu *MainMenu1;
	TMenuItem *menuJob;
	TMenuItem *menuTube;
	TMenuItem *menuSettings;
	TMenuItem *menuCrashForward;
	TMenuItem *menuView;
	TMenuItem *menuTypeSize;
	TMenuItem *menuMeasures;
	TMenuItem *menuWork;
	TMenuItem *menuReturnMode;
	TMenuItem *menuQuit;
	TMenuItem *menuDiagnostics;
	TMenuItem *menuTest;
	TMenuItem *menuSignalsState;
	TMenuItem *menuProtocol;
	TMenuItem *menuSaveTube;
	TMenuItem *menuLoadTube;
	TMenuItem *menuClearCharts;
	TMenuItem *menuManagement;
	TMenuItem *menuSolidGroup;
	TMenuItem *menuTestAdvantech;
	TMenuItem *menuHelp;
	TMenuItem *menuF1;
	TMenuItem *menuColors;
	TMenuItem *menuViewEtalons;
	TMenuItem *menuSGTest;
	TMenuItem *menuTestASUConnection;
	TGroupBox *gbCrossDef;
	TGroupBox *gbLinearDef;
	TGroupBox *gbThickness;
	TGroupBox *gbSummary;
	TChart *CrossDefectChart;
	TChart *LinearDefectChart;
	TChart *ThicknessChart;
	TChart *SummaryChart;
	TEdit *eCrossBorder1;
	TEdit *eCrossBorder2;
	TEdit *eLinearBorder2;
	TEdit *eLinearBorder1;
	TEdit *eThicknessBorder2;
	TEdit *eThicknessBorder1;
	TEdit *eMin_Good_Length;
	TEdit *eCrossDeadZoneStart;
	TEdit *eCrossDeadZoneFinish;
	TEdit *eLinearDeadZoneFinish;
	TEdit *eLinearDeadZoneStart;
	TStatusBar *StatusBarBottom;
	TStatusBar *StatusBarTop;
	TCheckBox *cbLinear;
	TCheckBox *cbThickness;
	TCheckBox *cbInterruptView;
	TCheckBox *cbEtalon;
	TCheckBox *cbNoMarking;
	TSaveDialog *SaveToFileDialog;
	TOpenDialog *OpenDialogFromFile;
	TLabel *lResult;
	TLabel *lCut1;
	TLabel *lCut2;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label13;
	TPanel *pStatistics;
	TLabeledEdit *eValid;
	TLabeledEdit *eBrack;
	TLabeledEdit *eBrackInDaStreet;
	TComboBox *cbTypeSize;
	TButton *bGoodTube;
	TPanel *pSolidGroup;
	TTimer *MixingTimer;
	TApplicationEvents *ApplicationEvents;
// пробные кноки
	TButton *Button1;
	TButton *Button2;
	TEdit *Edit1;
	TMemo *Memo1;
	TButton *bBadTube;
	TLabel *Label14;
	TLabel *Label4;
	TLabel *Label6;
	TLabel *Label3;
	TLabel *Label5;
	TBarSeries *Series1;
	TBarSeries *Series2;
	TBarSeries *Series3;
	TBarSeries *Series4;
	TBarSeries *Series5;
	TBarSeries *Series6;
	TBarSeries *Series7;
	TBarSeries *Series8;
	TBarSeries *Series9;
	TBarSeries *Series10;
	TBarSeries *Series11;
	TBarSeries *Series12;
	TBarSeries *Series13;
	TBarSeries *Series14;
	TBarSeries *Series15;
	TBarSeries *Series16;
	TBarSeries *Series17;
	TBarSeries *Series18;
	TBarSeries *Series19;
	TBarSeries *Series20;
	TBarSeries *Series21;
	TBarSeries *Series22;
	TButton *bManualSG;
	TMenuItem *N1;

// мои функции
	void __fastcall ClearCharts(TObject *Sender);			// очищает все графики
	void __fastcall MainFormInit(TObject *Sender);			// выравнивает объекты на форме, работает в начале работы и при Resize
// стандартные функции
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall FormActivate(TObject *Sender);
// нажатие пунктов главного меню
	void __fastcall menuTypeSizeClick(TObject *Sender);
	void __fastcall menuMeasuresClick(TObject *Sender);
	void __fastcall menuQuitClick(TObject *Sender);
	void __fastcall menuSignalsStateClick(TObject *Sender);
	void __fastcall menuSaveTubeClick(TObject *Sender);
	void __fastcall menuLoadTubeClick(TObject *Sender);
	void __fastcall menuTestLcardClick(TObject *Sender);
	void __fastcall menuSolidGroupClick(TObject *Sender);
	void __fastcall menuTestAdvantechClick(TObject *Sender);
	void __fastcall menuF1Click(TObject *Sender);
	void __fastcall menuAboutClick(TObject *Sender);
	void __fastcall menuViewEtalonsClick(TObject *Sender);
	void __fastcall menuProtocolClick(TObject *Sender);
	void __fastcall menuColorsClick(TObject *Sender);
	void __fastcall menuSGTestClick(TObject *Sender);
	void __fastcall menuTestASUConnectionClick(TObject *Sender);
	void __fastcall menuGraphicsSGClick(TObject *Sender);
// нажатие кнопок на форме
	void __fastcall bTestClick(TObject *Sender);
	void __fastcall bViewClick(TObject *Sender);
	void __fastcall bWorkClick(TObject *Sender);
	void __fastcall bCrashForwardClick(TObject *Sender);
	void __fastcall bReturnModeClick(TObject *Sender);
	void __fastcall bCancelWorkClick(TObject *Sender);
	void __fastcall bManageClick(TObject *Sender);
	void __fastcall bGoodTubeClick(TObject *Sender);
	void __fastcall bStatsToNullClick(TObject *Sender);
	void __fastcall ChartClick(TObject *Sender);
	void __fastcall cbTypeSizeSelect(TObject *Sender);
	void __fastcall cbEtalonClick(TObject *Sender);
	void __fastcall eSettingsChange(TObject *Sender);
	void __fastcall ApplicationEventsMessage(tagMSG &Msg, bool &Handled);
	void __fastcall ReportViewClick(TObject *Sender);
	void __fastcall eBrackInDaStreetChange(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall cbNoMarkingClick(TObject *Sender);
	void __fastcall MixingTimerTimer(TObject *Sender);
	void __fastcall bBadTubeClick(TObject *Sender);
	void __fastcall bManualSGClick(TObject *Sender);
	void __fastcall N1Click(TObject *Sender);

private:	// User declarations

   	// настройки типоразмеров
	void LoadSettings(TObject *Sender);		// читает настройки из ini файла, заполняет едиты, устанавли пороги, мертвые зоны
	TIniFile *ini;							// ини файл с настройками
	void UpdateComboBox();					// обновляет ComboBox с типоразмерами (после добавления или удаления, в начале работы)

	// работа с группой прочности
	int tempSGbegin;						// температура модуля ГП в начале цикла работы
	void SetSolidGroup(TObject *Sender);	// читает эталоны определенного типоразмера из БД и инициализирует их
	void IdentifySolidGroup();				// определяет группу прочности прошедешей трубы
	SolidGroups::Tube tubeSG;				// труба с параметрами группы прочности
	SolidGroups::Analyzer anal;				// анализатор группы прочности
	// параллельные потоки
	WorkThreadClass *workthread;            // тред работы
	AlarmThreadClass *alarmthread;			// тред проверки аварийных ситуаций
	ReturnThread *returnthread;				// тред режима Возврат
	CrashForwardThread *cfthread;			// тред режима Выгон
	// ЛИР
	ThreadOnLine *workonline;				// тред отрисовки в реальном времени
	Advantech1784 *Counter;                 // счетчик ЛИР
	// блокировка
	void SetAbleButtons(bool state , TButton *exc1 = NULL, TMenuItem *exc2 = NULL,
					TButton *exc3 = NULL, TButton *exc4 = NULL);	// блокировка/разблокировка клавиш
	void SetProjectSpecialty();				// устанавливает характеристики проекта
	bool GoodTube;							// оператор меняет результат принудительно на Годно
	bool BadTube;							// оператор меняет результат на Брак
	Statistics *stats;						// экземпляр статистики прогона труб
	void UpdateStats();						// обновить статистику
	// костыли ГП Пыть-ях
	int manual_num;
	String manual_name;
	void ReadFromFile(UnicodeString path);
			//для реализации Drag & Drop
	void __fastcall WmDropFiles(TWMDropFiles& Message);
	void __fastcall ReadFile(AnsiString FileName);
	BEGIN_MESSAGE_MAP MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WmDropFiles) END_MESSAGE_MAP(TForm);
public:		// User declarations

	__fastcall TMainForm(TComponent* Owner);
	LCard791 *l1;			// плата поперечного контроля, температуры и магнитного поля
 //	LCard791 *l2;			// плата продольного контроля, Группы прочности
	Result pp, pr;			// поперечный и продольный результат
	ThicknessResult th;		// результат по толщине
	SummaryResult sum;		// сумммарный результат



	void __fastcall CompleteWork(TObject *Sender);		// событие OnTerminate для треда работы
	void __fastcall	AlarmFinished(TObject *Sender);		// событие OnTermioate для аварийного треда
	void __fastcall ReturnFinished(TObject *Sender);	// событие OnTerminate для треда возврата трубы
	void __fastcall TransitFinished(TObject *Sender);	// событие OnTerminate для треда выгона/транзита трубы

};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif

