//---------------------------------------------------------------------------


#pragma hdrstop

#include "ComPort.h"
#include "Main.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

MyComPort *MyCom;
UINT TubeNumTimeout = 4000;		// ожидание посылки с номером
UINT ResultTimeout = 5000;		// ожидание ответа после посылки с результатом
UINT TestTimeout = 3000;			// ожидание тестовой посылки
UINT SolidGroupTimeout = 5000;	// ожидание группы прочности
//---------------------------------------------------------------------------
MyComPort::MyComPort()
{
// создаем событие
	DataReceived = new TEvent( (_SECURITY_ATTRIBUTES *)NULL, true, false, String( "rxEvent" ), false );
	DataReceived->ResetEvent();
// создаем COM порт
	CommPort = new TCommPort(MainForm);
	SetComPortSettings();
	CommPort->OnDataReceived = MyComPortDataReceived;
	CommPort->Open = true;
	ASUMode = 2;	// обмен по 2 протоколу - Пыть-Ях (по образцу Талинки, Приютово)
	abonent = 2;	// номер абонента АСУ в сети
	length = 0;		// длина заполняется при передаче
	function = 0;	// аналогично
	plant_cut = 0;
}
//---------------------------------------------------------------------------
MyComPort::~MyComPort()
{
	CommPort->Open = false;
	delete CommPort;
	delete DataReceived;
}
//---------------------------------------------------------------------------
//------настройки соединения-------------------------------------------------
void MyComPort::SetComPortSettings()
{
	PortName = "COM1";
	BaudRate = 19200;
	DataBits = 8;
	StopBits = 1;
	Parity = 0;

	CommPort->ComNumber = 1;
	CommPort->BaudRate = cbr19200;
	CommPort->DataBits = 8;
	CommPort->Parity = paNone;
	CommPort->StopBits = sb1_0;
}
//---------------------------------------------------------------------------
//------рассчет контрольной суммы по 1 протоколу (Стрежевой)-----------------
unsigned char MyComPort::countCRC16_1(unsigned char* buff, unsigned int size)
{
	unsigned long sum = 0;
	for (int i = 0; i < size; i++)
	{
		sum +=buff[i];
	}
	sum = sum & 0xFF;
	return sum;
}
//---------------------------------------------------------------------------
//------рассчет контрольной суммы по 2 протоколу (Пыть-Ях)-------------------
unsigned short MyComPort::countCRC16_2(unsigned char* bufData, unsigned int sizeData)
{
	unsigned char j;
	unsigned int __CRC;
	unsigned int TmpCRC;
	unsigned int i;

	__CRC = 0x7FFFFFFF;

	for(i=0; i < sizeData; i++)
	{
		__CRC = __CRC ^ bufData[i];
		for(j=0; j < 8; j++)
		{
			int testbit = __CRC & 0x0001;
			__CRC = (__CRC >> 1 ) & 0x7FFF;
			if( testbit )
				__CRC ^= 0xA001;
		}
	}
	//__CRC = __CRC >> 16;

	return __CRC;
}
//---------------------------------------------------------------------------
//------общая функция запроса номера трубы-----------------------------------
long MyComPort::GetTubeNumber()
{
	if (ASUMode == 1)
		return GetTubeNumber_1();
	else if (ASUMode == 2)
		return GetTubeNumber_2();
}
//---------------------------------------------------------------------------
//----общая функция посылки результата---------------------------------------
/*
bool MyComPort::SendResultToASU(short res, short c1, short c2, short len , SolidGroups::Group sgg)
{
	if (ASUMode == 1)
		return SendResultToASU_1(res, c1, c2, len , sgg);
	else if (ASUMode == 2)
		return SendResultToASU_2(res, c1, c2, len , sgg);
}
*/
//---------------------------------------------------------------------------
//-----запрос номера трубы по 1 протоколу (Стрежевой)------------------------
long MyComPort::GetTubeNumber_1()
{
	MainForm->StatusBarTop->Panels->Items[2]->Text = "Ждем номера трубы ";
	// делаем запрос номера трубы (отправка данных)
	CommPort->PutChar('?');
	bool ok = false;
	// ждем ответа - события
	for( DWORD tt = GetTickCount(); GetTickCount() - tt < TubeNumTimeout; Sleep(10) )
	{
		Application->ProcessMessages();
		if( DataReceived->WaitFor( 200 ) == wrSignaled )
		{
			DataReceived->ResetEvent();
			ok = true;
			break;
		}
	}

	if ( !ok )
	{
		MainForm->StatusBarTop->Panels->Items[2]->Text = "Не дождались посылки с номером трубы!";
		CommPort->PutChar('\25');
		return -1;
	}
	else
	{
		unsigned char* b;
		b = &data[1];
		unsigned char check_sum = countCRC16_1( b , 15 );		// рассчетная контр сумма
		unsigned char got_sum = data[16];					// присланная контр сумма
		if ( check_sum != got_sum)
		{
			MainForm->StatusBarTop->Panels->Items[2]->Text = "Неправильная контрольная сумма!";
			CommPort->PutChar('\25');
			return -1;
		}
		else
		{
			String *str = new String((char*)data);
			String rs = (*str).SubString(2,5);
			unsigned long numb = StrToInt64(rs);
			MainForm->StatusBarTop->Panels->Items[2]->Text = "Номер трубы: "+ IntToStr((int)numb);
			CommPort->PutChar('\6');
			return numb;
		}
	}
}
//---------------------------------------------------------------------------
// послыает результат в АСУ: res = признак годно/брак, c1 = рез 1, с2 = рез 2, len = длина в зонах, sgg - группа прочности
bool MyComPort::SendResultToASU(short res, short c1, short c2, short len , SolidGroups::Group sgg)
{
	MainForm->StatusBarTop->Panels->Items[2]->Text = "Ждем передачи результата ";
	// делаем запрос сеанса обмена 2 (отправка данных)
	CommPort->PutChar('\5');
	bool ok = false;
	// ждем ответа - события
	for( DWORD tt = GetTickCount(); GetTickCount() - tt < ResultTimeout; Sleep(10) )
	{
		Application->ProcessMessages();
		if( DataReceived->WaitFor( 200 ) == wrSignaled )
		{
			DataReceived->ResetEvent();
			ok = true;
			break;
		}
	}
	if ( !ok )
	{
		MainForm->StatusBarTop->Panels->Items[2]->Text = "Не дождались посылки \"ACK\" от АСУ!";
		return false;
	}
	else
	{
		if ( data_size == 1 && data[0] == 21 )
		{
			MainForm->StatusBarTop->Panels->Items[2]->Text = "АСУ не готов к приему результата!";
			return false;
		}
		else if ( data_size != 1)
		{
			MainForm->StatusBarTop->Panels->Items[2]->Text = "Неверная длина посылки!";
			return false;
		}
		else if ( data_size == 1 && data[0] == 6 )
		{
			// формируем посылку
			unsigned char sending[18] = { 2 ,0,0,0,0,0,0,0,0,0,0,0,0,0 ,48,48,0, 3 };
			// байты 1-5 = номер трубы
			String tubnum = IntToStr((__int64) Globals::tube_number);
			while ( tubnum.Length() < 5 )
				tubnum = "0" + tubnum;
			for (int i=1; i <= 5; i++)
				sending[i] = (char)tubnum[ i ];
			// байт 6 - результат
			sending[6] = res + 48;
			// байт 7 - группа прочности
			if ( sgg.value() == SolidGroups::Group::sgUnknown )
				sgg = SolidGroups::Group::sgK;
			sending[7] = sgg.GroupToEnglish();
			// байты 8,9 - рез1
			String temp = IntToStr(c1);
			while ( temp.Length() < 2 )
				temp = "0" + temp;
			for (int i=0; i <= 1; i++)
				sending[i + 8] = (char)temp[ i+1 ];
			// байты 10,11 - рез2
			temp = IntToStr(c2);
			while ( temp.Length() < 2 )
				temp = "0" + temp;
			for (int i=0; i <= 1; i++)
				sending[i + 10] = (char)temp[ i+1 ];
			// байты 12,13 - длина трубы в зонах
			temp = IntToStr( len );
			while ( temp.Length() < 2 )
				temp = "0" + temp;
			for (int i=0; i <= 1; i++)
				sending[i + 12] = (char)temp[ i+1 ];
			// байт 16 - контрольная сумма
			unsigned char* b;
			b = &sending[1];
			sending[16] = countCRC16_1( b , 15);
			CommPort->PutBlock( sending , sizeof (sending) );
			// ждем ответа - события
			ok = false;
			for( DWORD tt = GetTickCount(); GetTickCount() - tt < ResultTimeout; Sleep(10) )
			{
				Application->ProcessMessages();
				if( DataReceived->WaitFor( 200 ) == wrSignaled )
				{
					DataReceived->ResetEvent();
					ok = true;
					break;
				}
			}
			if ( !ok )
			{
				MainForm->StatusBarTop->Panels->Items[2]->Text = "Не дождались посылки \"ACK\"от АСУ!";
				return false;
			}
			else
			{
				if ( data_size == 1 && data[0] == 21 )
				{
					MainForm->StatusBarTop->Panels->Items[2]->Text = "АСУ принял результат с ошибкой!";
					return false;
				}
				else if ( data_size != 1)
				{
					MainForm->StatusBarTop->Panels->Items[2]->Text = "Неверная длина посылки от АСУ!";
					return false;
				}
				else if ( data_size == 1 && data[0] == 6 )
				{
					MainForm->StatusBarTop->Panels->Items[2]->Text = "Результат отправлен успешно";
					return true;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
//------функция получения посылки группы прочности (Нягань НРС)--------------
SolidGroups::Tube MyComPort::GetSolidGroup()
{
	using namespace SolidGroups;
	SolidGroups::Tube tubeSG;	// группа прочности трубы
	tubeSG = SolidGroups::Tube();
	short measures[8] = {0};
	vector<double>t;
	t.resize(8);

	CommPort->PutChar('0');
	bool ok = false;
	// ждем ответа - события
	for( DWORD tt = GetTickCount(); GetTickCount() - tt < SolidGroupTimeout; Sleep(10) )
	{
		Application->ProcessMessages();
		if( DataReceived->WaitFor( 200 ) == wrSignaled )
		{
			DataReceived->ResetEvent();
			ok = true;
			break;
		}
	}

	if (!ok)
	{
		MainForm->StatusBarTop->Panels->Items[2]->Text = "Не дождались посылки с ГП!";
		return tubeSG;
	}

	for (int i = 0; i < 8; i++)
	{
		measures[i] = (int16_t)(data[2 * i + 1] << 8) | (data[2 * i]);
		t[i]        = (double) measures[i];
	}
	CommPort->FlushInBuffer();

	tubeSG.setCoordinates(t);
	tubeSG.setTubeType(TubeType(Globals::current_diameter));
	return tubeSG;
}
//---------------------------------------------------------------------------
//-----событие: получение данных по COM порту--------------------------------
void __fastcall MyComPort::MyComPortDataReceived(TObject *Sender, DWORD Count)
{
	TThread::CurrentThread->Sleep(200);
//	если группа прочности по COM порту (Нягань НРС)
	if ( !SystemConst::ComWithASU && SystemConst::SolidGroupSource == sgsCOMPort )
	{
		if (CommPort->InBuffUsed == 16)
		{
			data_size = CommPort->GetBlock( data, CommPort->InBuffUsed);
			DataReceived->SetEvent();
		}
	}
// если обмен с цехом по COM порту  (Стрежевой)
	else if ( SystemConst::ComWithASU && SystemConst::SolidGroupSource != sgsCOMPort )
	{
		if (ASUMode == 1)		// Стрежевой - Пионерный
		{
			// номер трубы
			if (CommPort->InBuffUsed == 18)
			{
				data_size = CommPort->GetBlock( data, CommPort->InBuffUsed);
				DataReceived->SetEvent();
			}
			// односимвольные ответы АСУ о готовности
			if (CommPort->InBuffUsed == 1)
			{
				data_size = CommPort->GetBlock( data , CommPort->InBuffUsed);
				DataReceived->SetEvent();
			}
		}
		else if (ASUMode == 2)		// Пыть-Ях - Приютово
		{
			// номер трубы
			if (CommPort->InBuffUsed == 17)
			{
				data_size = CommPort->GetBlock( data, CommPort->InBuffUsed);
				if (CheckSentMessage())
					DataReceived->SetEvent();
			}
			// пятисимвольные ответы АСУ  - тест и правильный результат
			if (CommPort->InBuffUsed == 5)
			{
				data_size = CommPort->GetBlock( data , CommPort->InBuffUsed);
				if (CheckSentMessage())
					DataReceived->SetEvent();
			}
		}
	}
	else if ( CommPort->InBuffUsed )
	{
		unsigned char buf[50];
		int kol = CommPort->GetBlock(buf, CommPort->InBuffUsed);
		MainForm->StatusBarTop->Panels->Items[2]->Text = "Неправильная посылка: " + IntToStr(kol) + " байт";
		MainForm->StatusBarTop->Refresh();
	}
	CommPort->FlushInBuffer();
}
//---------------------------------------------------------------------------
//----проверка присланной посылки (только 2 протокол)----------------------------------
bool MyComPort::CheckSentMessage()
{
	String text;
	if (data[0] != length)	// можно сделать ожидание другой посылки?
	{
		text = "Неверная длина посылки от АСУ!";
		MainForm->StatusBarTop->Panels->Items[2]->Text = text;
		ProtocolForm->SendToProtocol(text);
		return false;
	}

	if (data[1] != abonent)	// можно сделать ожидание другой посылки?
	{
		text = "Посылка от АСУ не нам!";
		MainForm->StatusBarTop->Panels->Items[2]->Text = text;
		ProtocolForm->SendToProtocol(text);
		return false;
	}

	if (data[2] != function)	// можно сделать ожидание другой посылки?
	{
		text = "Неверный код функции передачи!";
		MainForm->StatusBarTop->Panels->Items[2]->Text = text;
		ProtocolForm->SendToProtocol(text);
		return false;
	}

	unsigned char* b;
	b = &data[0];
	unsigned short check_sum = countCRC16_2( b , length-2 );		// рассчетная контр сумма
	if( data[length-1] != ((check_sum >> 8) & 0x00FF) || data[length-2] != (check_sum & 0x00FF)  )
	{
		text = "Неправильная контрольная сумма!";
		MainForm->StatusBarTop->Panels->Items[2]->Text = text;
		ProtocolForm->SendToProtocol(text);
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
//----тест канала связи (только 2 протокол)----------------------------------
bool MyComPort::TestConnection()
{
	length = 5;		// ожидаемая длина ответа от АСУ
	function = 1;	// функция обмена с АСУ
	MainForm->StatusBarTop->Panels->Items[2]->Text = "Тест связи по COM-порту";
	// делаем запрос по функции 1
	unsigned char test[5];
	test[0] = length;
	test[1] = abonent;
	test[2] = function;

	unsigned short crc16 = countCRC16_2( test, 3 );
	// в пакете сначала идёт младший байт, потом старший
	test[4] = (crc16 >> 8) & 0x00FF;
	test[3] = crc16 & 0x00FF;
	CommPort->PutBlock( test, sizeof(test) );

	bool ok = false;
	// ждем ответа - события
	for( DWORD tt = GetTickCount(); GetTickCount() - tt < TestTimeout; Sleep(10) )
	{
		Application->ProcessMessages();
		if( DataReceived->WaitFor( 200 ) == wrSignaled )
		{
			DataReceived->ResetEvent();
			ok = true;
			break;
		}
	}
	if ( !ok )
	{
		MainForm->StatusBarTop->Panels->Items[2]->Text = "Не дождались тестовой посылки от АСУ!";
		return false;
	}

	MainForm->StatusBarTop->Panels->Items[2]->Text = "Тест связи пройден";
	return true;
}
//---------------------------------------------------------------------------
//-----запрос номера трубы по 2 протоколу------------------------------------
long MyComPort::GetTubeNumber_2()
{
	length = 17;		// ожидаемая длина посылки от АСУ
	function = 2;		// функия обмена с АСУ
	plant_cut = 0;		// зона реза по шаблонированию
	MainForm->StatusBarTop->Panels->Items[2]->Text = "Запрос номера трубы из АСУ";
	// делаем запрос по функции 1
	unsigned char numb[5] = { 0x5, 0, 0x2 ,0,0};	// длина посылки 5, абонент, функция обмена 2
	numb[1] = abonent;

	unsigned short crc16 = countCRC16_2( numb, 3 );
	// в пакете сначала идёт младший байт, потом старший
	numb[4] = (crc16 >> 8) & 0x00FF;
	numb[3] = crc16 & 0x00FF;
	CommPort->PutBlock( numb, sizeof(numb) );

	bool ok = false;
	// ждем ответа - события
	for( DWORD tt = GetTickCount(); GetTickCount() - tt < TubeNumTimeout; Sleep(10) )
	{
		Application->ProcessMessages();
		if( DataReceived->WaitFor( 200 ) == wrSignaled )
		{
			DataReceived->ResetEvent();
			ok = true;
			break;
		}
	}
	if ( !ok )
	{
		MainForm->StatusBarTop->Panels->Items[2]->Text = "Не дождались номера трубы от АСУ!";
		ProtocolForm->SendToProtocol("Не дождались номера трубы от АСУ");
		return -1;
	}

	String *str = new String((char*)data);
	String rs = (*str).SubString(4,8);
	unsigned long number = StrToInt64(rs);
	short transit = data[12];
	plant_cut = data[13] + data[14] * 256;
	return number;
}
//---------------------------------------------------------------------------
// посылает результат в АСУ: результаты попер, прод, толщ, общ , sgg - группа прочности (костыль)
//bool MyComPort::SendResultToASU(Result &pp1, Result &pr1, ThicknessResult &th1, SummaryResult &sum1 , SolidGroups::Group sgg)
bool MyComPort::SendResultToASU(Result &pp1, Result &pr1, ThicknessResult &th1, SummaryResult &sum1 , int solid_num)
{
// для EliseevRes, костылища
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	Brack = (TColor) ini->ReadInteger("Color","Brack",0);
	Class2 = (TColor) ini->ReadInteger("Color","SecondClass",0);
	Good = (TColor) ini->ReadInteger("Color","Valid",0);
	delete ini;
	pp2 = pp1;
	pr2 = pr1;
	th2 = th1;
	sum2 = sum1;

	length = 5;		// ожидаемая длина посылки от АСУ
	function = 5;	// функция обмена
	int i;
	unsigned char send[167];
	send[0] = 167;
	send[1] = abonent;
	send[2] = function;
	// номер трубы - 8 байт
	String num = IntToStr((__int64)Globals::tube_number);
	for ( i = 0; i < num.Length(); i++ )
		send[10-i] = (unsigned char) num[ num.Length() - i ];
	for ( i; i < 8 ; i++)
		send[10-i] = 0x30;
	// пороги поперечного контроля
	short bord[2] = {0,0};
	for (i = 0; i < pp1.borders.Length; i++)
		bord[i] = (short) pp1.borders[i];
	send[11] = bord[0] & 0x00FF;
	send[12] = (bord[0] >> 8) & 0x00FF;
	send[13] = bord[1] & 0x00FF;
	send[14] = (bord[1] >> 8) & 0x00FF;
	// пороги продольного контроля
	if ( !MainForm->cbLinear->Checked )
		for (int j = 0; j < 4; j++)
			send[j+15] = 0;
	else
	{
		bord[0]=0;
		bord[1]=0;
		for (i = 0; i < pr1.borders.Length; i++)
			bord[i] = (short) pr1.borders[i];
		send[15] = bord[0] & 0x00FF;
		send[16] = (bord[0] >> 8) & 0x00FF;
		send[17] = bord[1] & 0x00FF;
		send[18] = (bord[1] >> 8) & 0x00FF;
	}
	// пороги толщинометрии
	if ( !MainForm->cbThickness->Checked )
		for (int j = 0; j < 4; j++)
			send[j+19] = 0;
	else
	{
		bord[0] = 0;
		bord[1] = 0;
		for (i = 0; i < th1.borders.Length; i++)
			bord[i] = (short) (th1.borders[i] * 10);
		send[19] = bord[0] & 0x00FF;
		send[20] = (bord[0] >> 8) & 0x00FF;
		send[21] = bord[1] & 0x00FF;
		send[22] = (bord[1] >> 8) & 0x00FF;
	}
	// длина трубы в зонах, рез1, рез2
	send[23] = sum1.zones & 0x00FF;
	send[24] = (sum1.zones >> 8) & 0x00FF;
	send[25] = sum1.cut1 & 0x00FF;
	send[26] = (sum1.cut1 >> 8) & 0x00FF;
	send[27] = sum1.cut2 & 0x00FF;
	send[28] = (sum1.cut2 >> 8) &0x00FF;
	// общий результат, результат муфты (всегда годная)
	char q = 0;		// типа брак
	if (sum1.decision.SubString(1,1) == "Г")
		q = 1;
	else if (sum1.decision.SubString(1,1) == "К")
		q = 2;
	send[29] = q;
	send[30] = 0;
	send[31] = 1;
	send[32] = 0;
	// группа прочности муфты (всегда ?), ГП трубы
	send[33] = 0;
//	send[34] = (unsigned char) sgg.GroupToNumber();
	send[34] = (unsigned char) solid_num;
	// результат контроля по зонам (всегда 65 зон)
	for ( i = 0; i < 65; i++)
	{
		if (i < th1.zones )
		{
			send[35 + 2*i] = (unsigned char) (th1.zone_data[i] * 10);
			if (th1.zone_data[i] == 8.0)
				send[35 + 2*i] = 255;
		}
		send[35 + 2*i + 1] = EliseevRes(i);
	}
	// добавляем контрольную сумму
	unsigned short crc16 = countCRC16_2( send, 165 );
	// в пакете сначала идёт младший байт, потом старший
	send[166] = (crc16 >> 8) & 0x00FF;
	send[165] = crc16 & 0x00FF;
	CommPort->PutBlock( send, sizeof(send) );

	bool ok = false;
	// ждем ответа - события
	for( DWORD tt = GetTickCount(); GetTickCount() - tt < ResultTimeout; Sleep(10) )
	{
		Application->ProcessMessages();
		if( DataReceived->WaitFor( 200 ) == wrSignaled )
		{
			DataReceived->ResetEvent();
			ok = true;
			break;
		}
	}
	if ( !ok )
	{
		MainForm->StatusBarTop->Panels->Items[2]->Text = "Не дождались ответа от АСУ!";
		ProtocolForm->SendToProtocol("Не дождались ответа от АСУ");
		return false;
	}
	MainForm->StatusBarTop->Panels->Items[2]->Text = "Результат успешно отправлен в АСУ";
	return true;
}
//---------------------------------------------------------------------------
//----функция записи результатов по-Елисеевски-------------------------------
unsigned char MyComPort::EliseevRes(int zone)
{
	// заполняем байт по правилам: первые 2 бита - результат поперечной
	// 00 - нет результата, 01 - Годно, 10 - Класс2, 11 - Брак
	unsigned char re = 0;
	TColor color;
	if (zone < pp2.zones )
	{
		color = pp2.ZoneColor(pp2.zone_data[zone]);
		if (color == Good)
			re += 1;
		else if (color == Class2)
			re += 2;
		else if (color == Brack)
			re += 3;
	}
	// еще 2 бита - результат продольной. Биты уже дороже: 4 и 8
	if (zone < pr2.zones )
	{
		color = pr2.ZoneColor(pr2.zone_data[zone]);
		if (color == Good)
			re += 4;
		else if (color == Class2)
			re += 8;
		else if (color == Brack)
			re += 12;
	}
	// еще два бита - результат толщинометрии. Биты 16 и 32
	if (zone < th2.zones )
	{
		color = th2.ZoneColor(th2.zone_data[zone]);
		if (color == Good)
			re += 16;
		else if (color == Class2)
			re += 32;
		else if (color == Brack)
			re += 48;
	}
	// еще два бита - общий результат. Биты 64 и 128
	if (zone < sum2.zones )
	{
		color = sum2.ZoneColor(sum2.zone_data[zone]);
		if (color == Good)
			re += 64;
		else if (color == Class2)
			re += 128;
		else if (color == Brack)
			re += 192;
	}
	return re;
}
//---------------------------------------------------------------------------
// рабочие посылки протокола 1 (Стрежевой)
/*
	CommPort->PutChar('?');		// делаем запрос номера (отправка)
	CommPort->PutChar('\5');    // запрос сеанса обмена 2
	CommPort->PutChar('\6');		// хорошо
	CommPort->PutChar('\25');		// плохо
*/
/*
// тоже рабочая
	unsigned char reply[4] = {63, 0x5, 6, 0x15};
	CommPort->PutBlock(reply, sizeof (reply) );
*/


