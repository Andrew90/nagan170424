//---------------------------------------------------------------------------

#ifndef ProtocolH
#define ProtocolH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Classes.hpp"
#include "IniFiles.hpp"

#include "Global.h"
//---------------------------------------------------------------------------
class TProtocolForm : public TForm
{
__published:	// IDE-managed Components
	TMemo *mProtocol;
	TCheckBox *cbProtocolToFile;
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall cbProtocolToFileClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	bool toFile;			// сохранять логи в файл
	AnsiString FileName;	// имя файла протокола
public:		// User declarations
	__fastcall TProtocolForm(TComponent* Owner);

	void SendToProtocol(AnsiString info);	// отправить строку в протокол
	void ClearProtocol();
};
//---------------------------------------------------------------------------
extern PACKAGE TProtocolForm *ProtocolForm;
//---------------------------------------------------------------------------
#endif
