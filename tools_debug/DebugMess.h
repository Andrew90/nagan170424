#ifndef DebugMess_h
#define DebugMess_h
#define XDEBUG
#ifdef XDEBUG
#include <windows.h>
struct TMapData
{
	unsigned char head;
	unsigned char tail;
	char data[256][512];
};
class DebugMess
{
    HANDLE hEvent;
	HANDLE hMapFile;
    TMapData *map;
public:
	~DebugMess(void);
	void Init();
	void print(char *, ...);
};

class ViewerDebugMess
{
    HANDLE hEvent;
	HANDLE hMapFile;
    TMapData *map;
public:
	ViewerDebugMess(void);
public:
	~ViewerDebugMess(void);
	char *get();
};

extern DebugMess debug;
#define dprint debug.print
#else
#define dprint
#endif
#endif


