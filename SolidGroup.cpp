//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SolidGroup.h"
#include "ViewEtalons.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSolidGroupForm *SolidGroupForm;
//---------------------------------------------------------------------------
__fastcall TSolidGroupForm::TSolidGroupForm(TComponent* Owner)
	: TForm(Owner)
{
	KeyPreview=true;
}
//---------------------------------------------------------------------------
void __fastcall TSolidGroupForm::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	if (Key == 27) SolidGroupForm->Close();
}
//---------------------------------------------------------------------------

void __fastcall TSolidGroupForm::FormCreate(TObject *Sender)
{
	int hm=50,vm=170;	// �������� � ���� ������
	int w=90, h=50;		// ������ � ������� ������
	int cols=4;			// ���-�� �������
// ������� 8 ������, ����� ������� � ��� ��������� ������ ���������
	for (int i = 0; i < 8; i++) {
		par[i] = new TEdit(this);
		par[i]->SetBounds( hm + (w+10) * ( i%cols ), vm + h*( i/cols ), w , h);
		par[i]->Parent = SolidGroupForm;
		par[i]->TabOrder=i;
		par[i]->OnKeyPress = TSolidGroupForm::FormKeyPress;
	}
	// ��������� ComboBox'�
	SolidGroups::Group g;
	TStringList * gr = g.getGroupNames();
	this->cbSolidGroup->Items->AddStrings(gr);

	SolidGroups::TubeType tt;
	TStringList * ti = tt.getTubeTypeNames();
	this->cbTypeSize->Items->AddStrings(ti);

	t = new SolidGroups::Tube();
	coord.resize(8);

//	this->cbSolidGroup->ItemIndex=0;
//	this->cbTypeSize->ItemIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TSolidGroupForm::FormShow(TObject *Sender)
{
	vector<double> coords = tube_current.Coordinates();
	if ( coords.size() > 0 )
		for (int i=0; i < 8; i++)
			par[i]->Text="" + FloatToStr( Math::RoundTo(coords[i],0) );
	if ( tube_current.type().value() != SolidGroups::TubeType::tsUnknown )
		cbTypeSize->ItemIndex = cbTypeSize->Items->IndexOf( String( (int)tube_current.type() ) );
	if ( tube_current.solidGroup().value() != SolidGroups::Group::sgUnknown )
		cbSolidGroup->ItemIndex = cbSolidGroup->Items->IndexOf( (String) tube_current.solidGroup() );

	StatusBarBottom->Panels->Items[0]->Text="";
	par[0]->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TSolidGroupForm::bAddInsertedClick(TObject *Sender)
{
	bool filled=true;
	for (int i = 0; i < 8; i++)
	{
		if ( par[i]->Text == "")
		{
			StatusBarBottom->Panels->Items[0]->Text="��������� �� ��� ����!";
			filled=false;
			break;
		}
		else
			coord[i] = par[i]->Text.ToDouble();
	}
	// ���� ��� ���� ���������, ��������� ������
	if ( (filled) && (cbSolidGroup->Text!="") && (cbTypeSize->Text!="") )
	{
		t->setCoordinates ( coord ) ;
		t->setTubeType( SolidGroups::TubeType( cbTypeSize->Text.ToInt() ) );
		t->setSolidGroup( SolidGroups::Group( cbSolidGroup->Text ) );
		DM->SaveSG(*t,"SGEtalon");
		StatusBarBottom->Panels->Items[0]->Text="������ ������ � ���� ������";
		SolidGroupForm->Tag=1;
	}
	else
		StatusBarBottom->Panels->Items[0]->Text="��������� �� ��� ����,���������� �� ���������.";

}
//---------------------------------------------------------------------------

void TSolidGroupForm::Send_Tube(SolidGroups::Tube temp_tube)
{
	tube_current = temp_tube;
}

void __fastcall TSolidGroupForm::bAddCurrentTubeClick(TObject *Sender)
{
	DM->SaveSG(tube_current,"SGEtalon");
	StatusBarBottom->Panels->Items[0]->Text="��������� ����� ������� � �� ��������";
	SolidGroupForm->Tag=1;
}
//---------------------------------------------------------------------------

void __fastcall TSolidGroupForm::FormDestroy(TObject *Sender)
{
	delete t;
}
//---------------------------------------------------------------------------


