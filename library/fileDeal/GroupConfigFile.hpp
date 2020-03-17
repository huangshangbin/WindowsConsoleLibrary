#pragma once

#include <library/utils/MapUtils.hpp>
#include <library/utils/FileUtils.hpp>
#include <library/utils/TypeConvertUtils.hpp>

#include "TextFile.hpp"


class GroupData
{
public:
	GroupData() {}

private:
	string m_name;
	map<string, string> m_keyValueMap;

public:
	void getKeyList(deque<string>& keyList)
	{
		MapUtils<string, string>::getMapKeyList(keyList, m_keyValueMap);
	}

	string getValue(string key)
	{
		return m_keyValueMap[key];
	}
	void setValue(string key, string value)
	{
		m_keyValueMap[key] = value;
	}

	void setName(string name)
	{
		m_name = name;
	}
	string getName()
	{
		return m_name;
	}
};


class GroupConfigFile
{
public:
	GroupConfigFile(string filePath)
	{
		TextFile tempFile(filePath);
		deque<string> dataList;
		tempFile.exportDataList(dataList);

		deque<int> groupFlagList;
		makeGroupFlagList(groupFlagList, dataList);

		for (int i = 0; i < groupFlagList.size() - 1; i++)
		{
			m_groupDataList.push_back(makeGroupData(dataList, groupFlagList[i], groupFlagList[i + 1]));
		}

		m_filePath = filePath;
	}
	~GroupConfigFile(void) {}

private:
	GroupConfigFile(){}
	GroupConfigFile(const GroupConfigFile& groupConfigFile){}

private:
	string m_filePath;

	deque<GroupData*> m_groupDataList;
	GroupData* m_currentGroup;

public:
	void setGroup(string groupName)
	{
		BOOL isExitGroup = FALSE;
		for (int i = 0; i < m_groupDataList.size(); i++)
		{
			if (groupName == m_groupDataList[i]->getName())
			{
				m_currentGroup = m_groupDataList[i];
				isExitGroup = TRUE;
			}
		}

		if (isExitGroup == FALSE)
		{
			GroupData* groupData = new GroupData;
			groupData->setName(groupName);

			m_groupDataList.push_back(groupData);
			m_currentGroup = groupData;
		}
	}

	void deleteFile()
	{
		FileUtils::deleteFile(m_filePath);
	}

	void getKeyList(deque<string>& keyList)
	{
		m_currentGroup->getKeyList(keyList);
	}

	void getValueList(deque<string>& valueList)
	{
		valueList.clear();

		deque<string> keyList;
		getKeyList(keyList);
		for (int i = 0; i < keyList.size(); i++)
		{
			valueList.push_back(m_currentGroup->getValue(keyList[i]));
		}
	}

	string getStringValue(string key)
	{
		return m_currentGroup->getValue(key);
	}

	int getIntValue(string key)
	{
		return TypeConvertUtils::getInt(getStringValue(key));
	}

	void setValue(string key, string value)
	{
		m_currentGroup->setValue(key, value);

		saveFile();
	}


//初始化
private:
	void makeGroupFlagList(deque<int>& flagList, deque<string>& dataList)
	{
		for (int i = 0; i < dataList.size(); i++)
		{
			if (dataList[i][0] == '[' && StringUtils::isUseStringEnd(dataList[i], "]") == TRUE)
			{
				flagList.push_back(i);
			}
		}

		flagList.push_back(dataList.size() - 1);
	}

	GroupData* makeGroupData(deque<string>& dataList, int startIndex, int endIndex)
	{
		GroupData* groupData = new GroupData();
		for (int i = startIndex; i < endIndex; i++)
		{
			if (i < 0 || i >= dataList.size())
			{
				break;
			}

			if (i == startIndex)
			{
				groupData->setName(StringUtils::getStringUsePos(dataList[i], 1, dataList[i].length() - 2));
			}

			if (StringUtils::isExistStringInString(dataList[i], "=") == TRUE)
			{
				deque<string> tempArray = StringUtils::splitString(dataList[i], "=");

				groupData->setValue(tempArray[0], tempArray[1]);
			}
		}

		return groupData;
	}


//setValue
private:
	void saveFile()
	{
		deque<string> dataList;
		for (int groupIndex = 0; groupIndex < m_groupDataList.size(); groupIndex++)
		{
			setGroup(m_groupDataList[groupIndex]->getName());

			dataList.push_back("[" + m_currentGroup->getName() + "]");

			deque<string> keyList;
			getKeyList(keyList);
			for (int keyIndex = 0; keyIndex < keyList.size(); keyIndex++)
			{
				dataList.push_back(keyList[keyIndex] + "=" + getStringValue(keyList[keyIndex]));
			}
			dataList.push_back("");
		}

		TextFile tempFile(m_filePath);
		tempFile.reWrite(dataList);
		tempFile.save();
	}


private:
	void deleteGroupDataList()
	{
		for (int i = 0; i < m_groupDataList.size(); i++)
		{
			delete m_groupDataList[i];
		}

		m_groupDataList.clear();
	}


//操作符重载
private:
	void operator = (const GroupConfigFile& groupConfigFile){}
};

