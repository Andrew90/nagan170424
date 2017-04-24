//---------------------------------------------------------------------------

#ifndef CommunicationTubeH
#define CommunicationTubeH
//---------------------------------------------------------------------------
struct CommunicationTube
{
	static void SetStrobes(int count, int middleDelay, unsigned *data);
	static int GetStrobes(int &middleDelay, unsigned (&data)[100]);
};
#endif
