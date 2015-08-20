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
		cout << "socket create failed£¡" << endl;
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
			cout << "reconnecting" << endl;
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
HANDLE num_semaphore = CreateSemaphore(NULL, 1, 1, NULL);

int Messenger::ServerAcceptClient(vector<ComputerInfo> & computers_vector, int numof_connection)
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
	addrServ.sin_port = htons(SWITCHGAZE_PORT);
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
	int m_count = 0;
	for (int i = 0; i < numof_connection; i++)
	{
		//accept will blocked until a connection is present
		//so a timer count is useless to stop it 

		//cout << GetTickCount() - m_time_count << endl;
		//server accept for 30s
		//if (GetTickCount() - m_time_count >= 30000)
		//{
		//	break;
		//}
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
			m_computerinfo.num = m_count;
			m_computerinfo.socket_server = cliSock;
			m_computerinfo.evaluate_point = 1000;
			computers_vector.push_back(m_computerinfo);
			m_count++;
		}
	}

	for (int i = 1; i < numof_connection; i++)
	{
		for (int j = 0; j < numof_connection - i; j++)
		{
			if (strcmp(computers_vector.at(j).local_hostname, computers_vector.at(j + 1).local_hostname) > 0)
			{
				ComputerInfo tempcom = computers_vector.at(j);
				computers_vector.at(j) = computers_vector.at(j + 1);
				computers_vector.at(j + 1) = tempcom;
			}
		}
	}

	for (int i = 0; i < numof_connection; i++)
	{
		computers_vector.at(i).num = i;
	}


	for (int i = 0; i < numof_connection; i++)
	{
		WaitForSingleObject(num_semaphore, INFINITE);
		HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, SocketServerThread,
			&computers_vector.at(i), 0, NULL);
	}
	return 0;
}

unsigned int __stdcall SocketServerThread(void *m_computerinfo_v)
{
	ComputerInfo *m_computerinfo = (ComputerInfo *)m_computerinfo_v;
	ReleaseSemaphore(num_semaphore, 1, NULL);

	cout << "this is num " << m_computerinfo->num << endl;

	char recvbuf[128];
	double m_deviation;
	char m_IP[128];
	while (1)
	{
		memset(recvbuf, 0, sizeof(recvbuf));
		m_deviation = 1000;
		memset(m_IP, 0, sizeof(m_IP));
		recv(m_computerinfo->socket_server, recvbuf, 128, 0);
		//cout << "this is num " << m_computerinfo->num <<
		//	" recv: " << recvbuf << 
		//	" from: " << m_computerinfo->local_hostname << endl;
		if (strlen(m_computerinfo->local_hostname) < 3)
		{
			break;
		}
		int flag = recvbuf[0];
		switch (flag)
		{
		case 'H':
			ComputerMonitor::ReceiveHostname(recvbuf, m_computerinfo);

		case 'D':
			//cout << "recv: " << recvbuf << "   from: " << m_computerinfo->local_hostname << endl;
			ComputerMonitor::ReceiveDeviation(recvbuf, m_computerinfo);
		}
	}

	return 0;
}

int Messenger::SendMessagetoServer(SOCKET socket_server, char * sendbuf)
{
	int ret = 0;
	//cout << "send: " << sendbuf << endl;
	ret = send(socket_server, sendbuf ,128, 0);
	if (ret == SOCKET_ERROR)
	{
		//cout << "send message failed"<<endl;
		return 1;
	}
	else
	{
		//cout << "send message succeed" << endl;
		return 0;
	}
}
