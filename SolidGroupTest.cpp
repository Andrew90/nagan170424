	//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SolidGroupTest.h"
#include "DebugMess.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Chart"
#pragma link "Series"
#pragma link "TeEngine"
#pragma link "TeeProcs"
#pragma resource "*.dfm"
TSGForm *SGForm;
//---------------------------------------------------------------------------
__fastcall TSGForm::TSGForm(TComponent* Owner)
	: TForm(Owner)
{
	for (int i = 0; i < 8; i++)
		thresholds[i] = new	TLineSeries(SignalChart);
}
//---------------------------------------------------------------------------
void __fastcall TSGForm::FormDestroy(TObject *Sender)
{
	for (int i = 0; i < 8; i++)
		delete thresholds[i];
	DragAcceptFiles(Handle, false); // ��������� �������������� ������
}
//---------------------------------------------------------------------------
void __fastcall TSGForm::FormCreate(TObject *Sender)
{

	for (int i = 0; i < quantity_thresholds; i++)
	{
		int left = 2;
		int width = pThresholds->Width-left*2;
		int top = 10;
		int height = 21;
		eThresholds[i] = new TEdit(pThresholds);
		eThresholds[i] -> Parent = pThresholds;
		eThresholds[i] -> SetBounds(left,height*i+top,width,height);
		eThresholds[i] -> Name = L"eThresholds" + IntToStr(i);
		eThresholds[i] -> Clear();
		eThresholds[i] -> OnKeyDown = EditKeyDown;
		eThresholds[i] -> Refresh();
		eThresholds[i] -> Tag = i;

	}
	tube_sg = SolidGroups::Tube();
	DragAcceptFiles(Handle, true); // ��������� �������������� ������
}
//------------------------------------------------------------------------------
void __fastcall TSGForm::EditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if( Key == 40)
	{
		int tag = ( (TEdit*) Sender )->Tag;
		if(tag == 7)
			tag = 0;
		else
			tag++;
		eThresholds[tag]->SetFocus();
	}
	else
	if( Key == 38)
	{
		int tag = ( (TEdit*) Sender )->Tag;
		if(tag == 0)
			tag = 7;
		else
			tag--;
		eThresholds[tag]->SetFocus();
	}

	if( Key == 0xD )
	{
		//��� �������� ���������� ����� ��������� ������
		vector<int> local_th;
		local_th.resize(quantity_thresholds);

		for(int i=0; i < quantity_thresholds; i++)
		{
			if( eThresholds[i]->Text == "")
				eThresholds[i]->Text = "0";
			local_th[i] = StrToInt(eThresholds[i]->Text);
		}
		tube_sg.setThresholds(local_th);
		DrawOnChart();
	}
	else
	if(!((Key >= '0' && Key <= '9') || (Key == VK_BACK) || (Key == ',') || ('-') )) Key = 0x00;

}
//------------------------------------------------------------------------------
void __fastcall TSGForm::FormShow(TObject *Sender)
{
	tube_sg.ReadThresholds();                                    //��������� ������ �� �������� ����������� �� ini-�����

	vector<int> local_th = tube_sg.getThresholds();
	int ad = local_th.size();                            //������
	for (int i = 0; i < quantity_thresholds; i++)
	{
		int aadsd = local_th[i];                         //������
		eThresholds[i] -> Text = StrToInt(local_th[i]);

	}
	buffer = local_th;	//�������� ����� ����� ������� � �����, ��� ������������ ���������

	lcard=MainForm->l1;
	StatusBar->Panels->Items[0]->Text="";
	StatusBar->Panels->Items[1]->Text="";
	Series->Clear();
	Series1->Clear();
	for (int i = 0; i < 8; i++)
		thresholds[i]->Clear();
	SignalChart->LeftAxis->Minimum = -3500;
	SignalChart->LeftAxis->Maximum = 3500;
	SignalChart->LeftAxis->Automatic = false;
	curPos = scbChartDragger->Position;
}
//---------------------------------------------------------------------------

void __fastcall TSGForm::bTestClick(TObject *Sender)
{
	bOpenTube->Enabled=false;
	bSaveTube->Enabled=false;
	eTestTime->Enabled=false;
	bTest->Enabled=false;
	lcard->ClearBuffer();
	int asd = lcard->getTemperatureForSG();
	tube_sg.setTemperature(asd);
    StatusBar->Panels->Items[2]->Text = "����������� "+IntToStr(asd);
	lcard->InitADC(16,0,"PP",2);
	lcard->InitDAC(true);
	Series->Clear();
	Series1->Clear();
	for (int i = 0; i < 8; i++)
		thresholds[i]->Clear();
	lcard->ClearBuffer();
	StatusBar->Panels->Items[0]->Text="";
	StatusBar->Refresh();

	if(cbIsSignal->Checked)
	{
		if( digital->SignalExists("�� ����") )
		{
//			digital->SetOutSignal("������ ����");
//			if(SystemConst::CentralizersCard)
//				digital->SetOutSignal("����� �������");
//			digital->SetOutSignal("��: ��� ��");
//			digital->SetOutSignal("��: �������� 1");
//			digital->SetOutSignal("��: stf");

//			int WaitingTime=30000;
			if( digital->WaitForInSignal("�� ����" , 20000) )
			{
				lcard->Start();
				StatusBar->Panels->Items[0]->Text="������� ���� ������";
				StatusBar->Refresh();
				if( digital->WaitForDropInSignal("�� ����", 45000) )
				{
					lcard->Stop();
					StatusBar->Panels->Items[0]->Text = "���� ������ �������� �������";
					Signal = lcard->PrepareSGData(true);
					DrawOnChart();
				}
				else
				{
					lcard->Stop();
					StatusBar->Panels->Items[0]->Text="���� ������ ���������� - �� ��������� ������ ����� �� ������";
				}
			}
			else
				StatusBar->Panels->Items[0]->Text="�� ��������� ����� � ������";
		}
		else
			StatusBar->Panels->Items[0]->Text="������� ������ �� �� ���������";

//		digital->ResetOutSignal("������ ����");
//		if(SystemConst::CentralizersCard)
//			digital->ResetOutSignal("����� �������");
//		digital->ResetOutSignal("��: �������� 1");
//		digital->ResetOutSignal("��: stf");

	}
	else
	{
		if(eTestTime->Text == "" )
			eTestTime->Text = 5;
		if(StrToInt(eTestTime->Text) > 30)
		   eTestTime->Text="30";
		if(StrToInt(eTestTime->Text) < 0)
		   eTestTime->Text="5";

		int Time=StrToInt(eTestTime->Text);
		StatusBar->Panels->Items[0]->Text="�������� ���� ������ � ��� ������������� "+UnicodeString(Time)+" ���";
		lcard->Start();

		for (DWORD tt = GetTickCount(); GetTickCount() - tt < Time*1000; Sleep(10))
			Application->ProcessMessages();

		lcard->Stop();
		StatusBar->Panels->Items[0]->Text="���� ������ �������� �������";

		Signal=lcard->PrepareSGData(true);
		DrawOnChart();

	}
	using namespace SolidGroups;
	//tube_sg = Tube(Signal, TubeType(Globals::current_diameter));    //������� ������
	String result = tube_sg.calcSolidGroup(tube_sg.Coordinates());

	StatusBar->Panels->Items[0]->Text = "������ ��������� "+result;
	StatusBar->Refresh();

	bOpenTube->Enabled=true;
	bSaveTube->Enabled=true;
	eTestTime->Enabled=true;
    bTest->Enabled=true;
}
//---------------------------------------------------------------------------
void TSGForm::DrawOnChart()
{
	 //������� �����
	Series->Clear();
	Series1->Clear();
	for (int i=0; i < 8; i++)
		thresholds[i]->Clear();
	// ������ ������� �������� � ���������� �������
	double xv = 0;
	for(UINT i = 0; i < Signal.size(); i += 2)
	{
		Series->AddXY(xv,Signal[i],"",clBlue);
		Series1->AddXY(xv,Signal[i+1],"",clRed);
		xv += 0.05;	//zoomFactor;
		dprint("%s %d\n", __FUNCTION__, Signal[i]);
	}
	SignalChart->LeftAxis->Automatic = true;
	SignalChart->BottomAxis->Automatic = false;
	SignalChart->BottomAxis->Minimum = 0;
	SignalChart->BottomAxis->Maximum = 50;
	SignalChart->Refresh();
	if (Signal.size())
	{
		tube_sg.setTubeType(SolidGroups::TubeType(Globals::current_diameter));
		tube_sg.setCoordinates(tube_sg.coordsFromSignal(Signal));
	}
	// ������� 8 ���������� � ������
	if (Signal.size() > 0)
	{
		vector <double> pars = tube_sg.Coordinates();
		String str = "";
		for (unsigned i = 0; i < pars.size(); i++)
			str += FloatToStr(pars[i]) + " ";
		StatusBar->Panels->Items[1]->Text = str;
		StatusBar->Refresh();
	}
	DrawThresholds();
}
//---------------------------------------------------------------------------
void TSGForm::LoadFromFile(UnicodeString FileName)
{
	FILE *file;
	file=fopen(AnsiString(FileName).c_str(), "r");
	if( file == NULL )
		Application->MessageBoxW( L"�� ������� ������� ���� ��� ������",L"������", MB_ICONERROR | MB_OK  );
	int size;
	fscanf(file,"%d \n",&size);
	if( size != 0 )
		{
			Signal.resize(size);
			for(int i = 0; i < size; i += 2 )
			{
				fscanf(file,"%d  ",&Signal[i]);
				fscanf(file,"%d \n",&Signal[i+1]);
			}
			DrawOnChart();
			StatusBar->Panels->Items[0]->Text="���� ������� ������";
		}
	else
		StatusBar->Panels->Items[0]->Text="������ ���� ����";
	fclose(file);
}
//---------------------------------------------------------------------------
void TSGForm::SaveToFile(UnicodeString FileName)
{
	FILE *file;
	file=fopen(AnsiString(FileName).c_str(), "a");
	if( file == NULL )
		Application->MessageBoxW( L"�� ������� ������� ���� ��� ������",L"������", MB_ICONERROR | MB_OK  );
	else
	{
		//������� ����� �������
		fprintf(file,"%d \n",Signal.size() );
		//������� ���� ������
		for( unsigned i = 0; i < Signal.size(); i += 2 )
		{
			fprintf(file,"%d  ",Signal[i]);
			fprintf(file,"%d \n",Signal[i+1]);
		}
		StatusBar->Panels->Items[0]->Text="���� ������� ��������";
	}
	fclose(file);
}
//---------------------------------------------------------------------------
void __fastcall TSGForm::bOpenTubeClick(TObject *Sender)
{
	if(OpenDialog->Execute( ) )
		LoadFromFile(OpenDialog->FileName);
	tbZoom->Position = 1;
	scbChartDragger->Position = 1;
}
//---------------------------------------------------------------------------
void __fastcall TSGForm::bSaveTubeClick(TObject *Sender)
{
	if(	SaveDialog->Execute() )
		SaveToFile(SaveDialog->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TSGForm::bShowCurrentTubeClick(TObject *Sender)
{
	tube_sg = Globals::tubeSG;                   //����� �� �������� ����� �� ��� �������� ��� �������������� ��������� =
	Signal = lcard->PrepareSGData(false);
	tbZoom->Position = 1;
	scbChartDragger->Position = 1;
	DrawOnChart();
}
//---------------------------------------------------------------------------
void TSGForm::DrawThresholds()
{
	vector<int> local_th = tube_sg.getThresholds();
	for (int i = 0; i < local_th.size(); i++)
	{
		thresholds[i]->AddXY(0,local_th[i] , "", clBlack);
		thresholds[i]->AddXY(SignalChart->Series[0]->MaxXValue(),local_th[i] , "", clBlack);
		SignalChart->AddSeries(thresholds[i]);
	}
}
//---------------------------------------------------------------------------

void __fastcall TSGForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == 27)
		SGForm->Close();
}
//---------------------------------------------------------------------------

void __fastcall TSGForm::N1Click(TObject *Sender)
{
	BottomPanel->Visible = !BottomPanel->Visible;

}
//---------------------------------------------------------------------------
void __fastcall TSGForm::tbZoomChange(TObject *Sender)
{
	long max = SignalChart->BottomAxis->Maximum;
	long min = SignalChart->BottomAxis->Minimum;
	long center = max - min;
	SignalChart->BottomAxis->Minimum = center - 25 * tbZoom->Position;
	SignalChart->BottomAxis->Maximum = center + 25 * tbZoom->Position;
	SignalChart->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TSGForm::scbChartDraggerChange(TObject *Sender)
{
	long length = SignalChart->Series[0]->MaxXValue();
	long max = SignalChart->BottomAxis->Maximum;
	long min = SignalChart->BottomAxis->Minimum;
	double step = (double)length / (double) (max-min) / 100.0;
	long change = scbChartDragger->Position - curPos;

	SignalChart->BottomAxis->Maximum = change * step + max;
	SignalChart->BottomAxis->Minimum = change * step + min;
	SignalChart->Refresh();
	lZoom->Caption = SignalChart->SeriesCount();
	curPos = scbChartDragger->Position;
}
//---------------------------------------------------------------------------

void __fastcall TSGForm::N2Click(TObject *Sender)
{
	pThresholds->Visible = !pThresholds->Visible;
}
//---------------------------------------------------------------------------


void __fastcall TSGForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
//������� �� �������� ��� ������ � ����� � �� ��� ���� ������ ����� ������� �����
	vector<int> local_th = tube_sg.getThresholds();
	bool result = false;
	for(int i = 0; i < quantity_thresholds; i++)
	{
		if(local_th[i] != buffer[i])
			{	result = true;	break;	}
	}
	if (result)
	{
		int ID = Application->MessageBoxW(L"��������, ������ ������ ��������� ���� ��������!\n ��������� ���������?",
				L"��������!",MB_YESNOCANCEL | MB_ICONWARNING);
		if( ID == IDYES )
		{
			Globals::tubeSG.setThresholds(local_th);
			Globals::tubeSG.WriteThresholds();
		}
		else
		if(ID == IDNO)
		{
			//������� ������ ��������
			tube_sg.setThresholds(buffer);
			tube_sg.WriteThresholds();
			Close();
		}
		else
		if(ID == IDCANCEL)
			CanClose = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TSGForm::WmDropFiles(TWMDropFiles& Message)
{
   HDROP drop_handle = (HDROP)Message.Drop;
   wchar_t fName[MAXPATH];
   int filenum = DragQueryFile(drop_handle, -1, NULL, NULL);
   for(int i = 0; i < filenum; i++){
	  DragQueryFile(drop_handle, i, fName, MAXPATH);
	  ReadFile(fName);
	 }
   DragFinish(drop_handle);
}
//---------------------------------------------------------------------------------
// ����� ��������������, ��������� ���������� � ��������� ���� -----
void __fastcall TSGForm::ReadFile(AnsiString FileName)
{
   AnsiString str = ExtractFileExt(FileName);
   if(str == ".sg")
		SGForm->LoadFromFile(FileName);
}
//---------------------------------------------------------------------------------

void __fastcall TSGForm::AddParametersClick(TObject *Sender)
{
// ���������� �������� �� ������ ���������
	SolidGroupForm->Send_Tube(tube_sg);
	SolidGroupForm->Show();
}
//---------------------------------------------------------------------------

