//---------------------------------------------------------------------------

#ifndef AlarmThreadH
#define AlarmThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
#include "AdvantechSignals.h"

class AlarmThreadClass : public TThread
{

protected:

	void __fastcall Execute();		// запуск треда на проверку аварий во время работы
	TMutex *readyM;					// мюьекс проверки сигнала Готовность
	bool control;					// проверрка сигнала Контроль

public:

	__fastcall AlarmThreadClass(bool CreateSuspended);	// конструктор класса
	void __fastcall UpdateCaption();					// обновляем главную форму
	bool is_alarm;										// была ли авария
	String reason;										// причина аварии
	void SetControl();									// проверяем Контроль
	void ResetControl();								// не проверяем Конртоль
	void SetPlantCycle();								// проверяем Цех цикл
	void ResetPlantCycle();								// не проверяем Цех цикл
	void SetStand(String st1, String st2 = "");			// какие сигналы стоек проверять
	void ResetStand();									// больше не проверять стойки

};
//---------------------------------------------------------------------------
#endif
