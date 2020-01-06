#pragma once

#include <windows.h>

#include <library/system/Process.hpp>
#include <library/utils/WndUtils.hpp>

#include <string>
using namespace std;

class SystemUtils
{
public:
	static void loadExe(string exePath)
	{
		ShellExecute(NULL, "open", exePath.c_str(), NULL, NULL, SW_SHOW);
	}

	static void loadExeWaitComplete(string exePath)
	{
		SHELLEXECUTEINFO ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = NULL;
		ShExecInfo.lpFile = exePath.c_str();
		ShExecInfo.lpParameters = NULL;
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = SW_SHOW;
		ShExecInfo.hInstApp = NULL;
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	}

	static void syncLoadExe(string exePath)
	{
		ShellExecute(NULL, "open", exePath.c_str(), NULL, NULL, SW_SHOW);

		while (true)
		{
			deque<Process> processList = Process::getAllProcess();
			for (Process processItem : processList)
			{
				if (processItem.getPath() == exePath)
				{
					return;
				}
			}

			Sleep(300);
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

