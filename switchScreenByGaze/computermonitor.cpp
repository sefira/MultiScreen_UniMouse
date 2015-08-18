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

#include <stdlib.h>
#include <time.h>  

#include <process.h>
#include <iostream>

#include "messenger.h"
#include "EvaluateMedia.h"

using namespace std;

vector<ComputerInfo> ComputerMonitor::computers_vector = vector<ComputerInfo>();

ComputerMonitor::ComputerMonitor()
{

}

ComputerMonitor::ComputerMonitor(Computer m_computer)
{
	num = m_computer.GetNum();

	amiamonitor = true;

	strcpy(monitor_hostname, m_computer.GetMonitorHostname());
	strcpy(monitor_IP, m_computer.GetMonitorIP());

	strcpy(local_hostname, m_computer.GetMonitorHostname());
	strcpy(local_IP, m_computer.GetMonitorIP());
}

ComputerMonitor::~ComputerMonitor()
{

}

int ComputerMonitor::ToString()
{
	cout << "for the vector of computers:" << endl;
	for (ComputerInfo m_computerinfo : computers_vector)
	{
		cout << "num:" << m_computerinfo.num << endl;
		cout << "hostname:" << m_computerinfo.local_hostname << endl;
		cout << "IP:" << m_computerinfo.local_IP << endl;
		cout << "evaulate:" << m_computerinfo.evaluate_point << endl;
		cout << endl;
	}
	return 0;
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
	char deviation_str[128] = { 0 };
	strcpy(deviation_str, &recvbuf[1]);
	cout << deviation_str << endl;
	double deviation = atof(deviation_str);
	cout << deviation << endl;

	return 0;
}

ComputerInfo &ComputerMonitor::FindNumComputerinVecotr(int num)
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

//unsigned int __stdcall ComputerMonitor::InterfacetoEvaluateMedia(void *)
//{
//	EvaluateMedia m_evaluatemedia = EvaluateMedia(false);
//	//m_evaluatemedia.TrackingFace();
//	//cout << m_evaluatemedia.GetDeviation() << endl;
//
//	//test
//	srand((unsigned)time(NULL));
//	m_evaluatemedia.SetDeviation(rand());
//	FindNumComputerinVecotr(0).evaluate_point =
//		EvaluateMedia::GetDeviation();
//	Sleep(TIMEINTERVAL);
//
//	return 0;
//}
//
int ComputerMonitor::BegintoWork()
{
	DetermineActivated();
	return 0;
}

int ComputerMonitor::DetermineActivated()
{
	int minmum_dev;
	int activated_num = 0;
	while (1)
	{
		if (computers_vector.empty())
		{
			return 1;
		}
		minmum_dev = 1000;
		for (int i = 0; i < computers_vector.size(); i++)
		{
			int temp_dev = computers_vector.at(i).evaluate_point;
			if (temp_dev < minmum_dev)
			{
				minmum_dev = temp_dev;
				activated_num = computers_vector.at(i).num;
			}
		}
		cout << "num " << activated_num << " is activated" << endl;
		m_keyboard_simulater.SwitchScreentoFX(activated_num);
		Sleep(TIMEINTERVAL);
	}
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


