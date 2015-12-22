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

#ifndef MASTERCONTROLCAPTUREDATA_MESSENGER_H_
#define MASTERCONTROLCAPTUREDATA_MEDDENGER_H_

#include <WinSock2.h>
#include <vector>

#include "computercomponent.h"

class Messenger
{
public:
	Messenger();

	~Messenger();

	//for Computer, connect to ComputerMonitor
	static int ClientConnectServer(char * serverIP, SOCKET & socket_client);

	//for ComputerMonitor, accept Computer connection and creat thread for it
	//save Computer information in vector include the socket of accept
	static int ServerAcceptClient(std::vector<ComputerInfo> &computers_vector, int numof_connection);

	//for Computer
	//send a given message to server which is indicated by socket_server
	static int SendMessagetoServer(SOCKET socket_server, char * sendbuf);

	/////////////////////////////////////////////////////////////////////////
	/////////////nonsense code just for mastercotrol/////////////////////////
	/////////////////////////////////////////////////////////////////////////
	static int SendMessagetoClient(char * sendbuf);
	static int ReceiveMessageFromServer(SOCKET socket_server, char * recvbuf);

};

#endif