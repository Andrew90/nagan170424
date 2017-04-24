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
	LCard791 *lpop;					   	// ��������� �� ����� ���������� ��������
  //	LCard791 *lprod;				   	// ��������� �� ����� ���������� ��������
	Result *respop;						// ��������� �� ��������� ����������
	Result *resprod;					// ��������� �� ��������� ����������
	TStatusBar *St1;					// ��������� �� ���������, ������������ ������� ��������
	bool Thick;							// ���������� �� �������������
	bool Linear;						// ���������� �� ����������
	bool Prepare;						// ����� �� �������� � ������ ������

	String stext1;						// ����� ��� ������� � ���������� � ������ 1
	String stext2;						// ����� ��� ������� � ���������� � ������ 2

	double cross_len;		// ����� ����������� ������ , ��
	double linear_len;		// ����� ����������� ������ , ��
	double st_len;			// ���������� ������ 1 - ������ 4
	int zone_len;			// ����� 1 ���� , ��
	AlarmThreadClass *lat;	// ��������� ���� ������ (�����)
	long MeasOutCrossModule;

public:

__fastcall WorkThreadClass(bool CreateSuspended,	 		// ��������� ����� ������������
					LCard791 &lpp,							// ��������� �� ����� ���������� ��������
				 //	LCard791 &lpr,							// ��������� �� ����� ���������� ��������
					Result &_respop,						// ��������� �� ��������� ����������
					Result &_resprod,						// ��������� �� ��������� ����������
					TStatusBar *St,							// ��������� �� ���������, ������������ ������� ��������
					bool _Thick,							// ���������� �� �������������
					bool _Linear,                           // ���������� �� ����������)
					AlarmThreadClass &alth,					// ����� ����� ������
					bool IsPrepare);						// ������ ���� (����� �� �������� � ������ �������)

	UnicodeString PrepareForWork( );		// ���������� �������� ����� �������

	bool WorkMode( );						// ����� ������!!!

	void __fastcall UpdateMainForm();		// ��������� ���������� ������� �����, ������������ �������
	bool cool;								// ������ Return Value, ������� ��� ���������
	bool transit;
	// ������� ��� �������� �����.
	DWORD cross_t1, cross_t2, cross_t3;		// ������� �����������
	DWORD lin_t1, lin_t2, lin_t3;			// ������� �����������
	DWORD st1 , st4 , st1_out;				// ������� ����� 1 � 4


};

//---------------------------------------------------------------------------
#endif
