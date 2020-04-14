#pragma once

#include <windows.h>
#include <shellapi.h> 

#include <library/system/Process.hpp>
#include <library/utils/WndUtils.hpp>

#include <library/utils/private/sole.hpp>

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

	//同步加载程序
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

	//分辨率
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

	//设置工作目录 类似cd命令
	static void setWorkDir(string worDir)
	{
		SetCurrentDirectory(worDir.c_str());
	}

	static int getRandom()
	{
		static bool isSrand = false;
		if (isSrand == false)
		{
			srand(time(0));
			isSrand = true;
		}

		return rand();
	}

	static string getUuid()
	{
		sole::uuid u0 = sole::uuid0();
		return u0.str();
	}
};

