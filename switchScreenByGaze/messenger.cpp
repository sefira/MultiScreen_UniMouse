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

#include "messenger.h"

#include <iostream>
#include <process.h>

#include "computermonitor.h"

using namespace std;

Messenger::Messenger()
{

}

Messenger::~Messenger()
{

}

int Messenger::ClientConnectServer(char * serverIP, SOCKET & socket_client)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		cout << "WSAStartup failed with error: " << err << endl;
		return 1;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		cout << "Could not find a usable version of Winsock.dll\n" << endl;
		WSACleanup();
		return 1;
	}
	else
	{
		cout << "The Winsock 2.2 dll was found okay" << endl;
	}

	socket_client = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_client == -1)
	{
		cout << "socket create failed��" << endl;
		return 1;
	}

	SOCKADDR_IN addrClient;
	addrClient.sin_addr.S_un.S_addr = inet_addr(serverIP);
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(SWITCHGAZE_PORT);

	while (1)
	{
		int ret = connect(socket_client, (sockaddr*)&addrClient, sizeof(SOCKADDR));
		if (ret)
		{
			cout << "connect " << serverIP << " failed" << endl;
			return 1;
		}
		else
		{
			cout << "connect " << serverIP << " succeed" << endl;
			break;
		}
	}

	return 0;
}

unsigned int __stdcall SocketServerThread(void *pPM);
//a semaphore to sync the thread
HANDLE m_semaphore = CreateSemaphore(NULL, 1, 1, NULL);

ComputerInfo &FindNumComputerinVecotr(vector<ComputerInfo> &computers_vector, int num)
{
	for (int i = 0; i < computers_vector.size(); i++)
	{
		ComputerInfo &m_computerinfo = computers_vector.at(i);
		if (m_computerinfo.num == num)
		{
			return m_computerinfo;
		}
	}
}
int Messenger::ServerAcceptClient(vector<ComputerInfo> & computers_vector)
{

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		cout << "WSAStartup failed with error: " << err << endl;
		return 1;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		cout << "Could not find a usable version of Winsock.dll" << endl;
		WSACleanup();
		return 1;
	}
	else
	{
		cout << "The Winsock 2.2 dll was found okay\n" << endl;
	}

	SOCKET servSock = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(6000);
	addrServ.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	bind(servSock, (SOCKADDR*)&addrServ, sizeof(SOCKADDR));

	if (listen(servSock, 20) == SOCKET_ERROR)
	{
		cout << "listen failed with error: " << WSAGetLastError() << endl;
		closesocket(servSock);
		WSACleanup();
		return 1;
	}

	int m_time_count = GetTickCount();
	ComputerInfo m_computerinfo;
	int m_count = 1;
	while (true)
	{
		//server accept for 30s
		if (GetTickCount() - m_time_count >= 30000)
		{
			break;
		}
		//wait for client connect, and create thread for it
		SOCKADDR_IN cliAddr;
		int length = sizeof(SOCKADDR);

		SOCKET cliSock = accept(servSock, (SOCKADDR*)&cliAddr, &length);

		if (INVALID_SOCKET == cliSock)
		{
			cout << "listen failed with error: " << WSAGetLastError() << endl;
			closesocket(cliSock);
			WSACleanup();
			return 1;
		}
		else
		{
			WaitForSingleObject(m_semaphore, INFINITE);
			m_computerinfo.num = m_count;
			m_computerinfo.socket_server = cliSock;
			computers_vector.push_back(m_computerinfo);
			m_count++;
			HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, SocketServerThread,
				&FindNumComputerinVecotr(computers_vector, m_count - 1), 0, NULL);
		}
	}
	return 0;
}

unsigned int __stdcall SocketServerThread(void *m_computerinfo_v)
{
	ComputerInfo *m_computerinfo = (ComputerInfo *)m_computerinfo_v;
	ReleaseSemaphore(m_semaphore, 1, NULL);

	char recvbuf[128];
	double m_deviation;
	char m_IP[128];
	while (1)
	{
		memset(recvbuf, 0, sizeof(recvbuf));
		m_deviation = 1000;
		memset(m_IP, 0, sizeof(m_IP));
		recv(m_computerinfo->socket_server, recvbuf, sizeof(recvbuf), 0);

		int flag = recvbuf[0];
		switch (flag)
		{
		case 1:
			ComputerMonitor::ReceiveHostname(recvbuf, m_computerinfo);

		case 2:
			ComputerMonitor::ReceiveDeviation(recvbuf, m_computerinfo);

		}
	}
	

	return 0;
}