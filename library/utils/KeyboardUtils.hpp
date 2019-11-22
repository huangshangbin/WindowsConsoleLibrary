#pragma once 

#include <windows.h>


class KeyboardUtils
{
public:
	static void setOpenCapital(bool isOpen)
	{
		// �жϼ���CapsLock���Ƿ���״̬,����״̬��Ϊ��д,����ΪСд
		if (GetKeyState(VK_CAPITAL))
		{
			// �����ǰ����״̬Ϊ��д,Ҫ���Сд,��ģ�ⰴ��CapsLock�л�״̬
			if (!isOpen)
			{
				keybd_event(VK_CAPITAL, NULL, KEYEVENTF_EXTENDEDKEY | 0, NULL);
				keybd_event(VK_CAPITAL, NULL, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, NULL);
			}
		}
		else
		{
			// �����ǰ����״̬ΪСд,Ҫ��Ĵ�д,��ģ�ⰴ��CapsLock�л�״̬
			if (isOpen)
			{
				keybd_event(VK_CAPITAL, NULL, KEYEVENTF_EXTENDEDKEY | 0, NULL);
				keybd_event(VK_CAPITAL, NULL, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, NULL);
			}
		}
	}
	

};

