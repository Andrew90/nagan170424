//---------------------------------------------------------------------------


#pragma hdrstop

#include "ExtTransport.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

ExtTransport::ExtTransport(){}
//---------------------------------------------------------------------------
String ExtTransport::Load()
{
	if( !digital->CheckInSignal("ТР Стеллаж") )
		return "Нет трубы на стелаже";
	else if( digital->CheckInSignal("ТР Начало линии") )
		return "Труба на линии";
	else
	{
		return WorkLoader(1);
    }
}
//---------------------------------------------------------------------------
String ExtTransport::UnloadTubeOutOfLine()
{
	String Value = NET;
	if (SystemConst::ExternalTransport)
	{
		Value = ok;
		if( digital->CheckInSignal("ТР конец линии") && !digital->CheckInSignal("ТР выгружена") )
		{
			Value = WorkLoader(2);
			for(int i=0; i < 3; i++)
			{
				if( (digital->CheckInSignal("ТР конец линии") && !digital->CheckInSignal("ТР выгружена") ) ||
				(!digital->CheckInSignal("ТР конец линии") && !digital->CheckInSignal("ТР выгружена")) )
					{
						Value = WorkLoader(2);
						TThread::CurrentThread->Sleep(3000);
					}
			}
			if( (digital->CheckInSignal("ТР конец линии") && !digital->CheckInSignal("ТР выгружена") ) ||
				(!digital->CheckInSignal("ТР конец линии") && !digital->CheckInSignal("ТР выгружена")) )
			Value = "Не удалось переложить трубу";
			return Value;
		}
	}
	return Value;
}
//---------------------------------------------------------------------------
String ExtTransport::WorkLoader(int number)
{
	String n = IntToStr(number);
	digital->ResetOutSignal("ТР перекл"+n+" вкл");
	if(LoaderTest(number) == ok)
	{
		digital->BlinkOutSignal("ТР перекл"+n+" вкл" , 2000);
		if( !digital->WaitForInSignal("ТР перекл"+n+" вверху", 8000) )
			return "Ошибка при движении вверх";

		digital->BlinkOutSignal("ТР перекл"+n+" выкл" , 2000);
		if( !digital->WaitForInSignal("ТР перекл"+n+" внизу", 8000) )
			return "Ошибка при движении вниз";
		else
			return ok;
	}
	else
		return ErrLoader;

}
//---------------------------------------------------------------------------
String ExtTransport::LoaderTest(int number)
{
	String n = IntToStr(number);
	if(digital->CheckInSignal("ТР перекл"+n+" вверху") ) 				//если вверху
	{
		 digital->BlinkOutSignal("ТР перекл"+n+" выкл" , 120);                  //опустим вниз

		 if( !digital->WaitForInSignal("ТР перекл"+n+" внизу", 8000) )
			return ErrLoader+n;
		 else
			return ok;
	}
	else
		return ok;
}
//---------------------------------------------------------------------------
void ExtTransport::ResetAllExtTransportSignal()
{
	digital->ResetOutSignal("ТР Скорость 1");
	digital->ResetOutSignal("ТР Скорость 2");
	digital->ResetOutSignal("ТР Скорость 3");
	digital->ResetOutSignal("ТР вращение вперед");
	digital->ResetOutSignal("ТР перекл1 вкл");
	digital->ResetOutSignal("ТР перекл1 выкл");
	digital->ResetOutSignal("ТР перекл2 вкл");
	digital->ResetOutSignal("ТР перекл2 выкл");
}
//---------------------------------------------------------------------------
void ExtTransport::StopMovingExtTransport()
{
	digital->ResetOutSignal("ТР Скорость 1");
	digital->ResetOutSignal("ТР Скорость 2");
	digital->ResetOutSignal("ТР Скорость 3");
	digital->ResetOutSignal("ТР вращение вперед");
    digital->ResetOutSignal("ТР вращение назад");
}
//---------------------------------------------------------------------------
