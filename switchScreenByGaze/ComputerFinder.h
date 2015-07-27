#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <lm.h>
#include <vector>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "netapi32.lib")

using namespace std;

struct NameIPPair
{
	char ComputerName[128];
	char ComputerIP[128];
};

class ComputerFinder
{
private:
	ComputerFinder();

	int find_ALL_Computer_Name();
	
	int get_IP_by_Compter_Name(char * Computer_Name);
	
	int build_Name_IP_Pair();

	int get_self_Name();

	vector<NameIPPair> Name_IP[128];

public:
	int get_NameIP_Pair(); 
};