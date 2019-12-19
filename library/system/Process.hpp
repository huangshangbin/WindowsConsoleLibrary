#pragma once

#include "Tlhelp32.h"

#include   "psapi.h"
#pragma   comment(lib,"psapi.lib")

#include <library/utils/FileUtils.hpp>

#include <Windows.h>

#include <string>

#include <iostream>
using namespace std;

class Process
{
public:
	Process(DWORD id) { m_id = id; }
	~Process() {}

private:
	DWORD m_id;

//�����÷�
public:
	string getPath()
	{
		string processPath;

		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, m_id);
		if (hProcess)
		{
			char pathChArray[MAX_PATH];
			if (GetModuleFileNameEx(hProcess, NULL, pathChArray, MAX_PATH) != 0)
			{
				processPath = pathChArray;
			}
			
			CloseHandle(hProcess);
		}

		return processPath;
	}

	string getDir()
	{
		return FileUtils::getPreviousLayerPath(getPath());
	}

	HANDLE getHandle()
	{
		return ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_id);
	}

	void close()
	{
		HANDLE hHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_id);
		TerminateProcess(hHandle, 0);
	}

//�߼��÷�
public:
	bool promotePower()
	{
		HANDLE processHandle = getHandle();
		HANDLE hToken;
		LUID sedebugnameValue;
		TOKEN_PRIVILEGES tkp;
		if (!OpenProcessToken(processHandle, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		{
			return false;
		}

		if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
		{
			CloseHandle(hToken);
			return false;
		}

		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Luid = sedebugnameValue;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
		{
			CloseHandle(hToken);
			return false;
		}

		return true;
	}

	// ע��32λ���� ֻ����32λ����ע��32λdll��Ŀ��32λ����64λͬ��
	void injectionDll(string dllPath)
	{
		SIZE_T dwThreadSize = 5 * 1024;
		DWORD dwWriteBytes;

		if (promotePower() == false)
		{
			cout<<"����Ȩ��ʧ��"<<endl;
		}

		HANDLE hTargetProcess = getHandle();
		if (!hTargetProcess)
		{
			cout << "��ȡ���̾��ʧ��" << endl;
			return;
		}

		void* pRemoteThread = VirtualAllocEx(hTargetProcess, NULL, dwThreadSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (!pRemoteThread)
		{
			cout << "pRemoteThread  error" << endl;
			return;
		}

		if (!WriteProcessMemory(hTargetProcess, pRemoteThread, (LPVOID)dllPath.c_str(), dwThreadSize, 0))
		{
			cout << "WriteProcessMemory  error" << endl;
			return;
		}

		LPVOID pFunc = LoadLibraryA;
		HANDLE hRemoteThread = CreateRemoteThread(hTargetProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunc,
			pRemoteThread, 0, &dwWriteBytes);
		if (!hRemoteThread)
		{
			cout << "hRemoteThread  error" << endl;
			return;
		}
		
		//WaitForSingleObject(hRemoteThread, INFINITE); //��dll�������ѭ������������
		
		VirtualFreeEx(hTargetProcess, pRemoteThread, dwThreadSize, MEM_COMMIT);

		CloseHandle(hRemoteThread);
		CloseHandle(hTargetProcess);
	}


public:
	static long getCurProcessId()
	{
		return ::GetCurrentProcessId();
	}

	static deque<Process> getAllProcess()
	{
		deque<Process> processList;

		HANDLE hHandle = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hHandle != INVALID_HANDLE_VALUE)
		{
			PROCESSENTRY32 stEntry;
			stEntry.dwSize = sizeof(PROCESSENTRY32);
			if (::Process32First(hHandle, &stEntry))
			{
				processList.push_back(Process(stEntry.th32ProcessID));
			}

			while (::Process32Next(hHandle, &stEntry))
			{
				processList.push_back(Process(stEntry.th32ProcessID));
			}
		}

		CloseHandle(hHandle);

		return std::move(processList);
	}
};
