#pragma once

#include <deque>
#include <iostream>

using namespace std;

template<class T>
class DequeUtils
{
public:
	static void copy(deque<T>& targetDeque, deque<T>& srcDeque)
	{
		targetDeque.clear();
		for (T& data : srcDeque)
		{
			targetDeque.push_back(data);
		}
	}

	static bool isExistData(deque<T>& dataList, T data)
	{
		for (T& itemData : dataList)
		{
			if (itemData == data)
			{
				return true;
			}
		}

		return false;
	}
};

