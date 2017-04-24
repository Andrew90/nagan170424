//#include "stdafx.h"
#include "DebugMess.h"
#ifdef XDEBUG
#include <stdio.h>
#define d_mess  printf
wchar_t name[] = L"Parallel hatch in space";
wchar_t eventName[] = L"Parallel hatch in space event";
CRITICAL_SECTION critical;
struct InitCriticalSectionDebug
{
	InitCriticalSectionDebug(){InitializeCriticalSection(&critical);}
	~InitCriticalSectionDebug(){EnterCriticalSection(&critical);(&critical);}
} initCriticalSection_Debug;

struct LockDebug
{
	LockDebug(){EnterCriticalSection(&critical);}
	~LockDebug(){LeaveCriticalSection(&critical);}
};
void DebugMess::Init()
{
	hEvent = CreateEvent(NULL, TRUE, FALSE, eventName);
	hMapFile = CreateFileMapping(
                 INVALID_HANDLE_VALUE,    // use paging file
                 NULL,                    // default security 
                 PAGE_READWRITE,          // read/write access
                 0,                       // max. object size 
                 sizeof(TMapData),                // buffer size  
                 name);                 // name of mapping object
 
   if (hMapFile == NULL || hMapFile == INVALID_HANDLE_VALUE) 
   { 
      d_mess("Could not create file mapping object (%d).\n", 
             GetLastError());
      return;
   }
   map = (TMapData *) MapViewOfFile(hMapFile,   // handle to map object
                        FILE_MAP_ALL_ACCESS, // read/write permission
                        0,                   
                        0,                   
                        sizeof(TMapData));           
 
   if (map == NULL)
   { 
      d_mess("Could not map view of file (%d).\n", 
             GetLastError()); 
      return;
   }
   if(ERROR_ALREADY_EXISTS != GetLastError())
   {
	 map->head = 0;
     map->tail = 0;
   }
}

DebugMess::~DebugMess(void)
{
   UnmapViewOfFile(map);
   CloseHandle(hMapFile);
   CloseHandle(hEvent);
}
#pragma warning(disable : 4996)
void DebugMess::print(char *c, ...)
{
	if(NULL != map)
	{
	    EnterCriticalSection(&critical);
		char *b = map->data[map->head];
		++map->head;
		LeaveCriticalSection(&critical);
		vsprintf(b, c, (char *)&c + sizeof(c));
		SetEvent(hEvent);
	}
}
//---------------------------------------------------------------------------------------
ViewerDebugMess::ViewerDebugMess() : map(NULL)
{
	hEvent = CreateEvent(NULL, TRUE, FALSE, eventName);
	hMapFile = CreateFileMapping(
                 INVALID_HANDLE_VALUE,    // use paging file
                 NULL,                    // default security 
                 PAGE_READWRITE,          // read/write access
                 0,                       // max. object size 
                 sizeof(TMapData),                // buffer size  
                 name);                 // name of mapping object
	if (hMapFile == NULL) 
   { 
      d_mess("Could not open file mapping object (%d).\n", 
             GetLastError());
	  return;
   } 
 
   map = (TMapData *)MapViewOfFile(hMapFile,    // handle to mapping object
               FILE_MAP_ALL_ACCESS,  // read/write permission
               0,                    
               0,                    
               sizeof(TMapData));                   
 
   if (map == NULL) 
   { 
      d_mess("Could not map view of file (%d).\n", 
			 GetLastError());
      return;
   }
   if(map->head - map->tail > 511) map->tail = map->head - 511;
   SetEvent(hEvent);
}
//----------------------------------------------------------------------------
ViewerDebugMess::~ViewerDebugMess()
{
   UnmapViewOfFile(map);
   CloseHandle(hMapFile);
   CloseHandle(hEvent);
}
//----------------------------------------------------------------------------
char *ViewerDebugMess::get()
{
    WaitForSingleObject(hEvent, INFINITE);
	if(map && map->tail != map->head)
	{
		char *b = map->data[map->tail];
		++map->tail;
		return b;
	}
	else
	{
	    ResetEvent(hEvent);
	}
	return NULL;
}
//-----------------------------------------------------------------------------
DebugMess debug;
#endif
