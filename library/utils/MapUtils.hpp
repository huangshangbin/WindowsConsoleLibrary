#pragma once

#include <iostream>
#include <map>
#include <deque>

using namespace std;

template<class KeyT, class ValueT>
class MapUtils
{
public:
	static void deleteKeyValue(map<KeyT, ValueT>& dataMap, KeyT keyName)
	{
		map<KeyT, ValueT>::iterator it = dataMap.begin();
		while (it != dataMap.end())
		{
			if (it->first == keyName)
			{
				dataMap.erase(it);
				return;
			}

			it++;
		}
	}

	static void getMapKeyList(deque<KeyT>& keyList, map<KeyT, ValueT>& dataMap)
	{
		keyList.clear();

		map<KeyT, ValueT>::iterator it = dataMap.begin();
		while (it != dataMap.end())
		{
			keyList.push_back(it->first);
			it++;
		}
	}

	static bool isExistMapKey(map<KeyT, ValueT>& dataMap, KeyT keyName)
	{
		map<KeyT, ValueT>::iterator it = dataMap.begin();
		for (; it != dataMap.end(); it++)
		{
			if (keyName == it->first)
			{
				return true;
			}
		}

		return false;
	}
};

