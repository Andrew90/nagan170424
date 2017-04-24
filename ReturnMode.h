//---------------------------------------------------------------------------
#ifndef ReturnModeH
#define ReturnModeH
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

#include "AdvantechSignals.h"
#include "ModulePosition.h"
#include "Protocol.h"
#include "AlarmThread.h"


// ����� ��������, ��������� ��� ������� � ���������� ����� �� ������ �����
class ReturnThread : public TThread
{
protected:
	void __fastcall Execute();
	TStatusBar *StatusBar;				// ��������� �� ���������, ������������ ������� ��������
	String stext1;						// ����� ��� ������� � ���������� � ������ 1
	String stext2;						// ����� ��� ������� � ���������� � ������ 2

public:

	__fastcall ReturnThread(bool CreateSuspended , TStatusBar *St, AlarmThreadClass &_alth);
	void __fastcall UpdateMainForm();									// ��������� ���������� ������� �����, ������������ �������
	bool ReturnMode();			// ����� ��������, �������� �������
	bool cool;															// ������ Return Value, ������� ��� ���������


};




#endif
