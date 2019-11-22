#pragma once 

#include <windows.h>


class KeyboardUtils
{
public:
	static void setOpenCapital(bool isOpen)
	{
		// 判断键盘CapsLock键是否开启状态,开启状态则为大写,否则为小写
		if (GetKeyState(VK_CAPITAL))
		{
			// 如果当前键盘状态为大写,要求改小写,则模拟按键CapsLock切换状态
			if (!isOpen)
			{
				keybd_event(VK_CAPITAL, NULL, KEYEVENTF_EXTENDEDKEY | 0, NULL);
				keybd_event(VK_CAPITAL, NULL, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, NULL);
			}
		}
		else
		{
			// 如果当前键盘状态为小写,要求改大写,则模拟按键CapsLock切换状态
			if (isOpen)
			{
				keybd_event(VK_CAPITAL, NULL, KEYEVENTF_EXTENDEDKEY | 0, NULL);
				keybd_event(VK_CAPITAL, NULL, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, NULL);
			}
		}
	}
	

};

