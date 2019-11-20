#pragma once

#include <windows.h>

#include <string>
using namespace std;

class SystemUtils
{
public:
	void loadExe(string exePath)
	{
		ShellExecute(NULL, "open", exePath.c_str(), NULL, NULL, SW_SHOW);
	}
};

