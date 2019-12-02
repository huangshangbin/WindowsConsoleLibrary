#pragma once



#include <string>
#include <deque>

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


public:
	template<class T>
	static void out(T data)
	{
		string typeStr = typeid(data).name();
		if (typeStr == "i" || typeStr == "d" || typeStr == "NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE"
			|| typeStr == "l" || typeStr == "c" || typeStr == "m" || typeStr == "b" || typeStr == "A5_c"
			|| typeStr == "PKc" || typeStr =="char const *")//»ù´¡ÀàÐÍ
		{
			cout << data << endl;
		}
	}

	template<class T>
	static void out(deque<T>& dataList)
	{
		for (int i = 0; i < dataList.size(); i++)
		{
			cout << dataList[i] << endl;
		}
	}
};

