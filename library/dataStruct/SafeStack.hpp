#pragma once

#include <stack>
#include <memory>
#include <mutex>
#include <condition_variable>

using namespace std;

template<typename T>
class SafeStack
{
public:
	SafeStack() {}

private:
	mutex m_mutex;
	condition_variable m_conditionVar;

	stack<T> m_stack;

public:
	void push(T data)
	{
		lock_guard<mutex> lockGuard(m_mutex);
		m_stack.push(data);

		m_conditionVar.notify_one();
	}

	bool pop(T& data)
	{
		lock_guard<mutex> lockGuard(m_mutex);
		if (m_stack.empty())
		{
			return false;
		}

		data = m_stack.top();
		m_stack.pop();

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

		T data = m_stack.top();
		m_stack.pop();

		return data;
	}

	bool empty()
	{
		lock_guard<mutex> lockGuard(m_mutex);

		return m_stack.empty();
	}

	int size()
	{
		lock_guard<mutex> lockGuard(m_mutex);

		return m_stack.size();
	}

	void clear()
	{
		lock_guard<mutex> lockGuard(m_mutex);

		stack<T> tempStack;
		m_stack.swap(tempStack);
	}

private:
	SafeStack(const SafeStack& safeStack) {}
	void operator = (const SafeStack& safeStack) {}
};