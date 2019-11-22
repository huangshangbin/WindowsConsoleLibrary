#pragma once 

#include <windows.h>

#include <library/utils/KeyboardUtils.hpp>

class HardwareUtils
{
//鼠标
public:
	static void mouseMove(int x, int y)
	{
		SetCursorPos(x, y);
	}

	static void mouseLeftButton(int x, int y)
	{
		SetCursorPos(x, y);
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, x, y, 0, 0);
	}

//键盘
public:
	static void keyboardText(string text, int spaceTime)
	{
		int iLen = 0;
		char *tmp = NULL;
		INPUT *keys = NULL;
		BOOL bOldState = FALSE;

		// 保存此操作前的键盘状态
		bOldState = (BOOL)GetKeyState(VK_CAPITAL);
		iLen = lstrlen(text.c_str());
		tmp = (char *)malloc(iLen);
		memmove(tmp, text.c_str(), iLen);
		for (int i = 0; i < iLen; i++)
		{
			Sleep(spaceTime);

			// 某些符号非直属键盘按键,这里只过滤转换两个,以后用到其它字符自行添加过滤
			if (tmp[i] == ' ')
			{
				// 产生一个击键消息步骤:按下->抬起
				keybd_event(VK_SPACE, NULL, KEYEVENTF_EXTENDEDKEY | 0, NULL);
				keybd_event(VK_SPACE, NULL, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, NULL);
			}
			else if (tmp[i] == ',')
			{
				keybd_event(VK_OEM_COMMA, NULL, KEYEVENTF_EXTENDEDKEY | 0, NULL);
				keybd_event(VK_OEM_COMMA, NULL, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, NULL);
			}
			else if (tmp[i] >= 'a' && tmp[i] <= 'z')
			{ // 根据字符大小写切换键盘大小写状态


				KeyboardUtils::setOpenCapital(false);

				// keybd_event只识别大写
				keybd_event((BYTE)tmp[i] - 32, NULL, KEYEVENTF_EXTENDEDKEY | 0, NULL);
				keybd_event((BYTE)tmp[i] - 32, NULL, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, NULL);
			}
			else if ((tmp[i] >= 'A' && tmp[i] <= 'Z') || (tmp[i] >= '0' && tmp[i] <= '9'))
			{
				KeyboardUtils::setOpenCapital(true);

				keybd_event((BYTE)tmp[i], NULL, KEYEVENTF_EXTENDEDKEY | 0, NULL);
				keybd_event((BYTE)tmp[i], NULL, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, NULL);
			}
			else
			{
				keybd_event((BYTE)tmp[i] + 64, NULL, KEYEVENTF_EXTENDEDKEY | 0, NULL);
				keybd_event((BYTE)tmp[i] + 64, NULL, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, NULL);
			}
		}
		// 恢复此操作之前的键盘状态
		if (bOldState == TRUE)
		{
			KeyboardUtils::setOpenCapital(true);
		}
		else
		{
			KeyboardUtils::setOpenCapital(false);
		}
		free(tmp);
	}

	static void keyboardBack()
	{
		keybd_event(VK_BACK, NULL, KEYEVENTF_EXTENDEDKEY | 0, NULL);
		keybd_event(VK_BACK, NULL, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, NULL);
	}


//keyboardText
private:

};

