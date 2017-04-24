//---------------------------------------------------------------------------

#ifndef ExtTransportH
#define ExtTransportH
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include "AdvantechSignals.h"
#include "Global.h"
//#include "Main.h"
//---------------------------------------------------------------------------
struct signal_result
{
	int Previous;
	int Current;
};
//��� ������� ���������� ������ ���� SystemConst::ExternalTransport = true
class ExtTransport
{

	public:
		ExtTransport();
		//������� ������ ����� ������, ��� ������ "ok" � ������ ������

		//��������� ����� �� ���
		static String Load();
		// ��������� ����� � ����� �� ����� ������
		static String UnloadTubeOutOfLine();
		//������� ��������� �������������, ���� ������ �� �������� (����� ������������: �� ��� - 1, ����� ��� - 2)
		static String LoaderTest(int number);
		//���������� ��� �������� ������� ���������� � �������������
		static void ResetAllExtTransportSignal();
		//������������� �������� �������� ����������
		static void StopMovingExtTransport();
		static int resultPrevious;
		static int resultCurrent;
	private:
		// ������ �������������. ����� ������������:�� ��� - 1, ����� ��� - 2
		static String WorkLoader(int number);
		static String NET;
		static String ErrLoader;
		static String ok;

};
	int ExtTransport::resultPrevious=-1;
	int ExtTransport::resultCurrent=-1;
	String ExtTransport::NET = "NO_EXTERNAL_TRANSPORT";
	String ExtTransport::ErrLoader = "������ ������ ������������";
	String ExtTransport::ok = "ok";

#endif
