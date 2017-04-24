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

	void __fastcall Execute();		// ������ ����� �� �������� ������ �� ����� ������
	TMutex *readyM;					// ������ �������� ������� ����������
	bool control;					// ��������� ������� ��������

public:

	__fastcall AlarmThreadClass(bool CreateSuspended);	// ����������� ������
	void __fastcall UpdateCaption();					// ��������� ������� �����
	bool is_alarm;										// ���� �� ������
	String reason;										// ������� ������
	void SetControl();									// ��������� ��������
	void ResetControl();								// �� ��������� ��������
	void SetPlantCycle();								// ��������� ��� ����
	void ResetPlantCycle();								// �� ��������� ��� ����
	void SetStand(String st1, String st2 = "");			// ����� ������� ����� ���������
	void ResetStand();									// ������ �� ��������� ������

};
//---------------------------------------------------------------------------
#endif
