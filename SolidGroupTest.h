//---------------------------------------------------------------------------

#ifndef SolidGroupTestH
#define SolidGroupTestH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "Main.h"
#include "Chart.hpp"
#include "Series.hpp"
#include "TeEngine.hpp"
#include "TeeProcs.hpp"
#include <Dialogs.hpp>
#include <dir.h>
#include "clustering.h"
//---------------------------------------------------------------------------
class TSGForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *pTop;
	TStatusBar *StatusBar;
	TButton *bTest;
	TButton *bOpenTube;
	TButton *bSaveTube;
	TChart *SignalChart;
	TLabel *lTestTime;
	TEdit *eTestTime;
	TOpenDialog *OpenDialog;
	TSaveDialog *SaveDialog;
	TCheckBox *cbIsSignal;
	TLineSeries *Series;
	TLineSeries *Series1;
	TButton *bShowCurrentTube;
	TPanel *BottomPanel;
	TPopupMenu *PopupMenuTestSG;
	TMenuItem *N1;
	TLabel *lZoom;
	TTrackBar *tbZoom;
	TScrollBar *scbChartDragger;
	TPanel *pThresholds;
	TMenuItem *N2;
	TMenuItem *AddParameters;
	void __fastcall bTestClick(TObject *Sender);
	void __fastcall bOpenTubeClick(TObject *Sender);
	void __fastcall bSaveTubeClick(TObject *Sender);
	void __fastcall bShowCurrentTubeClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall N1Click(TObject *Sender);
	void __fastcall tbZoomChange(TObject *Sender);
	void __fastcall scbChartDraggerChange(TObject *Sender);
	void __fastcall N2Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
 //	void __fastcall Edit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall AddParametersClick(TObject *Sender);
private:	// User declarations
	static const int quantity_thresholds = 8;
	vector<int> buffer;
	TEdit *eThresholds[quantity_thresholds];    //Поля для отображения порогов
	LCard791 *lcard;							//Указатель на поперечную плату в которая открывается при открытии проги
	void DrawOnChart();							//рисует на чарте
	void DrawThresholds();						// рисует пороги на чарте
	TLineSeries *thresholds[8];					// массив серий с порогами
	vector <int> Signal;						//Вектор с сигналом
	void SaveToFile(UnicodeString FileName);	//сохраняет трубу
	SolidGroups::Tube tube_sg;					// труба с характеристиками группы прочности
//	double zoomFactor;							// для навигации и удобного зума графика
	long curPos;
	void __fastcall WmDropFiles(TWMDropFiles& Message);
	void __fastcall ReadFile(AnsiString FileName);
	BEGIN_MESSAGE_MAP MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WmDropFiles) END_MESSAGE_MAP(TForm);
public:		// User declarations
	void LoadFromFile(UnicodeString FileName);	//открывает трубу
	__fastcall TSGForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSGForm *SGForm;
//---------------------------------------------------------------------------
#endif
