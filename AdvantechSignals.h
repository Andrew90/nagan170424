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

	long  startPort;      			// ��������� ����, �� ��������� =0  (���������� ���� �����)
	long  portCount;				// ����� ������, �� ��������� =4 (���������� ���� �����)
	BYTE  bufferForWriting[4];		// �����, ���������� ��� �������� ������� �� 4 ������
	BYTE  bufferForReading[4];		// �����, ���������� ��� ������� ������� �� 4 ������
	short SleepTime;		      	// �����, ����� �������� ���������, ���������� �� �������� ������
	short blinkTime;				// ����� �������� ��������� ������� � ������� BlinkOut
	short bouncePeriod;				// ����� �������� - ��������, ����� ������� ����� ����������� ��������� �������
	short bounceRepeat;             // ���-�� ���������� ��������

public:
	Advantech1730();                // �����������, ������ ��
	~Advantech1730();               // ����������, ��������� ���

	unsigned int	DoValue;        // �������� ����� 32 ���
	unsigned int	DiValue;        // ������� ����� 32 ���

	bool Init(long DevNum, bool ReadOnly);		// ������������� �����, returns true is succeed
	bool Close();                   // �������� �����
	bool SetOutWord(unsigned int OutWord);// ���������� �������� �����, ���������� true � ������ ������
	bool SetOutBit(byte bit);		// ���������� �������� ���, ���������� true � ������ ������
	bool ResetOutBit(byte bit);		// ���������� �������� ���, ���������� true � ������ ������
	bool BlinkOutBit(byte bit , int userTime=0);		// �������������� ���������� � ���������� �������� ���, ���������� true � ������ ������
	bool CheckOutWord();			// ��������� �������� ����� � ���������� ��� � DoValue, ���������� true � ������ ������
	bool CheckOutBit(byte bit);		// ��������� ������� ��������� ���� � ���������� ������� ���� (���������� ����� ��� ���)
	bool CheckInWord();		        // ��������� ������� ����� � ���������� ��� � DiValue, ���������� true � ������ ������
	bool CheckInBit(byte bit);		// ��������� ������� �������� ����, ���������� ������� ���� (����/��� ���������� �� �����)
	bool CheckInBouncelessBit(byte bit, bool check_status);	// ��������� ������� ��� � ������ ��������, ���������� ������� ����
	bool WaitForInBit(byte bit, unsigned int DelayTimeMSec);			// ���� �������� ���� ������������ �����, ���������� ������� ����
	bool WaitForDropInBit(byte bit, unsigned int DelayTimeMSec);		// ���� ���������� ���� ������������ �����, ���������� TRUE ���� ������ ������

	bool WaitForInBit(byte bit, unsigned int DelayTimeMSec, TThread *thread);			// ���� �� ���� ����� ����� � ��������� ��������� �����, ���������� ������� ����
	bool WaitForDropInBit(byte bit, unsigned int DelayTimeMSec, TThread *thread);		// ���� �� ���� ����� ����� � ��������� ��������� �����, ���������� ������� ����

	unsigned Read();
};

// ��������� ������ - �������� ����� ����� � ����� ������, �� ������� ������� ������
struct Signal
{
	byte card;
	byte channel;
//	Signal operator = (int code);
//	Signal operator = (Signal s);
//	Signal& operator = (Signal& );
};


// ����� ������ �� ����� ��������� ���������, ���������� �� �����, �� ������� ������� ������
class AllSignals {
private:
  //	static bool __stdcall EnumDeviceCallBack(BDaqEnumerator::EnumDeviceArgs *args, void *ctx);
  //	static BDaqEnumerator::EnumDeviceArgs devarg[7];
 //	BDaqEnumerator en;							// ���������� ��� �������� ������������ ����
	void LoadSignalNames();						// ������ ����� �������� �������� �� ini �����

public:
	Advantech1730 *AdvCards[2];					// 2 ��������� �� ����� Advantech1730
	AllSignals();								// �����������, ����������� ������ ��� ��������� �� �����
	~AllSignals();								// ����������, ����������� ������

	static byte cards_exist;					// ���-�� ����, ������������ � ������� (������� �� EnumDeviceCallBack)
	byte cards_inited;							// ���-�� ����, ��������������������� (����� �� �������)
	byte cards_needed;							// ���-�� ����, ������� ����� ��� ������ (������� �� ����� Settings*.ini)

	map <String,Signal> *inmap;					// ����� �������, ��� ���� - ��� �������, ������ - ����� ����� � ������
	map <String,Signal> *outmap;					// ����� �������, ��� ���� - ��� �������, ������ - ����� ����� � ������

	void FindCards();							// ���� ��� ��������� ����� Advantceh �� �����
	short InitializeCards(bool ReadOnly);		// ������������� ���� ���� , ���������� ����� ������������������ ����
	bool CloseCards();                  		// �������� ���� ���� Advantech
	Signal FindSignal(String SignalName, map<String,Signal> *mapname );	// ������� ��������� ������ �� ������
	bool SignalExists(String SignalName);		// ���������� true , ���� ������ ���� � ����� �������� (����� IO_Management.ini)
	bool SetOutSignal(String SignalName);		// ���������� ������ , returns true is succeed
	bool ResetOutSignal(String SignalName);		// ���������� ������ , returns true is succeed
	bool BlinkOutSignal(String SignalName , int userTime = 0);		// ����������, � ����� ����� �������� ���������� ������ , returns true is succeed
	bool CheckOutSignal(String SignalName);		// ���������, ���� �� ������������ �������� ������, ���������� ������ �������
	bool CheckInSignal(String SignalName);		// ���������, ���� �� ������������ ������� ������, returns STATUS of signal!!!!
	bool CheckInBouncelessSignal(String SignalName, bool check_status);		// ��������� ������� �������� ������� c ������ ��������
	bool WaitForInSignal(String SignalName, int DelayTimeMSec);				// ���� �������� ������� ����������� �����
	bool WaitForDropInSignal(String SignalName, int DelayTimeMSec);			// ���� ���������� �������� ������� ������������ �����, ���������� TRUE ���� ������ ������

	bool WaitForInSignal(String SignalName, int DelayTimeMSec, TThread *thread);			// ���� �� ���� ����� ����� � ��������� ��������� �����, ���������� ������� ����
	bool WaitForDropInSignal(String SignalName, int DelayTimeMSec, TThread *thread);		// ���� �� ���� ����� ����� � ��������� ��������� �����, ���������� ������� ����
	unsigned int GetInWord(int adv_card);        // ���������� ������� ����� 32 ��� ������ �����
	unsigned int GetOutWord(int adv_card);       // ���������� �������� ����� 32 ��� ������ �����

};


	byte AllSignals::cards_exist = 0;
	//BDaqEnumerator::EnumDeviceArgs AllSignals::devarg[7];
//---------------------------------------------------------------------------
// ���������� ���������, ����������� ��������� ��������� �� ����� ����� ���������.
extern AllSignals *digital;
//---------------------------------------------------------------------------
#endif
