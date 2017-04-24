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
// ��������� ����� ���������
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
	// ������ �������� ���������� �� ����� ("������") ��������
	String wSpeed = ini->ReadString("OtherSettings","WorkSpeed", "1");
	String tSpeed = ini->ReadString("OtherSettings","TransitSpeed", "2");
	String rSpeed = ini->ReadString("OtherSettings","ReturnSpeed", "2");
	workSpeed	 = speeds[wSpeed];
	transitSpeed = speeds[tSpeed];
	returnSpeed	 = speeds[rSpeed];
	// ������ �������� ����������� ������ �� �������� �����������
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
	ThicknessMass = digital->SignalExists("���� ���� ���")
					&& digital->SignalExists("���� ���� �����")
					&& digital->SignalExists("���� ���� ������");
	ThickModuleManage = digital->SignalExists("���� ��")
						&& digital->SignalExists("���� ��")
						&& digital->SignalExists("���� � ��")
						&& digital->SignalExists("���� � ��");
	ThickCentralizer = digital->SignalExists("���� ����� ���")
					&& digital->SignalExists("���� ����� �����")
					&& digital->SignalExists("���� ����� �������");
	LinearMass = digital->SignalExists("���� ���� ���")
					&& digital->SignalExists("���� ���� �����")
					&& digital->SignalExists("���� ���� ������");
	LinearCentralizer = digital->SignalExists("���� ����� ���")
					&& digital->SignalExists("���� ����� �����")
					&& digital->SignalExists("���� ����� �������");
	CrossCentralizer = digital->SignalExists("����� ����� ���")
					&& digital->SignalExists("����� ����� �����")
					&& digital->SignalExists("����� ����� �������");
	CentralizersCard = digital->SignalExists("����� ����")
					&& digital->SignalExists("����� �������")
					&& digital->SignalExists("����� ������")
					&& digital->SignalExists("����� ������");
	TransportManage = digital->SignalExists("��: �� �")
					&& digital->SignalExists("��: �� RUN")
					&& digital->SignalExists("��: STR")
					&& digital->SignalExists("��: STF")
					&& digital->SignalExists("��: �������� 1")
					&& digital->SignalExists("��: �������� 2")
					&& digital->SignalExists("��: �������� 3")
					&& digital->SignalExists("��: ��� ��");
	ExternalTransport = digital->SignalExists("�� �������� 1")
					&& digital->SignalExists("�� �������� 2")
					&& digital->SignalExists("�� �������� 3")
					&& digital->SignalExists("�� ������� ��")
					&& digital->SignalExists("�� �������� ������")
					&& digital->SignalExists("�� �������� �����")
					&& digital->SignalExists("�� ������1 ���")
					&& digital->SignalExists("�� ������1 ����")
					&& digital->SignalExists("�� ������2 ���")
					&& digital->SignalExists("�� ������2 ����")
					&& digital->SignalExists("�� �������")
					&& digital->SignalExists("�� ������ �����")
					&& digital->SignalExists("�� ����� �����")
					&& digital->SignalExists("�� ���������")
					&& digital->SignalExists("�� �� �")
					&& digital->SignalExists("�� �� RUN")
					&& digital->SignalExists("�� ���������")
					&& digital->SignalExists("�� ���")
					&& digital->SignalExists("�� �����")
					&& digital->SignalExists("�� ������1 �����")
					&& digital->SignalExists("�� ������1 ������")
					&& digital->SignalExists("�� ������2 �����")
					&& digital->SignalExists("�� ������2 ������");

	// ������ ��������� �� �����
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
0=���� ����
1=���� �����
2=���� ��
3=���� ��
4=����� ����
5=����� �����
6=����� ��
7=����� ��
8=���� ����
9=���� �����
10=���� ��
11=���� ��
12=����������
13=��������
14=���������
15=���� ����

[ACard1_IN_TTL]
0=���� ����������
1=������ ����������
2=��: �� �
3=�� �����
4=��: �� RUN
5=���� �� �
6=�� �������
7=���� �� RUN
8=������
9=����� ������
10=����� ������
11=������ ������
12=������ ������
14=���� ������
15=���� ����

[ACard1_OUT_Digital]
0=�������� ������
1=�������� �������
2=���� � ��
3=���� � ��
4=����� � ��
5=����� � ��
6=���� � ��
7=���� � ��
8=������
9=����
10=����
11=������
12=����� ����
13=����� �������
14=������ ����
15=������ �������

[ACard1_OUT_TTL]
0=���� �������� 1
1=���� ��� ��
2=��: STR
3=��: STF
4=��: �������� 3
5=��: �������� 2
6=��: �������� 1
7=��: ��� ��
8=��� ����������
9=���������������
12=���� ��� ��
13=���� STF
14=���� �������� 3
15=���� �������� 2

[ACard0_IN_Digital]
0=��� ����
1=��� �������
2=��� ��1
3=��� ��2
4=������ 1
5=������ 2
6=������ 3

[ACard0_OUT_Digital]
0=��� ������
1=��� ���������
2=��� �������
3=��� �����


*/
