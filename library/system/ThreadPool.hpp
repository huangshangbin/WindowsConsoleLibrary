#pragma once


#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <condition_variable>

using namespace std;


class ThreadPool
{
private:
	vector<thread> m_workThreadList;
	mutex m_workThreadMutex;
	condition_variable m_workThreadCondVar;

	queue<function<void()>> m_taskQueue;
	mutex m_taskQueueMutex;


public:
	ThreadPool(int threadCount)
	{

	}
	~ThreadPool()
	{

	}


};

