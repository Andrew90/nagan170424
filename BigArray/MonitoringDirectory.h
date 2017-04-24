#pragma once
class MonitoringDirectory
{
public:
	static void Run(bool(*proc)(), wchar_t *path);
};

