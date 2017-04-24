//---------------------------------------------------------------------------
#ifndef LCARD791H
#define LCARD791H
#include "intsafe.h"
//#include <windows.h>
#include "include\ioctl.h"
#include "include\ifc_ldev.h"
#include "include\create.h"
#include "include\791cmd.h"
#include "IniFiles.hpp"
#include "Math.h"
#include "clustering.h"



//---------------------------------------------------------------------------
class LCard791
{

public:
	int Size;                       		//размер буфера куда будем копировать данные
	ULONG Index; 							//индекс для большого буфера
	ULONG *Base;					  		//сюда пишем данные с карты
	bool collect;                    	    //Разрешение/запрет  на сбор данных
	ULONG Number;                           //Кол-во собранных половинок
	int Mode;                               //Режим работы АЦП : ГП+Дефектоскопия/Дефектоскопия/ГП
	ULONG Num;                              //кол-во собранных частей "part"
	int part;                               //столько измерений в одной части

	int countTimeOffs;
	int frameBuffer;
	unsigned timeOffs[512];

	LCard791(int size);                     //Size-задает память под буфер АЦП
	~LCard791();                            //Деструктор О_о
	void FindCard(void);					//ищет платы и записывает их в ини файл
	bool Initilalize(UnicodeString SerNum,
	bool isDiff);          		  			//инициализация плат и загрузка .DLL
	bool Close();                		    //Закрытие платы
	bool Start();                		    //Запускаем сбор в буффер
	bool Stop();                   		    //Останавливаем сбор
	bool SetDAC(double Volt, int channel);  //Выстовляем напряжение на ЦАП
	void InitDAC(bool Write);               //Инициализация ЦАП
	void InitADC(int Channel, double dKadr,
	UnicodeString Type,int Mode);			//Инициализация АЦП (кол-во используемых каналов,
																			//частота,задержка между кадрами,тип АЦП продольный/поперечный
	float GetMagnetic();                    //Возвращает значение датчика магнитного поля в В.
	int getTemperatureForSG();
	float GetTemperature(int Num);          //Получает номер датчика температуры 0 или 1, возвращает значение температуры в В.
	void ClearBuffer();                     //Функция очищения буффера для многократного сбора данных
	bool isOkTemp(UnicodeString type);      //возвращает True если аварийная ситуация,False если все заебок 1-t1,2-t2,MG-магнитное поле
	void SetMeasInSG();                     //Запоминает сколько было измерений когда труба вошла в МР
	void SetMeasOutSG();                    //Запоминает сколько было измерений когда труба вышла из МР
	long GetNum();							//Возвращает кол-во собранных данных на данный момент
	vector <int> PrepareSGData(bool Test);	//возвращает вектор значений по ГП
	inline bool getStarted() {return FlStart;}
private:

	long MeasOutSG;                         //Cколько было измерений когда труба вышла из МР
	long MeasInSG;                          //Cколько было измерений когда труба вошла в МР
	bool FlStart;                           //Плата стартовала сбор
	void* pI_void;                          //Handle платы
	bool isDiff;                            //Режим инициализации платы, дифференциальный/с общей землей
	int fl1,k;							    //Cчетчики для чтения потока
	TIniFile *Ini;                          //Файл настроек
	ULONG Tid;                              //Указатель на поток
	PLATA_DESCR_L791 pd;
	LUnknown* pIUnknown;
	ULONG *data1,*sync1;            	    //Для ЦАП
	ULONG *sync,*data;              	    //Для АЦП
	HANDLE hThread;                  	    //Указатель на поток для сбора данных
	ULONG SizeBufDAC;                       //Память под ЦАП
	ULONG SizeBufADC;                   	//Память под АЦП

	ULONG LCard791::SetChn(int _gain,int _channel);      //Устанавливает входной диапазон на конкретный канал
	void IniWrite(UnicodeString Name,int n); 			 //Пишет имя платы в Ini файл
	void Run(void);                       				 //Сбор данных с АЦП
	static DWORD WINAPI Thread(PVOID p);
	bool DacAsyncOutCode(int16_t u1Code, int16_t ch);    //вспомогательная функция для SetDAC

};
#endif



