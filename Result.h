//---------------------------------------------------------------------------
#ifndef ResultH
#define ResultH

#include "Classes.hpp"
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include "Chart.hpp"

#include "LCard791.h"

//---------------------------------------------------------------------------


// ����� ���������� �������������
class Result
{
protected:
	TForm *SensorForm;					// ����� ��� ��������� ��������� �� �������
	TChart *SensorChart;				// ���� ��� ��������� ��������� �� �������
	TColor Dead;						// ���� ������� ����
	TColor Brack;						// ���� ������
	TColor Class2;						// ���� ������� ������
	TColor Good;						// ���� ������, ������� �����
	DynamicArray < DynamicArray <double> > sensor_data;		// ������ �������� �������� �� ��������
//todo	DynamicArray < DynamicArray < DynamicArray < double > > > data;		// ������ ���� �������� - �� ���� �������� , �� ���� �����
public:
vector < vector < vector < double > > > data;
protected:
public:
	double meas_dead_zone_finish;
	double meas_dead_zone_start;
	vector < double > gain;

private:
	void _medianfilter(double* signal, double* result, int N);      // ��������������� ������� ��� ����������
	int WidthMedianFilter;                                          // ������ ���� ��� ��������� ����������

public:
	void medianfilter(double* signal, double* result, int N);		//N - ����� �������
   	short zones;						// ���-�� ������������ ��� � ����������
	short sensors;                      // ���-�� ������������ �������� (��������)
	int meas_per_zone;                  // ���-�� ��������� � 1 ���� 1 �������
	int dead_zone_start;	            // ������� ���� � ������ �����
	int dead_zone_finish;	            // ������ ���� � ����� �����
	long total_meas;					// ����� ���-�� ��������� �� ������ �������
	DynamicArray <double> borders;      // ������ ������� - [0] - ����� �����, [1] - ����� ������ 2 � �.�.
	DynamicArray <double> zone_data;    // ������ �������� �������� �� �����
	DynamicArray <int> meas_per_sensor;	// ������ ���������� ��������� �� ������ � ���� (����)


	Result();										// �����������
	~Result();										// ����������
	virtual void CreateTestData(short tag);			// ������� �������� ������ (������� �����)
	virtual void ComputeZoneData();					// ������� ������, �������� ������� "zone_data" � "sensor_data"
	TColor ZoneColor(double meas,int XCoord = -1);	// ���������� ���� ���������/�������/����
	virtual void PutResultOnChart(TChart *C, TComponent *Owner);	// ������������ ���������� �� ����� �

	void PutDataOnChart(TChart *C,TComponent *Owner, int zone, int sensor);		// ������ ��������� � ����/������� �� �������

	#if 0
	void PrepareData(LCard791 *LC,				   // ��������� ��������� ������ L791 �������� �� ����� � ��������
					int _sensors,int DefSensors, int MeasOutCrossTrash = 0);
#else
void PrepareData(LCard791 *LC, vector<vector<vector<double> > >(&pop)
     , vector<vector<vector<double> > >(&prod)) ;

#endif
	void __fastcall SeriesDblClick(TChartSeries *Sender,  	// ������� ���� �� ������� ��������� ���� � �����������
			int ValueIndex, TMouseButton Button,  TShiftState Shift, int X, int Y);

	void __fastcall EscapeKeyPress(TObject *Sender, wchar_t &Key);	// ���������� ������� "����� Esc - ���� ������"
	virtual void SaveTubeToFile(UnicodeString FileName);					// ��������� ������ �� ����� � ����
	virtual void LoadTubeFromFile(FILE *file);  							// �������� ������ �� ����� �� �����
	void DeleteData();												// ������� ��� ���������
	void SetGains(vector<double> _Gain);							// ������������� ��� ��������� ��������
	void SetSensorGain(double _gain, int sensor);					// ������������� �������� 1 ����������� �������
	void CalculateDeadZone(int LengthZone);							// ����������� ������� ����
	void CalculateDeadFront(int LengthZone);						// ������� ������� ���� � ������
	void CalculateDeadBack(int LengthZone);							// ������� ������� ���� � �����
	void AddZone( DynamicArray < DynamicArray <double> > new_data);	// ��������� ������������ ���� � �������
    void Clear(TChart *C);
};
//--------------------------------------------------------------------------------

// ��������� ������ Result - ��������� �� �������
class ThicknessResult : public Result
{
public:
	ThicknessResult();					// �����������
	~ThicknessResult();					// ����������
	void CreateTestData(short tag);		// ������� �������� ���� (����� ��������� �����)
	void ComputeZoneData();				// ������� ������ �� ����� � ���� (����� ��������� �����)
	TColor ZoneColor(double meas);		// ���������� ���� ����
	void PutResultOnChart(TChart *Chart, TComponent *Owner);	// ������������ ��������� �� ������������ �����
	void SaveTubeToFile(UnicodeString FileName);				// ��������� ������ � ���� (������ zone_data)
	void LoadTubeFromFile(FILE*file);							// ��������� ������ �� ����� (zone_data)
	void Clear(TChart *Chart);
};
//--------------------------------------------------------------------------------

// ��������� ������ Result - ��������� ��������� �� ���� �������
class SummaryResult : public Result
{
public:
	short cut1,cut2;			// ����� ���� ���� 1 � 2
	short min_good_length;		// ����������� ������ �������
	short end_cut;				// ������� � ����� ������ �����
	String decision;			// ������� � ����� (�����, ����, ����� 2)

	SummaryResult();			//�����������
	~SummaryResult();			// ����������
	void ComputeZonesData(Result &_pp, Result &_pr, ThicknessResult &_th, bool bPr, bool bThick);	// ������� ��������� ��������� �� ���� �������, ��������� ������ zone_data
	TColor ZoneColor(double meas);      // ���������� ���� ����
	void PutResultOnChart(TChart *Chart, TComponent *Owner);        // ������ ��������� �� �����
	void MakeDecision();	  	// ��������� ������� �� ���� ����� � ��������� ���� ������
	void AddZone(Result &_pp, Result &_pr, ThicknessResult &_th);			// ��������� ���� � �������
};

//---------------------------------------------------------------------------
#endif
