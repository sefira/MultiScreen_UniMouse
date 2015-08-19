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

#include "evaluatemedia.h"
#include "computer.h"
#include "computermonitor.h"

#include <iostream>
#include <process.h>
using namespace std;

Computer myself_computer;

unsigned int __stdcall MonitorAsClientToo(void *)
{
	myself_computer.ConncetWithServer();
	myself_computer.SendHostname();
	myself_computer.BegintoWork();
	myself_computer.SendDeviation();
	
	return 0;
}

int main()
{
	cout << "please input monitor name:" << endl;

	char monitor_name[128];

	cin >> monitor_name;
	//cout << monitor_name << endl;
	ComputerMonitor monitor_computer;
	Computer::QueryHostIPbyName(monitor_name, monitor_computer);
	cout << "The monitor is		:" << monitor_computer.GetHostname() << endl;
	cout << "The monitor's IP is:" << monitor_computer.GetIP() << endl;

	char * myself_hostname = myself_computer.GetHostname();
	myself_computer.SetMonitorHostname(monitor_computer.GetHostname());
	myself_computer.SetMonitorIP(monitor_computer.GetIP());
	Computer::QueryHostIPbyName(myself_hostname, myself_computer);
	myself_computer.ToString();
	if (myself_computer.AmIaMonitor())
	{
		cout << "I am a monitor." << endl;
		int numof_connection = 0;
		cout << "How many Client do you want to connect:" << endl;
		cin >> numof_connection;
		ComputerMonitor myself_computermonitor(myself_computer);
		HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, MonitorAsClientToo, NULL, 0, NULL);
		myself_computermonitor.ConnectWithClient(numof_connection);
		myself_computermonitor.Configuration(numof_connection);
		myself_computermonitor.BegintoWork();
	}
	else
	{
		cout << "I am not a monitor." << endl;
		myself_computer.ConncetWithServer();
		myself_computer.SendHostname();
		myself_computer.BegintoWork();
		myself_computer.SendDeviation();
	}

	cout << "main thread finshed" << endl;
	return 0;
}