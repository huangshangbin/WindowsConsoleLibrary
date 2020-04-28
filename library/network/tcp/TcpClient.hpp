#pragma once 

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <memory>
#include <string>

using namespace std;

class TcpClient
{
public:
	static unique_ptr<string> send(string host, int port, string& data)//加上dns解析
	{
		unique_ptr<string> result(new string);

		WORD versionRequired;
		WSADATA wsaData;
		versionRequired = MAKEWORD(1, 1);

		int err = WSAStartup(versionRequired, &wsaData);
		if (err)
		{
			*result = "";
			return std::move(result);
		}

		SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

		linger m_sLinger;
		m_sLinger.l_onoff = 1;  // (在closesocket()调用,但是还有数据没发送完毕的时候容许逗留)
		m_sLinger.l_linger = 0; // (容许逗留的时间为0秒)
		setsockopt(clientSocket, SOL_SOCKET, SO_LINGER, (const char*)&m_sLinger, sizeof(linger));
		err = GetLastError();

		SOCKADDR_IN clientSockIn;
		clientSockIn.sin_addr.S_un.S_addr = inet_addr(host.c_str());
		clientSockIn.sin_family = AF_INET;
		clientSockIn.sin_port = htons(port);

		connect(clientSocket, (SOCKADDR*)&clientSockIn, sizeof(SOCKADDR));
		err = GetLastError();

		::send(clientSocket, data.c_str(), data.length(), 0);
		err = GetLastError();

		char receiveBuf[100];
		int size = recv(clientSocket, receiveBuf, 100, 0);
		while (true)
		{
			if (size <= 0)
			{
				
			}

			result->assign(receiveBuf, size);
		}
		
		err = GetLastError();

		closesocket(clientSocket);
		WSACleanup();

		return std::move(result);
	}
};

