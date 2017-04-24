 //---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ADCSettings.h"
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TADCSettForm *ADCSettForm;
//---------------------------------------------------------------------------
__fastcall TADCSettForm::TADCSettForm(TComponent* Owner)
	: TForm(Owner)
{
	KeyPreview=true;
	LIRPage->Visible = SystemConst::IsOnline;
}
//---------------------------------------------------------------------------
void __fastcall TADCSettForm::FormCreate(TObject *Sender)
{
	ini = new TIniFile(Globals::IniFileName);

	int vmar = 40, hmar = 55;
	int height = 15, width = 60;
	for (int i = 0; i < Cross_sensors; i++)
	{
// создаем элементы для настроек поперечной платы
		arLabelPp[i] = new  TLabel( ADCSettForm->ppPage );
		arLabelPp[i] -> Parent = gbPP;
		arLabelPp[i] -> SetBounds(16, vmar + i*(height+20),width,height);
		arLabelPp[i] -> Visible = true;
		arLabelPp[i] -> Caption = "Датчик " + IntToStr(i+1);

		arComboBoxPp[i] = new TComboBox ( ADCSettForm->ppPage );
		arComboBoxPp[i] -> Parent = gbPP;
		arComboBoxPp[i] -> SetBounds(150, vmar + i*(height+20),width,height);
		arComboBoxPp[i] -> Name = L"Range" + IntToStr(i);
		arComboBoxPp[i] -> Items->Add(L"±10");
		arComboBoxPp[i] -> Items->Add(L"±5");
		arComboBoxPp[i] -> Items->Add(L"±2.5");
		arComboBoxPp[i] -> Items->Add(L"±1.25");
		arComboBoxPp[i] -> Items->Add(L"±0.625");
		arComboBoxPp[i] -> Visible = true;
		arComboBoxPp[i] -> Style=csDropDownList;
		arComboBoxPp[i] -> Font->Size=10;
		arComboBoxPp[i] -> ItemIndex = 0;

		arSpinEditPp[i] = new  TCSpinEdit( ADCSettForm->ppPage );
		arSpinEditPp[i] -> Parent = gbPP;
		arSpinEditPp[i] -> SetBounds(100, vmar + i*(height+20),width,height);
		arSpinEditPp[i] -> Name = L"Channel" + IntToStr(i);
		arSpinEditPp[i] -> MinValue = 0;
		arSpinEditPp[i] -> MaxValue = 31;
		arSpinEditPp[i] -> Width = 35;
		arSpinEditPp[i] -> Font->Size=10;
		arSpinEditPp[i] -> AutoSize=false;
		arSpinEditPp[i] -> Visible = true;

		arGainPP[i] = new TEdit( ADCSettForm->ppPage );
		arGainPP[i] -> Parent = gbPP;
		arGainPP[i] -> SetBounds(220, vmar + i*(height+20),width,height+8);
		arGainPP[i] -> AutoSize=false;
		arGainPP[i] -> Name = L"ppGain" + IntToStr(i);
		arGainPP[i] -> Clear();
		arGainPP[i] -> OnKeyPress=ppSizeBufferKeyPress;
		arGainPP[i] -> Refresh();
	}
	for (int i = 0; i < lin_sensors; i++)
	{
// создаем элементы для настроек продольной платы
		arLabelPr[i] = new  TLabel( ADCSettForm->prPage );
		arLabelPr[i] -> Parent = gbPR;
		arLabelPr[i] -> SetBounds(16, vmar + i*(height+20),width,height);
		arLabelPr[i] -> Visible = true;
		arLabelPr[i] -> Caption = "Датчик " + IntToStr(i+1);

		arComboBoxPr[i] = new TComboBox ( ADCSettForm->prPage );
		arComboBoxPr[i] -> Parent = gbPR;
		arComboBoxPr[i] -> SetBounds(150, vmar + i*(height+20),width,height);
		arComboBoxPr[i] -> Name = L"prRange" + IntToStr(i);
		arComboBoxPr[i] -> Items->Add(L"±10");
		arComboBoxPr[i] -> Items->Add(L"±5");
		arComboBoxPr[i] -> Items->Add(L"±2.5");
		arComboBoxPr[i] -> Items->Add(L"±1.25");
		arComboBoxPr[i] -> Items->Add(L"±0.625");
		arComboBoxPr[i] -> Visible = true;
		arComboBoxPr[i] -> Style=csDropDownList;
		arComboBoxPr[i] -> Font->Size=10;
		arComboBoxPr[i] -> ItemIndex = 0;

		arSpinEditPr[i] = new  TCSpinEdit( ADCSettForm->prPage );
		arSpinEditPr[i] -> Parent = gbPR;
		arSpinEditPr[i] -> SetBounds(100, vmar + i*(height+20),width,height);
		arSpinEditPr[i] -> Name = L"prChannel" + IntToStr(i);
		arSpinEditPr[i] -> MinValue = 0;
		arSpinEditPr[i] -> MaxValue = 31;
		arSpinEditPr[i] -> Width = 35;
		arSpinEditPr[i] -> Font->Size=10;
		arSpinEditPr[i] -> AutoSize=false;
		arSpinEditPr[i] -> Visible = true;

		arGainPR[i] = new TEdit( ADCSettForm->prPage );
		arGainPR[i] -> Parent = gbPR;
		arGainPR[i] -> SetBounds(220, vmar + i*(height+20),width,height+8);
		arGainPR[i] -> Name = L"prGain" + IntToStr(i);
		arGainPR[i] -> AutoSize=false;
		arGainPR[i] -> Clear();
		arGainPR[i] -> OnKeyPress=ppSizeBufferKeyPress;
	}

	for (int i = 0; i < Counters; i++)
	{
// создаем элементы для настроек ЛИРов
		arPerimiterOfLir[i] = new TLabeledEdit( ADCSettForm->LIRPage );
		arPerimiterOfLir[i] -> Parent = LIRPage;
		arPerimiterOfLir[i] -> SetBounds(120, vmar + i*(height+20),80,height+8);
		arPerimiterOfLir[i] -> Name = L"PerimeterOfLir" + IntToStr(i);
		arPerimiterOfLir[i] -> AutoSize=false;
		arPerimiterOfLir[i] -> EditLabel->Caption = "Счетчик "+ IntToStr(i+1);
		arPerimiterOfLir[i] -> LabelPosition = lpLeft;
		arPerimiterOfLir[i] -> LabelSpacing = 5;
		arPerimiterOfLir[i] -> Clear();
		arPerimiterOfLir[i] -> OnKeyPress=ppSizeBufferKeyPress;

		arLirChannel[i] = new  TCSpinEdit( ADCSettForm->LIRPage );
		arLirChannel[i] -> Parent = LIRPage;
		arLirChannel[i] -> SetBounds(220, vmar + i*(height+20),width,height);
		arLirChannel[i] -> Name = L"LIRChannel" + IntToStr(i);
		arLirChannel[i] -> MinValue = 0;
		arLirChannel[i] -> MaxValue = 3;
		arLirChannel[i] -> Width = 35;
		arLirChannel[i] -> Font->Size=10;
		arLirChannel[i] -> AutoSize=false;
		arLirChannel[i] -> Visible = true;
	}

	FillAllControls();
}
//---------------------------------------------------------------------------
void TADCSettForm::FillAllControls()
{
// читаем все настройки по датчикам из ini файла
	String gain_str = "Gain "+ IntToStr(Globals::current_diameter);
	for (int i = 0; i < Cross_sensors; i++)
	{
		arComboBoxPp[i] -> ItemIndex = ReadRange( (ini->ReadInteger("PP","Range"+IntToStr(i),555) ) );
		arSpinEditPp[i] -> Value=StrToInt(ini->ReadInteger("PP","Number"+UnicodeString(i),i));
		arGainPP[i] -> Text = ini->ReadFloat( gain_str , "Gain"+IntToStr(i) , 10);
		MainForm->pp.SetSensorGain( StrToFloat(arGainPP[i]->Text) , i);
	}
	for (int i = 0; i < lin_sensors; i++)
	{
		arComboBoxPr[i] -> ItemIndex = ReadRange(ini->ReadInteger("PR","Range" + IntToStr(i),1) );
		arSpinEditPr[i] -> Value=StrToInt(ini->ReadInteger("PR","Number"+UnicodeString(i),i));
		arGainPR[i] -> Text=ini->ReadFloat("PR","Gain"+IntToStr(i),1);
		MainForm->pr.SetSensorGain( StrToFloat(arGainPR[i]->Text) , i);
	}
	this->ppKadr->Text=ini->ReadInteger("PP", "Kadr",0);
	this->ppRate->Text=ini->ReadInteger("PP", "Rate",1);
	this->ppSizeBuffer->Text=ini->ReadInteger("PP", "SizeBuf",100);

	this->prKadr->Text=ini->ReadInteger("PR", "Kadr",0);
	this->prRate->Text=ini->ReadInteger("PR", "Rate",1);
	this->prSizeBuffer->Text=ini->ReadInteger("PR", "SizeBuf",100);
    spinTemp->Value = ini->ReadInteger("OtherSettings", "channelTempSG",5);
//Читаем настройки температуры соленоидов и контроля магнитного поля
	SpinTemp1->Value=ini->ReadInteger("OtherSettings","Temp1",29);
	SpinTemp2->Value=ini->ReadInteger("OtherSettings","Temp2",30);
	SpinMagnetic->Value=ini->ReadInteger("OtherSettings","Magnetic",31);
	//датчики группы прочности
	SpinSG->Value=ini->ReadInteger("OtherSettings","SensorSG",16);
	SpinCurr->Value=ini->ReadInteger("OtherSettings","SensorCurr",17);
	//Вариант расчета ГП
	cbCalcSolidGroup->Checked = ini->ReadBool("OtherSettings","IsCalcMS_DOS", 1);

	if(ini->ReadString("OtherSettings","isMore1","Больше")=="Больше")
		ComboBoxT1->ItemIndex=0;
	else
		ComboBoxT1->ItemIndex=1;

	if(ini->ReadString("OtherSettings","isMore2","Больше") == "Больше")
		ComboBoxT2->ItemIndex=0;
	else
		ComboBoxT2->ItemIndex=1;

	if(ini->ReadString("OtherSettings","isMoreMG","Меньше") == "Больше")
		ComboBoxMG->ItemIndex=0;
	else
		ComboBoxMG->ItemIndex=1;

	EditT1->Text=ini->ReadInteger("OtherSettings","isVMore1",8);
	EditT2->Text=ini->ReadInteger("OtherSettings","isVMore2",8);
	EditMG->Text=ini->ReadInteger("OtherSettings","isVMoreMG",1);
	cbComWithASU->Checked = ini->ReadBool("OtherSettings","ComWithASU", 0);
	rgSolidGroupSource->ItemIndex = ini->ReadInteger("OtherSettings","SolidGroupSource", -1 );
	cbTemp->Checked = ini->ReadBool("OtherSettings", "isTempSensor", 0);
	// заполняем ЛИРы
	if ( SystemConst::IsOnline )
	{
		for (int i = 0; i < Counters; i++)
		{
			arPerimiterOfLir[i] -> Text = ini->ReadFloat("LIR","Perimeter_"+UnicodeString(i),50);
			arLirChannel[i] -> Text = ini->ReadInteger("LIR","Channel_"+UnicodeString(i),i);
		}
		eLirToCross->Text = ini->ReadInteger("LIR","LirToCross", 1000 );
		eLirToLinear->Text = ini->ReadInteger("LIR","LirToLinear", 2000 );
		eLirToThickness->Text = ini->ReadInteger("LIR","LirToThickness", 2500 );
		eLirToMarker->Text = ini->ReadInteger("LIR","LirToMarker", 2600 );
		eImpulsePerRound->Text = ini->ReadInteger("LIR","ImpulsePerRound", 2300 );
		eMarkerTime->Text = ini->ReadInteger("LIR","MarkerTime", 100 );
		ePauseMixing->Text = ini->ReadInteger("LIR", "PauseMixing" , 300000 ) / 1000 / 60; // минуты
		eMixingTime->Text = ini->ReadInteger("LIR", "TimeMixing" , 1000 ) / 1000;	// секунды
		cbCalibrate->Checked = ini->ReadBool("LIR" , "IsCalibrate" , 0 );
	}
	// заполняем скорости
	cbWorkSpeed->Clear();
	cbWorkSpeed->Items->AddStrings(Globals::speeds.getSpeedNames());
	cbWorkSpeed->ItemIndex = cbWorkSpeed->Items->IndexOf( ini->ReadInteger("OtherSettings","WorkSpeed" , 8));
	cbTransitSpeed->Clear();
	cbTransitSpeed->Items->AddStrings(Globals::speeds.getSpeedNames());
	cbTransitSpeed->ItemIndex = cbTransitSpeed->Items->IndexOf( ini->ReadInteger("OtherSettings","TransitSpeed" , 8));
	cbReturnSpeed->Clear();
	cbReturnSpeed->Items->AddStrings(Globals::speeds.getSpeedNames());
	cbReturnSpeed->ItemIndex = cbReturnSpeed->Items->IndexOf( ini->ReadInteger("OtherSettings","ReturnSpeed" , 8));
	cbMedianFilter->Checked = ini->ReadBool("OtherSettings","isMedianFilter", 0);
	ComboBoxWidthMF->ItemIndex = ComboBoxWidthMF->Items->IndexOf(ini->ReadInteger("OtherSettings","WidthMedianFilter",3));
}
//---------------------------------------------------------------------------

void TADCSettForm::block()
{
	cbTemp->Enabled = fl;
	spinTemp->Enabled = fl;
	cbCalcSolidGroup->Enabled=fl;
	ComboBoxWidthMF->Enabled=fl;
	ppSizeBuffer->Enabled=fl;
	ppKadr->Enabled=fl;
	ppRate->Enabled=fl;
	prSizeBuffer->Enabled=fl;
	prKadr->Enabled=fl;
	prRate->Enabled=fl;
	SpinTemp1->Enabled=fl;
	SpinTemp2->Enabled=fl;
	SpinMagnetic->Enabled=fl;
	ComboBoxT1->Enabled=fl;
	ComboBoxT2->Enabled=fl;
	ComboBoxMG->Enabled=fl;
	EditT1->Enabled=fl;
	EditT2->Enabled=fl;
	EditMG->Enabled=fl;
	SpinSG->Enabled=fl;
	SpinCurr->Enabled=fl;
	cbComWithASU->Enabled = fl;
	rgSolidGroupSource->Enabled = fl;
	eLirToCross->Enabled = fl;
	eLirToLinear->Enabled = fl;
	eLirToThickness->Enabled = fl;
	eLirToMarker->Enabled = fl;
	eImpulsePerRound->Enabled = fl;
//	eMarkerTime->Enabled = fl;
	cbWorkSpeed->Enabled = fl;
	cbTransitSpeed->Enabled = fl;
	cbReturnSpeed->Enabled = fl;
	cbMedianFilter->Enabled = fl;
//	cbCalibrate->Enabled = fl;
	for(int i=0; i<Cross_sensors; i++)
	{
//		arComboBoxPp[i]->Enabled=fl;
		arSpinEditPp[i]->Enabled=fl;
		arGainPP[i] -> Enabled=fl;
	}
	for(int i=0; i<lin_sensors; i++)
	{
		arComboBoxPr[i]->Enabled=fl;
		arSpinEditPr[i]->Enabled=fl;
		arGainPR[i] -> Enabled=fl;
	}
	for (int i = 0; i < Counters; i++)
	{
		arLirChannel[i] -> Enabled = fl;
		arPerimiterOfLir[i] -> Enabled = fl;
	}
}
//------------------------------------------------------------------------------
void __fastcall TADCSettForm::ApplicationEventsMessage(tagMSG &Msg, bool &Handled)
{
		//Проверяем на нажатие сочетания клавиш Ctrl+Alt+Enter
	if (Msg.message == WM_HOTKEY) // сообщение наше
		   if (Msg.wParam == 0x00F) // идентификатор наш
				if(fl) fl=false;
				else fl=true;
				block();
}
//---------------------------------------------------------------------------

void __fastcall TADCSettForm::FormActivate(TObject *Sender)
{
	bool RHKret = RegisterHotKey(ADCSettForm->Handle,         // Handle окна, которому отправлять сообщения WM_HOTKEY
								0x00F,                 // УСЛОВНЫЙ идентификатор горячего ключа
								MOD_ALT + MOD_CONTROL, // модификаторы
								VK_RETURN              // код клавиши
								);
	FillAllControls();
	fl=false;
	block();
}
//------------------------------------------------------------------------------
int TADCSettForm::WriteRange(int r)
{
	switch (r)
	{      //Получаем индекс возвращаем усиление
	   case 0: { return 1;  break; }
	   case 1: { return 2;  break; }
	   case 2: { return 4;  break; }
	   case 3: { return 8;  break; }
	   case 4: { return 16; break; }
	default:;
	}
}
//------------------------------------------------------------------------------
int TADCSettForm::ReadRange(int r)
{
	switch(r)
	{       //Получаем усиление и возвращаем индекс комбобокса
	   case 1: { return 0;  break; }
	   case 2: { return 1;  break; }
	   case 4: { return 2;  break; }
	   case 8: { return 3;  break; }
	   case 16:{ return 4;  break; }
	default:;
	}
}

//------------------------------------------------------------------------------
void __fastcall TADCSettForm::bSaveClick(TObject *Sender)
{
	vector<double> ppGain,prGain;
	String gain_str = "Gain "+ IntToStr(Globals::current_diameter);
	//Сохраняем данные по дачтикам
	for(int i=0; i<Cross_sensors; i++)
	{
		ini->WriteInteger("PP","Range"+UnicodeString(i),WriteRange(StrToInt(arComboBoxPp[i] -> ItemIndex)) );
		ini->WriteInteger("PP","Number"+UnicodeString(i),arSpinEditPp[i]->Value);
		ini->WriteFloat( gain_str , "Gain"+IntToStr(i) , StrToFloat(arGainPP[i]->Text) );
		ppGain.push_back(StrToFloat(arGainPP[i]->Text));
	}
	for(int i=0; i<lin_sensors; i++)
	{
		ini->WriteInteger("PR","Range"+UnicodeString(i),WriteRange(StrToInt(arComboBoxPr[i] -> ItemIndex)) );
		ini->WriteInteger("PR","Number"+UnicodeString(i),arSpinEditPr[i]->Value);
		ini->WriteFloat("PR","Gain"+IntToStr(i),StrToFloat(arGainPR[i]->Text) );
		prGain.push_back(StrToFloat(arGainPR[i]->Text));
	}
	MainForm->pp.SetGains(ppGain);
	MainForm->pr.SetGains(prGain);
	//пишем данные по размеру буфера, частоте на 1канал, задержку между кадрами
	ini->WriteInteger("PP","SizeBuf",StrToInt(ppSizeBuffer->Text) );
	ini->WriteInteger("PP","Rate",StrToInt(ppRate->Text) );
	ini->WriteFloat("PP","Kadr",StrToFloat(ppKadr->Text) );

	ini->WriteInteger("PR","SizeBuf",StrToInt(prSizeBuffer->Text) );
	ini->WriteInteger("PR","Rate",StrToInt(prRate->Text) );
	ini->WriteFloat("PR","Kadr",StrToFloat(prKadr->Text) );
	//нужен ли нам датчик температуры ГП
	ini->WriteBool("OtherSettings", "isTempSensor", cbTemp->Checked);
	ini->WriteInteger("OtherSettings", "channelTempSG",spinTemp->Value);
	//сохраняем прочие настройки
	ini->WriteInteger("OtherSettings","Temp1",SpinTemp1->Value);
	ini->WriteInteger("OtherSettings","Temp2",SpinTemp2->Value);
	ini->WriteInteger("OtherSettings","Magnetic",SpinMagnetic->Value);

	ini->WriteInteger("OtherSettings","SensorSG",     SpinSG->Value);
	ini->WriteInteger("OtherSettings","SensorCurr",   SpinCurr->Value);
	ini->WriteBool   ("OtherSettings","IsCalcMS_DOS", cbCalcSolidGroup->Checked);

	ini->WriteString("OtherSettings","isMore1",ComboBoxT1->Text);
	ini->WriteString("OtherSettings","isMore2",ComboBoxT2->Text);
	ini->WriteString("OtherSettings","isMoreMG",ComboBoxMG->Text);

	ini->WriteInteger("OtherSettings","isVMore1",StrToInt(EditT1->Text) );
	ini->WriteInteger("OtherSettings","isVMore2",StrToInt(EditT2->Text) );
	ini->WriteInteger("OtherSettings","isVMoreMG",StrToInt(EditMG->Text) );

	ini->WriteBool("OtherSettings","isMedianFilter", cbMedianFilter->Checked);
	ini->WriteBool("OtherSettings","ComWithASU", cbComWithASU->Checked);
	ini->WriteInteger("OtherSettings","SolidGroupSource", rgSolidGroupSource->ItemIndex );
	if(SystemConst::IsOnline)
	{
		// сохраняем данные о ЛИРах
		for (int i = 0; i < Counters; i++)
		{
			ini->WriteFloat("LIR","Perimeter_"+UnicodeString(i), arPerimiterOfLir[i]->Text.ToDouble() );
			ini->WriteInteger("LIR","Channel_"+UnicodeString(i) , arLirChannel[i]->Value );
		}
		ini->WriteInteger("LIR","LirToCross",StrToInt(eLirToCross->Text) );
		ini->WriteInteger("LIR","LirToLinear",StrToInt(eLirToLinear->Text) );
		ini->WriteInteger("LIR","LirToThickness",StrToInt(eLirToThickness->Text) );
		ini->WriteInteger("LIR","LirToMarker",StrToInt(eLirToMarker->Text) );
		ini->WriteInteger("LIR","ImpulsePerRound",StrToInt(eImpulsePerRound->Text) );
		ini->WriteInteger("LIR","MarkerTime",StrToInt(eMarkerTime->Text) );
		ini->WriteInteger("LIR", "PauseMixing" , ePauseMixing->Text.ToInt() * 1000 * 60); // минуты
		ini->WriteInteger("LIR", "TimeMixing" , eMixingTime->Text.ToInt() * 1000);	// секунды
		ini->WriteBool("LIR" , "IsCalibrate" , cbCalibrate->Checked );
	}
	// сохраняем данные о скоростях
	ini->WriteInteger("OtherSettings","WorkSpeed" ,	cbWorkSpeed->Text.ToInt());
	ini->WriteInteger("OtherSettings","TransitSpeed" , cbTransitSpeed->Text.ToInt());
	ini->WriteInteger("OtherSettings","ReturnSpeed" , cbReturnSpeed->Text.ToInt());
	ini->WriteInteger("OtherSettings","WidthMedianFilter",ComboBoxWidthMF->Text.ToInt());
	SystemConst::SetSystemConstants();
}
//---------------------------------------------------------------------------

void __fastcall TADCSettForm::bExitClick(TObject *Sender)
{
	ADCSettForm->Close();
}
//---------------------------------------------------------------------------


void __fastcall TADCSettForm::ppSizeBufferKeyPress(TObject *Sender, wchar_t &Key)
{
	if(!((Key >= '0' && Key <= '9') || (Key == VK_BACK) || (Key == ','))) Key = 0x00;
}
//---------------------------------------------------------------------------

void __fastcall TADCSettForm::FormDestroy(TObject *Sender)
{
	for (int i = 0; i < Cross_sensors; i++)
	{
		delete arLabelPp[i];
		delete arGainPP[i];
		delete arComboBoxPp[i];
//		delete arLabelPp[i];
		delete arSpinEditPp[i];
	}
	for (int i = 0; i < lin_sensors; i++)
	{
		delete arGainPR[i];
		delete arComboBoxPr[i];
//		delete arLabelPr[i];
		delete arSpinEditPr[i];
	}
}
//---------------------------------------------------------------------------

void __fastcall TADCSettForm::PageControlChange(TObject *Sender)
{
	LIRPage->Visible = SystemConst::IsOnline;
}
//---------------------------------------------------------------------------

