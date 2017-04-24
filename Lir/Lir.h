#pragma once
#include "bdaqctrl.h"
#include "AdvantechSignals.h"
class Lir;

struct CrossUnit//поперечные дефекты
{
	Lir &owner;
	void (CrossUnit::*ptr)();//указатель на текущий метод
	unsigned bitStart;//
	unsigned bitStop;//
	unsigned bitStartUp;//
	unsigned bitStopUp;//
	unsigned bitStartDown;//
	unsigned bitStopDown;//
	unsigned startOffset;//смещение
	unsigned lirOffsetUp;
	unsigned lirOffsetDown;
	int zonesTimeOffset;
	unsigned zonesTime[100];
	unsigned zonesOffs[100];
	bool onceUp;

	CrossUnit(Lir &o);
	void Start();//начало цикла
	void StartUp();
	void StopUp();
	void StartDown();
	void StopDown();
	void Noop();//труба вышла из модуля
};

struct ThicknessUnit//толщиномер
{
	Lir &owner;
	void (ThicknessUnit::*ptr)();//указатель на текущий метод
	unsigned bitStart;//
	unsigned bitStop;//
	unsigned bitStartUp;//
	unsigned bitStopUp;//
	unsigned bitStartDown;//
	unsigned bitStopDown;//
	unsigned startOffset;//смещение
	int zonesTimeOffset;
	unsigned zonesTime[100];
	unsigned zonesOffs[100];
	bool onceUp;

	ThicknessUnit(Lir &o);
	void Start();//начало цикла
	void StartUp();
	void StopUp();
	void StartDown();
	void StopDown();
	void Noop();//труба вышла из модуля
};
struct LongUnit//продольные дефекты
{
	Lir &owner;
	void (LongUnit::*ptr)();//указатель на текущий метод
	unsigned bitStart;//
	unsigned bitStop;//
	unsigned bitStartUp;//
	unsigned bitStopUp;//
	unsigned bitStartDown;//
	unsigned bitStopDown;//
	unsigned startOffset;//смещение
	int zonesTimeOffset;
	unsigned zonesTime[100];
	unsigned zonesOffs[100];
	bool onceUp;

	LongUnit(Lir &o);
	void Start();//начало цикла
	void StartUp();
	void StopUp();
	void StartDown();
	void StopDown();
	void Noop();//труба вышла из модуля
};

struct LirChange
{
    Lir &owner;
	unsigned delay;
	unsigned bit;
	unsigned lirOffsetUp;
	unsigned lirOffsetDown;
	void (LirChange::*ptr)();//указатель на текущий метод
	LirChange(Lir &o);
	void Start();//начало цикла
	void StartUp();
	void StartDown();
	void Delay();
	void Noop();//труба вышла из модуля
};

class CntrEventHandler:public Automation::BDaq::UdCntrEventListener
{
	HANDLE &h;
	volatile unsigned &tabIndex;//смещение лира
	volatile unsigned &tabIndex2;//смещение лира
public:
	unsigned lastCounter;
public:
   CntrEventHandler(Lir &o);
   void BDAQCALL UdCntrEvent(void * sender, Automation::BDaq::tagUdCntrEventArgs *args);
};

class Lir
{
	friend struct CrossUnit;//поперечные дефекты
	friend struct ThicknessUnit;//толщиномер
	friend struct LongUnit;//продольные дефекты
	friend struct LirChange;

	friend class CntrEventHandler;//обработчик прерывания
	friend class CounterCtrl;

	static const int numberPulsesPerInterrupt = 1;//импульсов на прерывание
	static const int zoneLength = 200;//длина зоны в мм
	static const int distantionSQ9_SQ26 = 3440;//  расстояние между SQ9-SQ26
public:
	CrossUnit crossUnit;
	ThicknessUnit thicknessUnit;
	LongUnit longUnit;

	LirChange lirChange;

	Automation::BDaq::UdCounterCtrl &udCounterCtrl;//объект платы Pci1784
public:
	CntrEventHandler cntrEventHandler;//обработчик платы 1784

	HANDLE h;
	HANDLE hThread;
	volatile unsigned tabIndex;//смещение лира
	volatile unsigned tabIndex2;//смещение 2 лира
	int samplesPerZoneParam[2];//отчётов лира0 на зону
	int samplesPerZone;//отчётов лира на зону
	Advantech1730 *dev;
	unsigned inputBits;
public:
	bool isThick, isLong;
private:
	void Run();
	static void __Run__(Lir *);//функция потока
	void StartLir(int);//старт лира
	Lir(void);
public:
	~Lir(void);
	void Init(Advantech1730 *);
	void Destroy();
	void Start(bool isThick, bool isLong);
	void Prepare();   //
	static Lir &Instance();//точка доступа к объекту
};

