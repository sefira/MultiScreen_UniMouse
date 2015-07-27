#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

void simulating_Keybd_Input(int key_Value)
{
	INPUT m_Keybd_Input;
	m_Keybd_Input.type = INPUT_KEYBOARD;
	m_Keybd_Input.ki.wScan = 0;
	m_Keybd_Input.ki.time = 0;
	m_Keybd_Input.ki.dwExtraInfo = 0;

	int F_Who = 0;
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
	return ;
}

int main()
{
	char command[1024];
	cin.getline(command,1024);
	cout << command << endl;
	
	int key_Value;

	if (strcmp(command, "xbu-pc") == 0)
	{
		key_Value = 1;
	}
	if (strcmp(command, "xxm_pc") == 0)
	{
		key_Value = 2;
	}

	simulating_Keybd_Input(key_Value);
	return 0;
}