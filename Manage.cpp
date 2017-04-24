//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

//using Microsoft::ManagementConsole::Advanced;


#include "Manage.h"
#include "Global.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TManageForm *ManageForm;
//---------------------------------------------------------------------------
__fastcall TManageForm::TManageForm(TComponent* Owner)
	: TForm(Owner)
{
// ������ ����� ��� ������ � ���������, �������� � �����
	centr_on = "����";
	centr_off = "������";
	centr_close = "�����";
	centr_open = "�������";
	centr_cl = new Graphics::TBitmap();
	centr_op = new Graphics::TBitmap();

	KeyPreview = true;

	mod_on = "� ������";
	mod_off = "������������";
	mod_mov = "������������";
	mod_work = "��������";
	mod_service = "���������";
	mod_dsbl = "����������";

	if (SystemConst::ExternalTransport) // GAN *********************
	{
		load_Up = "������";
		load_Down = "�����";
		load_mow = "������������";
		load_st = "����������� ";

		load_work = "����������";

		load_onon = "���������!";
		load_offoff = "���������!";

		load_UP = new Graphics::TBitmap();
		load_Dwn = new Graphics::TBitmap();
		load_mvg = new Graphics::TBitmap();
	}

	mod_serv = new Graphics::TBitmap();
	mod_wrk = new Graphics::TBitmap();
	mod_mvg = new Graphics::TBitmap();
	try
	{
		mod_serv ->LoadFromFile(  "..\\..\\Pix\\Module-Service.bmp");
		mod_wrk ->LoadFromFile(  "..\\..\\Pix\\Module-Work.bmp");
		mod_mvg ->LoadFromFile(  "..\\..\\Pix\\Module-Move.bmp");
		centr_cl ->LoadFromFile(  "..\\..\\Pix\\Centralizer-close.bmp");
		centr_op ->LoadFromFile(  "..\\..\\Pix\\Centralizer-open.bmp");
		is_pix_ok = true;
	}
	catch (...)
	{
		is_pix_ok=false;
		Application->MessageBoxW(L"����������� ��� ������������ ������ �� �������. \n ������ 500",L"������",MB_ICONERROR);
	}


// �������, ����� ���������� �������� ��� ����������
		this->lThicknessState->Visible = SystemConst::ThickModuleManage;
		this->pThicknessState->Visible = SystemConst::ThickModuleManage;
		this->bThicknessState->Visible = SystemConst::ThickModuleManage;

		this->lThicknessCentralizer->Visible = SystemConst::ThickCentralizer;
		this->pThicknessCentralizer->Visible = SystemConst::ThickCentralizer;
		this->bThicknessCentralizer->Visible = SystemConst::ThickCentralizer;

		this->lLinearCentralizer->Visible = SystemConst::LinearCentralizer;
		this->pLinearCentralizer->Visible = SystemConst::LinearCentralizer;
		this->bLinearCentralizer->Visible = SystemConst::LinearCentralizer;

		this->gbTransport->Visible = SystemConst::TransportManage;
		this->gbCounters->Visible = SystemConst::IsOnline;

		this->GroupPerekl->Visible = SystemConst::ExternalTransport;
}
//---------------------------------------------------------------------------
void TManageForm::SendHandleToSignals(LCard791 &lcard)
{
	lt = &lcard;
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::FormShow(TObject *Sender)
{
// ������ ����� �� �����
	TIniFile *ini = new TIniFile(Globals::IniFileName);

	on = ini->ReadInteger( "Color" , "ActiveIn" , 0 );
	off = ini->ReadInteger( "Color" , "NotActiveIn" , 0 );
	move = ini->ReadInteger( "Color" , "NotActiveOut" , 0 );

	delete ini;

// �������� ������, ������������� ��������� ������ � �������
	this->InputTimer->Enabled=true;
	butt_enabled = true;
	this->StatusBarBottom->Panels->Items[0]->Text="";
	this->StatusBarBottom->Refresh();
	TemperatureTimer->Enabled = true;

}
//---------------------------------------------------------------------------
void __fastcall TManageForm::FormClose(TObject *Sender, TCloseAction &Action)
{
// ��������� ������, ������������� ��������� ������ � �������
	InputTimer->Enabled = false;
	TemperatureTimer->Enabled = false;
// ���������� �������, ���� ������������ ����� �� ����
	digital -> ResetOutSignal("�������� ������");
	digital -> ResetOutSignal("�������� �������");
//	digital -> ResetOutSignal("���� ��� ��");
	digital -> ResetOutSignal("���� �������� 1");
	digital -> ResetOutSignal("���� �������� 2");
	digital -> ResetOutSignal("���� �������� 3");
	digital -> ResetOutSignal("���� STF");
	bStopClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TManageForm::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	if (Key == 27)
		ManageForm->Close();
}
//---------------------------------------------------------------------------
void TManageForm::ShowCentralizerOpened(TBitBtn *btn, TPanel *pan)
{
// ����� ������, ������ ��������������� ��������� ��������, ��������� � ���
	pan->Color = off;
	pan->Caption = centr_off;
	btn->Caption = centr_close;
	btn->Glyph = centr_cl;
}
//---------------------------------------------------------------------------

void TManageForm::ShowCentralizerClosed(TBitBtn *btn, TPanel *pan)
{
// ����� ����, ������ ��������������� ��������� ��������, ��������� � ���
	pan->Color = on;
	pan->Caption = centr_on;
	btn->Caption = centr_open;
	btn->Glyph = centr_op;
}
//---------------------------------------------------------------------------
void TManageForm::ShowModuleinWork(TBitBtn *btn, TPanel *pan)
{
// ������ � ������ , ������ ��������������� ��������� ��������, ��������� � ���
	pan->Color = on;
	pan->Caption = mod_on;
	btn->Caption = mod_service;
	btn->Glyph = mod_serv;

	if ( butt_enabled )
		btn->Enabled = true;
	else
		btn->Enabled = false;
}
//---------------------------------------------------------------------------
void TManageForm::ShowModuleinService(TBitBtn *btn, TPanel *pan)
{
// ������ � ��������� ������������ , ������ ��������������� ��������� ��������, ��������� � ���
	pan->Color = off;
	pan->Caption = mod_off;
	btn->Caption = mod_work;
	btn->Glyph = mod_wrk;

	if ( butt_enabled )
		btn->Enabled = true;
	else
		btn->Enabled = false;
}
//---------------------------------------------------------------------------

void TManageForm::ShowModuleMoving(TBitBtn *btn, TPanel *pan)
{
// ������ ������������ �� ������ ��������� � ������ , ������ ��������������� ��������� ��������, ��������� � ���
	pan->Color = move;
	pan->Caption = mod_mov;
	btn->Enabled = false;
	btn->Caption = mod_dsbl;
	btn->Glyph = mod_mvg;
}
//---------------------------------------------------------------------------
void TManageForm::ShowModuleState( byte state, TBitBtn *btn, TPanel *pan )
{
	switch (state)
	{
		case 0:
			ShowModuleMoving(btn, pan);
			break;
		case 1:
			ShowModuleinWork(btn, pan);
			break;
		case 2:
			ShowModuleinService(btn, pan);
			break;
	}
}

//---------------------------------------------------------------------------
void TManageForm::ShowLoaderUp(TBitBtn *btn, TPanel *pan)
{
	pan->Color = off;                      //  GAN ****************
	pan->Caption = load_Up;
	btn->Glyph = load_UP;
	btn->Caption = load_st + IntToStr(btn->Tag)+" ������";
}
//---------------------------------------------------------------------------
void TManageForm::ShowLoaderDown(TBitBtn *btn, TPanel *pan)
{
	pan->Color = on;                      //  GAN ****************
	pan->Caption = load_Down;
	btn->Glyph = load_Dwn;
	btn->Caption = load_st + IntToStr(btn->Tag)+" ������";
}
//---------------------------------------------------------------------------
void TManageForm::ShowLoaderMoving(TBitBtn *btn, TPanel *pan)
{
	pan->Color = move;                // GAN *********************
	pan->Caption = load_mow;
	btn->Caption = load_work;
	btn->Glyph = mod_mvg;
}
//---------------------------------------------------------------------------
void TManageForm::ShowLoaderState( byte state, TBitBtn *btn, TPanel *pan )
{
	switch (state)
	{                      				// GAN *********************
		case 0:
			ShowLoaderMoving(btn, pan);
			break;
		case 1:
			ShowLoaderDown(btn, pan);
			break;
		case 2:
			ShowLoaderUp(btn, pan);
			break;
	}
}
//---------------------------------------------------------------------------
void TManageForm::ShowLoadPosAllowed(TPanel *pan1, TPanel *pan2)
{
	pan1->Color = on;                // GAN *********************
	pan2->Color = on;
	pan2->Caption = load_offoff;
	pan1->Caption = "�����������";
}
//---------------------------------------------------------------------------
void TManageForm::ShowLoadPosDenied(TPanel *pan1, TPanel *pan2)
{
	pan1->Color = move;                // GAN *********************
	pan2->Color = move;
	pan2->Caption = load_onon;
	pan1->Caption = "�����������";
}
//---------------------------------------------------------------------------
void TManageForm::ShowLoadPosition( byte position, TPanel *pan1, TPanel *pan2 )
{
	switch (position)
	{                      // GAN *********************
     	case 0,1:
			ShowLoadPosAllowed(pan1, pan2);
			break;
		case 2:
			ShowLoadPosDenied(pan1, pan2);
			break;
		case 3:
			ShowLoadPosDenied(pan1, pan2);
			break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::InputTimerTimer(TObject *Sender)
{
// ������� ��������� �������
	if (SystemConst::LinearCentralizer)
	{
		if ( digital->CheckInSignal("���� ����� ���") )
			ShowCentralizerOpened(bLinearCentralizer,pLinearCentralizer);
		else ShowCentralizerClosed(bLinearCentralizer,pLinearCentralizer);
	}
	if (SystemConst::ThickCentralizer)
	{
		if ( digital->CheckInSignal("���� ����� ���") )
			ShowCentralizerOpened(bThicknessCentralizer,pThicknessCentralizer);
		else ShowCentralizerClosed(bThicknessCentralizer,pThicknessCentralizer);
	}
// ������� ��������� ������� � ������������� �� ����� ������� �����
	byte pp_state, pr_state, th_state, load1_state, load2_state, load1_position, load2_position; // GAN *****************
	pr_state = digital->CheckInSignal("���� ��")  + ( ( digital->CheckInSignal("���� ��") ) <<1 );
	pp_state = digital->CheckInSignal("����� ��") + ( ( digital->CheckInSignal("����� ��") )<<1 );

	ShowModuleState ( pr_state , bLinearState	, pLinearState		);
	ShowModuleState ( pp_state , bCrossState	, pCrossState		);

	if (SystemConst::ThickModuleManage)
	{
		th_state = digital->CheckInSignal("���� ��") + ( ( digital->CheckInSignal("���� ��") ) <<1 );
		ShowModuleState ( th_state , bThicknessState, pThicknessState	);
	}

	if (SystemConst::ExternalTransport)   // GAN *****************
	{
		load1_state = digital->CheckInSignal("�� ������1 �����") + ((digital->CheckInSignal("�� ������1 ������")) <<1);  // GAN *****************
		load2_state = digital->CheckInSignal("�� ������2 �����") + ((digital->CheckInSignal("�� ������2 ������")) <<1);  // GAN *****************
		load1_position = digital->CheckInSignal("�� �������") + ((digital->CheckInSignal("�� ������ �����")) <<1);
		load2_position = digital->CheckInSignal("�� ����� �����") + ((digital->CheckInSignal("�� ���������")) <<1);
		ShowLoaderState ( load1_state , bLoader1, pLoader1 ); // GAN *****************
		ShowLoaderState ( load2_state , bLoader2, pLoader2 ); // GAN *****************
		ShowLoadPosition ( load1_position, pStillage, pBegin); // GAN *****************
		ShowLoadPosition ( load2_position, pEndLines, pNextLines); // GAN *****************
	}

// �������, �������� �� ������� ���������� ��� ���
	if ( !( digital->CheckOutSignal("�������� �������") ) )
		bPowerSolenoid->Kind = bkCancel;
	else
		bPowerSolenoid->Kind = bkOK;
	bPowerSolenoid->Cancel = false;
	bPowerSolenoid->Caption = "�������";

// �������, �������� �� ���������� ���������� ��� ���
	if ( !( digital->CheckOutSignal("�������� ������")) )
		bCoolerSolenoid->Kind = bkCancel;
	else
		bCoolerSolenoid->Kind = bkOK;
	bCoolerSolenoid->Cancel = false;
	bCoolerSolenoid->Caption = "����������";

// �������, �������� �� �������� ����������� ��� ���
	if ( digital->CheckOutSignal("���� STF"))
	{
		bRotation->Glyph = mod_mvg;
		cbRL->Enabled = false;
		cbRM->Enabled = false;
		cbRH->Enabled = false;
	}
	else
	{
		bRotation->Glyph = mod_wrk;
		cbRL->Enabled = true;
		cbRM->Enabled = true;
		cbRH->Enabled = true;
	}
// ������� �� ���� ����������, ���� ���� �������� ����������� ��� ����������
	if ( !digital->CheckInSignal("���� ����������") && digital->CheckOutSignal("���� STF"))
	{
//		digital->ResetOutSignal("������� ��");
		digital->ResetOutSignal("���� STF");
		digital->ResetOutSignal("���� �������� 1");
		digital->ResetOutSignal("���� �������� 2");
		digital->ResetOutSignal("���� �������� 3");
		StatusBarBottom->Panels->Items[0]->Text="������ ���������� ��������";
	}
	if ( !digital->CheckInSignal("���� ����������")
		&& ( digital->CheckOutSignal("��: STF") || digital->CheckOutSignal("��: STR") ) )
	{
		bStopClick(Sender);
		StatusBarBottom->Panels->Items[0]->Text="��������� ���������� ��������";
	}
}
//---------------------------------------------------------------------------
void __fastcall TManageForm::bAnyCentralizerClick(TObject *Sender)
{
	TBitBtn *Item = dynamic_cast<TBitBtn *>(Sender);
	String SignalName;

	if ( Item->Name == bThicknessCentralizer->Name )
		SignalName = "���� �����";	// ���� ����� �������
	else if ( Item->Name == bLinearCentralizer->Name)
		SignalName = "���� �����";	// ���� ����� �����������

	if (Item->Caption==centr_open)
		SignalName += " �������";		// "�������" ��������� (�����)
	else if (Item->Caption==centr_close)
		SignalName += " �����";		// "�����" ��������� (�����)

	digital->BlinkOutSignal(SignalName);
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bAnyStateClick(TObject *Sender)
{
	TBitBtn *Item = dynamic_cast<TBitBtn *>(Sender);
	String Module;				// �������� ������, � ������� ��������

	if ( Item->Name == bThicknessState->Name)
		Module = "����";				// ���� ������� �������

	else if ( Item->Name == bLinearState->Name)
		Module = "����";				// ���� ������� ����������

	else if ( Item->Name == bCrossState->Name)
		Module = "�����";				// ���� ������� ����������

	String job = Item->Caption;				// ������� �� ������ ��������� �� ������, ������� ���� ��������� � �������

	StatusBarBottom->Panels->Items[0]->Text=" ";
	StatusBarBottom->Refresh();

	// ���������, ��� �� ����� �� ����� � ������ �� ������
	if  ( !( (digital->CheckInBouncelessSignal(Module + " ����",false))
			|| (digital->CheckInBouncelessSignal(Module + " �����",false) )))
	{
		if ( job == mod_service )
			StatusBarBottom->Panels->Items[0]->Text = ModulePosition::SetModuleInService(Module);
		else if ( job == mod_work )
			StatusBarBottom->Panels->Items[0]->Text = ModulePosition::SetModuleInWork ( Module );
	}
	else
		StatusBarBottom->Panels->Items[0]->Text="����� � ������ - �������� ���������!";
}
//---------------------------------------------------------------------------
void __fastcall TManageForm::bForwardClick(TObject *Sender)
{
// user ����� ������� ����� ������, ��������� ����������

	if (  ( digital->CheckInSignal("���� ��") && !digital->CheckInSignal("���� ����� ���") ) ||
				( digital->CheckInSignal("���� ��") && !digital->CheckInSignal("���� ����� ���") ) ) {
			StatusBarBottom->Panels->Items[0]->Text="������ �� � ��������!";
	}

	if ( ( !digital->CheckInSignal("���� ��") && !digital->CheckInSignal("���� ��") ) ||
				( !digital->CheckInSignal("���� ��") && !digital->CheckInSignal("���� ��") ) ||
				( !digital->CheckInSignal("����� ��") && !digital->CheckInSignal("����� ��") )  ) {
			StatusBarBottom->Panels->Items[0]->Text="������ ���������!";
		 }
	else
	{
		digital->SetOutSignal("������ �������");
		if ( digital->WaitForInSignal("������ ������",5000) )
		{
			digital->SetOutSignal("��: ��� ��");
			for (int s = 0; s < Globals::speeds.transitSpeed.size(); s++)
				digital->SetOutSignal("��: �������� " + IntToStr(Globals::speeds.transitSpeed[s]));
			digital->SetOutSignal("��: STF");
			if(digital->SignalExists("��� ������") )
				digital->SetOutSignal("��� ������");
			bBackward->Enabled = false;
			butt_enabled = false;

			if (SystemConst::ExternalTransport)     //GAN************
			{
				digital->SetOutSignal("�� ������� ��"); //GAN************
				for (int t = 0; t< Globals::speeds.transitSpeed.size(); t++)   //GAN************
					digital->SetOutSignal("�� �������� " + IntToStr(Globals::speeds.transitSpeed[t]));
				digital->SetOutSignal("�� �������� ������");   //GAN************
			}
			StatusBarBottom->Panels->Items[0]->Text="����� �������� ������";
		}
		else
			StatusBarBottom->Panels->Items[0]->Text="������ ����������� �����!";
	}

	StatusBarBottom->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bBackwardClick(TObject *Sender)
{
// user ����� ����� �����, ��������� ��� ����������
	if ( !( digital->CheckInSignal("���� ��")) ||
			!( digital->CheckInSignal("���� ��" )) ||
			!( digital->CheckInSignal("����� ��" ) ) ) {
		StatusBarBottom->Panels->Items[0]->Text="������ �� � ��������� ������������!";
	}
	else if ( digital->CheckInSignal("���� ����") ||
				digital->CheckInSignal("���� �����") ||
				digital->CheckInSignal("���� ����")  ||
				digital->CheckInSignal("���� �����") ||
				digital->CheckInSignal("����� ����") ||
				digital->CheckInSignal("����� �����") ) {
		StatusBarBottom->Panels->Items[0]->Text="����� � �������!";
	}
// ���������� ������, ������������� ����� ������ ����������� �����
	else
	{
		digital->SetOutSignal("������ �������");
		if ( digital->WaitForInSignal("������ ������",5000) )
		{
			digital->SetOutSignal("��: ��� ��");
			for (int s = 0; s < Globals::speeds.returnSpeed.size(); s++)
				digital->SetOutSignal("��: �������� " + IntToStr(Globals::speeds.returnSpeed[s]));
			digital->SetOutSignal("��: STR");
			if( digital->SignalExists("��� ��") )
				digital->SetOutSignal("��� ��");
			bForward->Enabled = false;
			butt_enabled = false;

			if (SystemConst::ExternalTransport)     //GAN************
			{
				digital->SetOutSignal("�� ������� ��");  //GAN************
				for (int t = 0; t < Globals::speeds.returnSpeed.size(); t++)
					digital->SetOutSignal("�� �������� " + IntToStr(Globals::speeds.returnSpeed[t]));
				digital->SetOutSignal("�� �������� �����");  //GAN************
			}

			StatusBarBottom->Panels->Items[0]->Text="����� �������� �����";
		}
		else
			StatusBarBottom->Panels->Items[0]->Text="������ ����������� �����!";
	}
	StatusBarBottom->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bStopClick(TObject *Sender)
{
// ���������� ������� � ��������������� �������
//	digital->ResetOutSignal("��: ��� ��");
	digital->ResetOutSignal("��: �������� 1");
	digital->ResetOutSignal("��: �������� 2");
	digital->ResetOutSignal("��: �������� 3");

    if (SystemConst::ExternalTransport)
	{
		digital->ResetOutSignal("�� �������� 1");
		digital->ResetOutSignal("�� �������� 2");
		digital->ResetOutSignal("�� �������� 3");
		digital->ResetOutSignal("�� �������� ������");
		digital->ResetOutSignal("�� �������� �����");
	}
	else
	{
		if( digital->SignalExists("��� ��") )
		digital->ResetOutSignal("��� ��");
	}

	digital->ResetOutSignal("��: STR");
	digital->ResetOutSignal("��: STF");
//���������� ������� � ����������� �����
	digital->ResetOutSignal("������ ����");
	digital->ResetOutSignal("������ �������");
	if (SystemConst::CentralizersCard)
	{
		digital->ResetOutSignal("����� ����");
		digital->ResetOutSignal("����� �������");
	}
	bForward->Enabled = true;
	bBackward->Enabled = true;
	butt_enabled = true;

	StatusBarBottom->Panels->Items[0]->Text="�������� ����� �����������";
	StatusBarBottom->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bPowerSolenoidClick(TObject *Sender)
{
	String s;
	if ( lt->isOkTemp("1") || lt->isOkTemp("2") )
	   //	StatusBarBottom->Panels->Items[0]->Text="��������� ���������!";
	   s = "��������� ���������!  ";

 //	else
	{
		if ( !( digital->CheckOutSignal("�������� �������") ) )
		{
			digital -> SetOutSignal("�������� �������");
			s += " �������� �������";
		}
		else //if ( digital->CheckOutSignal("�������� �������") )
		{
			digital -> ResetOutSignal("�������� �������");
			s += " �������� ��������";
        }
	  	StatusBarBottom->Panels->Items[0]->Text= s;
	}
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bCoolerSolenoidClick(TObject *Sender)
{
	if ( !( digital->CheckOutSignal("�������� ������")) )
		digital -> SetOutSignal("�������� ������");
	else if ( digital->CheckOutSignal("�������� ������"))
		digital -> ResetOutSignal("�������� ������");
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::FormDestroy(TObject *Sender)
{
	delete centr_cl;
	delete centr_op;
	delete mod_serv;
	delete mod_wrk;
	delete mod_mvg;
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bRotationClick(TObject *Sender)
{
// user ����� ������� ������ ����������, ��������� ����������
	if ( !digital->CheckInSignal("���� ��") )
		StatusBarBottom->Panels->Items[0]->Text="������ �� � ������� ���������!";

	else if ( !cbRL->Checked && !cbRM->Checked && !cbRH->Checked )
		StatusBarBottom->Panels->Items[0]->Text="�������� �������� �� �������!";

	else
	{
		if ( !digital->CheckOutSignal("���� STF") )
		{
			digital->SetOutSignal("���� ��� ��");
			digital->SetOutSignal("���� STF");
			if ( cbRL->Checked )
				digital->SetOutSignal("���� �������� 1");
			if ( cbRM->Checked )
				digital->SetOutSignal("���� �������� 2");
			if ( cbRH->Checked )
				digital->SetOutSignal("���� �������� 3");
			StatusBarBottom->Panels->Items[0]->Text="������ ���������";
		}
		else if ( digital->CheckOutSignal("���� STF") )
		{
//			digital->ResetOutSignal("���� ��� ��");
			digital->ResetOutSignal("���� STF");
			digital->ResetOutSignal("���� �������� 1");
			digital->ResetOutSignal("���� �������� 2");
			digital->ResetOutSignal("���� �������� 3");
			StatusBarBottom->Panels->Items[0]->Text="������ ����������";
		}
	}
	StatusBarBottom->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::TemperatureTimerTimer(TObject *Sender)
{
	UnicodeString a;
	double buf   = lt->GetTemperature(0);
	EditT1->Text = a.sprintf(L"%0.1f", buf);
	a            = "";
	buf          = lt->GetTemperature(1);
	EditT2->Text = a.sprintf(L"%0.1f", buf);
	a            = "";
	buf          = lt->GetMagnetic();
	EditMg->Text = a.sprintf(L"%0.1f", buf);
}
//---------------------------------------------------------------------------


void __fastcall TManageForm::bStartLIRClick(TObject *Sender)
{
	lCounter = new Advantech1784(false);
	CounterTimer->Enabled = true;
	StatusBarBottom->Panels->Items[0]->Text="�������� ��������";
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bResetLIRClick(TObject *Sender)
{
	lCounter->Reset();
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bStopLIRClick(TObject *Sender)
{
	CounterTimer->Enabled = false;
	lCounter->Terminate();
	bool tr = lCounter->WaitFor();
	if (tr)
		StatusBarBottom->Panels->Items[0]->Text="�������� ����������� ��� ������";
	else if (!tr)
		StatusBarBottom->Panels->Items[0]->Text="������� ����������� c ��������";

}
//---------------------------------------------------------------------------

void __fastcall TManageForm::CounterTimerTimer(TObject *Sender)
{
	memoCounters->Clear();
	memoCounters->Lines->Add("������� 1: " + IntToStr((__int64)lCounter->GetCounter(0)) );
	memoCounters->Lines->Add("������� 2: " + IntToStr((__int64)lCounter->GetCounter(1)));
	memoCounters->Lines->Add("������� 3: " + IntToStr((__int64)lCounter->GetCounter(2)));
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bLoader1Click(TObject *Sender)
{
//GAN************
	bLoader1->Enabled=false;
	bLoader2->Enabled=false;
	if  (pBegin->Caption=="���������!")
	{
		String res = ExtTransport::Load();
		if (res == "ok")
			StatusBarBottom->Panels->Items[0]->Text="����������� 1 ��������� �����";
		else
			StatusBarBottom->Panels->Items[0]->Text="������ ������ ������������ � ������ �����!";
	}
	else
		StatusBarBottom->Panels->Items[0]->Text="������ ������ ������������ � ������ �����!";
		StatusBarBottom->Refresh();
	bLoader1->Enabled=true;
	bLoader2->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TManageForm::bLoader2Click(TObject *Sender)
{
//GAN************
	bLoader1->Enabled=false;
	bLoader2->Enabled=false;
	if  (pNextLines->Caption=="���������!")
	{
		String res = ExtTransport::UnloadTubeOutOfLine();
		if (res == "ok")
			StatusBarBottom->Panels->Items[0]->Text="����������� 2 ��������� �����";
		else
			StatusBarBottom->Panels->Items[0]->Text="������ ������ ������������ 2!";

	}
	StatusBarBottom->Panels->Items[0]->Text="������ ������ ������������ � ����� �����!";
	StatusBarBottom->Refresh();
	bLoader1->Enabled=true;
	bLoader2->Enabled=true;
}
//---------------------------------------------------------------------------

