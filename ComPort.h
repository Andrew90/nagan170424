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
// параметры передачи
	String PortName;			// им€ порта (COM1, COM2 ...)
	int BaudRate;				// бод рейт (скорость)
	int DataBits;				// биты данных
	int StopBits;				// стоповые биты
	int Parity;					// четность
// внутренние переменные
	int ASUMode;				// режим передачи (дл€ разных проектов)
	int length;					// длина посылки от ј—” (5 или 17)
	int abonent;				// адрес абонента в сети (всегда const)
	int function;				// функци€ обмена	(1, 2 или 5)
	unsigned char data[50];		// прин€тые данные с кома
	int data_size;				// длина прин€тых данных
	TEvent* DataReceived;		// событие - данные получены
	TCommPort *CommPort;		// элемент —лы ƒиман :)
// внутренние функции
	void SetComPortSettings();	// установить настройки
	long GetTubeNumber_1();		// запрос по первому протоколу (—трежевой)
	long GetTubeNumber_2();		// запрос во второму протоколу	(ѕыть-ях)
	bool CheckSentMessage();	// провер€ет присланные данные (только 2-ой протокол )
// запись результатов по ≈лисеевски, жесть
	unsigned char EliseevRes(int zone);	// конвертит результаты контрол€ по ≈лисеевской формуле
	int plant_cut;						// зона реза по шаблонированию, жесть
	Result pp2;
	Result pr2;
	ThicknessResult th2;
	SummaryResult sum2;
	TColor Good;			// костыли дл€ извлечени€ результатов
	TColor Class2;
	TColor Brack;

public:
	MyComPort();		// конструктор нашего класса
	~MyComPort();		// деструктор

	void __fastcall MyComPortDataReceived(TObject *Sender, DWORD Count);	// событие - данные прин€ты
		// рассчет контрольной суммы
	unsigned char countCRC16_1(unsigned char* buff, unsigned int size);	// контр сумма по 1 протоколу
	unsigned short countCRC16_2(unsigned char* bufData, unsigned int sizeData);	// контр сумма по 2 протоколу
		// тест канала св€зи
	bool TestConnection();
		// запрос номера трубы (весь обмен)
	long GetTubeNumber();
		// посылка результата (весь обмен)
	bool SendResultToASU(short res, short c1, short c2, short len, SolidGroups::Group sgg);		// 1 - —трежевой
//	bool SendResultToASU(Result &pp1, Result &pr1, ThicknessResult &th1, SummaryResult &sum1 , SolidGroups::Group sgg);	// 2 - ѕыть-ях
	bool SendResultToASU(Result &pp1, Result &pr1, ThicknessResult &th1, SummaryResult &sum1 , int solid_num);	// 2 - ѕыть-ях
		// прин€ть группу прочности (весь обмен)
	SolidGroups::Tube GetSolidGroup();
		// зона реза по шаблонированию
	inline int GetPlantCut() { return plant_cut;}


};

//---------------------------------------------------------------------------
// глобальный экземпл€р, управл€ющий цифровыми сигналами из любой точки программы.
extern MyComPort *MyCom;
//---------------------------------------------------------------------------

#endif
