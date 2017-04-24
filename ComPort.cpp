//---------------------------------------------------------------------------


#pragma hdrstop

#include "ComPort.h"
#include "Main.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

MyComPort *MyCom;
UINT TubeNumTimeout = 4000;		// �������� ������� � �������
UINT ResultTimeout = 5000;		// �������� ������ ����� ������� � �����������
UINT TestTimeout = 3000;			// �������� �������� �������
UINT SolidGroupTimeout = 5000;	// �������� ������ ���������
//---------------------------------------------------------------------------
MyComPort::MyComPort()
{
// ������� �������
	DataReceived = new TEvent( (_SECURITY_ATTRIBUTES *)NULL, true, false, String( "rxEvent" ), false );
	DataReceived->ResetEvent();
// ������� COM ����
	CommPort = new TCommPort(MainForm);
	SetComPortSettings();
	CommPort->OnDataReceived = MyComPortDataReceived;
	CommPort->Open = true;
	ASUMode = 2;	// ����� �� 2 ��������� - ����-�� (�� ������� �������, ��������)
	abonent = 2;	// ����� �������� ��� � ����
	length = 0;		// ����� ����������� ��� ��������
	function = 0;	// ����������
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
//------��������� ����������-------------------------------------------------
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
//------������� ����������� ����� �� 1 ��������� (���������)-----------------
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
//------������� ����������� ����� �� 2 ��������� (����-��)-------------------
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
//------����� ������� ������� ������ �����-----------------------------------
long MyComPort::GetTubeNumber()
{
	if (ASUMode == 1)
		return GetTubeNumber_1();
	else if (ASUMode == 2)
		return GetTubeNumber_2();
}
//---------------------------------------------------------------------------
//----����� ������� ������� ����������---------------------------------------
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
//-----������ ������ ����� �� 1 ��������� (���������)------------------------
long MyComPort::GetTubeNumber_1()
{
	MainForm->StatusBarTop->Panels->Items[2]->Text = "���� ������ ����� ";
	// ������ ������ ������ ����� (�������� ������)
	CommPort->PutChar('?');
	bool ok = false;
	// ���� ������ - �������
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
		MainForm->StatusBarTop->Panels->Items[2]->Text = "�� ��������� ������� � ������� �����!";
		CommPort->PutChar('\25');
		return -1;
	}
	else
	{
		unsigned char* b;
		b = &data[1];
		unsigned char check_sum = countCRC16_1( b , 15 );		// ���������� ����� �����
		unsigned char got_sum = data[16];					// ���������� ����� �����
		if ( check_sum != got_sum)
		{
			MainForm->StatusBarTop->Panels->Items[2]->Text = "������������ ����������� �����!";
			CommPort->PutChar('\25');
			return -1;
		}
		else
		{
			String *str = new String((char*)data);
			String rs = (*str).SubString(2,5);
			unsigned long numb = StrToInt64(rs);
			MainForm->StatusBarTop->Panels->Items[2]->Text = "����� �����: "+ IntToStr((int)numb);
			CommPort->PutChar('\6');
			return numb;
		}
	}
}
//---------------------------------------------------------------------------
// �������� ��������� � ���: res = ������� �����/����, c1 = ��� 1, �2 = ��� 2, len = ����� � �����, sgg - ������ ���������
bool MyComPort::SendResultToASU(short res, short c1, short c2, short len , SolidGroups::Group sgg)
{
	MainForm->StatusBarTop->Panels->Items[2]->Text = "���� �������� ���������� ";
	// ������ ������ ������ ������ 2 (�������� ������)
	CommPort->PutChar('\5');
	bool ok = false;
	// ���� ������ - �������
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
		MainForm->StatusBarTop->Panels->Items[2]->Text = "�� ��������� ������� \"ACK\" �� ���!";
		return false;
	}
	else
	{
		if ( data_size == 1 && data[0] == 21 )
		{
			MainForm->StatusBarTop->Panels->Items[2]->Text = "��� �� ����� � ������ ����������!";
			return false;
		}
		else if ( data_size != 1)
		{
			MainForm->StatusBarTop->Panels->Items[2]->Text = "�������� ����� �������!";
			return false;
		}
		else if ( data_size == 1 && data[0] == 6 )
		{
			// ��������� �������
			unsigned char sending[18] = { 2 ,0,0,0,0,0,0,0,0,0,0,0,0,0 ,48,48,0, 3 };
			// ����� 1-5 = ����� �����
			String tubnum = IntToStr((__int64) Globals::tube_number);
			while ( tubnum.Length() < 5 )
				tubnum = "0" + tubnum;
			for (int i=1; i <= 5; i++)
				sending[i] = (char)tubnum[ i ];
			// ���� 6 - ���������
			sending[6] = res + 48;
			// ���� 7 - ������ ���������
			if ( sgg.value() == SolidGroups::Group::sgUnknown )
				sgg = SolidGroups::Group::sgK;
			sending[7] = sgg.GroupToEnglish();
			// ����� 8,9 - ���1
			String temp = IntToStr(c1);
			while ( temp.Length() < 2 )
				temp = "0" + temp;
			for (int i=0; i <= 1; i++)
				sending[i + 8] = (char)temp[ i+1 ];
			// ����� 10,11 - ���2
			temp = IntToStr(c2);
			while ( temp.Length() < 2 )
				temp = "0" + temp;
			for (int i=0; i <= 1; i++)
				sending[i + 10] = (char)temp[ i+1 ];
			// ����� 12,13 - ����� ����� � �����
			temp = IntToStr( len );
			while ( temp.Length() < 2 )
				temp = "0" + temp;
			for (int i=0; i <= 1; i++)
				sending[i + 12] = (char)temp[ i+1 ];
			// ���� 16 - ����������� �����
			unsigned char* b;
			b = &sending[1];
			sending[16] = countCRC16_1( b , 15);
			CommPort->PutBlock( sending , sizeof (sending) );
			// ���� ������ - �������
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
				MainForm->StatusBarTop->Panels->Items[2]->Text = "�� ��������� ������� \"ACK\"�� ���!";
				return false;
			}
			else
			{
				if ( data_size == 1 && data[0] == 21 )
				{
					MainForm->StatusBarTop->Panels->Items[2]->Text = "��� ������ ��������� � �������!";
					return false;
				}
				else if ( data_size != 1)
				{
					MainForm->StatusBarTop->Panels->Items[2]->Text = "�������� ����� ������� �� ���!";
					return false;
				}
				else if ( data_size == 1 && data[0] == 6 )
				{
					MainForm->StatusBarTop->Panels->Items[2]->Text = "��������� ��������� �������";
					return true;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
//------������� ��������� ������� ������ ��������� (������ ���)--------------
SolidGroups::Tube MyComPort::GetSolidGroup()
{
	using namespace SolidGroups;
	SolidGroups::Tube tubeSG;	// ������ ��������� �����
	tubeSG = SolidGroups::Tube();
	short measures[8] = {0};
	vector<double>t;
	t.resize(8);

	CommPort->PutChar('0');
	bool ok = false;
	// ���� ������ - �������
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
		MainForm->StatusBarTop->Panels->Items[2]->Text = "�� ��������� ������� � ��!";
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
//-----�������: ��������� ������ �� COM �����--------------------------------
void __fastcall MyComPort::MyComPortDataReceived(TObject *Sender, DWORD Count)
{
	TThread::CurrentThread->Sleep(200);
//	���� ������ ��������� �� COM ����� (������ ���)
	if ( !SystemConst::ComWithASU && SystemConst::SolidGroupSource == sgsCOMPort )
	{
		if (CommPort->InBuffUsed == 16)
		{
			data_size = CommPort->GetBlock( data, CommPort->InBuffUsed);
			DataReceived->SetEvent();
		}
	}
// ���� ����� � ����� �� COM �����  (���������)
	else if ( SystemConst::ComWithASU && SystemConst::SolidGroupSource != sgsCOMPort )
	{
		if (ASUMode == 1)		// ��������� - ���������
		{
			// ����� �����
			if (CommPort->InBuffUsed == 18)
			{
				data_size = CommPort->GetBlock( data, CommPort->InBuffUsed);
				DataReceived->SetEvent();
			}
			// �������������� ������ ��� � ����������
			if (CommPort->InBuffUsed == 1)
			{
				data_size = CommPort->GetBlock( data , CommPort->InBuffUsed);
				DataReceived->SetEvent();
			}
		}
		else if (ASUMode == 2)		// ����-�� - ��������
		{
			// ����� �����
			if (CommPort->InBuffUsed == 17)
			{
				data_size = CommPort->GetBlock( data, CommPort->InBuffUsed);
				if (CheckSentMessage())
					DataReceived->SetEvent();
			}
			// �������������� ������ ���  - ���� � ���������� ���������
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
		MainForm->StatusBarTop->Panels->Items[2]->Text = "������������ �������: " + IntToStr(kol) + " ����";
		MainForm->StatusBarTop->Refresh();
	}
	CommPort->FlushInBuffer();
}
//---------------------------------------------------------------------------
//----�������� ���������� ������� (������ 2 ��������)----------------------------------
bool MyComPort::CheckSentMessage()
{
	String text;
	if (data[0] != length)	// ����� ������� �������� ������ �������?
	{
		text = "�������� ����� ������� �� ���!";
		MainForm->StatusBarTop->Panels->Items[2]->Text = text;
		ProtocolForm->SendToProtocol(text);
		return false;
	}

	if (data[1] != abonent)	// ����� ������� �������� ������ �������?
	{
		text = "������� �� ��� �� ���!";
		MainForm->StatusBarTop->Panels->Items[2]->Text = text;
		ProtocolForm->SendToProtocol(text);
		return false;
	}

	if (data[2] != function)	// ����� ������� �������� ������ �������?
	{
		text = "�������� ��� ������� ��������!";
		MainForm->StatusBarTop->Panels->Items[2]->Text = text;
		ProtocolForm->SendToProtocol(text);
		return false;
	}

	unsigned char* b;
	b = &data[0];
	unsigned short check_sum = countCRC16_2( b , length-2 );		// ���������� ����� �����
	if( data[length-1] != ((check_sum >> 8) & 0x00FF) || data[length-2] != (check_sum & 0x00FF)  )
	{
		text = "������������ ����������� �����!";
		MainForm->StatusBarTop->Panels->Items[2]->Text = text;
		ProtocolForm->SendToProtocol(text);
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
//----���� ������ ����� (������ 2 ��������)----------------------------------
bool MyComPort::TestConnection()
{
	length = 5;		// ��������� ����� ������ �� ���
	function = 1;	// ������� ������ � ���
	MainForm->StatusBarTop->Panels->Items[2]->Text = "���� ����� �� COM-�����";
	// ������ ������ �� ������� 1
	unsigned char test[5];
	test[0] = length;
	test[1] = abonent;
	test[2] = function;

	unsigned short crc16 = countCRC16_2( test, 3 );
	// � ������ ������� ��� ������� ����, ����� �������
	test[4] = (crc16 >> 8) & 0x00FF;
	test[3] = crc16 & 0x00FF;
	CommPort->PutBlock( test, sizeof(test) );

	bool ok = false;
	// ���� ������ - �������
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
		MainForm->StatusBarTop->Panels->Items[2]->Text = "�� ��������� �������� ������� �� ���!";
		return false;
	}

	MainForm->StatusBarTop->Panels->Items[2]->Text = "���� ����� �������";
	return true;
}
//---------------------------------------------------------------------------
//-----������ ������ ����� �� 2 ���������------------------------------------
long MyComPort::GetTubeNumber_2()
{
	length = 17;		// ��������� ����� ������� �� ���
	function = 2;		// ������ ������ � ���
	plant_cut = 0;		// ���� ���� �� ��������������
	MainForm->StatusBarTop->Panels->Items[2]->Text = "������ ������ ����� �� ���";
	// ������ ������ �� ������� 1
	unsigned char numb[5] = { 0x5, 0, 0x2 ,0,0};	// ����� ������� 5, �������, ������� ������ 2
	numb[1] = abonent;

	unsigned short crc16 = countCRC16_2( numb, 3 );
	// � ������ ������� ��� ������� ����, ����� �������
	numb[4] = (crc16 >> 8) & 0x00FF;
	numb[3] = crc16 & 0x00FF;
	CommPort->PutBlock( numb, sizeof(numb) );

	bool ok = false;
	// ���� ������ - �������
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
		MainForm->StatusBarTop->Panels->Items[2]->Text = "�� ��������� ������ ����� �� ���!";
		ProtocolForm->SendToProtocol("�� ��������� ������ ����� �� ���");
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
// �������� ��������� � ���: ���������� �����, ����, ����, ��� , sgg - ������ ��������� (�������)
//bool MyComPort::SendResultToASU(Result &pp1, Result &pr1, ThicknessResult &th1, SummaryResult &sum1 , SolidGroups::Group sgg)
bool MyComPort::SendResultToASU(Result &pp1, Result &pr1, ThicknessResult &th1, SummaryResult &sum1 , int solid_num)
{
// ��� EliseevRes, ���������
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	Brack = (TColor) ini->ReadInteger("Color","Brack",0);
	Class2 = (TColor) ini->ReadInteger("Color","SecondClass",0);
	Good = (TColor) ini->ReadInteger("Color","Valid",0);
	delete ini;
	pp2 = pp1;
	pr2 = pr1;
	th2 = th1;
	sum2 = sum1;

	length = 5;		// ��������� ����� ������� �� ���
	function = 5;	// ������� ������
	int i;
	unsigned char send[167];
	send[0] = 167;
	send[1] = abonent;
	send[2] = function;
	// ����� ����� - 8 ����
	String num = IntToStr((__int64)Globals::tube_number);
	for ( i = 0; i < num.Length(); i++ )
		send[10-i] = (unsigned char) num[ num.Length() - i ];
	for ( i; i < 8 ; i++)
		send[10-i] = 0x30;
	// ������ ����������� ��������
	short bord[2] = {0,0};
	for (i = 0; i < pp1.borders.Length; i++)
		bord[i] = (short) pp1.borders[i];
	send[11] = bord[0] & 0x00FF;
	send[12] = (bord[0] >> 8) & 0x00FF;
	send[13] = bord[1] & 0x00FF;
	send[14] = (bord[1] >> 8) & 0x00FF;
	// ������ ����������� ��������
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
	// ������ �������������
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
	// ����� ����� � �����, ���1, ���2
	send[23] = sum1.zones & 0x00FF;
	send[24] = (sum1.zones >> 8) & 0x00FF;
	send[25] = sum1.cut1 & 0x00FF;
	send[26] = (sum1.cut1 >> 8) & 0x00FF;
	send[27] = sum1.cut2 & 0x00FF;
	send[28] = (sum1.cut2 >> 8) &0x00FF;
	// ����� ���������, ��������� ����� (������ ������)
	char q = 0;		// ���� ����
	if (sum1.decision.SubString(1,1) == "�")
		q = 1;
	else if (sum1.decision.SubString(1,1) == "�")
		q = 2;
	send[29] = q;
	send[30] = 0;
	send[31] = 1;
	send[32] = 0;
	// ������ ��������� ����� (������ ?), �� �����
	send[33] = 0;
//	send[34] = (unsigned char) sgg.GroupToNumber();
	send[34] = (unsigned char) solid_num;
	// ��������� �������� �� ����� (������ 65 ���)
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
	// ��������� ����������� �����
	unsigned short crc16 = countCRC16_2( send, 165 );
	// � ������ ������� ��� ������� ����, ����� �������
	send[166] = (crc16 >> 8) & 0x00FF;
	send[165] = crc16 & 0x00FF;
	CommPort->PutBlock( send, sizeof(send) );

	bool ok = false;
	// ���� ������ - �������
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
		MainForm->StatusBarTop->Panels->Items[2]->Text = "�� ��������� ������ �� ���!";
		ProtocolForm->SendToProtocol("�� ��������� ������ �� ���");
		return false;
	}
	MainForm->StatusBarTop->Panels->Items[2]->Text = "��������� ������� ��������� � ���";
	return true;
}
//---------------------------------------------------------------------------
//----������� ������ ����������� ��-����������-------------------------------
unsigned char MyComPort::EliseevRes(int zone)
{
	// ��������� ���� �� ��������: ������ 2 ���� - ��������� ����������
	// 00 - ��� ����������, 01 - �����, 10 - �����2, 11 - ����
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
	// ��� 2 ���� - ��������� ����������. ���� ��� ������: 4 � 8
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
	// ��� ��� ���� - ��������� �������������. ���� 16 � 32
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
	// ��� ��� ���� - ����� ���������. ���� 64 � 128
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
// ������� ������� ��������� 1 (���������)
/*
	CommPort->PutChar('?');		// ������ ������ ������ (��������)
	CommPort->PutChar('\5');    // ������ ������ ������ 2
	CommPort->PutChar('\6');		// ������
	CommPort->PutChar('\25');		// �����
*/
/*
// ���� �������
	unsigned char reply[4] = {63, 0x5, 6, 0x15};
	CommPort->PutBlock(reply, sizeof (reply) );
*/


