#pragma once
#include "bdaqctrl.h"
#include "AdvantechSignals.h"
class Lir;

struct CrossUnit//���������� �������
{
	Lir &owner;
	void (CrossUnit::*ptr)();//��������� �� ������� �����
	unsigned bitStart;//
	unsigned bitStop;//
	unsigned bitStartUp;//
	unsigned bitStopUp;//
	unsigned bitStartDown;//
	unsigned bitStopDown;//
	unsigned startOffset;//��������
	unsigned lirOffsetUp;
	unsigned lirOffsetDown;
	int zonesTimeOffset;
	unsigned zonesTime[100];
	unsigned zonesOffs[100];
	bool onceUp;

	CrossUnit(Lir &o);
	void Start();//������ �����
	void StartUp();
	void StopUp();
	void StartDown();
	void StopDown();
	void Noop();//����� ����� �� ������
};

struct ThicknessUnit//����������
{
	Lir &owner;
	void (ThicknessUnit::*ptr)();//��������� �� ������� �����
	unsigned bitStart;//
	unsigned bitStop;//
	unsigned bitStartUp;//
	unsigned bitStopUp;//
	unsigned bitStartDown;//
	unsigned bitStopDown;//
	unsigned startOffset;//��������
	int zonesTimeOffset;
	unsigned zonesTime[100];
	unsigned zonesOffs[100];
	bool onceUp;

	ThicknessUnit(Lir &o);
	void Start();//������ �����
	void StartUp();
	void StopUp();
	void StartDown();
	void StopDown();
	void Noop();//����� ����� �� ������
};
struct LongUnit//���������� �������
{
	Lir &owner;
	void (LongUnit::*ptr)();//��������� �� ������� �����
	unsigned bitStart;//
	unsigned bitStop;//
	unsigned bitStartUp;//
	unsigned bitStopUp;//
	unsigned bitStartDown;//
	unsigned bitStopDown;//
	unsigned startOffset;//��������
	int zonesTimeOffset;
	unsigned zonesTime[100];
	unsigned zonesOffs[100];
	bool onceUp;

	LongUnit(Lir &o);
	void Start();//������ �����
	void StartUp();
	void StopUp();
	void StartDown();
	void StopDown();
	void Noop();//����� ����� �� ������
};

struct LirChange
{
    Lir &owner;
	unsigned delay;
	unsigned bit;
	unsigned lirOffsetUp;
	unsigned lirOffsetDown;
	void (LirChange::*ptr)();//��������� �� ������� �����
	LirChange(Lir &o);
	void Start();//������ �����
	void StartUp();
	void StartDown();
	void Delay();
	void Noop();//����� ����� �� ������
};

class CntrEventHandler:public Automation::BDaq::UdCntrEventListener
{
	HANDLE &h;
	volatile unsigned &tabIndex;//�������� ����
	volatile unsigned &tabIndex2;//�������� ����
public:
	unsigned lastCounter;
public:
   CntrEventHandler(Lir &o);
   void BDAQCALL UdCntrEvent(void * sender, Automation::BDaq::tagUdCntrEventArgs *args);
};

class Lir
{
	friend struct CrossUnit;//���������� �������
	friend struct ThicknessUnit;//����������
	friend struct LongUnit;//���������� �������
	friend struct LirChange;

	friend class CntrEventHandler;//���������� ����������
	friend class CounterCtrl;

	static const int numberPulsesPerInterrupt = 1;//��������� �� ����������
	static const int zoneLength = 200;//����� ���� � ��
	static const int distantionSQ9_SQ26 = 3440;//  ���������� ����� SQ9-SQ26
public:
	CrossUnit crossUnit;
	ThicknessUnit thicknessUnit;
	LongUnit longUnit;

	LirChange lirChange;

	Automation::BDaq::UdCounterCtrl &udCounterCtrl;//������ ����� Pci1784
public:
	CntrEventHandler cntrEventHandler;//���������� ����� 1784

	HANDLE h;
	HANDLE hThread;
	volatile unsigned tabIndex;//�������� ����
	volatile unsigned tabIndex2;//�������� 2 ����
	int samplesPerZoneParam[2];//������� ����0 �� ����
	int samplesPerZone;//������� ���� �� ����
	Advantech1730 *dev;
	unsigned inputBits;
public:
	bool isThick, isLong;
private:
	void Run();
	static void __Run__(Lir *);//������� ������
	void StartLir(int);//����� ����
	Lir(void);
public:
	~Lir(void);
	void Init(Advantech1730 *);
	void Destroy();
	void Start(bool isThick, bool isLong);
	void Prepare();   //
	static Lir &Instance();//����� ������� � �������
};

