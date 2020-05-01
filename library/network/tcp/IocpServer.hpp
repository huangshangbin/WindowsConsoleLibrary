#pragma once

#include <iostream>
#include <string>
#include <deque>

#include <atomic>
#include <thread>

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>

using namespace std;

#define IOCP_BUFFER_SIZE 1024 * 2



struct IocpSocketData
{
	WSAOVERLAPPED	m_overlapped;
	WSABUF			m_wsaBuffer;   //�첽�շ���Ϣ��buffer,ֻ��һ����ַ����Ҫ���Լ���buffer
	SOCKET			m_clientSocket;
	
	char			m_buffer[IOCP_BUFFER_SIZE];
	string m_totalBuffer;
};

class IocpServer
{
public:
	IocpServer()
	{
		m_isStop = false;
		m_iocpHandle = INVALID_HANDLE_VALUE;

		for (int i = 0; i < std::thread::hardware_concurrency(); i++)
		{
			m_workThreadList.emplace_back([this]() {

				DWORD sendByteSize = 0;
				DWORD dwFlags = 0;
				DWORD receiveByteSize = 0;
				IocpSocketData* iocpSocketData;
				IocpSocketData* iocpKey;

				while (true)
				{
					if (this->m_isStop)
					{
						break;
					}
					if (this->m_iocpHandle == INVALID_HANDLE_VALUE)
					{
						continue;
					}

					BOOL isSuccess = GetQueuedCompletionStatus(this->m_iocpHandle, &receiveByteSize,
						(PULONG_PTR)&iocpKey, (LPOVERLAPPED*)&iocpSocketData, INFINITE);

					if (isSuccess == FALSE)//�ͻ��˶Ͽ�������
					{
						closesocket(iocpSocketData->m_clientSocket);
						delete iocpSocketData;
						continue;
					}

					if (receiveByteSize == 0)//���ܿͻ��˷�����һ���հ�����
					{
						closesocket(iocpSocketData->m_clientSocket);
						delete iocpSocketData;
						continue;
					}

					iocpSocketData->m_totalBuffer.append(iocpSocketData->m_wsaBuffer.buf, receiveByteSize);
					if (this->isCompleteData(iocpSocketData->m_totalBuffer))
					{
						string response;
						this->handle(iocpSocketData->m_totalBuffer, response);
						iocpSocketData->m_totalBuffer.clear();

						::send(iocpSocketData->m_clientSocket, response.c_str(), response.length(), 0);//ͬ�����ͣ��������ݿ���
					}

					ZeroMemory(&(iocpSocketData->m_overlapped), sizeof(OVERLAPPED));				
					ZeroMemory(iocpSocketData->m_wsaBuffer.buf, IOCP_BUFFER_SIZE);
					iocpSocketData->m_wsaBuffer.len = IOCP_BUFFER_SIZE;

					dwFlags = 0;
					WSARecv(iocpSocketData->m_clientSocket, &(iocpSocketData->m_wsaBuffer), 1, &receiveByteSize, &dwFlags,
						(LPWSAOVERLAPPED)&(iocpSocketData->m_overlapped), NULL);
				}
			});


		}
	}

	~IocpServer()
	{
		m_isStop = true;
		for (thread& workThread : m_workThreadList)
		{
			workThread.join();
		}
	}

private:
	atomic_bool m_isStop;

	HANDLE m_iocpHandle;
	deque<thread> m_workThreadList;


public:
	virtual bool isCompleteData(string& dataBuffer) = 0;
	virtual void handle(string& request, string& response) = 0;

public:
	bool listen(string ip, int port)
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);

		SOCKET serverSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

		SOCKADDR_IN serverSocketAddr;
		serverSocketAddr.sin_family = PF_INET;
		serverSocketAddr.sin_port = htons(port);
		serverSocketAddr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());

		if (::bind(serverSocket, (struct sockaddr*)&serverSocketAddr, sizeof(SOCKADDR_IN)))
		{
			return false;
		}
		if (::listen(serverSocket, 5))
		{
			return false;
		}

		m_iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);


		SOCKADDR_IN clientScoketAddr;
		int addrLength = sizeof(SOCKADDR_IN);

		SOCKET clientSocket;
		DWORD receiveByteSize;
		DWORD flags = 0;

		while (true)
		{
			clientSocket = WSAAccept(serverSocket, (struct sockaddr*)&clientScoketAddr, &addrLength, NULL, NULL);

			IocpSocketData* iocpSocketData = new IocpSocketData();
			iocpSocketData->m_clientSocket = clientSocket;
			iocpSocketData->m_wsaBuffer.len = IOCP_BUFFER_SIZE;
			iocpSocketData->m_wsaBuffer.buf = iocpSocketData->m_buffer;

			iocpSocketData->m_totalBuffer.clear();

			m_iocpHandle = CreateIoCompletionPort((HANDLE)clientSocket, m_iocpHandle, (DWORD)iocpSocketData, 0);


			WSARecv(clientSocket, &(iocpSocketData->m_wsaBuffer), 1, &receiveByteSize, &flags,
		          &(iocpSocketData->m_overlapped), NULL);
		}

		return true;
	}
};

