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

#include "computermonitor.h"

#include <iostream>

#include "messenger.h"

using namespace std;

ComputerMonitor::ComputerMonitor()
{

}

ComputerMonitor::ComputerMonitor(Computer m_computer)
{
	num = m_computer.GetNum();

	amiamonitor = true;

	strcpy(monitor_hostname, m_computer.GetMonitorHostname());
	strcpy(monitor_IP, m_computer.GetMonitorIP());

	strcpy(local_hostname, m_computer.GetHostname());
	strcpy(local_IP, m_computer.GetIP());
}

ComputerMonitor::~ComputerMonitor()
{

}

int ComputerMonitor::ConnectWithClient()
{
	return Messenger::ServerAcceptClient(computers_vector);
}

int ComputerMonitor::ReceiveHostname(char *recvbuf, ComputerInfo * m_computerinfo)
{
	char remote_hostname[128] = { 0 };
	strcpy(remote_hostname, &recvbuf[1]);
	cout << remote_hostname << endl;
	strcpy(m_computerinfo->local_hostname,remote_hostname);
	
	return 0;
}

int ComputerMonitor::ReceiveDeviation(char *recvbuf, ComputerInfo * m_computerinfo)
{
	//TODO socket receive, a really receive action
	char deviation_str[128] = { 0 };
	strcpy(deviation_str, &recvbuf[1]);
	cout << deviation_str << endl;
	double deviation = atof(deviation_str);
	cout << deviation << endl;

	return 0;
}

bool ComputerMonitor::isaNewHost(char * remote_hostname)
{
	bool isanewhost = true;

	for (ComputerInfo m_computer : computers_vector)
	{
		if (!strcmp(m_computer.local_hostname, remote_hostname))
		{
			isanewhost = false;
			break;
		}
	}
	return isanewhost;
}


