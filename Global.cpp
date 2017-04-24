//---------------------------------------------------------------------------


#pragma hdrstop

#include "Global.h"
#include "IniFiles.hpp"
#include "AdvantechSignals.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
Speeds::Speeds()
{
// заполняем карту скоростей
	vector <int> temp;
	temp.push_back(1);
	speeds["1"]=temp;
	temp.clear();
	temp.push_back(2);
	speeds["2"]=temp;
	temp.clear();
	temp.push_back(1);
	temp.push_back(2);
	speeds["3"]=temp;
	temp.clear();
	temp.push_back(3);
	speeds["4"]=temp;
	temp.clear();
	temp.push_back(1);
	temp.push_back(3);
	speeds["5"]=temp;
	temp.clear();
	temp.push_back(2);
	temp.push_back(3);
	speeds["6"]=temp;
}
//---------------------------------------------------------------------------
TStrings* Speeds::getSpeedNames()
{
	TStringList *speednames = new TStringList();
	map <String, vector<int> >::iterator it;
	for (it = speeds.begin(); it != speeds.end(); it++)
		speednames->Add(it->first);
	return speednames;
}
//---------------------------------------------------------------------------
void Speeds::setSpeeds(String TypesizeName)
{
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	String Sector = "Type_" + TypesizeName;
	// задаем скорости транспорта из общих ("других") настроек
	String wSpeed = ini->ReadString("OtherSettings","WorkSpeed", "1");
	String tSpeed = ini->ReadString("OtherSettings","TransitSpeed", "2");
	String rSpeed = ini->ReadString("OtherSettings","ReturnSpeed", "2");
	workSpeed	 = speeds[wSpeed];
	transitSpeed = speeds[tSpeed];
	returnSpeed	 = speeds[rSpeed];
	// задаем скорости продольного исходя из текущего типоразмера
	String iSpeed = ini->ReadString(Sector , "InSpeed" ,"1");
	String lSpeed = ini->ReadString(Sector , "LinearSpeed" , "2");
	String oSpeed = ini->ReadString(Sector , "OutSpeed" , "4");
	inSpeed		 = speeds[iSpeed];
	linearSpeed	 = speeds[lSpeed];
	outSpeed	 = speeds[oSpeed];

	delete ini;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void SystemConst::SetSystemConstants()
{
	ThicknessMass = digital->SignalExists("Толщ Груз Исх")
					&& digital->SignalExists("Толщ Груз отвод")
					&& digital->SignalExists("Толщ Груз подвод");
	ThickModuleManage = digital->SignalExists("толщ по")
						&& digital->SignalExists("толщ рп")
						&& digital->SignalExists("толщ в по")
						&& digital->SignalExists("толщ в рп");
	ThickCentralizer = digital->SignalExists("толщ схват исх")
					&& digital->SignalExists("толщ схват сжать")
					&& digital->SignalExists("толщ схват разжать");
	LinearMass = digital->SignalExists("Прод Груз Исх")
					&& digital->SignalExists("прод Груз отвод")
					&& digital->SignalExists("прод Груз подвод");
	LinearCentralizer = digital->SignalExists("прод схват исх")
					&& digital->SignalExists("прод схват сжать")
					&& digital->SignalExists("прод схват разжать");
	CrossCentralizer = digital->SignalExists("Попер схват исх")
					&& digital->SignalExists("Попер схват сжать")
					&& digital->SignalExists("Попер схват разжать");
	CentralizersCard = digital->SignalExists("схват авто")
					&& digital->SignalExists("схват транзит")
					&& digital->SignalExists("схват работа")
					&& digital->SignalExists("схват прогон");
	TransportManage = digital->SignalExists("ТС: ПЧ А")
					&& digital->SignalExists("ТС: ПЧ RUN")
					&& digital->SignalExists("ТС: STR")
					&& digital->SignalExists("ТС: STF")
					&& digital->SignalExists("ТС: Скорость 1")
					&& digital->SignalExists("ТС: Скорость 2")
					&& digital->SignalExists("ТС: Скорость 3")
					&& digital->SignalExists("ТС: Пит ПЧ");
	ExternalTransport = digital->SignalExists("ТР Скорость 1")
					&& digital->SignalExists("ТР Скорость 2")
					&& digital->SignalExists("ТР Скорость 3")
					&& digital->SignalExists("ТР Питание ПЧ")
					&& digital->SignalExists("ТР вращение вперед")
					&& digital->SignalExists("ТР вращение назад")
					&& digital->SignalExists("ТР перекл1 вкл")
					&& digital->SignalExists("ТР перекл1 выкл")
					&& digital->SignalExists("ТР перекл2 вкл")
					&& digital->SignalExists("ТР перекл2 выкл")
					&& digital->SignalExists("ТР стеллаж")
					&& digital->SignalExists("ТР начало линии")
					&& digital->SignalExists("ТР конец линии")
					&& digital->SignalExists("ТР выгружена")
					&& digital->SignalExists("ТР ПЧ А")
					&& digital->SignalExists("ТР ПЧ RUN")
					&& digital->SignalExists("ТР двигатели")
					&& digital->SignalExists("ТР ТПП")
					&& digital->SignalExists("ТР выход")
					&& digital->SignalExists("ТР перекл1 внизу")
					&& digital->SignalExists("ТР перекл1 вверху")
					&& digital->SignalExists("ТР перекл2 внизу")
					&& digital->SignalExists("ТР перекл2 вверху");

	// читаем параметры из файла
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	isSolidGroupMS_DOS = ini->ReadBool("OtherSettings","IsCalcMS_DOS", 1);
	ComWithASU = ini->ReadBool("OtherSettings","ComWithASU", 0);
	SolidGroupSource = ini->ReadInteger("OtherSettings","SolidGroupSource", -1 );
	isMedianFilter = ini->ReadBool("OtherSettings","isMedianFilter", 0);
	isTempSensor = ini->ReadBool("OtherSettings","isTempSensor", 0);
	delete ini;
}

//---------------------------------------------------------------------------

/*
[ACard1_IN_Digital]
0=Толщ Вход
1=Толщ Выход
2=Толщ РП
3=Толщ ПО
4=Попер Вход
5=Попер Выход
6=Попер РП
7=Попер ПО
8=Прод Вход
9=Прод Выход
10=Прод РП
11=Прод ПО
12=Готовность
13=Контроль
14=Результат
15=Брак зоны

[ACard1_IN_TTL]
0=Цепи Управления
1=Защита двигателей
2=ТС: ПЧ А
3=ГП труба
4=ТС: ПЧ RUN
5=Прод ПЧ А
6=ГП включен
7=Прод ПЧ RUN
8=Воздух
9=Схват Прогон
10=Схват Работа
11=Стойка Прогон
12=Стойка Работа
14=Марк Воздух
15=Марк вход

[ACard1_OUT_Digital]
0=Соленоид Охлажд
1=Соленоид Питание
2=Толщ в РП
3=Толщ в ПО
4=Попер в ПО
5=Попер в РП
6=Прод в РП
7=Прод в ПО
8=Работа
9=Цикл
10=База
11=Синхро
12=Схват Авто
13=Схват Транзит
14=Стойка Авто
15=Стойка Транзит

[ACard1_OUT_TTL]
0=Прод Скорость 1
1=Прод Пит СУ
2=ТС: STR
3=ТС: STF
4=ТС: Скорость 3
5=ТС: Скорость 2
6=ТС: Скорость 1
7=ТС: Пит ПЧ
8=Бак перемешать
9=Дефектоотметчик
12=Прод Пит ПЧ
13=Прод STF
14=Прод Скорость 3
15=Прод Скорость 2

[ACard0_IN_Digital]
0=Цех цикл
1=Цех Транзит
2=Цех ТР1
3=Цех ТР2
4=Стойка 1
5=Стойка 2
6=Стойка 3

[ACard0_OUT_Digital]
0=Цех вперед
1=Цех Результат
2=Цех Признак
3=Цех назад


*/
