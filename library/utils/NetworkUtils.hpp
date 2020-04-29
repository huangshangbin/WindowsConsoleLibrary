#pragma once

#include <iostream>
#include <deque>

using namespace std;

#include <WinSock2.h>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")


class NetworkUtils
{
public:
	static deque<string> getHostIpList(string hostName)//支持局域网主机名，公网域名
	{
		WSADATA wsData;
		::WSAStartup(MAKEWORD(2, 2), &wsData);

		hostent* hn;
		hn = gethostbyname(hostName.c_str());

		in_addr addr;
		deque<string> ipList;
		for (int i = 0; ; i++)
		{
			char* addrP = hn->h_addr_list[i];
			if (addrP == NULL)
			{
				break;
			}

			memcpy(&addr.S_un.S_addr, addrP, hn->h_length);

			string ip = ::inet_ntoa(addr);
			ipList.push_back(ip);
		}

		::WSACleanup();

		return std::move(ipList);
	}

};

