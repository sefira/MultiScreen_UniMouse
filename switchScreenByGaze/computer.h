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

#ifndef SWITCHSCREENBYGAZE_Computer_H_
#define SWITCHSCREENBYGAZE_Computer_H_

#include <WinSock2.h>

class Computer
{
public:
	Computer();
	
	~Computer();

	int ToString();

	//a static method, to Query a remote host IP by a given name
	int static QueryHostIPbyName(
		char * hostname, Computer &m_comouter);

	//connect server, which is indicated by monitor_IP
	//then the connection will be saved in socket_client
	int ConncetWithServer();

	//send deviation broadcastly or to monitor, get the deviation firstly
	//by using QueryDeviation()
	int SendDeviation();

	//send local hostname
	int SendHostname();

	bool AmIaMonitor();

	//////////////gets sets///////////////////////////
	char * GetMonitorHostname();
	char * GetMonitorIP();
	int SetMonitorHostname(char monitorhostname[]);
	int SetMonitorIP(char monitorIp[]);
	int GetNum();
	char * GetHostname();
	char * GetIP();

	SOCKET socket_client;

protected:
	//the num which be allot by monitor
	int num;

	char monitor_hostname[128];
	char monitor_IP[128];

	//am i a monotor, be update after call AmIaMonitor()
	bool amiamonitor;
	char local_hostname[128];
	char local_IP[128];
	double evaluate_point;

private:
	//Query deviation from evaluatemedia
	double QueryDeviation();

	//Query local hostname, then put it into local_hostname
	int QueryLocalHostname();

};



#endif
