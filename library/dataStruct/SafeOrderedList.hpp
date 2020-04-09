#pragma once

#include <mutex>
#include <condition_variable>

using namespace std;

template<typename T>
class ListNode
{
public:
	ListNode() { m_previous = nullptr; m_next = nullptr; }

public:
	T m_data;

	ListNode<T>* m_previous;
	ListNode<T>* m_next;
};


template<typename T>
class SafeOrderedList
{
public:
	SafeOrderedList() { m_size = 0; m_head = nullptr; m_tail = nullptr; }
	~SafeOrderedList()
	{
		lock_guard<mutex> lockGuard(m_mutex);

		if (m_size == 0)
		{
			return;
		}

		ListNode<T>* pNode = m_head;
		while (true)
		{
			if (pNode == nullptr)
			{
				break;
			}

			ListNode<T>* deleteNode = pNode;
			pNode = pNode->m_next;

			delete deleteNode;
			m_size--;
		}
	}

private:
	int m_size;

	ListNode<T>* m_head;
	ListNode<T>* m_tail;

	mutex m_mutex;
	condition_variable m_conditionVar;

public:
	void insert(T data)
	{
		lock_guard<mutex> lockGuard(m_mutex);

		ListNode<T>* listNode = new ListNode<T>();
		listNode->m_data = data;

		if (m_size == 0)
		{
			m_head = listNode;
			m_tail = listNode;

			m_size++;
			m_conditionVar.notify_one();
			return;
		}

		//接下来是已经有节点，大小为一不存在在中间的情况
		//data最小
		if (m_head->m_data >= data)
		{
			listNode->m_next = m_head;
			m_head->m_previous = listNode;
			m_head = listNode;

			m_size++;
			m_conditionVar.notify_one();
			return;
		}
		//data最大
		if (m_tail->m_data <= data)
		{
			m_tail->m_next = listNode;
			listNode->m_previous = m_tail;
			m_tail = listNode;

			m_size++;
			m_conditionVar.notify_one();
			return;
		}

		//在链表中间
		ListNode<T>* pNode = m_head->m_next;
		while (true)
		{
			if (pNode->m_data >= data)
			{
				ListNode<T>* previousNode = pNode->m_previous;
				previousNode->m_next = listNode;
				listNode->m_previous = previousNode;

				pNode->m_previous = listNode;
				listNode->m_next = pNode;

				m_size++;
				m_conditionVar.notify_one();
				return;
			}

			pNode = pNode->m_next;
		}
	}

	unique_ptr<T> front()
	{
		unique_lock<mutex> uniqueLock(m_mutex);
		if (m_size <= 0)
		{
			return unique_ptr<T>(new T);
		}
		else
		{
			return unique_ptr<T>(new T(m_head->m_data));
		}
	}

	bool popFront(T& data)
	{
		lock_guard<mutex> lockGuard(m_mutex);

		if (m_size == 0)
		{
			return false;
		}

		ListNode<T>* pNode = m_head;
		m_head = m_head->m_next;

		data = pNode->m_data;
		delete pNode;

		return true;
	}

	T waitPopFront()
	{
		unique_lock<mutex> uniqueLock(m_mutex);
		m_conditionVar.wait(uniqueLock, [this] {
			if (m_size == 0)
			{
				return false;
			}
			else
			{
				return true;
			}
		});

		ListNode<T>* pNode = m_head;
		m_head = m_head->m_next;

		T data = pNode->m_data;
		delete pNode;

		return data;
	}

	unique_ptr<T> back()
	{
		unique_lock<mutex> uniqueLock(m_mutex);
		if (m_size <= 0)
		{
			return unique_ptr<T>(new T);
		}
		else
		{
			return unique_ptr<T>(new T(m_tail->m_data));
		}
	}

	bool popBack(T& data)
	{
		lock_guard<mutex> lockGuard(m_mutex);

		if (m_size == 0)
		{
			return false;
		}

		ListNode<T>* pNode = m_tail;
		m_tail = m_tail->m_previous;

		data = pNode->m_data;
		delete pNode;

		return true;
	}

	T waitPopBack()
	{
		unique_lock<mutex> uniqueLock(m_mutex);
		m_conditionVar.wait(uniqueLock, [this] {
			if (m_size == 0)
			{
				return false;
			}
			else
			{
				return true;
			}
		});

		ListNode<T>* pNode = m_tail;
		m_tail = m_tail->m_previous;

		T data = pNode->m_data;
		delete pNode;

		return data;
	}

	vector<T> getDataListUsePos(int startIndex, int endIndex)
	{
		lock_guard<mutex> lockGuard(m_mutex);

		vector<T> dataList;

		if (startIndex < 0 || endIndex >= m_size)
		{
			return std::move(dataList);
		}
		
		int dataCount = endIndex - startIndex + 1;
		ListNode<T>* nodePtr = m_head;
		for (int i = 0; i < startIndex; i++)
		{
			nodePtr = nodePtr->m_next;
		}

		for (int i = 0; i < dataCount; i++)
		{
			dataList.push_back(nodePtr->m_data);

			nodePtr = nodePtr->m_next;
		}

		return std::move(dataList);
	}
	
	void remove(T data)
	{
		lock_guard<mutex> lockGuard(m_mutex);

		bool isRemove = false;
		while (true)
		{
			if (m_size == 0)
			{
				m_head = m_tail = nullptr;
				break;
			}

			//接下来是个数大于0
			//删除头部
			if (m_head->m_data == data)
			{
				ListNode<T>* pNode = m_head;
				m_head = m_head->m_next;
				if (m_head != nullptr)
				{
					m_head->m_previous = nullptr;
				}

				delete pNode;
				m_size--;
				continue;
			}

			//删除尾部
			if (m_tail->m_data == data)
			{
				ListNode<T>* pNode = m_tail;
				m_tail = m_tail->m_previous;
				if (m_tail != nullptr)
				{
					m_tail->m_next = nullptr;
				}

				delete pNode;
				m_size--;
				continue;
			}

			//删除中部
			ListNode<T>* pNode = m_head->m_next;
			while (pNode != nullptr)
			{
				if (pNode->m_data == data)
				{
					ListNode<T>* previousNode = pNode->m_previous;
					ListNode<T>* nextNode = pNode->m_next;

					previousNode->m_next = nextNode;
					nextNode->m_previous = previousNode;

					delete pNode;
					m_size--;
					isRemove = true;
					break;
				}

				pNode = pNode->m_next;
			}	

			if (isRemove == true)
			{
				isRemove = false;
				continue;
			}

			break;
		}
	}

	int size()
	{
		lock_guard<mutex> lockGuard(m_mutex);

		return m_size;
	}

	


	void out()
	{
		ListNode<T>* pNode = m_head;
		while (true)
		{
			cout << pNode->m_data << endl;
			pNode = pNode->m_next;

			if (pNode == nullptr)
			{
				break;
			}
		}
	}
};



