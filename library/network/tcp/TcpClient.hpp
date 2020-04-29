#pragma once 

#include <library/utils/NetworkUtils.hpp>

#include <iostream>
#include <memory>
#include <string>

using namespace std;

class TcpClient
{
public:
	static unique_ptr<string> send(string host, int port, string& data)
	{
		host = NetworkUtils::getHostIpList(host)[0];//dns解析

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

		const int bufferLength = 1024 * 20;//尽量大,recv阻塞可能性小
		char receiveBuf[bufferLength];
		
		while (true)
		{
			int size = recv(clientSocket, receiveBuf, bufferLength, 0);//服务端发送数据刚好为bufferLength(或者倍数),这里会阻塞，没有协议，无法解决
			if (size <= 0)
			{
				break;
			}

			result->append(receiveBuf, size);

			if (size < bufferLength)
			{
				break;
			}
		}
		
		err = GetLastError();

		closesocket(clientSocket);
		WSACleanup();

		return std::move(result);
	}
};

