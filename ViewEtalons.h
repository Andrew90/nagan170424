//---------------------------------------------------------------------------

#ifndef ViewEtalonsH
#define ViewEtalonsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <DBCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include "GraphicsCenters.h"
#include <map>
#include <cstdio.h>
#include "Global.h"
//---------------------------------------------------------------------------
class TFormViewEtalons : public TForm
{
__published:	// IDE-managed Components
	TDBGrid *DBGridEtalons;
	TPanel *Panel;
	TLabel *LSG;
	TComboBox *ComboBoxSG;
	TLabel *LTS;
	TComboBox *ComboBoxTS;
	TButton *View;
	TButton *ViewAll;
	TDBNavigator *DBNavigatorEtalons;
	TPopupMenu *pmExport;
	TMenuItem *menuExport;
	TSaveDialog *SaveDialog;
	TMenuItem *N1;
	TOpenDialog *OpenDialog;
	TMenuItem *txt1;
	void __fastcall ViewClick(TObject *Sender);
	void __fastcall ViewAllClick(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall menuExportClick(TObject *Sender);
	void __fastcall N1Click(TObject *Sender);
	void __fastcall txt1Click(TObject *Sender);
private:	// User declarations

public:		// User declarations
	__fastcall TFormViewEtalons(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormViewEtalons *FormViewEtalons;
//---------------------------------------------------------------------------
#endif
