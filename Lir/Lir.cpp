#include "Lir.h"
#include "ItemIni.h"
#include "DebugMess.h"


static wchar_t *fileName = L"..\\..\\Settings\\Lir.ini";
wchar_t *Lir_inputs = L"INPUTS";
wchar_t *Lir_description = L"DESCRIPTION";

CntrEventHandler::CntrEventHandler(Lir &o)
	: h(o.h)
	, tabIndex(o.tabIndex)
	, tabIndex2(o.tabIndex2)
{}

void BDAQCALL CntrEventHandler::UdCntrEvent(void * sender, Automation::BDaq::tagUdCntrEventArgs * args)
{
	switch(args->SrcId)
	{
	case EvtCntPatternMatch0://событие 1 лира
		//  case EvtCntPatternMatch2://событие 1 лира
		tabIndex = ((Automation::BDaq::UdCounterCtrl *)sender)->getValue();//текущее значение счЄтчика лира
   		lastCounter = 0;
		SetEvent(h);
		break;
	case EvtCntPatternMatch1://событие 2 лира
		//   case EvtCntPatternMatch3://событие 2 лира
		{
			unsigned t = ((Automation::BDaq::UdCounterCtrl *)sender)->getValue();
			tabIndex += t - lastCounter;//прибавл€етс€ приращение второго лира к текущему значению
			lastCounter = t;
			tabIndex2 = t;
			SetEvent(h);
		}
		break;

	}
}
   
Lir::Lir(void)
	: crossUnit(*this)
	, thicknessUnit(*this)
	, longUnit(*this)
	, lirChange(*this)
	, udCounterCtrl(*AdxUdCounterCtrlCreate())
	, cntrEventHandler(*this)
{
	h = CreateEvent(NULL, TRUE, FALSE, NULL);
}

Lir::~Lir(void)
{
}

void Lir::Init(Advantech1730 *_dev)
{
	dev = _dev;
	unsigned t = ItemIni::Get(Lir_inputs, L"crossUnit_bitStart", 0, fileName);
	crossUnit.bitStart = 1 << t;
	t = ItemIni::Get(Lir_inputs, L"crossUnit_bitStop", 1, fileName);
	crossUnit.bitStop = 1 << t;

	t = ItemIni::Get(Lir_inputs, L"thicknessUnit_bitStart", 4, fileName);
	thicknessUnit.bitStart = 1 << t;
	t = ItemIni::Get(Lir_inputs, L"thicknessUnit_bitStop", 5, fileName);
	thicknessUnit.bitStop = 1 << t;

	t = ItemIni::Get(Lir_inputs, L"longUnit_bitStart", 8, fileName);
	longUnit.bitStart = 1 << t;
	t = ItemIni::Get(Lir_inputs, L"longUnit_bitStop", 9, fileName);
	longUnit.bitStop = 1 << t;

	t = ItemIni::Get(Lir_inputs, L"LirChange_bit", 15, fileName);
	lirChange.bit = 1 << t;

	samplesPerZoneParam[0] = ItemIni::Get(Lir_inputs, L"samplesPerZoneParam0", 180, fileName);
	samplesPerZoneParam[1] = ItemIni::Get(Lir_inputs, L"samplesPerZoneParam1", 180, fileName);


	wchar_t buf[128] = L"PCI-1784,BID#0";
	wchar_t *pBuf;
	pBuf = ItemIni::Get(Lir_description, L"Advantech1784", buf, fileName);
  #ifndef NO_1784
	ErrorCode errorCode = udCounterCtrl.setSelectedDevice(DeviceInformation(pBuf));
	if (BioFailed(errorCode))throw(Exception("A1730::A1784U: не смогли открыть плату Advantech1784U"));
	udCounterCtrl.addUdCntrEventListener(cntrEventHandler);
 #endif
	//запуск потока чтобы не нагружать обработчик платы 1784
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)__Run__, this, 0, NULL);
}

void Lir::Destroy()
{
	TerminateThread(hThread, 0);
}

void Lir::StartLir(int n)
{
  samplesPerZone = samplesPerZoneParam[n];
  if(n == udCounterCtrl.getChannel()) return;
//отключение одного лира
	udCounterCtrl.setChannel((n + 1) % 2);
	udCounterCtrl.setEnabled(false);

//подключение другого лира(смотри документацию на 1784)
	udCounterCtrl.setChannel(n);
	udCounterCtrl.setCountingType(TwoPulse);//AbPhaseX1);
	udCounterCtrl.CompareSetInterval(0, numberPulsesPerInterrupt, 100000000);
	udCounterCtrl.setEnabled(true);
}

void Lir::Start(bool isThick_, bool isLong_)
{
	isThick = isThick_;
	isLong = isLong_;
	crossUnit.Start();
	if(isThick)thicknessUnit.Start();
	if(isLong)longUnit.Start();
	lirChange.Start();
	StartLir(0);
}

void Lir::Run()
{
	while(true)
	{
		WaitForSingleObject(h, INFINITE);
		inputBits = dev->Read();
		(crossUnit.*crossUnit.ptr)();
		(thicknessUnit.*thicknessUnit.ptr)();
		(longUnit.*longUnit.ptr)();
		(lirChange.*lirChange.ptr)();
		ResetEvent(h);
	}
}

void Lir::__Run__(Lir *lir)
{
	lir->Run();
}

void Lir_Prepare(unsigned start, unsigned stop, unsigned (&time)[100], int &len)
{
	unsigned timeUnit = stop - start; //врем€ прохождени€ через модуль
	timeUnit /= 2;
	unsigned zoneTime = time[1] - time[0]; // прохождение 1 зоны

	int zonesUnit = timeUnit / zoneTime; //
	int remZone = timeUnit % zoneTime;

	unsigned tmpTime[100];
	//
	unsigned startTime = start + timeUnit; //врем€ захода трубы в зону измерени€
	tmpTime[0] = startTime;
	for(int i = 0; i < zonesUnit; ++i)
	{
		tmpTime[i + 1] = startTime + (i + 1) * zoneTime;
	}

	for(int i = 0, j = zonesUnit; i < len; ++i, ++j)
	{
	   tmpTime[j] = time[i] - remZone;
    }
	len += zonesUnit;
	memcpy(time, tmpTime, sizeof(time));
}
 /*
void correct(unsigned *d, int len)
{
	//for(int i = 1; i < len - 1; ++i) if(d[i] <= 0) d[i] = (d[i - 1] - d[i + 1]) / 2;
	for(int i = 1; i < len - 1; ++i)
	{
		int a = d[i] - d[i - 1];
		int b = d[i + 1] - d[i];
		if(a > b)  d[i] = d[i - 1] + (d[i + 1] - d[i - 1]) / 2;
		else if(a < b)  d[i] = d[i - 1] + (d[i + 1] - d[i - 1]) / 2;
    }
}
 */
void Lir::Prepare()
{
   Lir_Prepare(
	 crossUnit.bitStartUp
	 , crossUnit.bitStopUp
	 , crossUnit.zonesTime
	 , crossUnit.zonesTimeOffset
	 );
  //	 correct(crossUnit.zonesTime, crossUnit.zonesTimeOffset);
	 for(int i = 1; i < crossUnit.zonesTimeOffset; ++i)
	 {
		 dprint("crossUnit %d %d %d %d\n", i
		 , crossUnit.zonesTime[i] - crossUnit.zonesTime[i - 1]
		 , crossUnit.zonesTime[i]
		 , crossUnit.zonesOffs[i] - crossUnit.zonesOffs[i - 1]
		 );
	 }

   if(isThick)
   {
   Lir_Prepare(
	 thicknessUnit.bitStartUp
	 , thicknessUnit.bitStopUp
	 , thicknessUnit.zonesTime
	 , thicknessUnit.zonesTimeOffset
	 );
  //	 correct(thicknessUnit.zonesTime, thicknessUnit.zonesTimeOffset);
	 for(int i = 1; i < crossUnit.zonesTimeOffset; ++i)
	 {
		 dprint("thicknessUnit %d %d %d %d\n", i
		   , thicknessUnit.zonesTime[i] - thicknessUnit.zonesTime[i - 1]
		   , thicknessUnit.zonesTime[i]
		   , thicknessUnit.zonesOffs[i] - thicknessUnit.zonesOffs[i - 1]
		   );
	 }
   }

   if(isLong)
   {
	Lir_Prepare(
	 longUnit.bitStartUp
	 , longUnit.bitStopUp
	 , longUnit.zonesTime
	 , longUnit.zonesTimeOffset
	 );

  //	 correct(longUnit.zonesTime, longUnit.zonesTimeOffset);

	 for(int i = 1; i < longUnit.zonesTimeOffset; ++i)
	 {
		 dprint("longUnit %d %d %d %d\n", i
		 , longUnit.zonesTime[i] - longUnit.zonesTime[i - 1]
		 , longUnit.zonesTime[i]
		 , longUnit.zonesOffs[i] - longUnit.zonesOffs[i - 1]
		 );
	 }
   }
}

Lir &Lir::Instance(){static Lir x; return x;}
//----------------------------------------------------------------------------------------------------------
CrossUnit::CrossUnit(Lir &o)
	: owner(o)
	, ptr(&CrossUnit::Noop)
{}
void CrossUnit::Start()
{
	onceUp = true;
	ptr = &CrossUnit::StartUp;
	zonesTimeOffset = 0;
}

void CrossUnit::StartUp()
{
	if(owner.inputBits & bitStart)
	{
		bitStartUp = timeGetTime();
		ptr = &CrossUnit::StopUp;
		dprint("%s\n", __FUNCTION__);
	}
}
void CrossUnit::StopUp()
{
	if(owner.inputBits & bitStop)
	{
	   lirOffsetUp = owner.tabIndex;
	   bitStopUp = timeGetTime();
	   ptr = &CrossUnit::StartDown;
	   startOffset = owner.tabIndex;
	   dprint("%s\n", __FUNCTION__);
	}
}
void CrossUnit::StartDown()
{
    if(owner.inputBits & bitStart)
	{
		int t = owner.tabIndex - startOffset;
		if(t < 0) startOffset = owner.tabIndex;
		int tt = t / owner.samplesPerZone;
		tt *= owner.samplesPerZone;
		bool bit = t - tt <= owner.samplesPerZone / 2;
		if(onceUp && bit)
		{
			onceUp = false;
			zonesTime[zonesTimeOffset] = timeGetTime();
			zonesOffs[zonesTimeOffset] = t;
			++zonesTimeOffset;
		}
		if(!bit) onceUp = true;
	}
	else
	{
		bitStartDown = timeGetTime();
		ptr = &CrossUnit::StopDown;
	}
}
void CrossUnit::StopDown()
{
	if(owner.inputBits & bitStop)
	{
		int t = owner.tabIndex - startOffset;
		if(t < 0) startOffset = owner.tabIndex;
		int tt = t / owner.samplesPerZone;
		tt *= owner.samplesPerZone;
		bool bit = t - tt <= owner.samplesPerZone / 2;
		if(onceUp && bit)
		{
			onceUp = false;
			zonesTime[zonesTimeOffset] = timeGetTime();
			zonesOffs[zonesTimeOffset] = t;
			++zonesTimeOffset;
		}
		if(!bit) onceUp = true;
	}
	else
	{
		lirOffsetDown = owner.tabIndex;
		bitStartDown = timeGetTime();
		ptr = &CrossUnit::Noop;
//тк срабатывание лира на певом датчике не корректно - сэмитируем
		static const int widthModule = 630; //расто€ние между датчиками
		int t = int((double)(zonesTime[1] - zonesTime[0]) * widthModule / Lir::zoneLength);
		bitStartUp = bitStopUp - t;
		/*
		dprint("%s bitStartUp %d  bitStopUp %d\n", __FUNCTION__);
		int dt = 0;
		bitStartUp += (zonesTime[1] - zonesTime[0])/2;
		bitStopUp +=  (zonesTime[1] - zonesTime[0])/2;
		for(int i = 1; i < zonesTimeOffset; ++i)
		{
			dt = zonesTime[i] - zonesTime[i - 1];
			zonesTime[i - 1] += dt / 2;
		}
		zonesTime[zonesTimeOffset - 1] += dt / 2;
	   	*/
	}
}

void CrossUnit::Noop()
{
}
//-------------------------------------------------------------------------
ThicknessUnit::ThicknessUnit(Lir &o)
	: owner(o)
	, ptr(&ThicknessUnit::Noop)
{}

void ThicknessUnit::Start()
{
	onceUp = true;
	ptr = &ThicknessUnit::StartUp;
	zonesTimeOffset = 0;
}

void ThicknessUnit::StartUp()
{
	if(owner.inputBits & bitStart)
	{
		bitStartUp = timeGetTime();
		ptr = &ThicknessUnit::StopUp;
		dprint("%s\n", __FUNCTION__);
	}
}
void ThicknessUnit::StopUp()
{
	if(owner.inputBits & bitStop)
	{
	   bitStopUp = timeGetTime();
	   ptr = &ThicknessUnit::StartDown;
	   startOffset = owner.tabIndex;
	   dprint("%s\n", __FUNCTION__);
	}
}
void ThicknessUnit::StartDown()
{
	if(owner.inputBits & bitStart)
	{
		int t = owner.tabIndex - startOffset;
		if(t < 0) startOffset = owner.tabIndex;
		int tt = t / owner.samplesPerZone;
		tt *= owner.samplesPerZone;
		bool bit = t - tt <= owner.samplesPerZone / 2;
		if(onceUp && bit)
		{
			onceUp = false;
			zonesTime[zonesTimeOffset] = timeGetTime();
			zonesOffs[zonesTimeOffset] = t;
			++zonesTimeOffset;
		}
		if(!bit) onceUp = true;
	}
	else
	{
		bitStartDown = timeGetTime();
		ptr = &ThicknessUnit::StopDown;
	}
}
void ThicknessUnit::StopDown()
{
	if(owner.inputBits & bitStop)
	{
		int t = owner.tabIndex - startOffset;
		if(t < 0) startOffset = owner.tabIndex;
		int tt = t / owner.samplesPerZone;
		tt *= owner.samplesPerZone;
		bool bit = t - tt <= owner.samplesPerZone / 2;
		if(onceUp && bit)
		{
			onceUp = false;
			zonesTime[zonesTimeOffset] = timeGetTime();
			zonesOffs[zonesTimeOffset] = t;
			++zonesTimeOffset;
			dprint("%s\n", __FUNCTION__);
		}
		if(!bit) onceUp = true;
	}
	else
	{
		bitStartDown = timeGetTime();
		ptr = &ThicknessUnit::Noop;
	}
}

void ThicknessUnit::Noop()
{
}
//-----------------------------------------------------------------------------------
LongUnit::LongUnit(Lir &o)
	: owner(o)
	, ptr(&LongUnit::Noop)
{}

void LongUnit::Start()
{
	onceUp = true;
	ptr = &LongUnit::StartUp;
	zonesTimeOffset = 0;
}

void LongUnit::StartUp()
{
	if(owner.inputBits & bitStart)
	{
		bitStartUp = timeGetTime();
		ptr = &LongUnit::StopUp;
		dprint("%s\n", __FUNCTION__);
	}
}
void LongUnit::StopUp()
{
	if(owner.inputBits & bitStop)
	{
	   bitStopUp = timeGetTime();
	   ptr = &LongUnit::StartDown;
	   startOffset = owner.tabIndex;
	   dprint("%s\n", __FUNCTION__);
	}
}
void LongUnit::StartDown()
{
	if(owner.inputBits & bitStart)
	{
		int t = owner.tabIndex - startOffset;
		if(t < 0) startOffset = owner.tabIndex;
		int tt = t / owner.samplesPerZone;
		tt *= owner.samplesPerZone;
		bool bit = t - tt <= owner.samplesPerZone / 2;
		if(onceUp && bit)
		{
			onceUp = false;
			zonesTime[zonesTimeOffset] = timeGetTime();
			zonesOffs[zonesTimeOffset] = t;
			++zonesTimeOffset;
		}
		if(!bit) onceUp = true;
	}
	else
	{
		bitStartDown = timeGetTime();
		ptr = &LongUnit::StopDown;
	}
}
void LongUnit::StopDown()
{
	if(owner.inputBits & bitStop)
	{
		int t = owner.tabIndex - startOffset;
		if(t < 0) startOffset = owner.tabIndex;
		int tt = t / owner.samplesPerZone;
		tt *= owner.samplesPerZone;
		bool bit = t - tt <= owner.samplesPerZone / 2;
		if(onceUp && bit)
		{
			onceUp = false;
			zonesTime[zonesTimeOffset] = timeGetTime();
			zonesOffs[zonesTimeOffset] = t;
			++zonesTimeOffset;
		}
		if(!bit) onceUp = true;
	}
	else
	{
		bitStartDown = timeGetTime();
		ptr = &LongUnit::Noop;
	}
}
void LongUnit::Noop()
{
}
//--------------------------------------------------------------------------------
LirChange::LirChange(Lir &o)
: owner(o)
, ptr(&LirChange::Noop)
{

}
void LirChange::Start()
{
	ptr = &LirChange::StartUp;
	dprint("%s\n", __FUNCTION__);
}
void LirChange::StartUp()
{
   if(owner.inputBits & bit)
   {
	   lirOffsetUp = owner.tabIndex;
	   delay = owner.tabIndex + 2 * owner.samplesPerZone;
	   ptr = &LirChange::Delay;
   }
}
void LirChange::Delay()
{
	 if(owner.tabIndex > delay)
	 {
		  owner.StartLir(1);
		  owner.cntrEventHandler.lastCounter = owner.udCounterCtrl.getValue();
		  ptr = &LirChange::StartDown;
     }
}
//lirOffsetDown = owner.tabIndex;
void LirChange::StartDown()
{
  if(0 == (owner.inputBits & bit))
  {
	 lirOffsetDown = owner.tabIndex;
	 ptr = &LirChange::Noop;
	 double up = (double)Lir::zoneLength
	   * (lirOffsetUp - owner.crossUnit.lirOffsetUp)/Lir::distantionSQ9_SQ26;
	 double down = (double)Lir::zoneLength
	   * (lirOffsetDown - owner.crossUnit.lirOffsetDown)/Lir::distantionSQ9_SQ26;
	 dprint("lir %f %f %d %d\n", up, down, lirOffsetUp, owner.crossUnit.lirOffsetUp);
  }
}
void LirChange::Noop()
{

}
