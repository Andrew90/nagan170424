//---------------------------------------------------------------------------


#pragma hdrstop

#include "ModulePosition.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
ModulePosition::ModulePosition()
{

}

//---------------------------------------------------------------------------
String ModulePosition::SetModuleInWork(String Module)
{
	if ( SystemConst::ThicknessMass && Module == "����" ||
		 SystemConst::LinearMass && Module == "����" )
		return SetModuleInWorkWithMass( Module );
	else
		return SetModuleInWorkSimple( Module );
}
//---------------------------------------------------------------------------

String ModulePosition::SetModuleInService( String Module )
{
	if ( SystemConst::ThicknessMass && Module == "����" ||
		 SystemConst::LinearMass && Module == "����" )
		return SetModuleInServiceWithMass( Module );
	else
		return SetModuleInServiceSimple( Module );
}

//---------------------------------------------------------------------------

String ModulePosition::SetModuleInWorkSimple( String Module )
{
	byte state;
	byte kol = 0;
	while ( kol < 3 )
	{
		state = digital->CheckInSignal(Module + " ��")
				+ ( digital->CheckInSignal(Module + " ��") << 1);
		switch (state)
		{
			case 0:
				TThread::CurrentThread->Sleep(SleepTemp);
				kol++;
				break;

			case 1:
				return "ok";
			case 2:
				digital->BlinkOutSignal(Module + " � ��");
				if ( !digital->WaitForInSignal(Module + " ��", WaitModule ))
					return "������ ���������� ������! " + Module;
				return "ok";
			case 3:
				Application->MessageBoxW(L"�� ��������� ����������� ������� ��������� ������. \n ������ 501",L"������!",MB_ICONERROR);
				return "������: ������ ��������";
		}
	}
	return "������ ��� ��������� ��������� ������ " + Module;
}
//---------------------------------------------------------------------------
String ModulePosition::SetModuleInServiceSimple( String Module )
{
	byte state;
	byte kol = 0;
	while ( kol < 3 )
	{
		state = digital->CheckInSignal(Module + " ��")
				+ ( digital->CheckInSignal(Module + " ��") << 1);
		switch (state)
		{
			case 0:
				TThread::CurrentThread->Sleep(SleepTemp);
				kol++;
				break;

			case 1:
				digital->BlinkOutSignal(Module + " � ��");
				if ( !digital->WaitForInSignal(Module + " ��", WaitModule ))
					return "������ ��������� ������! " + Module;
				return "ok";
			case 2:
				return "ok";
			case 3:
				Application->MessageBoxW(L"�� ��������� ����������� ������� ��������� ������. \n ������ 501",L"������!",MB_ICONERROR);
				return "������: ������ ��������";
		}
	}
	return "������ ��� ��������� ��������� ������ " + Module;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

String ModulePosition::SetModuleInServiceWithMass( String Module )
{
	String Mass = Module + " ����";
	byte kol=0;
	byte state;
	if( Module == "�����")
		return SetModuleInServiceSimple(Module);
// ������� ��������� ������ �� ����� ������� �����
	while( kol < 3 )
	{
		state = digital->CheckInSignal( Module + " ��" ) +
			( ( digital->CheckInSignal( Module + " ��" ) ) <<1 ) +
			( ( digital->CheckInSignal( Mass + " ���" ) ) <<2 );

		switch ( state )
		{
			case 1:
				digital->BlinkOutSignal(Mass + " �����");
				if ( !digital->WaitForInSignal(Mass + " ���", WaitMass ) )
					return "������ ��������� �����! " + Mass;

				digital->BlinkOutSignal(Module + " � ��");
				if ( !digital->WaitForInSignal(Module + " ��", WaitModule ))
					return "������ ��������� ������! " + Module;

				return "ok";

			case 2:
				digital->BlinkOutSignal(Mass + " �����");
				if ( !digital->WaitForInSignal(Mass + " ���", WaitMass ) )
					return "������ ��������� �����! " + Mass;

				return "ok";

			case 5:
				digital->BlinkOutSignal(Module + " � ��");
				if ( !digital->WaitForInSignal(Module + " ��", WaitModule ) )
					return "������ ��������� ������!" + Module;

				return "ok";

			case 6:
				return "ok";

			case 0: case 4:
				TThread::CurrentThread->Sleep(SleepTemp);
				kol++;
				break;

			case 7: case 3:
				Application->MessageBoxW(L"�� ��������� ����������� ������� ��������� ������. \n ������ 501",L"������!",MB_ICONERROR);
				return "������: ������ ��������";
		}
	}
	return "������ ��������� ������! " + Module;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

String ModulePosition::SetModuleInWorkWithMass( String Module )
{
	String Mass = Module + " ����";
	byte kol=0;		// ���-�� �������� ���������, ���� ������ � ��������
	byte state;		// ��������� ������ � �����
	if(Module == "�����")
    	return SetModuleInWorkSimple(Module);
// ������� ��������� ������ �� ����� ������� �����
	while ( kol < 3 )
	{
		state = digital->CheckInSignal( Module + " ��" ) +
			( ( digital->CheckInSignal( Module + " ��" ) ) <<1 ) +
			( ( digital->CheckInSignal( Mass + " ���" ) ) <<2 );

		switch ( state )
		{
			case 1:
				return "ok";

			case 2:
				digital->BlinkOutSignal(Mass + " �����");
				if ( !digital->WaitForInSignal(Mass + " ���", WaitMass ) )
					return "������ ��������� �����! " + Mass;

				digital->BlinkOutSignal(Module + " � ��");
				if ( !digital->WaitForInSignal(Module + " ��", WaitModule ))
					return "������ ���������� ������! " + Module;

				digital->BlinkOutSignal(Mass + " ������");
				if ( !digital->WaitForDropInSignal(Mass + " ���" , WaitMass) )
					return "������ ���������� �����! " + Mass;

				return "ok";

			case 5:
				digital->BlinkOutSignal(Mass + " ������");
				if ( !digital->WaitForDropInSignal(Mass + " ���" , WaitMass) )
					return "������ ���������� �����!" + Mass;

				return "ok";

			case 6:
				digital->BlinkOutSignal(Module + " � ��");
				if ( !digital->WaitForInSignal(Module + " ��", WaitModule ))
					return "������ ���������� ������! " + Module;

				digital->BlinkOutSignal(Mass + " ������");
				if ( !digital->WaitForDropInSignal(Mass + " ���" , WaitMass) )
					return "������ ���������� �����! " + Mass;

				return "ok";

			case 0: case 4:
				TThread::CurrentThread->Sleep(SleepTemp);
				kol++;
				break;

			case 7: case 3:
				Application->MessageBoxW(L"�� ��������� ����������� ������� ��������� ������. \n ������ 501",L"������!",MB_ICONERROR);
				return "������: ������ ��������";
		}
	}
	return "������ ��������� ������! " + Module;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
String ModulePosition::SetAllModulesInWork(String Mod1, String Mod2, String Mod3)
{
	if ( !SystemConst::ThicknessMass && !SystemConst::LinearMass )
	{
		digital->BlinkOutSignal(Mod1 + " � ��");
		digital->BlinkOutSignal(Mod2 + " � ��");
		digital->BlinkOutSignal(Mod3 + " � ��");

		if ( !digital->WaitForInSignal(Mod1 + " ��", WaitModule ))
			return "������ ���������� ������! " + Mod1;
		if ( !digital->WaitForInSignal(Mod2 + " ��", WaitModule ))
			return "������ ���������� ������! " + Mod2;
		if ( !digital->WaitForInSignal(Mod3 + " ��", WaitModule ))
			return "������ ���������� ������! " + Mod3;
		return "ok";
	}
	else
	{
		if( SetModuleInWorkWithMass(Mod1) != "ok" )
			return "������ ��������� ������! " + Mod1;
		else if( SetModuleInWorkWithMass(Mod2) != "ok" )
			return "������ ��������� ������! " + Mod2;
		else if( SetModuleInWorkWithMass(Mod3) != "ok" )
			return "������ ��������� ������! " + Mod3;
		else
			return "ok";
	}
}
//---------------------------------------------------------------------------
String ModulePosition::SetAllModulesInService(String Mod1, String Mod2, String Mod3)
{
	if ( !SystemConst::ThicknessMass && !SystemConst::LinearMass )
	{
		digital->BlinkOutSignal(Mod1 + " � ��");
		digital->BlinkOutSignal(Mod2 + " � ��");
		digital->BlinkOutSignal(Mod3 + " � ��");

		if ( !digital->WaitForInSignal(Mod1 + " ��", WaitModule ))
			return "������ ��������� ������! " + Mod1;
		if ( !digital->WaitForInSignal(Mod2 + " ��", WaitModule ))
			return "������ ��������� ������! " + Mod2;
		if ( !digital->WaitForInSignal(Mod3 + " ��", WaitModule ))
			return "������ ��������� ������! " + Mod3;
		return "ok";
	}
	else
	{
		if(SetModuleInServiceWithMass(Mod1) != "ok" )
			return "������ ��������� ������! " + Mod1;
		else if(SetModuleInServiceWithMass(Mod2) != "ok" )
			return "������ ��������� ������! " + Mod2;
		else if(SetModuleInServiceWithMass(Mod3) != "ok" )
			return "������ ��������� ������! " + Mod3;
		else
			return "ok";
	}
}

