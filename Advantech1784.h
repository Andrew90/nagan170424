//------------------------------------------------------------------------------

#ifndef Advantech1784H
#define Advantech1784H

#include "Protocol.h"
#include "Main.h"

#include "include1784\Driver.h"
#include "include1784\paras.h"
#include "include1784\event.h"
#include "IniFiles.hpp"

#pragma comment(lib, "Adsapi32.lib")
//------------------------------------------------------------------------------

class Advantech1784 : public TThread
{
protected:
		void __fastcall Execute();

public:
		__fastcall Advantech1784(bool CreateSuspended); //����������� ��� ����������� ����� ����� ������(���������� �������� �����)
		Advantech1784();            		// ��������� �����������
		ULONG GetCounter(int num);  		// ���������� �������� �������� num
		bool CloseDevice();					// ������� �����
		bool Reset(int NumOfCounter = -1);	// �������� ��� �������� �� 0
		String Error;						// ������ � �������

private:
		int Channel[Globals::Counters];		// ������ 3 ������� �����
		TIniFile *ini;
		bool m_bContinue;         			// ������� ����������� ����� ������
		ULONG Counter[Globals::Counters];	// ������ ��������� (3 �����)
		LRESULT m_ErrCde;                	// ��� ������
		char *Errmsg;						// ����� ������
		LONG DriverHandle;	            	// Handle �����
		void MyThreadCounter(void);			// �����, ����������� �������� Counters[]
};

#endif
