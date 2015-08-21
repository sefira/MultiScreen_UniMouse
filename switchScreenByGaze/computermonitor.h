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

#include <vector>

#include "keyboardsimulater.h"
#include "computercomponent.h"


class ComputerMonitor:public Computer
{
public:
	ComputerMonitor();

	ComputerMonitor(Computer m_computer);

	~ComputerMonitor();

	int ToString();

	//accept client connect for 30s
	//and save the connection in vector, socket_server
	int ConnectWithClient(int numof_connection);

	//receive remote computers' message about themselves hostname
	//use isaNewHost() to judge weather it has been saved into computers_vector
	static int ReceiveHostname(char *recvbuf, ComputerInfo *m_computerinfo);

	//receive remote computers' message about themselves deviation
	static int ReceiveDeviation(char *recvbuf, ComputerInfo *m_computerinfo);

	//configuration the vector and file according to the hostname
	int Configuration(int numof_connection);
	
	//begin to work, start face tracking in a thread
	//add self_monitor into computers_vector
	//Lock the screen by m_keyboard_simulater
	//static unsigned int __stdcall InterfacetoEvaluateMedia(void *);
	int BegintoWork();

	//determine who is activated
	int DetermineActivated();

	//simulater keyboard press and release
	KeyBoardSimulater m_keyboard_simulater;

	//Find computer according given num
	static ComputerInfo &FindNumComputerinVecotr(int num);

private:
	//generate configruation file for synergy
	int WriteFile(std::vector<ComputerInfo> computers_vector);

	bool isaNewHost(char * remote_hostname);

	//all receives will update this vector
	static std::vector<ComputerInfo> computers_vector;
};