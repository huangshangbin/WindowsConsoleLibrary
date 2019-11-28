#pragma once

#include "Tlhelp32.h"

#include   "psapi.h"
#pragma   comment(lib,"psapi.lib")

#include <library/utils/FileUtils.hpp>

#include <Windows.h>

#include <string>

#include <iostream>
using namespace std;

class Process
{
public:
	Process(DWORD id) { m_id = id; }
	~Process() {}

private:
	DWORD m_id;

public:
	string getPath()
	{
		string processPath;

		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, m_id);
		if (hProcess)
		{
			char pathChArray[MAX_PATH];
			if (GetModuleFileNameEx(hProcess, NULL, pathChArray, MAX_PATH) != 0)
			{
				processPath = pathChArray;
			}
			
			CloseHandle(hProcess);
		}

		return processPath;
	}

	string getDir()
	{
		return FileUtils::getPreviousLayerPath(getPath());
	}

	void close()
	{
		HANDLE hHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_id);
		TerminateProcess(hHandle, 0);
	}
};
