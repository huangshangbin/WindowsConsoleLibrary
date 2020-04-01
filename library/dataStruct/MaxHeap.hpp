#pragma once

#include <vector>
using namespace std;

template <typename T>
class MaxHeap
{
public:
	MaxHeap(int defaltCapacity = 100)
	{
		m_dataList.reserve(defaltCapacity);
		m_size = 0;
	}


private:
	vector<T> m_dataList;
	int m_size;

public:
	void insert(T data)
	{
		m_dataList.push_back(data);
		upSift(m_size);

		m_size++;
	}

	T popMax()
	{
		if (m_size <= 0)
		{
			return T();
		}

		T maxData = m_dataList[0];

		m_dataList[0] = m_dataList[m_size - 1];
		m_size--;

		downSift(0);	

		return maxData;
	}

	T getMax()
	{
		if (m_size <= 0)
		{
			return T();
		}

		return m_dataList[0];
	}

	int size()
	{
		return m_size;
	}


private:
	void upSift(int index)
	{
		if (index <= 0)
		{
			return;
		}

		int parentIndex = (index - 1) / 2;
		if (m_dataList[parentIndex] < m_dataList[index])
		{
			T tempData = m_dataList[parentIndex];
			m_dataList[parentIndex] = m_dataList[index];
			m_dataList[index] = tempData;

			upSift(parentIndex);
		}
	}

	void downSift(int index)
	{
		if (index >= m_size)
		{
			return;
		}

		int leftChildIndex = index * 2 + 1;
		int rightChildIndex = index * 2 + 2;
		if (leftChildIndex >= m_size)
		{
			return;
		}
		
		int maxChildIndex = leftChildIndex;
		if ((rightChildIndex < m_size) && (m_dataList[leftChildIndex] < m_dataList[rightChildIndex]))
		{
			maxChildIndex = rightChildIndex;
		}

		if (m_dataList[maxChildIndex] > m_dataList[index])
		{
			T tempData = m_dataList[index];
			m_dataList[index] = m_dataList[maxChildIndex];
			m_dataList[maxChildIndex] = tempData;
		}
		
		downSift(maxChildIndex);
	}
};