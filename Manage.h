//---------------------------------------------------------------------------

#ifndef ManageH
#define ManageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
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

#include "AdvantechSignals.h"
//#include "Advantech1784.h"
//#include "ThreadOnLine.h"
#include "Main.h"
#include "LCard791.h"
#include "ModulePosition.h"

class Advantech1784;
//---------------------------------------------------------------------------
class TManageForm : public TForm
{
__published:	// IDE-managed Components
	TTimer *InputTimer;
	TGroupBox *gbLinear;
	TGroupBox *gbThickness;
	TGroupBox *gbCross;
	TGroupBox *gbTransport;
	TLabel *lLinearCentralizer;
	TPanel *pLinearCentralizer;
	TPanel *pLinearState;
	TLabel *Label2;
	TBitBtn *bThicknessState;
	TPanel *pThicknessState;
	TLabel *lThicknessState;
	TBitBtn *bThicknessCentralizer;
	TPanel *pThicknessCentralizer;
	TLabel *lThicknessCentralizer;
	TBitBtn *bCrossState;
	TPanel *pCrossState;
	TLabel *Label5;
	TStatusBar *StatusBarBottom;
	TBitBtn *bBackward;
	TBitBtn *bForward;
	TBitBtn *bStop;
	TLabel *Label6;
	TBitBtn *bPowerSolenoid;
	TBitBtn *bCoolerSolenoid;
	TCheckBox *cbRL;
	TCheckBox *cbRM;
	TCheckBox *cbRH;
	TBitBtn *bLinearCentralizer;
	TBitBtn *bLinearState;
	TBitBtn *bRotation;
	TPanel *RealTimePanel;
	TLabel *Lt1;
	TLabel *Lt2;
	TLabel *Lm;
	TEdit *EditT1;
	TEdit *EditT2;
	TEdit *EditMg;
	TTimer *TemperatureTimer;
	TGroupBox *gbCounters;
	TButton *bStartLIR;
	TButton *bStopLIR;
	TButton *bResetLIR;
	TTimer *CounterTimer;
	TMemo *memoCounters;
	TGroupBox *GroupPerekl;
	TBitBtn *bLoader1;
	TBitBtn *bLoader2;
	TPanel *pLoader1;
	TPanel *pLoader2;
	TPanel *pBegin;
	TPanel *pStillage;
	TPanel *pEndLines;
	TPanel *pNextLines;
	void __fastcall InputTimerTimer(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall bAnyStateClick(TObject *Sender);
	void __fastcall bAnyCentralizerClick(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall bForwardClick(TObject *Sender);
	void __fastcall bBackwardClick(TObject *Sender);
	void __fastcall bStopClick(TObject *Sender);
	void __fastcall bPowerSolenoidClick(TObject *Sender);
	void __fastcall bCoolerSolenoidClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall bRotationClick(TObject *Sender);
	void __fastcall TemperatureTimerTimer(TObject *Sender);
	void __fastcall bStartLIRClick(TObject *Sender);
	void __fastcall bResetLIRClick(TObject *Sender);
	void __fastcall bStopLIRClick(TObject *Sender);
	void __fastcall CounterTimerTimer(TObject *Sender);
	void __fastcall bLoader1Click(TObject *Sender);
	void __fastcall bLoader2Click(TObject *Sender);

private:	// User declarations

	void ShowCentralizerOpened	( TBitBtn *btn, TPanel *pan );	// отображение разомкнутого центратора (схвата)
	void ShowCentralizerClosed	( TBitBtn *btn, TPanel *pan );	// отображение замкнутого центратора (схвата)
	void ShowModuleinWork		( TBitBtn *btn, TPanel *pan );	// отображение модуля в режиме РП (рабочее положение)
	void ShowModuleinService	( TBitBtn *btn, TPanel *pan );	// отображение модуля в режиме ПО (положение обслуживания)
	void ShowModuleMoving		( TBitBtn *btn, TPanel *pan );	// отображение модуля в движении
	void ShowModuleState		( byte state, TBitBtn *btn, TPanel *pan );	// отображение состояния модуля
// GAN **********************
	void ShowLoaderUp	     	( TBitBtn *btn, TPanel *pan );  				// отображение положения ЛЮБОГО перекладчика вверху
	void ShowLoaderDown	    	( TBitBtn *btn, TPanel *pan );  				// отображение положения ЛЮБОГО перекладчика внизу
	void ShowLoaderMoving      	( TBitBtn *btn, TPanel *pan );  				// отображение движения ЛЮБОГО перекладчика
	void ShowLoaderState		( byte state, TBitBtn *btn, TPanel *pan ); 		// отображение состояния ЛЮБОГО перекладчика
	void ShowLoadPosition		( byte position, TPanel *pan1, TPanel *pan2 ); 	// отображение состояния концевых датчиков (начало или конец)
	void ShowLoadPosAllowed		( TPanel *pan1, TPanel *pan2 );  // отображение того, что движение перекладчика разрешено
	void ShowLoadPosDenied		( TPanel *pan1, TPanel *pan2 );  // отображение того, что движение перекладчика запрещено


	bool is_pix_ok;						// загружены ли картинки для кнопок
	bool butt_enabled;					// блокировка кнопок при движении трубы
	TColor on, off, move; 		   	    // цвета вкл , выкл сигнала и движения модуля
	String centr_on, centr_off;			// надписи вкл и выкл центраторов
	String centr_close, centr_open;		// надписи кнопок вкл и выкл центраторов
	String mod_on, mod_off, mod_mov;	// надписи модуль в работе, в по и в движении
	String mod_work, mod_service, mod_dsbl;			// надписи кнопок поднятия и опускания модулей
// GAN *********************
	String load_Up, load_Down, load_mow, load_st, load_st1, load_work;  	// надписи перекладчиков
	String load_offoff, load_onon;   						  	// надписи панелей
	Graphics::TBitmap *load_UP, *load_Dwn, *load_mvg;  			// рисунки для кнопок перекладчиков

	Graphics::TBitmap *centr_cl, *centr_op;						// рисунки для кнопок центраторов
	Graphics::TBitmap *mod_serv, *mod_wrk,*mod_mvg;				// рисунки для кнопок cмены состояния модулей

	LCard791 *lt;
	Advantech1784 *lCounter;

public:		// User declarations

	__fastcall TManageForm(TComponent* Owner);
	void SendHandleToSignals(LCard791 &lcard);		// посылает экземпляр с сигналами из главной формы сюда, в управление

};
//---------------------------------------------------------------------------
extern PACKAGE TManageForm *ManageForm;
//---------------------------------------------------------------------------
#endif
