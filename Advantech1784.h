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
		__fastcall Advantech1784(bool CreateSuspended); //Конструктор где запускается поток сбора данных(происходит открытие платы)
		Advantech1784();            		// дефолтный конструктор
		ULONG GetCounter(int num);  		// возвращает значения счетчика num
		bool CloseDevice();					// закрыть плату
		bool Reset(int NumOfCounter = -1);	// сбросить все счетчики на 0
		String Error;						// строка с ошибкой

private:
		int Channel[Globals::Counters];		// номера 3 каналов сбора
		TIniFile *ini;
		bool m_bContinue;         			// условие продолжение сбора данных
		ULONG Counter[Globals::Counters];	// массив счетчиков (3 штуки)
		LRESULT m_ErrCde;                	// код ошибки
		char *Errmsg;						// текст ошибки
		LONG DriverHandle;	            	// Handle платы
		void MyThreadCounter(void);			// поток, обновляющий значения Counters[]
};

#endif
