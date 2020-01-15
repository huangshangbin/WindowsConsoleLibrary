#pragma once

#include <library/utils/DequeUtils.hpp>
#include <library/utils/StringUtils.hpp>
#include <library/fileDeal/ByteStreamFile.hpp>


class TextFile
{
private:
	string m_filePath;
	deque<string> m_dataList;

public:
	TextFile(string filePath)
	{
		m_filePath = filePath;
		initDataList();
	}
	~TextFile() {}

public:
	void reWrite(deque<string>& dataList)
	{
		DequeUtils<string>::copy(m_dataList, dataList);
	}

	void push(string str)
	{
		m_dataList.push_back(str);
	}

	void set(int row, string data)
	{
		if (row >= m_dataList.size() || row < 0)
		{
			return;
		}

		m_dataList[row] = data;
	}

	string get(int row)
	{
		if (row >= m_dataList.size() || row < 0)
		{
			return "";
		}

		return m_dataList[row];
	}

	int size()
	{
		return m_dataList.size();
	}

	void erase(int row)
	{
		if (row < 0 || row >= m_dataList.size())
		{
			return;
		}

		auto eraseIt = m_dataList.begin();
		for (int i = 0; i < row; i++)
		{
			eraseIt++;
		}

		m_dataList.erase(eraseIt);
	}

	void save()
	{
		string data;
		if (m_dataList.size() > 0)
		{
			data = m_dataList[0];
			for (int i = 1; i < m_dataList.size(); i++)
			{
				data = data + "\n" + m_dataList[i];
			}
		}

		ByteStreamFile::writeFile(m_filePath, data.c_str(), data.length());
	}


private:
	void initDataList()
	{
		ByteStreamFile byteStreamFile(m_filePath);

		string data(byteStreamFile.getBuffer());
		m_dataList = StringUtils::splitString(data, "\n");
	}


//²âÊÔ
public:
	void out()
	{
		for (auto& data : m_dataList)
		{
			cout << data << endl;
		}
	}
};

