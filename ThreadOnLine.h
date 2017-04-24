//---------------------------------------------------------------------------

#ifndef ThreadOnLineH
#define ThreadOnLineH

#include <vector>
#include "IniFiles.hpp"

#include "Global.h"
#include "Advantech1784.h"
#include "AdvantechSignals.h"
#include "LCard791.h"
#include "Main.h"
#include "Result.h"
#include "ComPort.h"
#include "Protocol.h"

//---------------------------------------------------------------------------
class Advantech1784;
class ThreadOnLine: public TThread
{
	protected:
		void __fastcall Execute();
		// ���������� �� ������� �����
		Advantech1784 *Counter;				// ��������� �� ����� 1784
		LCard791 *lpop;						//���������� �����
		LCard791 *lprod;					//���������� �����
		Result *respop;						// ��������� �� ��������� ����������
		Result *resprod;					// ��������� �� ��������� ����������
		ThicknessResult *resth;				// ��������� �� ��������� ����������
		SummaryResult *ressum;				// ��������� �� ��������� ���������
		TStatusBar *StB;					// ��������� �� ���������, ������������ ������� ��������
		bool Thick;							// ���������� �� �������������
		bool Linear;						// ���������� �� ����������
		AlarmThreadClass *lat;				// ��������� ���� ������ (�����)
//		bool Prepare;						// ����� �� �������� � ������ ������

		void __fastcall DrawResultPP();		// ������ ����������� ���������
		void __fastcall DrawResultPR();		// ������ ���������� ���������
		void __fastcall RedrawGraphics();	// �������������� ��� ������� ������� �����
		void __fastcall OnlineZones();		// ������������� ���� ������ (��������)
		// ��� ���������, ��������� �� ���������� (����� ���������, ������ ��� ������� �� ����� �������)
		double perimeters[Globals::Counters];		// ����� ������� ����
		long imp_per_zone[Globals::Counters];		// ���-�� ��������� �� ���� ���� (��� ������� ����)
		long ppOffset[Globals::Counters];			// ������ �� ������ � ��������� (��� ������� ����)
		long prOffset[Globals::Counters];			// ������ �� ����� � ��������� (��� ������� ����)
		long thOffset[Globals::Counters];			// ������ �� ���� � ��������� (��� ������� ����)
		long markOffset[Globals::Counters];			// ������ �� ���� � ��������� (��� ������� ����)
		int current_counter;						// ����� ��������, ������������� � ������� ������
		long Counter0Fin;							// ��������� �������� �������� 0 ����� �������������
		long Counter1Fin;							// ��������� �������� �������� 1 ����� �������������
		long TotalCounter;							// ���������, ����������� �������� 3-�� ���������
		long PoperStart;							// �������� �������� ��� ��������� ����� ����
		long PoperFinish;							// �������� �������� ��� ���������� ����� ����
		long ProdStart;								// �������� �������� ��� ��������� ���� ����
		long ProdFinish;							// �������� �������� ��� ���������� ���� ����
		long TolshStart;							// �������� �������� ��� ��������� ���� ����
		long TolshFinish;							// �������� �������� ��� ���������� ���� ����
		long MarkStart;								// �������� �������� ��� ��������� �� ����� (��������� � ���������������� ������)
		long MarkFinish;							// �������� �������� ��� ���������� �� �����
		// ��� ���������, ��������� �� ������ (�-����)
		DynamicArray < DynamicArray <double> > ppData;	// ������ �������� �������� �� ��������
		DynamicArray < DynamicArray <double> > prData;	// ������ �������� �������� �� ��������
		int meas_per_zone;                       //���-�� ��������� � ����� ����
		ULONG ppFirst;							 //������ ���� ���������� �����
		ULONG ppSecond;							 //����� ���� ���������� �����
		ULONG prFirst;                           //������ ���� ���������� �����
		ULONG prSecond;                          //����� ���� ���������� �����
		long meas_per_sensor;                    //���-�� ��������� � ���� �� ������ �������
		ULONG *Buffer;					  		 //����� ��� ��������� ������
		long ppNumber;                           //���-�� ��������� ������ ������ ��� ��
		long prNumber;                           //���-�� ��������� ������ ������ ��� ���

		String stext1;						// ����� ��� ������� � ���������� � ������ 1
		String stext2;						// ����� ��� ������� � ���������� � ������ 2
		int plantCut;						// ���� ���� �� �������������� (��������)

		UnicodeString PrepareForWork( );	// ���������� �������� ����� �������
		void Finally();                     // ��������� ����� (���������� �������� � �.�.)

	public:                           //�����������
		__fastcall ThreadOnLine(bool CreateSuspended,
								Advantech1784 *_Counter,	// ����� Advantech1784
								LCard791 &_l1,				//���������� �����
								LCard791 &_l2,				//���������� �����
								Result &_respop,			// ��������� �� ��������� ����������
								Result &_resprod,			// ��������� �� ��������� ����������
								ThicknessResult &_resth,	// ��������� �� ��������� �������
								SummaryResult &_ressum,		// ��������� �� ��������� ���������
								TStatusBar *St,				// ��������� �� ���������, ������������ ������� ��������
								bool _Thick,				// ���������� �� �������������
								bool _Linear,				// ���������� �� ����������)
								AlarmThreadClass &alth);	// ����� ����� ������

		bool Collect;							//������� ��������� ������
		bool OnlineCycle(void);					//����� ��������� ���� ���� ����������
		void __fastcall UpdateStatus();			// ��������� ���������� ������� �����, ������������ �������
		bool cool;								// ������ Return Value, ������� ��� ���������
		bool transit;							// ������� ����� �������

		// ������ ����� ��� ��������, � ������ ������ ����� �������
		int ppOnline;			// ���� �� ���������� � ������� ������
		int ppOld;				// ���� �� ���������� ����������
		int prOnline;           // ���� �� ���������� � ������� ������
		int prOld;              // ���� �� ���������� ����������
		int thOnline;           // ���� �� ������� � ������� ������
		int thOld;              // ���� �� ������� ����������
		int markOnline;         // ���� �� ���������������� � ������� ������
		int markOld;            // ���� �� ���������������� ����������
		void CalcOnlineZones();	// ������� ������� ���� �� ������ ������
		void CalcOffsets();		// ������� ������� ������� �� ���� (1 ��� � ������)
		// ���������� ����� (������� ����� ������ ������ �� ����� ���������)
		long St2in;		// ����� ������ �� 2 ������ ( 0 ������� )
		long St4in;		// ����� ������ �� 4 ������ ( 0 ������� )
		long St1out;	// ����� ������ �  1 ������ ( 1 ������� )
		long St3out;	// ����� ������ �  3 ������ ( 1 ������� )
		long St4out;	// ����� ������ �  4 ������ ( 2 ������� )
		long GPout;		// ����� ������ �  �� ������ ( 2 ������� )
		void Calibrate();
};
//---------------------------------------------------------------------------

#endif
