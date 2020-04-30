#pragma once

#include <time.h>
#include <sys/timeb.h>


class SpaceTime
{
private:
	timeb m_startTime;
	timeb m_endTime;
	

public:
	void recordStartTime()
	{
		ftime(&m_startTime);
	}

	void recordEndTime()
	{
		ftime(&m_endTime);
	}


public:
	long long getSpaceMilliSecond()
	{
		long long startTimeRecord = m_startTime.time * 1000 + m_startTime.millitm;

		long long endTimeRecord = m_endTime.time * 1000 + m_endTime.millitm;

		return endTimeRecord - startTimeRecord;
	}

	long long getSpaceSecond()
	{
		return getSpaceMilliSecond() / 1000;
	}

};

