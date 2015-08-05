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
#include "computerfinder.h"
#include "wservicestart.h"
#include "evaluatemedia.h"

#include <iostream>
using namespace std;

int main()
{
	KeyBoardSimulater m_KeyBoardSimulater;
	ComputerFinder m_ComputerFinder;
	WServiceStart m_WServiceStart;
	//m_WServiceStart.StartWServiceSynergy();

	//char command[1024];
	//cin.getline(command,1024);
	//cout << command << endl;
	//
	//int key_Value;

	//if (strcmp(command, "xbu-pc") == 0)
	//{
	//	key_Value = 1;
	//}
	//if (strcmp(command, "xxm-pc") == 0)
	//{
	//	key_Value = 2;
	//}
	//Sleep(5000);
	//m_ComputerFinder.FindALLComputersName();
	//m_ComputerFinder.GetSelfName();
	//m_ComputerFinder.GetIPbyCompterName("xxm-pc");
	//m_KeyBoardSimulater.Switch_Screen_to_FX(key_Value);

	EvaluateMedia m_evaluatemedia = EvaluateMedia(false);
	m_evaluatemedia.TrackingFace();
	return 0;
}