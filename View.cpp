//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "View.h"
#include "Main.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TViewForm *ViewForm;
//---------------------------------------------------------------------------
__fastcall TViewForm::TViewForm(TComponent* Owner)
	: TForm(Owner)
{
	int CrossPos = 0;			//Начальная позиция Поперечного
	int LinearPos = 0;			//Начальная позиция Продольного
	GainEnable = true;           //Начальная позиция Навального
}
//---------------------------------------------------------------------------
void __fastcall TViewForm::FormDestroy(TObject *Sender)
{
	delete ini;
}
//---------------------------------------------------------------------------

void TViewForm::ViewFormInit()
{
	this->ViewCrossChart->Title->Visible=false;
	this->ViewCrossChart->BottomAxis->Visible=true;
	this->ViewCrossChart->BottomAxis->Automatic=false;
	this->ViewCrossChart->BottomAxis->Minimum=0;
	this->ViewCrossChart->BottomAxis->Maximum=Globals::max_zones;
	this->ViewCrossChart->LeftAxis->Visible=true;
	this->ViewCrossChart->LeftAxis->Automatic=false;
	this->ViewCrossChart->LeftAxis->Minimum=0;
	this->ViewCrossChart->LeftAxis->Maximum=12;
	this->ViewCrossChart->BottomAxis->EndPosition=99;
	//this->LinearDefectChart->Zoom->Allow=false;
	//this->LinearDefectChart->AllowPanning = TPanningMode::pmNone;
	for (int i =0; i < 12; i++)
	{
		ViewCrossChart->Series[i]->ColorEachPoint=true;
		ViewCrossChart->Series[i]->Marks->Visible=false;
		((TBarSeries*) ViewCrossChart->Series[i])->BarWidthPercent=100;
		((TBarSeries*) ViewCrossChart->Series[i])->MultiBar = mbStacked;//TMultiBar(3);
		((TBarSeries*) ViewCrossChart->Series[i])->SideMargins=false;
		((TBarSeries*) ViewCrossChart->Series[i])->OffsetPercent=50;
	}

	this->ViewLinearChart->Title->Visible=false;
	this->ViewLinearChart->BottomAxis = ViewCrossChart->BottomAxis;
	this->ViewLinearChart->LeftAxis = ViewCrossChart->LeftAxis;
	this->ViewLinearChart->LeftAxis->Maximum = Globals::LinSensors;
	for (int i = 0; i < Globals::LinSensors; i++)
	{
		ViewLinearChart->Series[i]->ColorEachPoint=true;
		ViewLinearChart->Series[i]->Marks->Visible=false;
		((TBarSeries*) ViewLinearChart->Series[i])->BarWidthPercent=100;
		((TBarSeries*) ViewLinearChart->Series[i])->MultiBar = mbStacked;//TMultiBar(3);
		((TBarSeries*) ViewLinearChart->Series[i])->SideMargins=false;
		((TBarSeries*) ViewLinearChart->Series[i])->OffsetPercent=50;
	}


	int ws = Screen->Width;
	total_charts = Math::Max(pp1->sensors,pr1->sensors);
	byte cols = 4;         		// кол-во столбцов, в которых расположены графики
	short vm = 10, hm = 20;		// верт отступ, гориз отступ
	short chh = 150, chw = 300;	// высота чарта, ширина чарта
	short bvm = 400;			// большой отступ сверху (для 2 главных графиков)
// если экран большой, то форма просмотра не на весь экран
	if (ws > 1360)
	{
		this->gbCross->Width = 1360;
		this->gbLinear->Width = 1360;
		ViewForm->Height=bvm+Ceil((float)total_charts/cols)*(chh+vm*3);
		ViewForm->Width=Math::Max(hm*2 + cols*(chw+hm),ViewCrossChart->Width+hm*2);
	}
	else
	{
		this->gbCross->Width = ws-2*hm;
		this->gbLinear->Width = ws-2*hm;
		ViewForm->WindowState=wsMaximized;
		chw=(ws-hm)/cols - hm;
	}

// создаем графики для результатов
	for (int i = 0; i < total_charts; i++)
	{
		arc[i] = new TChart(ViewForm);
		arc[i]->Parent = ViewForm;
		arc[i]->SetBounds( hm + (i%cols)*(chw+hm), bvm + (i/cols)*(chh+vm),chw,chh);
		arc[i]->Name="Chart_"+IntToStr(i);
		arc[i]->Legend->Visible = false;
		arc[i]->View3D = false;
		arc[i]->BottomAxis->Visible = true;
		arc[i]->BottomAxis->Automatic = true;
		arc[i]->LeftAxis->Automatic = false;
		arc[i]->ParentColor = false;
		arc[i]->LeftAxis->Minimum = 0;
		arc[i]->LeftAxis->Maximum = 100;
		arc[i]->Walls->Back->Transparent = false;
		arc[i]->Walls->Back->Transparency = 30;
		arc[i]->BackWall->Color = clDkGray;
		arc[i]->Title->Caption = L"Датчик " + IntToStr(i+1);
		arc[i]->AddSeries(new TBarSeries(Owner));
		arc[i]->AddSeries(new TLineSeries(Owner));
		arc[i]->AddSeries(new TLineSeries(Owner));
		arc[i]->Series[0]->Marks->Visible = false;
		((TBarSeries*) arc[i]->Series[0])->BarPen->Visible = false;

		arGain[i]=new TEdit(ViewForm);
		arGain[i]->Parent=ViewForm;
		arGain[i]->SetBounds(hm + (i%cols)*(chw+hm)+(chw-40),bvm + (i/cols)*(chh+vm),33,21);
		arGain[i]->Name="gain"+IntToStr(i);
		arGain[i]->Tag=i;
		arGain[i]->Visible = !SystemConst::HideInGain;
		arGain[i]->BringToFront();
		arGain[i]->OnKeyDown = FormKeyDown;
		arGain[i]->OnKeyPress = EditKeyPress;
		arGain[i]->Clear();
		arGain[i]->Refresh();
	}
	ViewForm->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TViewForm::FormShow(TObject *Sender)
{
	ViewFormInit();
	pp1->PutResultOnChart(ViewCrossChart,ViewForm);
	pr1->PutResultOnChart(ViewLinearChart,ViewForm);
}
//---------------------------------------------------------------------------
void TViewForm::SendResults(Result &res1, Result &res2)
{
	pp1 = &res1;
	pr1 = &res2;
}
//---------------------------------------------------------------------------
void __fastcall TViewForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	if(SystemConst::HideInGain)
		UnregisterHotKey(MainForm->Handle, // Handle окна
			0x01F); // наш идентификатор горячего ключа

	for (int i = 0; i < total_charts; i++)
	{
		arc[i]->Series[0]->Clear();
		arc[i]->Series[1]->Clear();
		arc[i]->Series[2]->Clear();
		arc[i]->RemoveAllSeries();
		delete arc[i] ;
		delete arGain[i];
	}
}
//---------------------------------------------------------------------------

void __fastcall TViewForm::ViewCrossChartClickSeries(TCustomChart *Sender, TChartSeries *Series,
		  int ValueIndex, TMouseButton Button, TShiftState Shift, int X, int Y)
{
// вывод измерений по всем датчикам в конкретной зоне (поперечный)
	String gain_str = "Gain " + IntToStr(Globals::current_diameter);
	CrossPos=ValueIndex;
	int i;
	for ( i = 0; i < pp1->sensors; i++)
	{
		arc[i]->Visible=true;
		arGain[i]->Visible=true;
//		ViewCrossChart->Series[i]->ValueColor[ValueIndex]=SelectedColor;
	 //todo 1605
	 pp1->PutDataOnChart(arc[i],Sender,ValueIndex,i);
		arGain[i]->Text = FloatToStr(ini->ReadFloat( gain_str,"Gain"+IntToStr(i),1) );
	}
// выводим номер зоны в соответствующую панель
	this->pCrossZone->Caption="Зона: "+IntToStr(ValueIndex+1);
	this->pCrossZone->Tag=ValueIndex;
	this->pLinearZone->Caption="";
// делаем ненужные чарты невидимыми
	for (; i < total_charts; i++)
	{
		arc[i]->Visible=false;
		arGain[i]->Visible=false;
	}
	ViewCrossChart->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TViewForm::ViewLinearChartClickSeries(TCustomChart *Sender, TChartSeries *Series,
		  int ValueIndex, TMouseButton Button, TShiftState Shift, int X, int Y)
{
// вывод измерений по всем датчикам в конкретной зоне (продольный)
	LinearPos=ValueIndex;
	int i;
	for (i = 0; i < pr1->sensors; i++)
	{
		arc[i]->Visible=true;
		arGain[i]->Visible=true;
//		ViewLinearChart->Series[i]->ValueColor[ValueIndex]=SelectedColor;
  //todo 1605
  pr1->PutDataOnChart(arc[i],Sender,ValueIndex,i);
		arGain[i]->Text=FloatToStr(ini->ReadFloat("PR","Gain"+IntToStr(i),1));
	}
	this->pLinearZone->Caption="Зона: "+IntToStr(ValueIndex+1);
	this->pLinearZone->Tag=ValueIndex;
	this->pCrossZone->Caption="";
	for (; i < total_charts; i++)
	{
		arc[i]->Visible=false;
		arGain[i]->Visible=false;
	}
	ViewLinearChart->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TViewForm::ChartClick(TObject *Sender)
{
	if ( repaint )
	{
		pp1->ComputeZoneData();
		pr1->ComputeZoneData();
		repaint = false;
	}
	pp1->PutResultOnChart(ViewCrossChart,ViewForm);
	pr1->PutResultOnChart(ViewLinearChart,ViewForm);
}
//---------------------------------------------------------------------------


void __fastcall TViewForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == 27)
		ViewForm->Close();

	if(Key == VK_RETURN)
	{   if( !arc[Globals::LinSensors]->Visible )    //Если продольная
		{
				ini->WriteFloat("PR","Gain"+IntToStr( ( (TEdit*)Sender )->Tag),StrToFloat(( (TEdit*)Sender )->Text ));
				pr1->SetSensorGain( StrToFloat(( (TEdit*)Sender )->Text) , ((TEdit*)Sender )->Tag);
				repaint = true;
		}
		else					//если поперечная
		{
				String gain_str = "Gain " + IntToStr(Globals::current_diameter);
				ini->WriteFloat( gain_str , "Gain"+IntToStr( ( (TEdit*)Sender )->Tag),StrToFloat(( (TEdit*)Sender )->Text ));
				pp1->SetSensorGain( StrToFloat(( (TEdit*)Sender )->Text) , ((TEdit*)Sender )->Tag);
				repaint = true;
		}
	}

}
//---------------------------------------------------------------------------

void __fastcall TViewForm::EditKeyPress(TObject *Sender, wchar_t &Key)
{
	if( !( (Key >= '0' && Key <= '9') || (Key == VK_BACK) || (Key == ',') ) )
		Key = 0x00;
}
//---------------------------------------------------------------------------


void __fastcall TViewForm::FormActivate(TObject *Sender)
{
	if(SystemConst::HideInGain)
	{
		bool RHKret = RegisterHotKey(MainForm->Handle,
			// Handle окна, которому отправлять сообщения WM_HOTKEY
			0x01F, // УСЛОВНЫЙ идентификатор горячего ключа
			MOD_ALT+MOD_CONTROL, // модификатор
			VK_ADD); // код клавиши +
	}

	SelectedColor=ini->ReadInteger("Color","SelectedZone",16711680);
	ViewCrossChart->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TViewForm::FormCreate(TObject *Sender)
{
	ini = new TIniFile(Globals::IniFileName);
}
//---------------------------------------------------------------------------


void __fastcall TViewForm::ApplicationEventsMessage(tagMSG &Msg, bool &Handled)
{
	if (Msg.message == WM_HOTKEY)
	{
		if (Msg.wParam == 0x01F)
		{
			if (SystemConst::HideInGain)
			{
				GainEnable = !arGain[0]->Visible;
				if( !arc[Globals::LinSensors]->Visible )
					for(int i=0; i < Globals::LinSensors; i++)
						arGain[i]->Visible=GainEnable;
				else
					for(int i=0; i<total_charts; i++)
						arGain[i]->Visible=GainEnable;
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TViewForm::ViewCrossChartKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == 27)
		ViewForm->Close();
	else
	{
	if(Key == 40)
	{
		if (pr1->zones > 0)
			ViewLinearChart->SetFocus();
	}
	else
		if(Key==39)
		{
			CrossPos++;
				if(CrossPos >= pp1->zones )
					CrossPos = pp1->zones-1;
		}
		else
		if(Key==37)
			{
				CrossPos--;
					if(CrossPos < 0)
						CrossPos=0;
			}
	if( (Key==37) || (Key==39) )
	{
		int i;
		pp1->PutResultOnChart(ViewCrossChart,ViewForm);
		String gain_str = "Gain " + IntToStr(Globals::current_diameter);
		for ( i = 0; i < pp1->sensors; i++)
		{
			arc[i]->Visible=true;
			arGain[i]->Visible=true;
			ViewCrossChart->Series[i]->ValueColor[CrossPos]=SelectedColor;
	//todo 1605		pp1->PutDataOnChart(arc[i],ViewCrossChart,CrossPos,i);
			arGain[i]->Text = FloatToStr(ini->ReadFloat(gain_str,"Gain"+IntToStr(i),1) );
		}
	// выводим номер зоны в соответствующую панель
		this->pCrossZone->Caption="Зона: "+IntToStr(CrossPos+1);
		this->pCrossZone->Tag=CrossPos;
		this->pLinearZone->Caption="";
	// делаем ненужные чарты невидимыми
		for (; i < total_charts; i++)
		{
			arc[i]->Visible=false;
			arGain[i]->Visible=false;
		}
	}
	}
}
//---------------------------------------------------------------------------

void __fastcall TViewForm::ViewLinearChartKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == 27)
		ViewForm->Close();
	else
	{
		if(Key == 38)
		{
			ViewCrossChart->SetFocus();
		}
		if(Key==39)
		{
			LinearPos++;
				if(LinearPos >= pr1->zones )
					LinearPos = pr1->zones-1;
		}
		else
		if(Key==37)
		{
			LinearPos--;
				if(LinearPos < 0)
					LinearPos=0;
		}
		if( (Key==37) || (Key==39) )
		{
			int i;
			pr1->PutResultOnChart(ViewLinearChart,ViewForm);
			for (i = 0; i < pr1->sensors; i++)
			{
				arc[i]->Visible=true;
				arGain[i]->Visible=true;
				ViewLinearChart->Series[i]->ValueColor[LinearPos]=SelectedColor;
			// 1605	pr1->PutDataOnChart(arc[i],ViewLinearChart,LinearPos,i);
				arGain[i]->Text=FloatToStr(ini->ReadFloat("PR","Gain"+IntToStr(i),1));
			}
			this->pLinearZone->Caption="Зона: "+IntToStr(LinearPos+1);
			this->pLinearZone->Tag=LinearPos;
			this->pCrossZone->Caption="";
			for (; i < total_charts; i++)
			{
				arc[i]->Visible=false;
				arGain[i]->Visible=false;
			}
		}
	}
}
//---------------------------------------------------------------------------


