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
//Все функции отработают только если SystemConst::ExternalTransport = true
class ExtTransport
{

	public:
		ExtTransport();
		//функции вернут текст ошибки, или строку "ok" в случае успеха

		//загружает трубу на ЛДФ
		static String Load();
		// выгружает трубу с линии на линию выхода
		static String UnloadTubeOutOfLine();
		//смотрим состояние перекладчиков, если вверху то опускаем (номер перекладчика: до ЛДФ - 1, после ЛДФ - 2)
		static String LoaderTest(int number);
		//сбрасывает все выходные сигналы транспорта и перекладчиков
		static void ResetAllExtTransportSignal();
		//Останавливает движение внешнего транспорта
		static void StopMovingExtTransport();
		static int resultPrevious;
		static int resultCurrent;
	private:
		// Работа перекладчиков. Номер перекладчика:до ЛДФ - 1, после ЛДФ - 2
		static String WorkLoader(int number);
		static String NET;
		static String ErrLoader;
		static String ok;

};
	int ExtTransport::resultPrevious=-1;
	int ExtTransport::resultCurrent=-1;
	String ExtTransport::NET = "NO_EXTERNAL_TRANSPORT";
	String ExtTransport::ErrLoader = "Ошибка работы перекладчика";
	String ExtTransport::ok = "ok";

#endif
