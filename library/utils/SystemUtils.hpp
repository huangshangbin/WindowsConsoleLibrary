#pragma once

#include <windows.h>

#include <string>
using namespace std;

class SystemUtils
{
public:
	static void loadExe(string exePath)
	{
		ShellExecute(NULL, "open", exePath.c_str(), NULL, NULL, SW_SHOW);
	}

	static void syncLoadExe(string exePath)
	{
		PROCESS_INFORMATION ProcessInfo;
		STARTUPINFO StartupInfo; //This is an [in] parameter
		ZeroMemory(&StartupInfo, sizeof(StartupInfo));
		StartupInfo.cb = sizeof StartupInfo; //Only compulsory field
		if (CreateProcess(exePath.c_str(), NULL,
			NULL, NULL, FALSE, 0, NULL,
			NULL, &StartupInfo, &ProcessInfo))
		{
			WaitForSingleObject(ProcessInfo.hProcess, INFINITE);

			CloseHandle(ProcessInfo.hThread);
			CloseHandle(ProcessInfo.hProcess);
		}
	}

	static void setResolution(int x, int y)
	{
		DEVMODE DevMode;
		EnumDisplaySettings(NULL, 0, &DevMode);
		DevMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
		DevMode.dmPelsWidth = x;
		DevMode.dmPelsHeight = y;
		DevMode.dmDisplayFrequency = 30;
		DevMode.dmBitsPerPel = 32;
		ChangeDisplaySettings(&DevMode, 0);
	}
};

