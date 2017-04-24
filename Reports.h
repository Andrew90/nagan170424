//---------------------------------------------------------------------------

#ifndef ReportsH
#define ReportsH

#include "DataModule.h"
#include "Comobjec.h"
#include "Global.h"
#include <vector>

//---------------------------------------------------------------------------
class Reports
{
	public:
		Reports();
		~Reports();
		void ExportAllTubes(TDateTime Begin, TDateTime End,UnicodeString SG ,UnicodeString Result,UnicodeString TS); //Принимает начало и конец периода
		void ExportTube(ULONG ID);                            	//Принимает номер трубы

		Variant App;                                         	//Объект Application
		Variant Sh;                                          	//Объект объект Лист
		Variant Rang;                                           //Объект ячейка(область ячеек)
	private:
		void DrawExcelAll();
		void DrawExcelZone();
		void ExcelInit(UnicodeString File);                  		//Инициализируем документ
		void ToExcelCell(int Row, int Column, Variant data);        //Пишет данные в ячейку
		void StrToExcelCell(int Row, int Column, AnsiString data);  //Пишет строку в ячейку
		TIniFile *Ini;                      						//Файл настроек
		bool Opened;                                                //Флаг для закрытия файла
		String AntiGovno(String Res);
};

#endif
