#pragma once

#include <windows.h>

#include <string>
#include <sstream>
#include <iomanip>

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

	static string getString(int intValue)
	{
		return std::to_string(intValue);
	}

	static string getString(double doubleValue, int saveDigit = 4)
	{
		std::stringstream ss;
		ss << std::setprecision(saveDigit) << doubleValue;

		return ss.str();
	}

	static string getString(char chArray[])
	{
		return chArray;
	}

	static string getString(wstring wstr)
	{
		LPCWSTR pwszSrc = wstr.c_str();
		int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
		if (nLen == 0)
			return std::string("");

		char* pszDst = new char[nLen];
		if (!pszDst)
			return std::string("");

		WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
		std::string str(pszDst);
		delete[] pszDst;
		pszDst = NULL;

		return str;
	}

	static string getString(long longValue)
	{
		return std::to_string(longValue);
	}

	static string getString(long long longlongValue)
	{
		return std::to_string(longlongValue);
	}

	static wstring getWString(string str)
	{
		LPCSTR pszSrc = str.c_str();
		int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
		if (nLen == 0)
			return std::wstring(L"");

		wchar_t* pwszDst = new wchar_t[nLen];
		if (!pwszDst)
			return std::wstring(L"");

		MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
		std::wstring wstr(pwszDst);
		delete[] pwszDst;
		pwszDst = NULL;

		return wstr;
	}

	static void getCharArray(char chArray[], string text)
	{
		strcpy_s(chArray, text.length() + 1, text.c_str());
	}

	static long getLong(HWND wnd)
	{
		long wndLong = (long)wnd;
		return wndLong;  //值是正确的， 可以用 0x + wnd值检测。  long value = 0xEC1789;
	}

	static int getInt(string str)//非数字转换为0
	{
		return atoi(str.c_str());
	}
};

