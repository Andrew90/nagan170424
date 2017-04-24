//---------------------------------------------------------------------------

#ifndef ModulePositionH
#define ModulePositionH

#include "AdvantechSignals.h"
#include "Global.h"
//---------------------------------------------------------------------------
class ModulePosition
{
private:

	ModulePosition();
	~ModulePosition();
	static int SleepTemp;
	static int WaitMass;
	static int WaitModule;


	static String SetModuleInWorkSimple( String Module );	 		// ��������� ������ � ������ ��� �����
	static String SetModuleInServiceSimple( String Module );		// ��������� ������ � ������������ ��� �����
	static String SetModuleInWorkWithMass( String Module ); 		// ��������� ������ � ������ c ������
	static String SetModuleInServiceWithMass( String Module );		// ��������� ������ � ������������ � ������


public:
	static String SetModuleInWork(String Module);
	static String SetModuleInService( String Module );
	static String SetAllModulesInWork(String Mod1, String Mod2, String Mod3);
	static String SetAllModulesInService(String Mod1, String Mod2, String Mod3);

};

	int ModulePosition::SleepTemp = 750;
	int ModulePosition::WaitMass = 4000;
	int ModulePosition::WaitModule = 8000;


//---------------------------------------------------------------------------
#endif
