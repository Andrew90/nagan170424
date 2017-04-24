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
// класс скоростей преобразователя частоты
class Speeds
{
public:
	// скорости транспорта
	vector <int> workSpeed;					// скорость транспорта в работе и выгоне
	vector <int> transitSpeed;				// скорость транспорта в транзите
	vector <int> returnSpeed; 				// скорость транспорта в возврате
	// скорости продольного модуля
	vector <int> inSpeed;					// скорость продольного на входе в модуль
	vector <int> linearSpeed;				// скорость продольного во время работы
	vector <int> outSpeed;					// скорость продольного при выходе трубы

	map <String, vector<int> > speeds;		// карта отношения Скорость юзера - скорость частотника

	Speeds();								// конструктор класса - создаем карту
	TStrings* getSpeedNames();				// считываем все имена из карты
	void setSpeeds(String TypesizeName);	// устанавливает скорости (читает из ini и генерит, нужен типоразмер)
};

//-------------------------------------------------------------------------------------------
// класс глобальных переменных
class Globals
{
public:

	static String current_typesize; 	// текущий типоразмер (может быть любой слово, название, удобное юзерам)
	static int current_diameter;		// текущий диаметр (строго 60,73 или 89)
	static bool isView;					// разрешение на просмотр
	static String IniFileName;          // имя и путь к файлу с настройками
	static String SignalsFileName;		// имя и путь к файлу с сигналами цифровыми
	static String PathFile_threshold_sg;// путь к файлу с порогами для гп с ms-dos
	static long tube_number;			// номер трубы
	static Speeds speeds;				// скорости частотников
	static SolidGroups::Tube tubeSG;	// труба с характеристиками группы прочности
	static TStringList *delay;			// сообщения в протокол с задержкой
	static bool ProtocolCreated;		// флаг, что протокол создан

	static const int max_sensors = 16;
	static const int max_zones = 60;
	static const int max_meas = 10000;
	static const int level100 = 8192; 	// 8192 макс значение кода АЦП с платы LCARD
	static const int Counters = 3;		// число счетчиков в проекте
	static const int LinSensors = 4;	// число датчиков продольного контроля в проекте

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
// класс констант, относящихся к особенностям проекта
class SystemConst
{
public:
	static bool isMedianFilter;		// работа с медианным фильтром
	static bool ThicknessMass;		// есть ли груз у толщиномера
	static bool ThickModuleManage;	// управляет ли система положением модуля толщинометрии
	static bool ThickCentralizer;	// есть ли центратор у толщиномера (управляемый из дефектоскопа)
	static bool LinearMass;			// есть ли груз у продольного
	static bool LinearCentralizer;	// есть ли центратор у продольного (управляемый из дефектоскопа)
	static bool CrossCentralizer;	// есть ли центратор у поперечного (управляемый из дефектоскопа)
	static bool CentralizersCard;	// есть ли плата DGV по управлению схватами (схват авто, транизт и т.д.)
	static bool TransportManage;	// Управляем ли мы транспортом
	static bool ComWithASU;			// обмен с АСУ цеха по COM порту
	static SGSource SolidGroupSource;// источник группы прочности
	static bool HideInGain;			 //	прятать ли усиления каналов и отключение модулей
	static bool IsOnline;			 // с ЛИРом или без, проект
	static bool ExternalTransport;	 // существует ли внешний транспорт (сопряженный)
	static bool isSolidGroupMS_DOS;	 // считаем группу прочности с помощью проги ms-dos,овской
	static bool isTempSensor;		 // использовать ли датчик температуры при определении ГП
	static void SetSystemConstants();	// установим постоянные характеристики проекта

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
