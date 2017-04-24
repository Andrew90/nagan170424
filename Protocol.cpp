//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Protocol.h"

#include "stdio.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TProtocolForm *ProtocolForm;
//---------------------------------------------------------------------------
__fastcall TProtocolForm::TProtocolForm(TComponent* Owner)
	: TForm(Owner)
{
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	toFile = ini->ReadBool( "Default" , "ProtocolToFile" , 0 );
	delete ini;

	cbProtocolToFile->Checked = toFile;
	FileName = "..\\..\\Protocols\\Protocol_" + DateToStr( Date() )+ ".txt";
	ProtocolForm->SendToProtocol("Открыли программу в " + TimeToStr( Time() ));
	Globals::ProtocolCreated = true;
	for (int i = 0; i < Globals::delay->Count; i++)
		this->SendToProtocol(Globals::delay->Strings[i]);

}
//---------------------------------------------------------------------------
void TProtocolForm::SendToProtocol(AnsiString info)
{
	if (Globals::ProtocolCreated)
	{
		mProtocol->Lines->Add(info);
		if ( toFile )
		{
			FILE *file;
			file = fopen( FileName.c_str() , "a");
			if( file == NULL )
			{
				CreateDirectoryW(L"..\\..\\Protocols", 0);
				file = fopen( FileName.c_str() , "a");
			}

			fprintf(file,"\n");
			fprintf(file, info.c_str() );

			fclose(file);
		}
	}
	else
		Globals::delay->Add(info);
}
//---------------------------------------------------------------------------
void __fastcall TProtocolForm::FormDestroy(TObject *Sender)
{
/*
	try
	{
		TIniFile *ini = new TIniFile(Globals::IniFileName);
		ini->WriteBool( "Default" , "ProtocolToFile" , toFile );
    	delete ini;
	}
	catch(...){}
	*/
}
//---------------------------------------------------------------------------

void __fastcall TProtocolForm::cbProtocolToFileClick(TObject *Sender)
{
	toFile = cbProtocolToFile->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TProtocolForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == 27)
		ProtocolForm->Close();
}
//---------------------------------------------------------------------------
void TProtocolForm::ClearProtocol()
{
	mProtocol->Clear();
	mProtocol->Lines->Clear();
}

void __fastcall TProtocolForm::FormShow(TObject *Sender)
{
	mProtocol->CaretPos = TPoint(0 , mProtocol->Lines->Count);
	mProtocol->Perform(WM_VSCROLL , SB_BOTTOM , 0);
	mProtocol->Refresh();
}
//---------------------------------------------------------------------------

