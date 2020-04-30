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

	static bool telnet(string host, int port)
	{
		host = getHostIpList(host)[0];

		WORD versionRequired;
		WSADATA wsaData;
		versionRequired = MAKEWORD(1, 1);

		int err = WSAStartup(versionRequired, &wsaData);

		bool result = true;
		if (err)
		{
			result = false;
		}

		SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

		linger m_sLinger;
		m_sLinger.l_onoff = 1;  // (在closesocket()调用,但是还有数据没发送完毕的时候容许逗留)
		m_sLinger.l_linger = 0; // (容许逗留的时间为0秒)
		setsockopt(clientSocket, SOL_SOCKET, SO_LINGER, (const char*)&m_sLinger, sizeof(linger));

		SOCKADDR_IN clientSockIn;
		clientSockIn.sin_addr.S_un.S_addr = inet_addr(host.c_str());
		clientSockIn.sin_family = AF_INET;
		clientSockIn.sin_port = htons(port);

		err = connect(clientSocket, (SOCKADDR*)&clientSockIn, sizeof(SOCKADDR));
		if (err)
		{
			result = false;
		}

		closesocket(clientSocket);
		WSACleanup();

		return result;
	}
};

