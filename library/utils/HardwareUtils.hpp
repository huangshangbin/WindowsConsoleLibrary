#pragma once 

#include <windows.h>

#include <library/utils/KeyboardUtils.hpp>

class HardwareUtils
{
//���
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

//����
public:
	static void keyboardText(string text, int spaceTime)
	{
		int iLen = 0;
		char *tmp = NULL;
		INPUT *keys = NULL;
		BOOL bOldState = FALSE;

		// ����˲���ǰ�ļ���״̬
		bOldState = (BOOL)GetKeyState(VK_CAPITAL);
		iLen = lstrlen(text.c_str());
		tmp = (char *)malloc(iLen);
		memmove(tmp, text.c_str(), iLen);
		for (int i = 0; i < iLen; i++)
		{
			Sleep(spaceTime);

			// ĳЩ���ŷ�ֱ�����̰���,����ֻ����ת������,�Ժ��õ������ַ�������ӹ���
			if (tmp[i] == ' ')
			{
				// ����һ��������Ϣ����:����->̧��
				keybd_event(VK_SPACE, NULL, KEYEVENTF_EXTENDEDKEY | 0, NULL);
				keybd_event(VK_SPACE, NULL, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, NULL);
			}
			else if (tmp[i] == ',')
			{
				keybd_event(VK_OEM_COMMA, NULL, KEYEVENTF_EXTENDEDKEY | 0, NULL);
				keybd_event(VK_OEM_COMMA, NULL, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, NULL);
			}
			else if (tmp[i] >= 'a' && tmp[i] <= 'z')
			{ // �����ַ���Сд�л����̴�Сд״̬


				KeyboardUtils::setOpenCapital(false);

				// keybd_eventֻʶ���д
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
		// �ָ��˲���֮ǰ�ļ���״̬
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

