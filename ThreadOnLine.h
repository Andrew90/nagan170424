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
		// присланные из главной формы
		Advantech1784 *Counter;				// указатель на плату 1784
		LCard791 *lpop;						//Поперечная плата
		LCard791 *lprod;					//Продольная плата
		Result *respop;						// указатель на результат поперечный
		Result *resprod;					// указатель на результат продольный
		ThicknessResult *resth;				// указатель на результат толщиномра
		SummaryResult *ressum;				// указатель на реузльтат суммарный
		TStatusBar *StB;					// указатель на статусбар, показывающий течение процесса
		bool Thick;							// используем ли толщинометрию
		bool Linear;						// используем ли продольный
		AlarmThreadClass *lat;				// локальный тред аварии (адрес)
//		bool Prepare;						// нужно ли готовить к началу работы

		void __fastcall DrawResultPP();		// рисует попероечный результат
		void __fastcall DrawResultPR();		// рисует продольный результат
		void __fastcall RedrawGraphics();	// перерисовывает все графики главной формы
		void __fastcall OnlineZones();		// отрисовыввает зоны онлайн (тестовое)
		// все параметры, связанные со счетчиками (стали массивами, потому что зависят от длины роликов)
		double perimeters[Globals::Counters];		// длины роликов ЛИРа
		long imp_per_zone[Globals::Counters];		// кол-во импульсов на одну зону (для каждого ЛИРа)
		long ppOffset[Globals::Counters];			// отступ до попера в импульсах (для каждого ЛИРа)
		long prOffset[Globals::Counters];			// отступ до прода в импульсах (для каждого ЛИРа)
		long thOffset[Globals::Counters];			// отступ до толщ в импульсах (для каждого ЛИРа)
		long markOffset[Globals::Counters];			// отступ до марк в импульсах (для каждого ЛИРа)
		int current_counter;						// номер счетчика, используемого в текущий момент
		long Counter0Fin;							// последнее значение счетчика 0 перед переключением
		long Counter1Fin;							// последнее значение счетчика 1 перед переключением
		long TotalCounter;							// суммарное, накопленное значение 3-ех счетчиков
		long PoperStart;							// значение счетчика при замыкании Попер вход
		long PoperFinish;							// значение счетчика при размыкании Попер вход
		long ProdStart;								// значение счетчика при замыкании Прод вход
		long ProdFinish;							// значение счетчика при размыкании Прод вход
		long TolshStart;							// значение счетчика при замыкании Толщ вход
		long TolshFinish;							// значение счетчика при размыкании Толщ вход
		long MarkStart;								// значение счетчика при замыкании ГП труба (ближайший к дефектоотметчику датчик)
		long MarkFinish;							// значение счетчика при размыкании ГП труба
		// все параметры, связанные со сбором (Л-Кард)
		DynamicArray < DynamicArray <double> > ppData;	// массив итоговых значений по датчикам
		DynamicArray < DynamicArray <double> > prData;	// массив итоговых значений по датчикам
		int meas_per_zone;                       //кол-во измерений в одной зоне
		ULONG ppFirst;							 //начало зоны поперечной платы
		ULONG ppSecond;							 //конец зоны поперечной платы
		ULONG prFirst;                           //начало зоны продольной платы
		ULONG prSecond;                          //конец зоны продольной платы
		long meas_per_sensor;                    //кол-во измерений в зоне по одному датчику
		ULONG *Buffer;					  		 //буфер для обработки данных
		long ppNumber;                           //Кол-во собранных частей буфера для ПП
		long prNumber;                           //кол-во собранных частей буфера для ПрП

		String stext1;						// текст для надписи в статусбаре в панели 1
		String stext2;						// текст для надписи в статусбаре в панели 2
		int plantCut;						// зона реза по шаблонированию (фэйспалм)

		UnicodeString PrepareForWork( );	// выставляем исходное перед работой
		void Finally();                     // финальные штуки (отключение сигналов и т.д.)

	public:                           //Конструктор
		__fastcall ThreadOnLine(bool CreateSuspended,
								Advantech1784 *_Counter,	// плата Advantech1784
								LCard791 &_l1,				//Поперечная плата
								LCard791 &_l2,				//Продольная плата
								Result &_respop,			// указатель на результат поперечный
								Result &_resprod,			// указатель на результат продольный
								ThicknessResult &_resth,	// указатель на результат толщины
								SummaryResult &_ressum,		// указатель на результат суммарный
								TStatusBar *St,				// указатель на статусбар, показывающий течение процесса
								bool _Thick,				// используем ли толщинометрию
								bool _Linear,				// используем ли продольный)
								AlarmThreadClass &alth);	// адрес треда аварии

		bool Collect;							//Признак остановки потока
		bool OnlineCycle(void);					//поток обработки всей кучи информации
		void __fastcall UpdateStatus();			// обновляет компоненты главной формы, обрабатывает события
		bool cool;								// вместо Return Value, который хуй откопаешь
		bool transit;							// цеховые хотят транзит

		// всякая херня для расчетов, у какого модуля какое событие
		int ppOnline;			// зона на поперечном в текущий момент
		int ppOld;				// зона на поперечном предыдущая
		int prOnline;           // зона на продольном в текущий момент
		int prOld;              // зона на продольном предыдущая
		int thOnline;           // зона на толщине в текущий момент
		int thOld;              // зона на толщине предыдущая
		int markOnline;         // зона на дефектоотметчике в текущий момент
		int markOld;            // зона на дефектоотметчике предыдущая
		void CalcOnlineZones();	// считает текущие зоны на каждом модуле
		void CalcOffsets();		// считает отступы модулей от ЛИРа (1 раз в начале)
		// калибровка ЛИРов (рассчет длины ролика исходя из числа импульсов)
		long St2in;		// время захода во 2 стойку ( 0 счетчик )
		long St4in;		// время захода во 4 стойку ( 0 счетчик )
		long St1out;	// время выхода с  1 стойки ( 1 счетчик )
		long St3out;	// время выхода с  3 стойки ( 1 счетчик )
		long St4out;	// время выхода с  4 стойки ( 2 счетчик )
		long GPout;		// время выхода с  ГП модуля ( 2 счетчик )
		void Calibrate();
};
//---------------------------------------------------------------------------

#endif
