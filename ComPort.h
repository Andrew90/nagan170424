//---------------------------------------------------------------------------

#ifndef ComPortH
#define ComPortH

#include "Global.h"
#include "clustering.h"
#include "Result.h"
#include "Main.h"

#include "slycomm\CommPort.h"
#pragma link "CommPort"

//---------------------------------------------------------------------------
class MyComPort
{
private:
// ��������� ��������
	String PortName;			// ��� ����� (COM1, COM2 ...)
	int BaudRate;				// ��� ���� (��������)
	int DataBits;				// ���� ������
	int StopBits;				// �������� ����
	int Parity;					// ��������
// ���������� ����������
	int ASUMode;				// ����� �������� (��� ������ ��������)
	int length;					// ����� ������� �� ��� (5 ��� 17)
	int abonent;				// ����� �������� � ���� (������ const)
	int function;				// ������� ������	(1, 2 ��� 5)
	unsigned char data[50];		// �������� ������ � ����
	int data_size;				// ����� �������� ������
	TEvent* DataReceived;		// ������� - ������ ��������
	TCommPort *CommPort;		// ������� ��� ����� :)
// ���������� �������
	void SetComPortSettings();	// ���������� ���������
	long GetTubeNumber_1();		// ������ �� ������� ��������� (���������)
	long GetTubeNumber_2();		// ������ �� ������� ���������	(����-��)
	bool CheckSentMessage();	// ��������� ���������� ������ (������ 2-�� �������� )
// ������ ����������� �� ����������, �����
	unsigned char EliseevRes(int zone);	// ��������� ���������� �������� �� ����������� �������
	int plant_cut;						// ���� ���� �� ��������������, �����
	Result pp2;
	Result pr2;
	ThicknessResult th2;
	SummaryResult sum2;
	TColor Good;			// ������� ��� ���������� �����������
	TColor Class2;
	TColor Brack;

public:
	MyComPort();		// ����������� ������ ������
	~MyComPort();		// ����������

	void __fastcall MyComPortDataReceived(TObject *Sender, DWORD Count);	// ������� - ������ �������
		// ������� ����������� �����
	unsigned char countCRC16_1(unsigned char* buff, unsigned int size);	// ����� ����� �� 1 ���������
	unsigned short countCRC16_2(unsigned char* bufData, unsigned int sizeData);	// ����� ����� �� 2 ���������
		// ���� ������ �����
	bool TestConnection();
		// ������ ������ ����� (���� �����)
	long GetTubeNumber();
		// ������� ���������� (���� �����)
	bool SendResultToASU(short res, short c1, short c2, short len, SolidGroups::Group sgg);		// 1 - ���������
//	bool SendResultToASU(Result &pp1, Result &pr1, ThicknessResult &th1, SummaryResult &sum1 , SolidGroups::Group sgg);	// 2 - ����-��
	bool SendResultToASU(Result &pp1, Result &pr1, ThicknessResult &th1, SummaryResult &sum1 , int solid_num);	// 2 - ����-��
		// ������� ������ ��������� (���� �����)
	SolidGroups::Tube GetSolidGroup();
		// ���� ���� �� ��������������
	inline int GetPlantCut() { return plant_cut;}


};

//---------------------------------------------------------------------------
// ���������� ���������, ����������� ��������� ��������� �� ����� ����� ���������.
extern MyComPort *MyCom;
//---------------------------------------------------------------------------

#endif
