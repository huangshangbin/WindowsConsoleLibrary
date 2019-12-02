#pragma once

#include <string>

#include <iostream>

using namespace std;



class TypeConvertUtils
{
public:
	static string getString(char ch)
	{
		string str;
		str.push_back(ch);

		return move(str);
	}

	static string getString(char chArray[])
	{
		return chArray;
	}

	static long getLong(HWND wnd)
	{
		long wndLong = (long)wnd;
		return wndLong;  //值是正确的， 可以用 0x + wnd值检测。  long value = 0xEC1789;
	}
};

