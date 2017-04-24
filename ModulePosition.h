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


	static String SetModuleInWorkSimple( String Module );	 		// установка модуля в работу без груза
	static String SetModuleInServiceSimple( String Module );		// установка модуля в обслуживание без груза
	static String SetModuleInWorkWithMass( String Module ); 		// установка модуля в работу c грузом
	static String SetModuleInServiceWithMass( String Module );		// установка модуля в обслуживание с грузом


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
