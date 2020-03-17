#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>

#include "winsvc.h"


#include <library/system/Console.hpp>

#include <string>
#include <deque>

#include <iostream>
using namespace std;

class Service
{
public:
	Service(string name)
	{
		m_serviceManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (m_serviceManager == NULL)
		{
			return;
		}

		m_service = OpenService(m_serviceManager, name.c_str(), SERVICE_QUERY_CONFIG|SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	}

	~Service() 
	{
		::CloseServiceHandle(m_serviceManager);
		::CloseServiceHandle(m_service);
	}

private:
	SC_HANDLE m_serviceManager;
	SC_HANDLE m_service;

public:
	string getPath()
	{
		if (m_service == NULL)
		{
			return "";
		}

		LPQUERY_SERVICE_CONFIG serviceConfig = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR, 8 * 1024);

		DWORD resumeHandle;
		BOOL queryStatus = QueryServiceConfig(m_service, serviceConfig, 8 * 1024, &resumeHandle);
		if (queryStatus == NULL)
		{
			return "";
		}

		return serviceConfig->lpBinaryPathName;
	}

	void start()
	{
		if (m_service == NULL)
		{
			return;
		}

		SERVICE_STATUS serviceStatus;
		::QueryServiceStatus(m_service, &serviceStatus);

		if (serviceStatus.dwCurrentState != SERVICE_RUNNING)
		{
			::StartService(m_service, NULL, NULL);

			::Sleep(serviceStatus.dwWaitHint);
			while (1)
			{
				::QueryServiceStatus(m_service, &serviceStatus);
				if (serviceStatus.dwCurrentState == SERVICE_RUNNING)
				{
					break;
				}
			}
		}
	}

	void stop()
	{
		if (m_service == NULL)
		{
			return;
		}

		SERVICE_STATUS serviceStatus;
		::QueryServiceStatus(m_service, &serviceStatus);

		if (serviceStatus.dwCurrentState == SERVICE_RUNNING)
		{
			::ControlService(m_service, SERVICE_CONTROL_STOP, &serviceStatus);

			::Sleep(serviceStatus.dwWaitHint);
			while (1)
			{
				::QueryServiceStatus(m_service, &serviceStatus);
				if (serviceStatus.dwCurrentState == SERVICE_STOPPED)
				{
					break;
				}
			}
		}
	}

	void restart()
	{
		stop();
		start();
	}


public:
	static deque<string> getAllServiceName()
	{
		deque<string> nameList;

		SC_HANDLE managerHandle = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (managerHandle == NULL)
		{
			return std::move(nameList);
		}

		DWORD byteNeed = NULL;
		DWORD serviceCount = NULL;
		DWORD resumeHandle = NULL;

		LPENUM_SERVICE_STATUS enumServiceStatus = (LPENUM_SERVICE_STATUS)LocalAlloc(LPTR, 1024 * 64);//1024¸ö
		BOOL status = EnumServicesStatus(managerHandle, SERVICE_WIN32, SERVICE_STATE_ALL, enumServiceStatus, 1024 * 64, &byteNeed, &serviceCount, &resumeHandle);
		if (status == NULL)
		{
			return std::move(nameList);
		}

		for (int i = 0; i < serviceCount; i++)
		{
			string serviceName = enumServiceStatus[i].lpServiceName;
			nameList.push_back(serviceName);
		}

		::CloseServiceHandle(managerHandle);

		return std::move(nameList);
	}
};

