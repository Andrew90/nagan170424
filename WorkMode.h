//---------------------------------------------------------------------------

#ifndef WorkModeH
#define WorkModeH
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
#include "IniFiles.hpp"

#include "AdvantechSignals.h"
#include "LCARD791.h"
#include "Result.h"
#include "Global.h"
#include "ModulePosition.h"
#include "AlarmThread.h"
#include "SMS.h"

class WorkThreadClass : public TThread
{

protected:

	void __fastcall Execute();
	LCard791 *lpop;					   	// указатель на плату поперечных дефектов
  //	LCard791 *lprod;				   	// указатель на плату продольных дефектов
	Result *respop;						// указатель на результат поперечный
	Result *resprod;					// указатель на результат продольный
	TStatusBar *St1;					// указатель на статусбар, показывающий течение процесса
	bool Thick;							// используем ли толщинометрию
	bool Linear;						// используем ли продольный
	bool Prepare;						// нужно ли готовить к началу работы

	String stext1;						// текст для надписи в статусбаре в панели 1
	String stext2;						// текст для надписи в статусбаре в панели 2

	double cross_len;		// длина поперечного модуля , мм
	double linear_len;		// длина продольного модуля , мм
	double st_len;			// расстояние стойка 1 - стойка 4
	int zone_len;			// длина 1 зоны , мм
	AlarmThreadClass *lat;	// локальный тред аварии (адрес)
	long MeasOutCrossModule;

public:

__fastcall WorkThreadClass(bool CreateSuspended,	 		// создавать поток замороженным
					LCard791 &lpp,							// указатель на плату поперечных дефектов
				 //	LCard791 &lpr,							// указатель на плату продольных дефектов
					Result &_respop,						// указатель на результат поперечный
					Result &_resprod,						// указатель на результат продольный
					TStatusBar *St,							// указатель на статусбар, показывающий течение процесса
					bool _Thick,							// используем ли толщинометрию
					bool _Linear,                           // используем ли продольный)
					AlarmThreadClass &alth,					// адрес треда аварии
					bool IsPrepare);						// первый цикл (нужно ли готовить к работе систему)

	UnicodeString PrepareForWork( );		// выставляем исходное перед работой

	bool WorkMode( );						// режим РАБОТЫ!!!

	void __fastcall UpdateMainForm();		// обновляет компоненты главной формы, обрабатывает события
	bool cool;								// вместо Return Value, который хуй откопаешь
	bool transit;
	// времена для расчетов трубы.
	DWORD cross_t1, cross_t2, cross_t3;		// времена поперечного
	DWORD lin_t1, lin_t2, lin_t3;			// времена поперечного
	DWORD st1 , st4 , st1_out;				// времена стоек 1 и 4


};

//---------------------------------------------------------------------------
#endif
