//---------------------------------------------------------------------------


#pragma hdrstop

#include "CommunicationTube.h"
#include "DataModule.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void Query(wchar_t *q, bool active = false)
{
    DM->Find->Active=false;
	DM->Find->Close();
	DM->Find->SQL->Clear();
	DM->Find->SQL->Add (q);
	DM->Find->ExecSQL();
	DM->Find->Active=active;
}
 void CommunicationTube::SetStrobes(int count, int middleDelay, unsigned *data)
 {
	char buf[64];
	String s = "INSERT INTO Strobes(Count, MiddleDelay";
	for(int i = 0; i < count; ++i)
	{
	   sprintf(buf, ",_%02d", i);
	   s += buf;
	}
	sprintf(buf, ")VALUES(%d,%d", count, middleDelay);
	s += buf;
	for(int i = 0; i < count; ++i)
	{
		sprintf(buf, ",%d", data[i]);
		s += buf;
	}
	s += ")";
  	Query(L"DELETE FROM Strobes");
	Query(s.w_str());
	DM->Find->Close();
 }

 int CommunicationTube::GetStrobes(int &middleDelay, unsigned (&data)[100])
 {
	  Query(L"SELECT * FROM Strobes", true);
	  int count = 0;
	  if(DM->Find->RecordCount)
	  {
		   count = DM->Find->FieldByName("Count")->AsInteger;
		   middleDelay = DM->Find->FieldByName("MiddleDelay")->AsInteger;
		   char buf[64];
		   for(int i = 0; i < count; ++i)
		   {
			  sprintf(buf, "_%02d", i);
			  data[i] = DM->Find->FieldByName(buf)->AsInteger;
		   }
	  }
	  DM->Find->Close();
	  return count;
 }
