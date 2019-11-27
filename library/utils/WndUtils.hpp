#pragma once

#include <windows.h>

#include <deque>
#include <string>
#include <iostream>
using namespace std;

class WndInfo
{
public:
	HWND m_hwnd;
	string m_text;

};

class WndUtils
{
public:
	static deque<WndInfo> getAllWndInfoList()
	{
		deque<WndInfo> wndInfoList;
		EnumWindows(getAllWndInfoListCall, (LPARAM)&wndInfoList);

		return move(wndInfoList);
	}

	static DWORD getWndProcessId(HWND wnd)
	{
		DWORD processId;
		GetWindowThreadProcessId(wnd, &processId);

		return processId;
	}


//getAllWndInfoList
private:
	static BOOL CALLBACK getAllWndInfoListCall(HWND hwnd, LPARAM lParam)
	{
		deque<WndInfo>* wndInfoList = (deque<WndInfo>*) lParam;

		char caption[200];
		memset(caption, 0, sizeof(caption));
		::GetWindowText(hwnd, caption, 200);

		string text = caption;
		if (text != "")
		{
			WndInfo wndInfo;
			wndInfo.m_hwnd = hwnd;
			wndInfo.m_text = text;

			wndInfoList->emplace_back(wndInfo);
		}

		return TRUE;
	}
};

