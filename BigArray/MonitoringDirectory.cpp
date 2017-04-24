#include "MonitoringDirectory.h"
#include "Windows.h"

wchar_t MonitoringDirectory_buffer[128];

void MonitoringDirectory_Run(bool(*proc)())
{
	HANDLE handle = FindFirstChangeNotification(MonitoringDirectory_buffer, FALSE, FILE_NOTIFY_CHANGE_FILE_NAME);
	WaitForSingleObject(handle, INFINITE);
	if (proc())
	{
		FindCloseChangeNotification(handle);
		return;
	}
	while (true)
	{
		if (!FindNextChangeNotification(handle))
		{
			FindCloseChangeNotification(handle);
			return;
		}
		WaitForSingleObject(handle, INFINITE);
		if (proc())
		{
			FindCloseChangeNotification(handle);
			return;
		}
	}
}

void MonitoringDirectory::Run(bool(*proc)(), wchar_t *path)
{
//\\Thickness\defectexchenge
 //	wcscpy_s(MonitoringDirectory_buffer, path);
 //	CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MonitoringDirectory_Run, proc, 0, NULL));
}

