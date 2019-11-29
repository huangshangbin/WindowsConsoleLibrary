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

	static int getIntUseHexStr(string hexStr)
	{

	}
};

