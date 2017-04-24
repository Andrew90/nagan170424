#include <vcl.h>
//---------------------------------------------------------------------------
#pragma hdrstop

#include "LCard791.h"
#include <Mmsystem.h>
#include "Global.h"
#include "Protocol.h"
#include "PrepareSG.h"
#include "DebugMess.h"

#pragma comment(lib, "Winmm.lib")
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#pragma package(smart_init)

unsigned long FIFO=1024;                   //
unsigned long IrqStep=1024;                // Переменные для настройки ЦАП/АЦП
unsigned long pages=128;

using namespace std;

CREATEFUNCPTR CreateInstance;
EXTERN_C const GUID DECLSPEC_SELECTANY IID_ILDEV={0x32bb8320,0xb41b,0x11cf,{0xa6,0xbb,0x00,0x80,0xc7,0xb2,0xd6,0x82}};


LCard791::LCard791(int _Size)
{
	Mode=0;
	pIUnknown=NULL;
	HINSTANCE hComponent;
	hComponent=LoadLibraryA("lcomp.dll");
	if (hComponent == NULL)
		Application->MessageBox(L"Hе могу загрузить DLL",L"Ошибка",MB_OK);

	CreateInstance=(CREATEFUNCPTR)GetProcAddress(hComponent,"CreateInstance");
	if (CreateInstance == NULL)
		Application->MessageBox(L"Не могу найти CreateInstance",L"Ошибка",MB_OK);

	MeasOutSG=0;
	MeasInSG=0;

	Ini = new TIniFile(Globals::IniFileName);
    Size=_Size;
	SizeBufDAC=0;
	SizeBufADC=0;
	Index=0;
	try
	{
		Base=new ULONG[Size];        //выделяем память ~100мб
	}
	catch (...)
	{
		Application->MessageBox(L"Не удалось выделить память под буффер",L"Ошибка",MB_OK);
	}
	FlStart=false;
}
//------------------------------------------------------------------------------
LCard791::~LCard791()
{
					 //Освобожджение памяти
	CreateInstance=NULL;
	delete[] Base;
	Base=NULL;
	delete Ini;
}
//------------------------------------------------------------------------------
void LCard791::ClearBuffer()
{
	ZeroMemory( Base,sizeof(Base) );
	Index=0;
}
//------------------------------------------------------------------------------
void LCard791::FindCard(void)
{
//	Ini->EraseSection("CardNames");
	IDaqLDevice* pI=NULL;
	int k=0;
	for(int i=0; i<10 ;i++)
	{
			if(pI!=NULL)
			{
				pI->CloseLDevice();
				pI->Release();
				pI=NULL;
			}
			pIUnknown = CreateInstance((ULONG)i);
			if(pIUnknown==NULL)
				continue;
			HRESULT hr = pIUnknown->QueryInterface(IID_ILDEV,(void**)&pI);
			pIUnknown->Release();
			if(!SUCCEEDED(hr))
			{
				pI=NULL;
				pIUnknown->Release();
				continue;
			}
			if(pI->OpenLDevice()==INVALID_HANDLE_VALUE)
				continue;
			SLOT_PAR sl;
			if(pI->GetSlotParam(&sl)!=L_SUCCESS)
				continue;
			if(sl.BoardType!=L791)
				continue;

			if(pI->ReadPlataDescr(&pd)!=L_SUCCESS)
				continue;
			AnsiString serial;
			serial=pd.SerNum;
			IniWrite(serial,k);
			k++;
	}
	if(pI!=NULL)
	{
		pI->CloseLDevice();
		pI->Release();
		pI=NULL;
	}

}
//------------------------------------------------------------------------------
bool LCard791::Initilalize(UnicodeString SerNum,bool _isDiff)
{   //инициализация плат и загрузка .DLL

	isDiff=_isDiff;

   IDaqLDevice* pI=(IDaqLDevice*)pI_void;

	//ищем платы
	bool isFind=false;
	for(int i=0; i<10 ;i++)
	{
		pIUnknown = CreateInstance((ULONG)i);
		if(pIUnknown==NULL)
			continue;
		HRESULT hr = pIUnknown->QueryInterface(IID_ILDEV,(void**)&pI);
		pIUnknown->Release();
		if(!SUCCEEDED(hr))
		{
			pI=NULL;
			pIUnknown->Release();
			continue;
		}
		if(pI->OpenLDevice()==INVALID_HANDLE_VALUE)
			continue;
		SLOT_PAR sl;
		if(pI->GetSlotParam(&sl)!=L_SUCCESS)
			continue;
		if(sl.BoardType!=L791)
			continue;
		PLATA_DESCR_L791 pd;
		if(pI->ReadPlataDescr(&pd)!=L_SUCCESS)
			continue;
		AnsiString serial;
		serial=pd.SerNum;
		if(serial!=SerNum)
			continue;
		isFind=true;
		break;
	}
	if (!isFind)
	{
		if(pI!=NULL)
				{
					pI->CloseLDevice();
					pI->Release();
				}
				Application->MessageBox(L"Не нашли плату L791 c 0 по 9 слот",L"Ошибка",MB_OK);
				return false;
	} else
		{
			pI_void=pI;
			return true;
		}
}
//---------------------------------------------------------------------------
void LCard791::IniWrite(UnicodeString Name, int n)         //пишет имя платы в Ini файл
{
	Ini->WriteString("CardNames","LCard"+(UnicodeString)n,Name);
}

//---------------------------------------------------------------------------
#if 0
void LCard791::InitADC(int Channel,double dKadr,UnicodeString Type,int Mode)
{
//задаем параметры АЦП
	ADC_PAR ap;
	ap.t2.s_Type=L_ADC_PARAM;
	ap.t2.AutoInit=1;
	ap.t2.dRate=Channel*Ini->ReadInteger(Type,"Rate",1);
	ap.t2.dKadr=dKadr;
	ap.t2.SynchroType=0;
	ap.t2.SynchroSrc= 0;
	ap.t2.NCh=Channel;
	//Задаем настройки для каждого канала
	if (Mode==0)                                          //Дефектоскопия
		for (int i = 0; i < Channel; i++)
			{
				int a=Ini->ReadInteger(Type,"Range"+IntToStr(i),i);
				int b=Ini->ReadInteger(Type,"Number"+IntToStr(i),i);
				ap.t2.Chn[i]=SetChn(a , b );
			}
	else                            //группа прочности
	if(Mode==1)
		{
			ap.t2.Chn[2]=SetChn(1,Ini->ReadInteger("OtherSettings","SensorSG",14));	     //датчик датчик ГП
			ap.t2.Chn[3]=SetChn(1,Ini->ReadInteger("OtherSettings","SensorCurr",15));   //датчик тока
		}
	else
	{
		for (int i = 0; i < Channel; i++)
			{
				int a=Ini->ReadInteger(Type,"Range"+IntToStr(i),i);
				int b=Ini->ReadInteger(Type,"Number"+IntToStr(i),i);
				ap.t2.Chn[i]=SetChn(a , b );
				int aaa=Ini->ReadInteger("OtherSettings","SensorSG",15);
				int bbb=Ini->ReadInteger("OtherSettings","SensorCurr",14);
				ap.t2.Chn[14]=SetChn(1,aaa);	  //датчик ГП
				ap.t2.Chn[15]=SetChn(1,bbb);   //датчик тока
			}
	}
	ap.t2.FIFO=FIFO;
	ap.t2.IrqStep=IrqStep;
	ap.t2.Pages=pages;
	ap.t2.IrqEna=0;
	ap.t2.AdcEna=1;

	IDaqLDevice* pI=(IDaqLDevice*)pI_void;

	pI->FillDAQparameters(&ap.t2);
	pI->RequestBufferStream(&SizeBufADC,L_STREAM_ADC);
	pI->SetParametersStream(&ap.t2, &SizeBufADC, (void**)&data, (void**)&sync,L_STREAM_ADC);
}
#else
void LCard791::InitADC(int Channel,double dKadr,UnicodeString Type,int Mode)
{
//задаем параметры АЦП
    Channel = 18;
	ADC_PAR ap;
	ap.t2.s_Type=L_ADC_PARAM;
	ap.t2.AutoInit=1;
	ap.t2.dRate=Channel*Ini->ReadInteger(Type,"Rate",1);
 //  ap.t2.dRate=18*Ini->ReadInteger(Type,"Rate",1);
	ap.t2.dKadr=dKadr;
	ap.t2.SynchroType=0;
	ap.t2.SynchroSrc= 0;
	ap.t2.NCh=Channel;
	//Задаем настройки для каждого канала
	if (Mode==0)                                          //Дефектоскопия
	/*
		for (int i = 0; i < Channel; i++)
			{
				int a=Ini->ReadInteger(Type,"Range"+IntToStr(i),i);
				int b=Ini->ReadInteger(Type,"Number"+IntToStr(i),i);
				ap.t2.Chn[i]=SetChn(a , b );
			}
			*/
		  for (int i = 0; i < 12; i++)
			{
				int a=Ini->ReadInteger("PP","Range"+IntToStr(i),i);
				int b=Ini->ReadInteger("PP","Number"+IntToStr(i),i);
				ap.t2.Chn[i]=SetChn(a , b );
			}
		  for (int k = 12; k < 12 + 4; k++)
			{
				int i = k - 12;
				int a=Ini->ReadInteger("PR","Range"+IntToStr(i),i);
				int b=Ini->ReadInteger("PR","Number"+IntToStr(i),i);
				ap.t2.Chn[k]=SetChn(a , b );
			}
		//  for (int i = 0; i < 2; i++)
			{
				int a=Ini->ReadInteger("OtherSettings","SensorSG",20);
				ap.t2.Chn[16]=SetChn(1, a);
				a=Ini->ReadInteger("OtherSettings", "SensorCurr",21);
				ap.t2.Chn[17]=SetChn(1, a );
			}
			/*
	else                            //группа прочности
	if(Mode==1)
		{
			ap.t2.Chn[2]=SetChn(1,Ini->ReadInteger("OtherSettings","SensorSG",14));	     //датчик датчик ГП
			ap.t2.Chn[3]=SetChn(1,Ini->ReadInteger("OtherSettings","SensorCurr",15));   //датчик тока
		}
	else
	{
		for (int i = 0; i < Channel; i++)
			{
				int a=Ini->ReadInteger(Type,"Range"+IntToStr(i),i);
				int b=Ini->ReadInteger(Type,"Number"+IntToStr(i),i);
				ap.t2.Chn[i]=SetChn(a , b );
				int aaa=Ini->ReadInteger("OtherSettings","SensorSG",15);
				int bbb=Ini->ReadInteger("OtherSettings","SensorCurr",14);
				ap.t2.Chn[14]=SetChn(1,aaa);	  //датчик ГП
				ap.t2.Chn[15]=SetChn(1,bbb);   //датчик тока
			}
	}
	*/
	ap.t2.FIFO=FIFO;
	ap.t2.IrqStep=IrqStep;
	ap.t2.Pages=pages;
	ap.t2.IrqEna=0;
	ap.t2.AdcEna=1;

	IDaqLDevice* pI=(IDaqLDevice*)pI_void;

	pI->FillDAQparameters(&ap.t2);
	pI->RequestBufferStream(&SizeBufADC,L_STREAM_ADC);
	pI->SetParametersStream(&ap.t2, &SizeBufADC, (void**)&data, (void**)&sync,L_STREAM_ADC);
}
#endif
//--------------------------------------------------------------------------------------
ULONG LCard791::SetChn(int _gain,int _channel) //Функция для установки входного напряжения на канал
{
		ULONG ret;
		if(isDiff)
				ret=_channel&15;
		else
		{
				ret=_channel&31;
				ret|=1<<5;
		}
		int gain;
		switch(_gain)
		{
		case 1:
				gain=0;
				break;
		case 2:
				gain=1;
				break;
		case 4:
				gain=2;
				break;
		case 8:
				gain=3;
				break;
		case 16:
				gain=4;
				break;
		case 32:
				gain=5;
				break;
		case 64:
				gain=6;
				break;
		case 128:
				gain=7;
				break;
		default:
				gain=0;
		}
		ret|=gain<<6;
		return(ret);
}
//---------------------------------------------------------------------------
void LCard791::InitDAC(bool Write)
{                 //Настриваем ЦАП
	DAC_PAR dacPar;
	dacPar.t2.s_Type = L_DAC_PARAM;
	dacPar.t2.AutoInit=0;
	dacPar.t2.dRate=2;
	dacPar.t2.FIFO=1024;
	dacPar.t2.IrqStep=1024;
	dacPar.t2.Pages=128;
	dacPar.t2.IrqEna=0;
	dacPar.t2.DacEna=1;
	IDaqLDevice* pI=(IDaqLDevice*)pI_void;

	pI->FillDAQparameters(&dacPar.t2);

	pI->RequestBufferStream(&SizeBufDAC, L_STREAM_DAC);
	pI->SetParametersStream(&dacPar.t2,&SizeBufDAC, (void **)&data1, (void **)&sync1,L_STREAM_DAC);

	if (Write)
		for(ULONG i=0;i<128*1024;i++)
			data1[i] = ((USHORT)(2048.0*sin((10.0*3.1415*i/1024.0)))&0xFFF) | (1<<12);

}


//---------------------------------------------------------------------------

bool LCard791::Close()
{                  //Закрытие платы
	IDaqLDevice* pI=(IDaqLDevice*)pI_void;
	if (pI->CloseLDevice()==L_ERROR)
		return false;
	else
	{
		pI->Release();
		return true;
	}
}
//------------------------------------------------------------------------------

#pragma optimize ("", off)
#if 0
void LCard791::Run(void)        //поток сбора
{
	Num=0;                   //Cчетчик собранных частей (1/16 половинки)
	int halfbuffer=(IrqStep*pages) / 2;      //вот столько измерений в половинке
	part = halfbuffer/256;                //а вот столько в 1части    //было 64
	while (collect)
		{
			if (sync[I_ADC_PCI_COUNT_L791]<=halfbuffer)                         //указатель в 1й части буфера
			 {
				int q=0;                                            //счетчик собраных частей внутри одной половинки
				while ((sync[I_ADC_PCI_COUNT_L791]<=halfbuffer)&&collect)
				{
					if(sync[I_ADC_PCI_COUNT_L791] > (q+1)*part)     //если частичка уже собралась
					{
						memcpy(&Base[part*Num],&data[q*part],sizeof(ULONG)*part);//то копируем ее в большой буфер
						q++;
						Num++;
					}
				  //	else
					   //	TThread::CurrentThread->Sleep(5);

				}
			//	 Sleep(10);

			 }
			 else    //указатель во второй части буфера
			 {
				 int q=0;
				 while ((sync[I_ADC_PCI_COUNT_L791]>halfbuffer)&&collect)
				 {
					if(sync[I_ADC_PCI_COUNT_L791] > halfbuffer+((q+1)*part) )   //если частичка  уже собралась
					{
						memcpy(&Base[part*Num], &data[halfbuffer+(q*part)], sizeof(ULONG)*part);//то копируем ее в большой буфер
						q++;
						Num++;
					}

				//	else
					 //	TThread::CurrentThread->Sleep(5);
				 }
			  //		 dprint("1 ");
					//   Sleep(10);
			 }
		 //	 Sleep(0);
		}
	Index=part*Num;              //всего данных
	ProtocolForm->SendToProtocol("Данных собрано "+UnicodeString(Index));
}
#else

void LCard791::Run(void)        //поток сбора
{
	Num=0;                   //Cчетчик собранных частей (1/16 половинки)
	int halfbuffer=(IrqStep*pages)/ 2;      //вот столько измерений в половинке
	frameBuffer = halfbuffer;
  //	part = halfbuffer/256;                //а вот столько в 1части    //было 64
	bool b = sync[I_ADC_PCI_COUNT_L791] < halfbuffer;
	int offs = 0;
	countTimeOffs = 0;
	timeOffs[0] = timeGetTime();
	while (collect)
	{
	   bool bb = sync[I_ADC_PCI_COUNT_L791] < halfbuffer;                         //указатель в 1й части буфера
	   if(b != bb)
	   {
	  // /*
		  if(bb)
		  {
			memcpy(&Base[halfbuffer * offs], &data[halfbuffer], sizeof(ULONG)*halfbuffer);
		  }
		  else
		  {
			 memcpy(&Base[halfbuffer * offs], data, sizeof(ULONG)*halfbuffer);
		  }
		//  */
		  //////////nodo отладка тест распределения
	   //	   unsigned long *x = &Base[halfbuffer * offs];
	 //	   for(int i = 0; i < halfbuffer; ++i)
	 //	   {
	 //		  x[i] = offs * 82 * 5;
	  //	   }
		  //test end
		  ++offs;
		  timeOffs[offs] = timeGetTime();
		  b = bb;
	   }
	   Sleep(10);
	}
	Index=halfbuffer*offs;              //всего данных
	ProtocolForm->SendToProtocol("Данных собрано "+UnicodeString(Index));
	countTimeOffs = offs + 1;
	dprint("countTimeOffs count frames  %d\n", countTimeOffs);
	/*
	FILE *f = fopen("c:\data.tmp", "r");
	if(NULL != f)
	{
	   fwrite(Base, sizeof(unsigned long), Index, f);
	   fclose(f);
	   dprint("file saved ok\n");
	}
	else
	{
	dprint("fale not saved\n");
	}
	*/
}
#endif
#pragma optimize ("", on)
//---------------------------------------------------------------------------
DWORD WINAPI LCard791::Thread(PVOID p)
{
	((LCard791 *)p)->Run();
	return (0);
}
//---------------------------------------------------------------------------

bool LCard791::Start()
{             //Запускаем сбор в буфер
	collect=true;
	FlStart=true;                       //запустили сбор
	IDaqLDevice* pI=(IDaqLDevice*)pI_void;
	if(pI->InitStartLDevice()==L_ERROR)
		return false;
	else
	{
		ULONG status;
		Index=0;
		status=pI->StartLDevice();
		if (status==L_ERROR)
		{
			Application->MessageBox(L"Не удалось запустить сбор данных в буффер",L"Ошибка",MB_OK);
			return false;
		}
		else
		{
			hThread=CreateThread(0,0x2000,LCard791::Thread,this,0,&Tid);
			bool thp = SetThreadPriority( hThread, -15);
			if (!thp)
				Application->MessageBox(L"Не удалось задать приоритет потока L791",L"Ошибка",MB_OK);
			return true;
		}
	}
}
//------------------------------------------------------------------------------
bool LCard791::Stop()         //Останавливаем сбор
{
	if(FlStart)                   //если сбор запускали то остановим
	{
		ULONG status;
		IDaqLDevice* pI=(IDaqLDevice*)pI_void;
		pI->StopLDevice();

		ProtocolForm->SendToProtocol("Index= "+UnicodeString(Index) );

		collect=false;
		WaitForSingleObject(hThread,100);       //Ждем завершения потока
		CloseHandle(hThread);                        //Закрываем поток
		FlStart=false;

		return true;
	}
	else
		return false;
}
//------------------------------------------------------------------------------
bool LCard791::DacAsyncOutCode(int16_t u1Code, int16_t ch)
{
	ASYNC_PAR pp;

	pp.s_Type = L_ASYNC_DAC_OUT;
	pp.Mode = 0;
	pp.Chn[0] = 1;
	pp.Chn[1] = 1;
	pp.Data[ch] = u1Code;

	IDaqLDevice* pI=(IDaqLDevice*)pI_void;

	if (pI->IoAsync(&pp) != L_SUCCESS)
	{
		Application->MessageBox(L"Не удалось выставить напряжение ",L"Ошибка",MB_OK);
		return false;
	}
	return true;
}
//------------------------------------------------------------------------------
//Выставляем напряжение на ЦАП
bool LCard791::SetDAC(double Volt, int channel)
{
	if ((Volt >= -5) && (Volt <=5))
	{
		if ((channel < 0) || (channel >= 1))
			return false;

		USHORT u1 = Volt / 5.0 * 0x7ff;
		return DacAsyncOutCode(u1,channel);
	}
	else
		return false;
}

//------------------------------------------------------------------------------
float LCard791::GetMagnetic()
{
	int Ch=Ini->ReadInteger("OtherSettings","Magnetic",31);
	IDaqLDevice* pI=(IDaqLDevice*)pI_void;
									//Устанавливаем параметры АЦП для одиночного сбора
	pI->InitStartLDevice();
	ASYNC_PAR pp;
	pp.s_Type=L_ASYNC_ADC_INP;
	pp.Chn[0]=SetChn(1,Ch);

	if(pI->IoAsync(&pp)!=L_SUCCESS)
		{
			Application->MessageBoxW(L"IoAsync выдала ошибку",L"Error",MB_OK);
			pI->StopLDevice();
			delete pI;
		}
	else
		{
			short Code=(short)(pp.Data[0]&0xFFFF);
			Code=(Code+pd.KoefADC[0])*pd.KoefADC[8];   //Коэффициенты корректировки значения АЦП
			float V=Code;
			V*=10;
			V/=8192;

			return V;
		}
}
int LCard791::getTemperatureForSG()
{
	int Ch=Ini->ReadInteger("OtherSettings", "channelTempSG",5);
	IDaqLDevice* pI=(IDaqLDevice*)pI_void;
									//Устанавливаем параметры АЦП для одиночного сбора
	pI->InitStartLDevice();
	ASYNC_PAR pp;
	pp.s_Type=L_ASYNC_ADC_INP;
	pp.Chn[0]=SetChn(1,Ch);

	if(pI->IoAsync(&pp)!=L_SUCCESS)
	{
		Application->MessageBoxW(L"IoAsync выдала ошибку",L"Error",MB_OK);
		pI->StopLDevice();
		delete pI;
	}
	else
	{
		short Code=(short)(pp.Data[0]&0xFFFF);
		Code=(Code+pd.KoefADC[0])*pd.KoefADC[8];   //Коэффициенты корректировки значения АЦП

		return Code;
	}
}
//---------------------------------------------------------------------------
//Возвращает значение в Вольтах, параметр номер датчика 0 или 1
float LCard791::GetTemperature(int Num)
{
	int Ch=0;
	switch (Num)
	{
		case 0:	{
					Ch=Ini->ReadInteger("OtherSettings","Temp1",29);
					break;
				}
		case 1: {
					Ch=Ini->ReadInteger("OtherSettings","Temp2",30);
					break;
				}
		default:
			 Application->MessageBoxW(L"Запрошен не существующий датчик температуры",L"Ошибка",MB_OK);
	}
   if (Ch!=0)
   {                                   			//Устанавливаем параметры АЦП для одиночного сбора
		IDaqLDevice* pI=(IDaqLDevice*)pI_void;
		pI->InitStartLDevice();
		ASYNC_PAR pp;
		pp.s_Type=L_ASYNC_ADC_INP;
		pp.Chn[0]=SetChn(1,Ch);
		if(pI->IoAsync(&pp)!=L_SUCCESS)
			{
				Application->MessageBoxW(L"Не удалось получить данные по температуре",L"Error",MB_OK);
				pI->StopLDevice();
				delete pI;
			}
		short Code=short(pp.Data[0]&0xFFFF);
		Code=(Code+pd.KoefADC[0])*pd.KoefADC[8];   //Коэффициенты корректировки значения АЦП
		float V=Code;
		V*=10;
		V/=8192;

		return V;
   }
}
//------------------------------------------------------------------------------
bool LCard791::isOkTemp(UnicodeString type)
{
	//Функция возвращает True если ситуация критическая

	bool fl1=true;
	double value;
	if( (type == "1") || (type == "2"))
	{
		value=GetTemperature(StrToInt(type)-1);
    }
	else
	{
		value=GetMagnetic();
		dprint("GetMagnetic %f\n", value);
	}
	int temp=Ini->ReadInteger("OtherSettings","isVMore"+type,4);

	//смотрим условия указаные в настройках
	String s = Ini->ReadString("OtherSettings","isMore"+type,"Fail");
	if(s  == "Больше" )
		if( value > temp )
			fl1=true;
		else
			fl1=false;
	else
		if( value < temp )
			fl1=true;
		else
			fl1=false;
	return fl1;

}
//----------------------------------------------------------------------------
//Запоминает сколько данных было собранно на момент входа трубы в МР
void LCard791::SetMeasInSG()
{
	MeasInSG=Num;
}
//----------------------------------------------------------------------------
//Запоминает сколько данных было собранно на момент
//выхода трубы из модуля размагничевания
void LCard791::SetMeasOutSG()
{
	MeasOutSG=Num;
}
//-----------------------------------------------------------------------------
long LCard791::GetNum()
{
	return Num;
}
//-----------------------------------------------------------------------------
//Выделяет из массива в вектор, значения по ГП, и токового сигнала
vector<int> LCard791::PrepareSGData( bool Test)
{
/*
	vector<int> SG;
	//для Когалыма ненадо отсекать мусор, его там просто нету т.к. попер.модуль стоит последним
	int begin;
	int trash=(Num-MeasInSG)/8;
	if ( Test )
		begin = 0;
	else
		begin = MeasInSG * part;
//	          отсекаем начало      и		конец трубы

//	for( int i=MeasInSG*part; i < Num*part; i+=16 )
	for( unsigned int i = begin; i < Num * part; i += 16 )
	{
		short qqq=(short)Base[i+14];
		short www=(short)Base[i+15];
		SG.push_back(qqq);
		SG.push_back(www);
	}
	int ppp=SG.size();
	*/
	return PrepareSG::Instance().Do(Base, Index, timeOffs);
}
//------------------------------------------------------------------------------



