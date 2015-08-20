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
#include <fstream>

#include "messenger.h"
#include "evaluatemedia.h"

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

HANDLE hostname_semaphore;

int ComputerMonitor::ConnectWithClient(int numof_connection)
{
	//waitting for all hostnames are received
	hostname_semaphore = CreateSemaphore(NULL, 0, numof_connection, NULL);
	return Messenger::ServerAcceptClient(computers_vector, numof_connection);
}

int ComputerMonitor::ReceiveHostname(char *recvbuf, ComputerInfo * m_computerinfo)
{
	char remote_hostname[128] = { 0 };
	strcpy(remote_hostname, &recvbuf[1]);
	//cout << remote_hostname << endl;
	strcpy(m_computerinfo->local_hostname,remote_hostname);
	ReleaseSemaphore(hostname_semaphore, 1, NULL);
	
	return 0;
}

int ComputerMonitor::ReceiveDeviation(char *recvbuf, ComputerInfo * m_computerinfo)
{
	char deviation_str[128] = { 0 };
	strcpy(deviation_str, &recvbuf[1]);
	//cout << deviation_str << endl;
	double deviation = atof(deviation_str);
	m_computerinfo->evaluate_point = deviation + sqrt(m_computerinfo->evaluate_point);
	//cout << deviation << endl;

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
std::string sectionscreens = "\t\thalfDuplexCapsLock = false\n" \
"\t\thalfDuplexNumLock = false\n" \
"\t\thalfDuplexScrollLock = false\n" \
"\t\txtestIsXineramaUnaware = false\n" \
"\t\tswitchCorners = none\n" \
"\t\tswitchCornerSize = 0\n";
std::string sectionoptions = "	relativeMouseMoves = false\n" \
"\tscreenSaverSync = true\n" \
"\twin32KeepForeground = false\n" \
"\tswitchCorners = none\n" \
"\tswitchCornerSize = 0\n";

int WriteFile(vector<ComputerInfo> computers_vector)
{
	string filename = "C:\\Users\\";

	char  infoBuf[128];
	DWORD  bufCharCount = 128;

	if (GetUserName(infoBuf, &bufCharCount))
	{
		filename.append(infoBuf);
		filename.append("\\synergy.sgc");
		//cout << filename.c_str() << endl;
	}
	else
	{
		//cout << filename.c_str() << endl;
		return 1;
	}
	FILE * pfile;
	fopen_s(&pfile, filename.c_str(), "wt+");
	if (pfile == NULL)
	{
		return 1;
	}
	fprintf(pfile, "%s\n", "section: screens");
	for (int i = 0; i < computers_vector.size(); i++)
	{
		fprintf(pfile, "\t%s:\n", computers_vector.at(i).local_hostname);
		fprintf(pfile, "%s", sectionscreens.c_str());
	}
	fprintf(pfile, "%s\n\n", "end");

	fprintf(pfile, "%s\n", "section: aliases");
	fprintf(pfile, "%s\n\n", "end");

	fprintf(pfile, "%s\n", "section: links");
	fprintf(pfile, "%s\n\n", "end");

	fprintf(pfile, "%s\n", "section: options");
	fprintf(pfile, "%s", sectionoptions.c_str());
	for (int i = 0; i < computers_vector.size(); i++)
	{
		fprintf(pfile,
			"\tkeystroke(F%d) = switchToScreen(%s)\n",
			13 + computers_vector.at(i).num,
			computers_vector.at(i).local_hostname);
		fprintf(pfile,
			"\tkeystroke(Control+Alt+Shift+F%d) = switchToScreen(%s)\n",
			1 + computers_vector.at(i).num,
			computers_vector.at(i).local_hostname);
	}
	fprintf(pfile, "%s\n\n", "end");
	fclose(pfile);
	return 0;
}

int ComputerMonitor::Configuration(int numof_connection)
{
	cout << "begin to configuration" << endl;
	//waitting for all hostnames are received
	for (int i = 0; i < numof_connection; i++)
	{
		WaitForSingleObject(hostname_semaphore,INFINITE);
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
		ComputerInfo &m_computerinfo = computers_vector.at(i);
		Computer tempcomputer;
		Computer::QueryHostIPbyName(m_computerinfo.local_hostname, tempcomputer);
		strcpy(m_computerinfo.local_IP, tempcomputer.GetIP());
	}
	//ToString();

	if (1 == WriteFile(computers_vector))
	{
		return 1;
	}
	
	return 0;
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
	int original_activated_num = 0;
	while (1)
	{
		if (computers_vector.empty())
		{
			return 1;
		}
		minmum_dev = 1000;
		original_activated_num = activated_num;
		for (int i = 0; i < computers_vector.size(); i++)
		{
			double temp_dev = computers_vector.at(i).evaluate_point;
			cout << "num: " << computers_vector.at(i).num <<
				"hostname: " << computers_vector.at(i).local_hostname <<
				" devitaion: " << temp_dev << endl;
			if (temp_dev < minmum_dev)
			{
				minmum_dev = temp_dev;
				activated_num = computers_vector.at(i).num;
			}
		}
		if (original_activated_num != activated_num)
		{
			original_activated_num = activated_num;
			cout << "num " << activated_num << " is activated" <<
				"its name is: " <<
				computers_vector.at(activated_num).local_hostname <<
				" win by evaluate: " << 
				computers_vector.at(activated_num).evaluate_point << endl;
			m_keyboard_simulater.SwitchScreentoFX(activated_num);
		}
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


