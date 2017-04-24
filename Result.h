//---------------------------------------------------------------------------
#ifndef ResultH
#define ResultH

#include "Classes.hpp"
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include "Chart.hpp"

#include "LCard791.h"

//---------------------------------------------------------------------------


// класс результата дефектоскопии
class Result
{
protected:
	TForm *SensorForm;					// форма для отрисовки измерений по датчику
	TChart *SensorChart;				// чарт для отрисовки измерений по датчику
	TColor Dead;						// Цвет мертвой зоны
	TColor Brack;						// цвет барака
	TColor Class2;						// цвет второго класса
	TColor Good;						// цвет годной, хорошей трубы
	DynamicArray < DynamicArray <double> > sensor_data;		// массив итоговых значений по датчикам
//todo	DynamicArray < DynamicArray < DynamicArray < double > > > data;		// массив всех значений - по всем датчикам , по всем зонам
public:
vector < vector < vector < double > > > data;
protected:
public:
	double meas_dead_zone_finish;
	double meas_dead_zone_start;
	vector < double > gain;

private:
	void _medianfilter(double* signal, double* result, int N);      // Вспомогательная функция для фильтрации
	int WidthMedianFilter;                                          // Ширина окна для медианной фильтрации

public:
	void medianfilter(double* signal, double* result, int N);		//N - длина сигнала
   	short zones;						// кол-во отображаемых зон в результате
	short sensors;                      // кол-во отображаемых сенсоров (датчиков)
	int meas_per_zone;                  // кол-во измерений в 1 зоне 1 датчике
	int dead_zone_start;	            // мертвая зона в начале трубы
	int dead_zone_finish;	            // мёртвая зона в конце трубы
	long total_meas;					// общее кол-во измерений по одному датчику
	DynamicArray <double> borders;      // массив порогов - [0] - порог брака, [1] - порог класса 2 и т.д.
	DynamicArray <double> zone_data;    // массив итоговых значений по зонам
	DynamicArray <int> meas_per_sensor;	// массив количество измерений на датчик в зоне (ЛИРы)


	Result();										// конструктор
	~Result();										// деструктор
	virtual void CreateTestData(short tag);			// создает тестовые данные (удалить позже)
	virtual void ComputeZoneData();					// считает данные, заполняя массивы "zone_data" и "sensor_data"
	TColor ZoneColor(double meas,int XCoord = -1);	// возвращает цвет измерения/датчика/зоны
	virtual void PutResultOnChart(TChart *C, TComponent *Owner);	// отрисовывает результаты на чарте С

	void PutDataOnChart(TChart *C,TComponent *Owner, int zone, int sensor);		// рисует измерения в зоне/датчике на графике

	#if 0
	void PrepareData(LCard791 *LC,				   // разбирает собранные платой L791 значения по зонам и датчикам
					int _sensors,int DefSensors, int MeasOutCrossTrash = 0);
#else
void PrepareData(LCard791 *LC, vector<vector<vector<double> > >(&pop)
     , vector<vector<vector<double> > >(&prod)) ;

#endif
	void __fastcall SeriesDblClick(TChartSeries *Sender,  	// двойной клик по датчику открывает окно с измерениями
			int ValueIndex, TMouseButton Button,  TShiftState Shift, int X, int Y);

	void __fastcall EscapeKeyPress(TObject *Sender, wchar_t &Key);	// обработчик события "нажал Esc - окно закрыл"
	virtual void SaveTubeToFile(UnicodeString FileName);					// сохраняем данные по трубе в файл
	virtual void LoadTubeFromFile(FILE *file);  							// загрузка данных по трубе из файла
	void DeleteData();												// удалить все измерения
	void SetGains(vector<double> _Gain);							// устанавливает все усиоления датчиков
	void SetSensorGain(double _gain, int sensor);					// устанавливает усиление 1 конкртеного датчика
	void CalculateDeadZone(int LengthZone);							// высчитывает мертвые зоны
	void CalculateDeadFront(int LengthZone);						// считает мертвую зону в начале
	void CalculateDeadBack(int LengthZone);							// считает мертвую зону в конце
	void AddZone( DynamicArray < DynamicArray <double> > new_data);	// добавляет рассчитанную зону в онлайне
    void Clear(TChart *C);
};
//--------------------------------------------------------------------------------

// наследник класса Result - результат по толщине
class ThicknessResult : public Result
{
public:
	ThicknessResult();					// конструктор
	~ThicknessResult();					// деструктор
	void CreateTestData(short tag);		// создате тестовую инфу (после прочтения сжечь)
	void ComputeZoneData();				// считает данные по зонам и проч (после прочтения сжечь)
	TColor ZoneColor(double meas);		// возвращает цвет зоны
	void PutResultOnChart(TChart *Chart, TComponent *Owner);	// отрисовывает результат на определенном чарте
	void SaveTubeToFile(UnicodeString FileName);				// сохраняет данные в файл (только zone_data)
	void LoadTubeFromFile(FILE*file);							// загружает данные из файла (zone_data)
	void Clear(TChart *Chart);
};
//--------------------------------------------------------------------------------

// наследник класса Result - суммарный результат по всем модулям
class SummaryResult : public Result
{
public:
	short cut1,cut2;			// номер зоны реза 1 и 2
	short min_good_length;		// минимальный годный участок
	short end_cut;				// отрезок с обоих концов трубы
	String decision;			// решение о трубе (Годно, Брак, Класс 2)

	SummaryResult();			//конструктор
	~SummaryResult();			// деструктор
	void ComputeZonesData(Result &_pp, Result &_pr, ThicknessResult &_th, bool bPr, bool bThick);	// считает суммарный результат по всем модулям, заполянет массив zone_data
	TColor ZoneColor(double meas);      // возвращает цвет зоны
	void PutResultOnChart(TChart *Chart, TComponent *Owner);        // рисует результат на чарте
	void MakeDecision();	  	// принимает решение по всей трубе и вычисляет зоны отреза
	void AddZone(Result &_pp, Result &_pr, ThicknessResult &_th);			// добавляет зону в онлайне
};

//---------------------------------------------------------------------------
#endif
