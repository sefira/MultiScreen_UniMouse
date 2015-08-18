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

#include "computer.h"
#include "evaluatemedia.h"

#include <stdlib.h>
#include <time.h>  

#include <iostream>
#include <process.h>
#include <Winsock2.h>

#include "messenger.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;


Computer::Computer()
{
	QueryLocalHostname();
	memset(local_IP, 0, sizeof(local_IP));
	evaluate_point = 1000;
	num = 0;
	amiamonitor = false;
}

Computer::~Computer()
{

}

int Computer::ToString()
{
	cout << "num:" << num << endl;
	cout << "hostname:" << local_hostname << endl;
	cout << "IP:" << local_IP << endl;
	cout << "evaulate:" << evaluate_point << endl;

	return 0;
}

int Computer::QueryLocalHostname()
{
	struct hostent *m_hostent;

	WSADATA wsaData; 

	// Initialize Winsock
	int result;
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		printf("WSAStartup failed: %d\n", result);
		return 1;
	}

	result = gethostname(local_hostname, sizeof(local_hostname));
	if (result != 0) {
		printf("gethostname failed: %d\n", result);
		return 1;
	}

	WSACleanup();

	return 0;
}

int Computer::QueryHostIPbyName(char * hostname, Computer &m_computer)
{

	//-----------------------------------------
	// Declare and initialize variables
	WSADATA wsaData;
	int result;

	DWORD dwError;
	int i = 0;

	struct hostent *remotehost;
	struct in_addr addr;

	char **pAlias;

	// Initialize Winsock
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("WSAStartup failed: %d\n", result);
		return 1;
	}

	//printf("Calling gethostbyname with %s\n", hostname);
	remotehost = gethostbyname(hostname);

	if (remotehost == NULL)
	{
		dwError = WSAGetLastError();
		if (dwError != 0) {
			if (dwError == WSAHOST_NOT_FOUND)
			{
				printf("Host not found\n");
				return 1;
			}
			else if (dwError == WSANO_DATA)
			{
				printf("No data record found\n");
				return 1;
			}
			else
			{
				printf("Function failed with error: %ld\n", dwError);
				return 1;
			}
		}
	}
	else
	{
		strcpy(m_computer.local_hostname, remotehost->h_name);

		if (remotehost->h_addr_list[0] != 0)
		{
			addr.s_addr = *(u_long *)remotehost->h_addr_list[i++];
			strcpy(m_computer.local_IP, inet_ntoa(addr));
		}
		else
		{
			return 1;
		}
		return 0;
	}
}

int Computer::ConncetWithServer()
{
	return Messenger::ClientConnectServer(monitor_IP, socket_client);
}

double Computer::QueryDeviation()
{
	double dev = EvaluateMedia::GetDeviation();

	return dev;
}

int Computer::SendHostname()
{
	//socket send , a really send action
	int ret = 1;
	for (int i = 0; i < 5; i++)
	{
		cout << local_hostname << endl;
		char sendbuf[128];
		sendbuf[0] = 1;
		sendbuf[1] = 0;
		strcpy(sendbuf, local_hostname);
		if (SOCKET_ERROR != Messenger::SendMessagetoServer(socket_client, sendbuf))
		{
			ret = 0;
		}
		Sleep(TIMEINTERVAL);
	}
	
	return ret;
}

unsigned int __stdcall Computer::InterfacetoEvaluateMedia(void *)
{
	EvaluateMedia m_evaluatemedia = EvaluateMedia(false);
	//m_evaluatemedia.TrackingFace();
	//cout << m_evaluatemedia.GetDeviation() << endl;

	//test
	srand((unsigned)time(NULL));
	m_evaluatemedia.SetDeviation(rand());

	return 0;
}

int Computer::BegintoWork()
{
	HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, Computer::InterfacetoEvaluateMedia, NULL, 0, NULL);
	
	return 0;
}

int Computer::SendDeviation()
{
	//socket send , a really send action
	int ret = 1;
	while (1)
	{
		double deviation = QueryDeviation();
		cout << deviation << endl;
		char sendbuf[128];
		sendbuf[0] = 2;
		sendbuf[1] = 0;
		char deviation_str[128];
		sprintf(deviation_str, "%f", deviation);
		strcpy(sendbuf, deviation_str);
		if (SOCKET_ERROR != Messenger::SendMessagetoServer(socket_client, sendbuf))
		{
			ret = 0;
		}
		Sleep(TIMEINTERVAL);
	}

	return ret;
}

char * Computer::GetMonitorHostname()
{
	return monitor_hostname;
}

char * Computer::GetMonitorIP()
{
	return monitor_IP;
}

int Computer::GetNum()
{
	return num;
}

int Computer::SetMonitorHostname(char monitorhostname[])
{
	strcpy(monitor_hostname, monitorhostname);
	return 0;
}

int Computer::SetMonitorIP(char monitorIP[])
{
	strcpy(monitor_IP, monitorIP);
	return 0;
}

char * Computer::GetHostname()
{
	return local_hostname;
}

char * Computer::GetIP()
{
	return local_IP;
}

bool Computer::AmIaMonitor()
{
	int res_cmp;
	res_cmp = strcmp(monitor_hostname, local_hostname);
	//cout << monitor_hostname << endl;
	//cout << local_hostname << endl;
	if (res_cmp == 0)
	{
		amiamonitor = true;
	}
	else
	{
		amiamonitor = false;
	}

	return amiamonitor;
}