//---------------------------------------------------------------------------

#include "intsafe.h"
#include "DataModule.h"
#ifndef StatisticsH
#define StatisticsH
//---------------------------------------------------------------------------
class Statistics
{
	public:
		Statistics();
		bool AddTube(UnicodeString Result);		   //Добавляет результат за смену
		void StatsToNull();                			// обнулдяет все
//		void GetValue(ULONG _Brack, ULONG _SecondClass, ULONG _Valid);   //Возвращает результаты за смену
		ULONG GetBrack();
		ULONG GetSecondClass();
		ULONG GetValid();
		ULONG GetBrackInDaStreet();
		void SetBrackInDaStreet( int BrackCount);
		void GetAllValue(ULONG &_Brack, ULONG &_SecondClass, ULONG &_Valid);  //возвращает данные по результатам  за все время(из базы)
	private:
		ULONG Brack;
		ULONG SecondClass;
		ULONG Valid;
		ULONG BrackInDaStreet;
		TIniFile *ini;

};
#endif
