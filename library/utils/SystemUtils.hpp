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

