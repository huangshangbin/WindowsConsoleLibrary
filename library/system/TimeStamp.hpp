#pragma once

#include <iostream>
#include <string>

#include <library/system/TimeStamp.hpp>

#include <library/utils/TypeConvertUtils.hpp>

#include <time.h>

using namespace std;

//时间戳精确到秒比较好序列化 ,未完成
class TimeStamp
{
public:
	TimeStamp() { update();  }
	TimeStamp(const TimeStamp& timeStamp)
	{
		m_time = timeStamp.m_time;
	}
	~TimeStamp() {}


private:
	time_t m_time;


public:
 	void update()
	{
		time(&m_time);
	}

	long toLong()
	{
		return m_time;
	}

	string toString()
	{
		return TypeConvertUtils::getString(m_time);
	}


public:
	void operator = (const TimeStamp& timeStamp)
	{
		m_time = timeStamp.m_time;
	}

	long operator - (TimeStamp& timeStamp)
	{
		return timeStamp.toLong() - this->toLong();
	}
};

