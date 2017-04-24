//---------------------------------------------------------------------------

#ifndef DataModuleH
#define DataModuleH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include "IniFiles.hpp"

#include "clustering.h"

//---------------------------------------------------------------------------
class TDM : public TDataModule
{
__published:	// IDE-managed Components
	TADOConnection *ADOCon;
	TDataSource *DataDefect;
	TDataSource *DataTube;
	TADOTable *TubeTable;
	TADOTable *DefectTable;
	TADOQuery *QueryThick;
	TLargeintField *DefectTableID_Tube;
	TWordField *DefectTableZone;
	TFloatField *DefectTableCrossDefect;
	TFloatField *DefectTableLineDefect;
	TADOQuery *Find;
	TDataSource *DataFind;
	TADOTable *TEtalons;
	TDataSource *DataEtalons;
	TSmallintField *TEtalonsp1;
	TSmallintField *TEtalonsp2;
	TSmallintField *TEtalonsp3;
	TSmallintField *TEtalonsp4;
	TSmallintField *TEtalonsp5;
	TSmallintField *TEtalonsp6;
	TSmallintField *TEtalonsp7;
	TSmallintField *TEtalonsp8;
	TSmallintField *TEtalonsTypeSize;
	TADOQuery *QEtalons;
	TMemoField *TEtalonsName;
	TLargeintField *TubeTableID_Tube;
	TWideStringField *TubeTableResult;
	TWideStringField *TubeTableSolidGroup;
	TDateTimeField *TubeTableDate;
	TFloatField *TubeTableMinThickness;
	TStringField *TubeTableCrossResult;
	TStringField *TubeTableLineResult;
	TFloatField *TubeTableLength;
	TStringField *TubeTableThicknessResult;
	TIntegerField *TubeTableTypeSize;
	TBooleanField *TubeTableisEtalon;
	TIntegerField *TubeTableDefectoscoperID;
	TIntegerField *TubeTableTubeGroup;
	TIntegerField *TubeTableCustomerID;
	TADOTable *tTemp;
	TAutoIncField *tTempID;
	TWideStringField *tTempName;
	TSmallintField *tTempp1;
	TSmallintField *tTempp2;
	TSmallintField *tTempp3;
	TSmallintField *tTempp4;
	TSmallintField *tTempp5;
	TSmallintField *tTempp6;
	TSmallintField *tTempp7;
	TSmallintField *tTempp8;
	TSmallintField *tTempTypeSize;
	TDateTimeField *tTempDate;
	TADOTable *Table;
	TLargeintField *TubeTableNumberTube;
	TDateTimeField *TEtalonsDate;
	void __fastcall DataModuleCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	bool Connect;
	void SaveTube( DynamicArray <double> CrossDef , DynamicArray <double> LineDef ,
					int lengthArray , String Result , String SolidGroup,
					int TypeSize,bool isEtalon );	 //Сохраняет все и обо всех

	int GetThick(DynamicArray <double> Thick, long ID);
	void SaveSG(SolidGroups::Tube Tube, UnicodeString TableName);
	vector<SolidGroups::Tube> TDM::ReadEtalon(int TypeSize);	// ,UnicodeString SG
	SolidGroups::Tube ReadCenter(int TypeSize,UnicodeString SG);
	int NumTube;
	ULONG AddTube(String result);

	TIniFile *Ini;                      //Файл настроек
		__fastcall TDM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDM *DM;
//---------------------------------------------------------------------------
#endif
