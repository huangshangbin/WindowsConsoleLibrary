#pragma once

/*
   github : https://github.com/yangxingwu/thread_safe_queue/blob/master/thread_safe_queue.cpp
*/

#include <queue>

#include <memory>
#include <mutex>
#include <condition_variable>

using namespace std;

template <typename T>
class SafeQueue
{
public:
	SafeQueue() {}

private:
	mutex m_mutex;
	condition_variable m_conditionVar;

	queue<T> m_queue;

public:
	void push(T data)
	{
		lock_guard<mutex> lockGuard(m_mutex);
		m_queue.push(data);

		m_conditionVar.notify_one();
	}

	bool pop(T& data)
	{
		lock_guard<mutex> lockGuard(m_mutex);
		if (m_queue.empty())
		{
			return false;
		}

		data = m_queue.front();
		m_queue.pop();

		return true;
	}

	T waitPop()
	{
		unique_lock<mutex> uniqueLock(m_mutex);
		m_conditionVar.wait(uniqueLock, [this] {
			if (m_queue.empty())
			{
				return false;
			}
			else
			{
				return true;
			}
		});

		T data = m_queue.front();
		m_queue.pop();

		return data;
	}

	bool empty()
	{
		lock_guard<mutex> lockGuard(m_mutex);

		return m_queue.empty();
	}

	int size()
	{
		lock_guard<mutex> lockGuard(m_mutex);

		return m_queue.size();
	}

	void clear()
	{
		lock_guard<mutex> lockGuard(m_mutex);

		queue<T> tempQueue;
		m_queue.swap(tempQueue);
	}



private:
	SafeQueue(const SafeQueue& safeQueue) {}
	void operator = (const SafeQueue& safeQueue) {}
};