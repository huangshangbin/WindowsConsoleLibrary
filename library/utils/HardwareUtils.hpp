#pragma once 

#include <windows.h>

class HardwareUtils
{
//ЪѓБъ
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

//МќХЬ
public:
	static void keyboardText(string text)
	{

	}


//keyboardText
private:

};

