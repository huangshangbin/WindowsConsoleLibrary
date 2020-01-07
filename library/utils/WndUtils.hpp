#pragma once

#include <windows.h>

#include <library/utils/StringUtils.hpp>

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
//����
public:
	static deque<WndInfo> getAllWndInfoList()
	{
		deque<WndInfo> wndInfoList;
		EnumWindows(getAllWndInfoListCall, (LPARAM)&wndInfoList);

		return move(wndInfoList);
	}

	static HWND getWnd(string wndExitText, bool isWait = false)//�����ı���һ����
	{
		while (true)
		{
			deque<WndInfo> wndList = getAllWndInfoList();
			for (WndInfo wndInfo : wndList)
			{
				if (StringUtils::isExistStringInString(wndInfo.m_text, wndExitText))
				{
					return wndInfo.m_hwnd;
				}
			}

			if (isWait == false)
			{
				break;
			}
		}

		return 0;
	}

	static DWORD getWndProcessId(HWND wnd)
	{
		DWORD processId;
		GetWindowThreadProcessId(wnd, &processId);

		return processId;
	}

	static deque<WndInfo> getChildWndInfoList(HWND wnd)
	{
		deque<WndInfo> wndInfoList;
		EnumChildWindows(wnd, getChildWndInfoListCall, (LPARAM)&wndInfoList);

		return move(wndInfoList);
	}

	static string getWndText(HWND wnd)
	{
		char caption[200];
		memset(caption, 0, sizeof(caption));
		::GetWindowText(wnd, caption, 200);

		string text = caption;
		return text;
	}

	static void closeWnd(HWND wnd)
	{
		PostMessageA(wnd, WM_CLOSE, 0, 0);
	}

//��Ϣ
public:
	static void sendCommandMsg(HWND wnd, int cmdId)
	{
		PostMessageA(wnd, WM_COMMAND, cmdId, 0);
	}

//�ؼ�
public:
	static int getCtrlId(HWND ctrlWnd)
	{
		return GetDlgCtrlID(ctrlWnd);
	}

	static HMENU getWndMenu(HWND wnd)
	{
		return GetMenu(wnd);
	}

//����
public:
	static void clickCheckBox(HWND checkBoxWnd)
	{
		LPARAM lparam = MAKELPARAM(3, 3); //x���꣬y����
		LRESULT result = ::SendMessage(checkBoxWnd, WM_LBUTTONDOWN, MK_LBUTTON, lparam);
		LRESULT result1 = ::SendMessage(checkBoxWnd, WM_LBUTTONUP, MK_LBUTTON, lparam);
	}

//��ť
public:
	static void clickButton(HWND buttonWnd)
	{
		SendMessageA(buttonWnd, BM_CLICK, 0, 0);
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

//getChildWndInfoList
private:
	static BOOL CALLBACK getChildWndInfoListCall(HWND hwnd, LPARAM lParam)
	{
		deque<WndInfo>* wndInfoList = (deque<WndInfo>*) lParam;

		char caption[200];
		memset(caption, 0, sizeof(caption));
		::GetWindowText(hwnd, caption, 200);

		string text = caption;
		//if (text != "") ��Щ�ؼ���ȡ�����ı������Բ��ܹ���
		WndInfo wndInfo;
		wndInfo.m_hwnd = hwnd;
		wndInfo.m_text = text;

		wndInfoList->emplace_back(wndInfo);

		return TRUE;
	}
};

