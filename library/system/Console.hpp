#pragma once

#include <string>
#include <iostream>

using namespace std;


class Console
{
public:
	static string readString()
	{
		string str;
		cin >> str;
		return move(str);
	}

};

