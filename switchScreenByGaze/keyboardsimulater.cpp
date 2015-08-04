/*
The MIT License(MIT)

Copyright(c) 2015 Bu Xingyuan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "keyboardsimulater.h"
#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

KeyBoardSimulater::KeyBoardSimulater()
{
	
}

KeyBoardSimulater::~KeyBoardSimulater()
{

}

void KeyBoardSimulater::Switch_Screen_to_FX(int key_Value)
{
	INPUT m_Keybd_Input;
	m_Keybd_Input.type = INPUT_KEYBOARD;
	m_Keybd_Input.ki.wScan = 0;
	m_Keybd_Input.ki.time = 0;
	m_Keybd_Input.ki.dwExtraInfo = 0;

	int F_Who = 0x41;
	switch (key_Value)
	{
	case 1:
		F_Who = VK_F1;
		break;
	case 2:
		F_Who = VK_F2;
		break;
	case 3:
		F_Who = VK_F3;
		break;
	}

	//press key
	m_Keybd_Input.ki.dwFlags = 0;	//0 for press
	//press ctrl
	m_Keybd_Input.ki.wVk = VK_CONTROL;
	SendInput(1, &m_Keybd_Input, sizeof(INPUT));
	//press alt
	m_Keybd_Input.ki.wVk = VK_MENU;
	SendInput(1, &m_Keybd_Input, sizeof(INPUT));
	//press shift
	m_Keybd_Input.ki.wVk = VK_SHIFT;
	SendInput(1, &m_Keybd_Input, sizeof(INPUT));
	//press F(x)
	m_Keybd_Input.ki.wVk = F_Who;
	SendInput(1, &m_Keybd_Input, sizeof(INPUT));

	//release key
	m_Keybd_Input.ki.dwFlags = KEYEVENTF_KEYUP;
	//press ctrl
	m_Keybd_Input.ki.wVk = VK_CONTROL;
	SendInput(1, &m_Keybd_Input, sizeof(INPUT));
	//press alt
	m_Keybd_Input.ki.wVk = VK_MENU;
	SendInput(1, &m_Keybd_Input, sizeof(INPUT));
	//press shift
	m_Keybd_Input.ki.wVk = VK_SHIFT;
	SendInput(1, &m_Keybd_Input, sizeof(INPUT));
	//press F(x)
	m_Keybd_Input.ki.wVk = F_Who;
	SendInput(1, &m_Keybd_Input, sizeof(INPUT));
	cout << "simulating input" << endl;
	return;
}