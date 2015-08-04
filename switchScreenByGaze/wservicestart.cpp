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

#include "WServiceStart.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>


WServiceStart::WServiceStart()
{

}

WServiceStart::~WServiceStart()
{

}

int WServiceStart::StartWServiceSynergy()
{
	HANDLE StdHandle;
	
	SC_HANDLE hService;
	SC_HANDLE hSCManager;
	SERVICE_STATUS ssStatus;
	char *hsService = "Synergy";
	StdHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	if ((hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
	{
		printf("��ϵͳ�������̨ʧ��", "%s");
		CloseServiceHandle(hSCManager);
	}

	if ((hService = OpenService(hSCManager, hsService, SERVICE_ALL_ACCESS)) == NULL)//ACCESS_SYSTEM_SECURITY
	{

		printf("�򿪷���ʧ�ܣ�\n", "%s");
		CloseServiceHandle(hService);
		CloseServiceHandle(hSCManager);
		//return 0;
	}
	else
	{
		//printf("����򿪳ɹ���\n");
		if (QueryServiceStatus(hService, &ssStatus))
		{
			printf("\n");
			printf("��ǰ��������״̬Ϊ:   ");
			if (ssStatus.dwCurrentState == SERVICE_STOPPED) printf("SERVICE_STOPPED\n");
			if (ssStatus.dwCurrentState == SERVICE_RUNNING) printf("SERVICE_RUNNING\n");
			if (ssStatus.dwCurrentState == SERVICE_START_PENDING) printf("SERVICE_START_PENDING\n");
			if (ssStatus.dwCurrentState == SERVICE_STOP_PENDING) printf("SERVICE_STOP_PENDING\n");
			if (ssStatus.dwCurrentState == SERVICE_CONTINUE_PENDING) printf("SERVICE_CONTINUE_PENDING\n");
			if (ssStatus.dwCurrentState == SERVICE_PAUSE_PENDING) printf("SERVICE_PAUSE_PENDING\n");
			if (ssStatus.dwCurrentState == SERVICE_PAUSED) printf("SERVICE_PAUSED\n");

			if (ssStatus.dwCurrentState == SERVICE_STOPPED)
			{
				printf("������������");
				if (StartService(hService, 0, NULL) == 0)
				{
					printf("��������ʧ��\n");
					CloseServiceHandle(hService);
					CloseServiceHandle(hSCManager);
					//return 0;
				}
				else
				{
					printf("���������ɹ�\n");
					//return 1;
				}
			}
			else
			{

			}
		}
	}
	Sleep(5000);
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
	return 1;
}