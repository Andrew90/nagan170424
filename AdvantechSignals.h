//---------------------------------------------------------------------------
#ifndef AdvantechSignalsH
#define AdvantechSignalsH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TeEngine.hpp"
#include "TeeProcs.hpp"
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <map.h>

//#include "c:/Advantech/BioDAQ/Inc/BDaqCL.h"
#include "bdaqctrl.h"
using namespace Automation::BDaq;



class Advantech1730   {
private:
 //	BDaqDevice *device;				// handle to the device
	ErrorCode errorCode;            // error code (0=Success)
 //	BDaqDio *dio;                   // handle to input output
	TMutex *AdvMutex;

	InstantDiCtrl* instantDiCtrl;
	InstantDoCtrl* instantDoCtrl;

	long  startPort;      			// начальный порт, по умолчанию =0  (внутренние дела платы)
	long  portCount;				// число портов, по умолчанию =4 (внутренние дела платы)
	BYTE  bufferForWriting[4];		// буфер, содержащий все выходные сигналы по 4 портам
	BYTE  bufferForReading[4];		// буфер, содержащий все входные сигналы по 4 портам
	short SleepTime;		      	// время, после которого проверяем, выставился ли выходной сигнал
	short blinkTime;				// время задержки выходного сигнала в команде BlinkOut
	short bouncePeriod;				// время дребезга - интервал, через который будет проверяться состояние сигнала
	short bounceRepeat;             // кол-во повторений дребезга

public:
	Advantech1730();                // конструктор, задаем всё
	~Advantech1730();               // деструктор, разрушаем все

	unsigned int	DoValue;        // выходное слово 32 бит
	unsigned int	DiValue;        // входное слово 32 бит

	bool Init(long DevNum, bool ReadOnly);		// инициализация платы, returns true is succeed
	bool Close();                   // закрытие платы
	bool SetOutWord(unsigned int OutWord);// выставляет выходное слово, возвращает true в случае успеха
	bool SetOutBit(byte bit);		// выставляет выходной бит, возвращает true в случае успеха
	bool ResetOutBit(byte bit);		// сбрасывает выходной бит, возвращает true в случае успеха
	bool BlinkOutBit(byte bit , int userTime=0);		// кратковременно выставляет и сбрасывает выходной бит, возвращает true в случае успеха
	bool CheckOutWord();			// считывает ВЫХОДНОЕ слово и записывает его в DoValue, возвращает true в случае успеха
	bool CheckOutBit(byte bit);		// проверяет наличие ВЫХОДНОГО бита и возвращает НАЛИЧИЕ бита (установлен выход или нет)
	bool CheckInWord();		        // считывает входное слово и записывает его в DiValue, возвращает true в случае успеха
	bool CheckInBit(byte bit);		// проверяет наличие входного бита, возвращает НАЛИЧИЕ бита (есть/нет напряжение на входе)
	bool CheckInBouncelessBit(byte bit, bool check_status);	// проверяет входной бит с учетом дребезга, возвращает НАЛИЧИЕ бита
	bool WaitForInBit(byte bit, unsigned int DelayTimeMSec);			// ждет входного бита определенное время, возвращает НАЛИЧИЕ бита
	bool WaitForDropInBit(byte bit, unsigned int DelayTimeMSec);		// ждет пропадания бита определенное время, возвращает TRUE если сигнал пропал

	bool WaitForInBit(byte bit, unsigned int DelayTimeMSec, TThread *thread);			// ждет вх бита опред время и проверяет состояние треда, возвращает НАЛИЧИЕ бита
	bool WaitForDropInBit(byte bit, unsigned int DelayTimeMSec, TThread *thread);		// ждет вх бита опред время и проверяет состояние треда, возвращает НАЛИЧИЕ бита

	unsigned Read();
};

// структура Сигнал - содержит номер платы и номер канала, на который заведен сигнал
struct Signal
{
	byte card;
	byte channel;
//	Signal operator = (int code);
//	Signal operator = (Signal s);
//	Signal& operator = (Signal& );
};


// класс работы со всеми цифровыми сигналами, независимо от платы, на которую посажен сигнал
class AllSignals {
private:
  //	static bool __stdcall EnumDeviceCallBack(BDaqEnumerator::EnumDeviceArgs *args, void *ctx);
  //	static BDaqEnumerator::EnumDeviceArgs devarg[7];
 //	BDaqEnumerator en;							// енумератор для подсчета существующих плат
	void LoadSignalNames();						// грузим имена цифровых сигналов из ini файла

public:
	Advantech1730 *AdvCards[2];					// 2 указателя на платы Advantech1730
	AllSignals();								// конструктор, резервируем память под указатели на платы
	~AllSignals();								// деструктор, освобождаем память

	static byte cards_exist;					// кол-во плат, существующих в системе (берется из EnumDeviceCallBack)
	byte cards_inited;							// кол-во плат, проинициализированных (никем не занятых)
	byte cards_needed;							// кол-во плат, которые нужны для работы (берется из файла Settings*.ini)

	map <String,Signal> *inmap;					// карта выходов, где ключ - имя сигнала, данные - номер платы и канала
	map <String,Signal> *outmap;					// карта выходов, где ключ - имя сигнала, данные - номер платы и канала

	void FindCards();							// ищет все доступные платы Advantceh на компе
	short InitializeCards(bool ReadOnly);		// инициализация всех плат , возвращает число инициализированных плат
	bool CloseCards();                  		// закрытие всех плат Advantech
	Signal FindSignal(String SignalName, map<String,Signal> *mapname );	// находит указанный сигнал на платах
	bool SignalExists(String SignalName);		// возвращает true , если сигнал есть в карте сигналов (файле IO_Management.ini)
	bool SetOutSignal(String SignalName);		// выставляет сигнал , returns true is succeed
	bool ResetOutSignal(String SignalName);		// сбрасывает сигнал , returns true is succeed
	bool BlinkOutSignal(String SignalName , int userTime = 0);		// выставляет, а затем после задержки сбрасывает сигнал , returns true is succeed
	bool CheckOutSignal(String SignalName);		// проверяет, есть ли определенный ВЫХОДНОЙ сигнал, возвращает статус сигнала
	bool CheckInSignal(String SignalName);		// проверяет, есть ли определенный ВХОДНОЙ сигнал, returns STATUS of signal!!!!
	bool CheckInBouncelessSignal(String SignalName, bool check_status);		// проверить наличие входного сигнала c учетом дребезга
	bool WaitForInSignal(String SignalName, int DelayTimeMSec);				// ждет входного сигнала определнное время
	bool WaitForDropInSignal(String SignalName, int DelayTimeMSec);			// ждет пропадания входного сигнала определенное время, возвращает TRUE если сигнал пропал

	bool WaitForInSignal(String SignalName, int DelayTimeMSec, TThread *thread);			// ждет вх сигн опред время и проверяет состояние треда, возвращает НАЛИЧИЕ бита
	bool WaitForDropInSignal(String SignalName, int DelayTimeMSec, TThread *thread);		// ждет вх сигн опред время и проверяет состояние треда, возвращает НАЛИЧИЕ бита
	unsigned int GetInWord(int adv_card);        // возвращает входное слово 32 бит нужной платы
	unsigned int GetOutWord(int adv_card);       // возвращает выходное слово 32 бит нужной платы

};


	byte AllSignals::cards_exist = 0;
	//BDaqEnumerator::EnumDeviceArgs AllSignals::devarg[7];
//---------------------------------------------------------------------------
// глобальный экземпляр, управляющий цифровыми сигналами из любой точки программы.
extern AllSignals *digital;
//---------------------------------------------------------------------------
#endif
