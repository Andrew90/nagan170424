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
		void ExportAllTubes(TDateTime Begin, TDateTime End,UnicodeString SG ,UnicodeString Result,UnicodeString TS); //��������� ������ � ����� �������
		void ExportTube(ULONG ID);                            	//��������� ����� �����

		Variant App;                                         	//������ Application
		Variant Sh;                                          	//������ ������ ����
		Variant Rang;                                           //������ ������(������� �����)
	private:
		void DrawExcelAll();
		void DrawExcelZone();
		void ExcelInit(UnicodeString File);                  		//�������������� ��������
		void ToExcelCell(int Row, int Column, Variant data);        //����� ������ � ������
		void StrToExcelCell(int Row, int Column, AnsiString data);  //����� ������ � ������
		TIniFile *Ini;                      						//���� ��������
		bool Opened;                                                //���� ��� �������� �����
		String AntiGovno(String Res);
};

#endif
