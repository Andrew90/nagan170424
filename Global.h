//---------------------------------------------------------------------------

#ifndef GlobalH
#define GlobalH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Chart.hpp"
#include "Series.hpp"
#include "TeEngine.hpp"
#include "TeeProcs.hpp"
#include <ExtCtrls.hpp>
#include <map>
#include <vector>
#include "Lir.h"

#include "clustering.h"
//---------------------------------------------------------------------------
enum SGSource { sgsLCard = 0, sgsCOMPort = 1, sgsNone = -1 };

//-------------------------------------------------------------------------------------------
// ����� ��������� ��������������� �������
class Speeds
{
public:
	// �������� ����������
	vector <int> workSpeed;					// �������� ���������� � ������ � ������
	vector <int> transitSpeed;				// �������� ���������� � ��������
	vector <int> returnSpeed; 				// �������� ���������� � ��������
	// �������� ����������� ������
	vector <int> inSpeed;					// �������� ����������� �� ����� � ������
	vector <int> linearSpeed;				// �������� ����������� �� ����� ������
	vector <int> outSpeed;					// �������� ����������� ��� ������ �����

	map <String, vector<int> > speeds;		// ����� ��������� �������� ����� - �������� ����������

	Speeds();								// ����������� ������ - ������� �����
	TStrings* getSpeedNames();				// ��������� ��� ����� �� �����
	void setSpeeds(String TypesizeName);	// ������������� �������� (������ �� ini � �������, ����� ����������)
};

//-------------------------------------------------------------------------------------------
// ����� ���������� ����������
class Globals
{
public:

	static String current_typesize; 	// ������� ���������� (����� ���� ����� �����, ��������, ������� ������)
	static int current_diameter;		// ������� ������� (������ 60,73 ��� 89)
	static bool isView;					// ���������� �� ��������
	static String IniFileName;          // ��� � ���� � ����� � �����������
	static String SignalsFileName;		// ��� � ���� � ����� � ��������� ���������
	static String PathFile_threshold_sg;// ���� � ����� � �������� ��� �� � ms-dos
	static long tube_number;			// ����� �����
	static Speeds speeds;				// �������� �����������
	static SolidGroups::Tube tubeSG;	// ����� � ���������������� ������ ���������
	static TStringList *delay;			// ��������� � �������� � ���������
	static bool ProtocolCreated;		// ����, ��� �������� ������

	static const int max_sensors = 16;
	static const int max_zones = 60;
	static const int max_meas = 10000;
	static const int level100 = 8192; 	// 8192 ���� �������� ���� ��� � ����� LCARD
	static const int Counters = 3;		// ����� ��������� � �������
	static const int LinSensors = 4;	// ����� �������� ����������� �������� � �������

private:

};


	String Globals::current_typesize = L"2";
	int Globals::current_diameter = 2;
	bool Globals::isView = false;
	String Globals::PathFile_threshold_sg ="..\\..\\Settings\\";
	String Globals::IniFileName = "..\\..\\Settings\\SettingsDefectoscope.ini";
	String Globals::SignalsFileName = "..\\..\\Settings\\IO_Management.ini";
	long Globals::tube_number = 5557;
	TStringList* Globals::delay = new TStringList();
	bool Globals::ProtocolCreated = false;
	Speeds Globals::speeds = Speeds::Speeds();
	SolidGroups::Tube Globals::tubeSG = SolidGroups::Tube();

//-------------------------------------------------------------------------------------------
// ����� ��������, ����������� � ������������ �������
class SystemConst
{
public:
	static bool isMedianFilter;		// ������ � ��������� ��������
	static bool ThicknessMass;		// ���� �� ���� � �����������
	static bool ThickModuleManage;	// ��������� �� ������� ���������� ������ �������������
	static bool ThickCentralizer;	// ���� �� ��������� � ����������� (����������� �� ������������)
	static bool LinearMass;			// ���� �� ���� � �����������
	static bool LinearCentralizer;	// ���� �� ��������� � ����������� (����������� �� ������������)
	static bool CrossCentralizer;	// ���� �� ��������� � ����������� (����������� �� ������������)
	static bool CentralizersCard;	// ���� �� ����� DGV �� ���������� �������� (����� ����, ������� � �.�.)
	static bool TransportManage;	// ��������� �� �� �����������
	static bool ComWithASU;			// ����� � ��� ���� �� COM �����
	static SGSource SolidGroupSource;// �������� ������ ���������
	static bool HideInGain;			 //	������� �� �������� ������� � ���������� �������
	static bool IsOnline;			 // � ����� ��� ���, ������
	static bool ExternalTransport;	 // ���������� �� ������� ��������� (�����������)
	static bool isSolidGroupMS_DOS;	 // ������� ������ ��������� � ������� ����� ms-dos,������
	static bool isTempSensor;		 // ������������ �� ������ ����������� ��� ����������� ��
	static void SetSystemConstants();	// ��������� ���������� �������������� �������

};
	bool SystemConst::isTempSensor = false;
	bool SystemConst::isMedianFilter = false;
	bool SystemConst::ThicknessMass = true;
	bool SystemConst::ThickModuleManage = false;
	bool SystemConst::ThickCentralizer = true;
	bool SystemConst::LinearMass = true;
	bool SystemConst::LinearCentralizer = true;
	bool SystemConst::CrossCentralizer = true;
	bool SystemConst::CentralizersCard = true;
	bool SystemConst::TransportManage = false;
	bool SystemConst::ComWithASU = false;
	SGSource SystemConst::SolidGroupSource = sgsNone;
	bool SystemConst::HideInGain = false;
	bool SystemConst::IsOnline = false;
	bool SystemConst::ExternalTransport = false;
	bool SystemConst::isSolidGroupMS_DOS= false;

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
