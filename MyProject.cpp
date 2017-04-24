//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>


//---------------------------------------------------------------------------
USEFORM("Settings.cpp", SettingsForm);
USEFORM("SettingsADC.cpp", FormSettADC);
USEFORM("SignalsState.cpp", SignalsStateForm);
USEFORM("ViewEtalons.cpp", FormViewEtalons);
USEFORM("View.cpp", ViewForm);
USEFORM("SolidGroup.cpp", SolidGroupForm);
USEFORM("ColorSettings.cpp", FormColor);
USEFORM("About.cpp", AboutBox);
USEFORM("Main.cpp", MainForm);
USEFORM("Manage.cpp", ManageForm);
USEFORM("DataModule.cpp", DM); /* TDataModule: File Type */
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{

	try
	{
        // �������� ������� �������.
		HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, 0, L"MyApp1.0");
        if(!hMutex)
          // �������� �� ����������. �� ����,
		  // ��� ������ ���������,
          // ������� �������.
		  hMutex = CreateMutex(0, 0, L"MyApp1.0");
		else
		{
          // ������� ���������� , �� ���� , �������
		  // ������ ���������, ������� ����� ��� �� ����� � ��������� �����.
		  Application->MessageBoxW(L"���������� ��� ��������!!!",L"������",MB_OK|MB_ICONERROR);
		   return 0;
		}

		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TDM), &DM);
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TViewForm), &ViewForm);
		Application->CreateForm(__classid(TSettingsForm), &SettingsForm);
		Application->CreateForm(__classid(TManageForm), &ManageForm);
		Application->CreateForm(__classid(TFormSettADC), &FormSettADC);
		Application->CreateForm(__classid(TSolidGroupForm), &SolidGroupForm);
		Application->CreateForm(__classid(TSignalsStateForm), &SignalsStateForm);
		Application->CreateForm(__classid(TFormViewEtalons), &FormViewEtalons);
		Application->CreateForm(__classid(TAboutBox), &AboutBox);
		Application->CreateForm(__classid(TFormColor), &FormColor);
		Application->Run();

		// ���������� ����������� ,
		// ����������� �������.
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
