//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include "DebugMess.h"

//---------------------------------------------------------------------------
USEFORM("ReportsView.cpp", FormReport);
USEFORM("Protocol.cpp", ProtocolForm);
USEFORM("Settings.cpp", SettingsForm);
USEFORM("Manage.cpp", ManageForm);
USEFORM("Main.cpp", MainForm);
USEFORM("ViewEtalons.cpp", FormViewEtalons);
USEFORM("View.cpp", ViewForm);
USEFORM("SolidGroup.cpp", SolidGroupForm);
USEFORM("SignalsState.cpp", SignalsStateForm);
USEFORM("SolidGroupTest.cpp", SGForm);
USEFORM("SolidGroupGraphics.cpp", Form1);
USEFORM("ADCSettings.cpp", ADCSettForm);
USEFORM("ABOUT_NTC_NK_URAN.cpp", AboutBox1);
USEFORM("GraphicsCenters.cpp", GraphicCentersForm);
USEFORM("ColorSettings.cpp", FormColor);
USEFORM("DataModule.cpp", DM); /* TDataModule: File Type */
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{

	try
	{
        // Пытаемся открыть мьютекс.
		HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, 0, L"MyApp1.0");
        if(!hMutex)
          // Мьютекса не существует. То есть,
		  // это первый экземпляр,
          // создаем мьютекс.
		  hMutex = CreateMutex(0, 0, L"MyApp1.0");
		else
		{
          // Мьютекс существует , то есть , запущен
		  // второй экземпляр, говорим юзеру что он олень и закрываем прогу.
		  Application->MessageBoxW(L"Приложение уже запущено!!!",L"Ошибка",MB_OK|MB_ICONERROR);
		   return 0;
		}

		#ifdef XDEBUG
		debug.Init();
		debug.print("\nOk\n");
		#endif

		Application->Initialize();
		Application->MainFormOnTaskBar = true;

		Application->CreateForm(__classid(TDM), &DM);
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TViewForm), &ViewForm);
		Application->CreateForm(__classid(TProtocolForm), &ProtocolForm);
		Application->CreateForm(__classid(TSettingsForm), &SettingsForm);
		Application->CreateForm(__classid(TManageForm), &ManageForm);
		Application->CreateForm(__classid(TSolidGroupForm), &SolidGroupForm);
		Application->CreateForm(__classid(TSignalsStateForm), &SignalsStateForm);
		Application->CreateForm(__classid(TFormViewEtalons), &FormViewEtalons);
		Application->CreateForm(__classid(TFormColor), &FormColor);
		Application->CreateForm(__classid(TFormReport), &FormReport);
		Application->CreateForm(__classid(TADCSettForm), &ADCSettForm);
		Application->CreateForm(__classid(TSGForm), &SGForm);
		Application->CreateForm(__classid(TGraphicCentersForm), &GraphicCentersForm);
		Application->CreateForm(__classid(TAboutBox1), &AboutBox1);
		Application->Run();

		// Приложение закрывается ,
		// освобождаем мьютекс.
		ReleaseMutex(hMutex);
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
