//---------------------------------------------------------------------------
#ifndef LCARD791H
#define LCARD791H
#include "intsafe.h"
//#include <windows.h>
#include "include\ioctl.h"
#include "include\ifc_ldev.h"
#include "include\create.h"
#include "include\791cmd.h"
#include "IniFiles.hpp"
#include "Math.h"
#include "clustering.h"



//---------------------------------------------------------------------------
class LCard791
{

public:
	int Size;                       		//������ ������ ���� ����� ���������� ������
	ULONG Index; 							//������ ��� �������� ������
	ULONG *Base;					  		//���� ����� ������ � �����
	bool collect;                    	    //����������/������  �� ���� ������
	ULONG Number;                           //���-�� ��������� ���������
	int Mode;                               //����� ������ ��� : ��+�������������/�������������/��
	ULONG Num;                              //���-�� ��������� ������ "part"
	int part;                               //������� ��������� � ����� �����

	int countTimeOffs;
	int frameBuffer;
	unsigned timeOffs[512];

	LCard791(int size);                     //Size-������ ������ ��� ����� ���
	~LCard791();                            //���������� �_�
	void FindCard(void);					//���� ����� � ���������� �� � ��� ����
	bool Initilalize(UnicodeString SerNum,
	bool isDiff);          		  			//������������� ���� � �������� .DLL
	bool Close();                		    //�������� �����
	bool Start();                		    //��������� ���� � ������
	bool Stop();                   		    //������������� ����
	bool SetDAC(double Volt, int channel);  //���������� ���������� �� ���
	void InitDAC(bool Write);               //������������� ���
	void InitADC(int Channel, double dKadr,
	UnicodeString Type,int Mode);			//������������� ��� (���-�� ������������ �������,
																			//�������,�������� ����� �������,��� ��� ����������/����������
	float GetMagnetic();                    //���������� �������� ������� ���������� ���� � �.
	int getTemperatureForSG();
	float GetTemperature(int Num);          //�������� ����� ������� ����������� 0 ��� 1, ���������� �������� ����������� � �.
	void ClearBuffer();                     //������� �������� ������� ��� ������������� ����� ������
	bool isOkTemp(UnicodeString type);      //���������� True ���� ��������� ��������,False ���� ��� ������ 1-t1,2-t2,MG-��������� ����
	void SetMeasInSG();                     //���������� ������� ���� ��������� ����� ����� ����� � ��
	void SetMeasOutSG();                    //���������� ������� ���� ��������� ����� ����� ����� �� ��
	long GetNum();							//���������� ���-�� ��������� ������ �� ������ ������
	vector <int> PrepareSGData(bool Test);	//���������� ������ �������� �� ��
	inline bool getStarted() {return FlStart;}
private:

	long MeasOutSG;                         //C������ ���� ��������� ����� ����� ����� �� ��
	long MeasInSG;                          //C������ ���� ��������� ����� ����� ����� � ��
	bool FlStart;                           //����� ���������� ����
	void* pI_void;                          //Handle �����
	bool isDiff;                            //����� ������������� �����, ����������������/� ����� ������
	int fl1,k;							    //C������� ��� ������ ������
	TIniFile *Ini;                          //���� ��������
	ULONG Tid;                              //��������� �� �����
	PLATA_DESCR_L791 pd;
	LUnknown* pIUnknown;
	ULONG *data1,*sync1;            	    //��� ���
	ULONG *sync,*data;              	    //��� ���
	HANDLE hThread;                  	    //��������� �� ����� ��� ����� ������
	ULONG SizeBufDAC;                       //������ ��� ���
	ULONG SizeBufADC;                   	//������ ��� ���

	ULONG LCard791::SetChn(int _gain,int _channel);      //������������� ������� �������� �� ���������� �����
	void IniWrite(UnicodeString Name,int n); 			 //����� ��� ����� � Ini ����
	void Run(void);                       				 //���� ������ � ���
	static DWORD WINAPI Thread(PVOID p);
	bool DacAsyncOutCode(int16_t u1Code, int16_t ch);    //��������������� ������� ��� SetDAC

};
#endif



