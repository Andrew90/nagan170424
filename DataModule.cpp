//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DataModule.h"
#include "Global.h"
#include "Main.h"
#include "DebugMess.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDM *DM;
//---------------------------------------------------------------------------
__fastcall TDM::TDM(TComponent* Owner)
	: TDataModule(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TDM::DataModuleCreate(TObject *Sender)//����������� � �� ������� �� udl �����
{

	Connect=false;
	try
	{
		ADOCon->Connected=false;
		ADOCon->ConnectionString="FILE NAME=..\\..\\Settings\\Connection.udl";
		ADOCon->Open();
		Connect=true;
	}
	catch (...)
	{
		Application->MessageBoxW(L"�� ������� ������������ � ���� ������, ��������� ��������� �����������",L"������",MB_OK|MB_ICONERROR);
		Connect=false;
	}
	if(Connect)
	{
		TubeTable->Active=true;
		DefectTable->Active=true;
		TEtalons->Active=true;
		NumTube=0;
	}
}
//---------------------------------------------------------------------------
void TDM::SaveTube( DynamicArray <double> CrossDef , DynamicArray <double> LineDef ,
					int lengthArray , String Result , String SolidGroup,
					int TypeSize,bool isEtalon ) //
{
	if(Connect)
	{
	ULONG NTube = 0;
		//���� ������ � ������������ �������, ������ ��������� ������� ������� ����������.
		//��� ��� ���� ����� ����������� ��������� �� ��� ����� � ������ ������� isEtalon
		//������ ������ ��� ������ ���� ������� ����� �������� � ����, � ��� ������� � Report
		//���� � ������� �������� isEtalon = false
		if(MainForm->cbThickness->Checked)
	{
		Find->Close();
		Find->Active=false;
		Find->SQL->Clear();
		Find->SQL->Add("Select Max(ID_Tube) AS maximum from dbo.InfoTube");
		Find->ExecSQL();
		Find->Active=true;
		NTube=DM->Find->FieldByName("maximum")->AsInteger;
		dprint("%s %d\n", __FUNCTION__, 0);
        	TubeTable->Close();
			dprint("%s %d\n", __FUNCTION__, 1);
		TubeTable->Open();
			dprint("%s %d\n", __FUNCTION__, 2);

		//���� ������ �� ������ ����� ������� ������ ����������
		TubeTable->Locate("ID_Tube",NTube,TLocateOptions() << loCaseInsensitive);
	}
	else
	{
		TubeTable->Append();
		TubeTable->FieldByName("Date")->AsDateTime = Date() + Time();
		TubeTable->FieldByName("MinThickness")->AsFloat = -1;
		TubeTable->Post();
	}
		//��������� �������, ��� ��������� �� ��� (������ ������)
		//TubeTable->Resfesh(); �� ����� ���� �� ������


		dprint("%s %d\n", __FUNCTION__, 3);
		//��������� ��� �������������� � ���������� ���� ���� ������
		ULONG TubeID=TubeTable->FieldByName("ID_Tube")->Value;
		TubeTable->Edit();
		TubeTable->FieldByName("Date")->AsDateTime = Date()+Time();
		TubeTable->FieldByName("Result")->Value = Result;
		TubeTable->FieldByName("SolidGroup")->AsString = SolidGroup;
		TubeTable->FieldByName("TypeSize")->AsInteger=TypeSize;
		TubeTable->FieldByName("Length")->AsFloat= 200.0 * lengthArray;
//		TubeTable->FieldByName("isEtalon")->AsBoolean=isEtalon;
//        TubeTable->FieldByName("NumTube")->AsLargeInt=NumberTube;
		TubeTable->Post();
			 dprint("%s %d\n", __FUNCTION__, 4);
		//��������� ������ � �������� ����� �� �����
		dprint("lengthArray  %d  TubeID  %d\n", lengthArray, TubeID);
		for (int i = 0; i < lengthArray ; i++)
		{
			DefectTable->Append();
			DefectTable->FieldByName("ID_Tube")->AsInteger=TubeID;
			DefectTable->FieldByName("Zone")->AsInteger=i;
			DefectTable->FieldByName("CrossDefect")->AsFloat=CrossDef[i] + 1;
			DefectTable->FieldByName("LineDefect")->AsFloat=LineDef[i] + 1;
			DefectTable->Post();
			Sleep(50);
		}
		  dprint("%s %d\n", __FUNCTION__, 5);
	 }
}//------------------------------------------------------------------------------
int TDM::GetThick(DynamicArray <double> Thick, long ID)//���������� ������ � ��������
{
	if(Connect)
	{

		Find->Close();
		Find->Active=false;
		Find->SQL->Clear();
		Find->SQL->Add("Select Max(ID_Tube) AS maximum from dbo.InfoTube");
		Find->ExecSQL();
		Find->Active=true;
		ULONG NumberTube=DM->Find->FieldByName("maximum")->AsInteger;
		ID=NumberTube;
		TubeTable->Last();


			//������ �� ������� ������ �� ������� � ��������
			QueryThick->Close();
			QueryThick->Active=false;
			QueryThick->SQL->Clear();
			QueryThick->SQL->Add("Select * from dbo.ThickDef where ID_Tube=:Par ORDER BY zone" );
			QueryThick->Parameters->ParamByName("par")->Value=NumberTube;
			QueryThick->ExecSQL();
			QueryThick->Active=true;
			ProtocolForm->SendToProtocol("���-�� ������� (���)  "+IntToStr(QueryThick->RecordCount));
			//����� � ������ ��������� ������
			for (int i = 0; i < QueryThick->RecordCount; i++)
			{
				Thick[i]=QueryThick->FieldByName("MinThickness")->AsFloat;
				QueryThick->Next();
			}
			ProtocolForm->SendToProtocol("�������� ��������� ��  ������� �� ����");
			return QueryThick->RecordCount;
	}
	return 0;
}

//------------------------------------------------------------------------------
void TDM::SaveSG(SolidGroups::Tube Tube, UnicodeString TableName)
{
if(Connect)
	{
	//�������� �������� ������� � ����� � ���, ���� �������� ������������ �� �������� � message
	if ( (TableName == "SGCenter") || (TableName == "SGDataBase") || (TableName == "SGEtalon" ) )
		{
			//���� ������������� ADOTable �� ����� �������
			Table->Active=false;
			Table->Connection=ADOCon;
			Table->TableName=TableName;
			Table->Active=true;
			//����� ���� � �������
			Table->Append();
			Table->FieldByName("TypeSize")->AsInteger = (int) Tube.type();
			Table->FieldByName("Name")->AsString = (String) Tube.solidGroup() ;
			Table->FieldByName("Date")->AsDateTime = Date() + Time();
			vector<double> coords=Tube.Coordinates();
			for(int i=0; i<8;i++)
				{
					Table->FieldByName("p"+UnicodeString(i+1))->Value=coords[i];
				}
			Table->Post();
		} else
				Application->MessageBoxW(L"�� ������ �������� �������",L"������",MB_OK);
	}
}
//----------------------------------------------------------------------------------
vector<SolidGroups::Tube> TDM::ReadEtalon(int TypeSize)	// ,UnicodeString SG
{
	vector <SolidGroups::Tube> array;
	//����� ������ ��� ������ ������ ��� ������
	Find->Close();
	Find->Active=false;
	Find->SQL->Clear();
	Find->SQL->Add("Select * from dbo.SGEtalon");
	Find->SQL->Add("where  (TypeSize=:par1)");	//(Name=:par) and
//	Find->Parameters->ParamByName("par")->Value=SG;
	Find->Parameters->ParamByName("par1")->Value=TypeSize;
	Find->ExecSQL();
	Find->Active=true;

	//��������� ������� ������� �������
	if (Find->RecordCount == 0 )
		{
			Application->MessageBoxW(L"�������������� ������� ������ ��������� �� ����������\n ������ 300",L"������",MB_ICONERROR);
			return array;
		}
	else
	{
	SolidGroups::Tube buf; 			//������ "�����"
	Find->First();

	for (int i = 0;  i < Find->RecordCount; i++)
	 {
			vector <double> param;
			for(int j=1; j<=8; j++)     //������ ������ � ������������ �����
				param.push_back( Find->FieldByName("p"+UnicodeString(j))->AsFloat );
			buf.setCoordinates(param); //����� ������ � "�����"
			buf.setTubeType( SolidGroups::TubeType( TypeSize ) );
			buf.setSolidGroup( SolidGroups::Group ( Find->FieldByName("Name")->AsString ) );
			array.push_back(buf);      //����� ������ � ������ ����
			Find->Next();
		}
	}


	//���������� ��������� � ������� ��������
	return array;
}
//------------------------------------------------------------------------------
SolidGroups::Tube TDM::ReadCenter(int TypeSize,UnicodeString SG)
{   //����� ������ ��� ������ ������ ��� ������
	SolidGroups::Tube tube;
	Find->Close();
	Find->Active=false;
	Find->SQL->Clear();
	Find->SQL->Add("Select * from dbo.SGCenter");
	Find->SQL->Add("where ((Name=:par) and (TypeSize=:par1))");
	Find->Parameters->ParamByName("par")->Value=SG;
	Find->Parameters->ParamByName("par1")->Value=TypeSize;
	Find->ExecSQL();
	Find->Active=true;
	//��������� ������� ������� �������
	if (Find->RecordCount == 0 )
		{
			Application->MessageBoxW(L"������������� ������ �� ����������",L"������",MB_OK);
			//tube.setTubeType( SolidGroupTube::tsUnknown );
			tube.setTubeType( SolidGroups::TubeType::tsUnknown );
			return tube;
		}
	else
	if (Find->RecordCount > 1 )
		{
			Application->MessageBoxW(L"����������� ����� ����� ������ ��������� �������",L"������",MB_OK);
			//tube.setTypeSize(SolidGroupTube::tsUnknown);
			tube.setTubeType( SolidGroups::TubeType::tsUnknown );
			return tube;
		}
	else
	{
		vector<double> param;
		for (int i = 1; i <=8; i++)                    //������ �������� ������� �� ������� � ������
			param.push_back(Find->FieldByName("p"+(UnicodeString)i)->AsFloat);


		tube.setCoordinates(param);             //������ ������ � ������
		tube.setTubeType( SolidGroups::TubeType(TypeSize) );
		tube.setSolidGroup( SolidGroups::Group( SG ) );

		return tube;
	}
}
//------------------------------------------------------------------------------
ULONG TDM::AddTube(String result)
{
	if( !Connect)
		Application->MessageBoxW(L"�� ������� ������������ � ���� ������, ��������� ��������� �����������",L"������",MB_OK|MB_ICONERROR);
	else
		{
			TubeTable->Append();
			TubeTable->FieldByName("ThicknessResult")->AsString=result;
			TubeTable->Post();


			ULONG ID=TubeTable->FieldByName("ID_Tube")->Value;
			return ID;
		}

}



