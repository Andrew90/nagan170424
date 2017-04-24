//---------------------------------------------------------------------------


#pragma hdrstop

#include "Compute.h"
#include "Result.h"
#include "Global.h"
#include "LCARD791.h"
#include "BigArray.h"
#include "Lir.h"
#include "Global.h"
#include "DebugMess.h"
#include "CommunicationTube.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
void SendStrobesThicknessUnit()
{
	Lir &lir = Lir::Instance();
	if(!lir.isThick) return;
	unsigned *t = lir.thicknessUnit.zonesTime;
	unsigned *o = lir.thicknessUnit.zonesOffs;
	unsigned middleDelay = lir.thicknessUnit.bitStopUp - lir.thicknessUnit.bitStartUp;
	unsigned first = t[0];
	for(int i = 0; i < lir.thicknessUnit.zonesTimeOffset; ++i)
	{
		t[i] -= first;
	}
	CommunicationTube::SetStrobes(
	   lir.thicknessUnit.zonesTimeOffset
	   , middleDelay
	   , lir.thicknessUnit.zonesTime
	   );
}

void Compute(LCard791 *l1, Result &pp, Result &pr)
{
	Lir &lir = Lir::Instance();
	lir.Prepare();

   	SendStrobesThicknessUnit();

	int DefSensors =(Globals::current_diameter == 60) ? (10) : (12);
	vector<vector<int> > offsets(2);

	BigArray::TimeToArrayOffsets(
		l1->timeOffs, l1->countTimeOffs, l1->frameBuffer
		, lir.crossUnit.zonesTime, lir.crossUnit.zonesTimeOffset
		, offsets[0]
	);
	BigArray::TimeToArrayOffsets(
		l1->timeOffs, l1->countTimeOffs, l1->frameBuffer
		, lir.longUnit.zonesTime, lir.longUnit.zonesTimeOffset
		, offsets[1]
	);
	for(int i = 0; i < offsets[0].size(); ++i)
	{
	   dprint("cross Offs  %d %d\n", i, offsets[0][i]);
	}
	for(int i = 0; i < offsets[1].size(); ++i)
	{
	   dprint("long Offs  %d %d\n", i, offsets[1][i]);
	}
//-----------------------------------------------------------------
	BigArray::Prepare(
		DefSensors
		, l1->Base
		, l1->Index
		, offsets
		, pp.data
		, pr.data
	);

	for(int i = 0; i < pp.data.size(); ++i)
	{
		dprint("BigArray::Prepare poper %d  %f\n", i, pp.data[i][0][0]);
		Sleep(10);
	}

	for(int i = 0; i < pr.data.size(); ++i)
	{
		dprint("BigArray::Prepare prod %d  %f\n", i, pr.data[i][0][0]);
		Sleep(10);
	}

	pp.ComputeZoneData();
	String section = "Type_" + Globals::current_typesize;
	int len = GetPrivateProfileInt(section.c_str()
	   , L"CrossDeadZoneStart", 200
	   , Globals::IniFileName.c_str()
	   );
	pp.CalculateDeadFront(len);
	len = GetPrivateProfileInt(section.c_str()
	   , L"CrossDeadZoneFinish", 200
	   , Globals::IniFileName.c_str()
	   );
	pp.CalculateDeadBack(len);
	pr.ComputeZoneData();
	len = GetPrivateProfileInt(section.c_str()
	   , L"LinearDeadZoneStart", 200
	   , Globals::IniFileName.c_str()
	   );
	pr.CalculateDeadFront(len);
	len = GetPrivateProfileInt(section.c_str()
	   , L"LinearDeadZoneFinish", 200
	   , Globals::IniFileName.c_str()
	   );
	pr.CalculateDeadBack(len);
}

void Compute(LCard791 *l1, Result &pp)
{
	 Lir::Instance().Prepare();

	 SendStrobesThicknessUnit();

	 int DefSensors =(Globals::current_diameter == 60) ? (10) : (12);

	vector<int>offsets;

	Lir &lir = Lir::Instance();
	BigArray::TimeToArrayOffsets(
		l1->timeOffs, l1->countTimeOffs, l1->frameBuffer
		, lir.crossUnit.zonesTime, lir.crossUnit.zonesTimeOffset
		, offsets
	);
//-----------------------------------------------------------------
	BigArray::Prepare(
		DefSensors
		, l1->Base
		, l1->Index
		, offsets
		, pp.data
	);

	pp.ComputeZoneData();

	String section = "Type_" + Globals::current_typesize;
	int len = GetPrivateProfileInt(section.c_str()
	   , L"CrossDeadZoneStart", 200
	   , Globals::IniFileName.c_str()
	   );
	pp.CalculateDeadFront(len);
	len = GetPrivateProfileInt(section.c_str()
	   , L"CrossDeadZoneFinish", 200
	   , Globals::IniFileName.c_str()
	   );
	pp.CalculateDeadBack(len);
}

