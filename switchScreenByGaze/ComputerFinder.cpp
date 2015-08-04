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

#include "computerfinder.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <lm.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "netapi32.lib")

using namespace std;

ComputerFinder::ComputerFinder()
{

}

ComputerFinder::~ComputerFinder()
{

}

int ComputerFinder::FindALLComputersName()
{
	LPSERVER_INFO_101 pBuf = NULL;
	LPSERVER_INFO_101 pTmpBuf;
	DWORD dwLevel = 101;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwTotalCount = 0;
	DWORD dwServerType = SV_TYPE_SERVER;        // all servers
	DWORD dwResumeHandle = 0;
	NET_API_STATUS nStatus;
	LPWSTR pszServerName = NULL;
	LPWSTR pszDomainName = NULL;
	DWORD i;

	// Call the NetServerEnum function to retrieve information
	// for all servers, specifying information level 101.
	//
	nStatus = NetServerEnum(pszServerName,
		dwLevel,
		(LPBYTE *)& pBuf,
		dwPrefMaxLen,
		&dwEntriesRead,
		&dwTotalEntries,
		dwServerType,
		pszDomainName,
		&dwResumeHandle);
	//
	// If the call succeeds,
	//
	if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)) 
	{
		if ((pTmpBuf = pBuf) != NULL)
		{
			//
			// Loop through the entries and 
			// print the data for all server types.
			//
			for (i = 0; i < dwEntriesRead; i++) 
			{
				assert(pTmpBuf != NULL);

				if (pTmpBuf == NULL) 
				{
					fprintf(stderr, "An access violation has occurred\n");
					break;
				}

				printf("\tPlatform: %d\n", pTmpBuf->sv101_platform_id);
				wprintf(L"\tName:     %s\n", pTmpBuf->sv101_name);
				printf("\tVersion:  %d.%d\n",
					pTmpBuf->sv101_version_major,
					pTmpBuf->sv101_version_minor);
				printf("\tType:     %d", pTmpBuf->sv101_type);
				//
				// Check to see if the server is a domain controller;
				// if so, identify it as a PDC or a BDC.
				//
				if (pTmpBuf->sv101_type & SV_TYPE_DOMAIN_CTRL)
				{
					wprintf(L" (PDC)");
				}
				else{
					if (pTmpBuf->sv101_type & SV_TYPE_DOMAIN_BAKCTRL)
					{
						wprintf(L" (BDC)");
					}
				}
				printf("\n");
				//
				// Also print the comment associated with the server.
				//
				wprintf(L"\tComment:  %s\n\n", pTmpBuf->sv101_comment);

				pTmpBuf++;
				dwTotalCount++;
			}
			// Display a warning if all available entries were
			// not enumerated, print the number actually 
			// enumerated, and the total number available.

			if (nStatus == ERROR_MORE_DATA) 
			{
				fprintf(stderr, "\nMore entries available!!!\n");
				fprintf(stderr, "Total entries: %d", dwTotalEntries);
			}

			printf("\nEntries enumerated: %d\n", dwTotalCount);
		}
		else 
		{
			printf("No servers were found\n");
			printf("The buffer (bufptr) returned was NULL\n");
			printf("  entriesread: %d\n", dwEntriesRead);
			printf("  totalentries: %d\n", dwEntriesRead);
		}
	}
	else
	{
		fprintf(stderr, "NetServerEnum failed with error: %d\n", nStatus);
	}
	//
	// Free the allocated buffer.
	//
	if (pBuf != NULL)
	{
		NetApiBufferFree(pBuf);
	}
	return 0;
}

int ComputerFinder::GetIPbyCompterName(char * computer_name)
{
	//-----------------------------------------
	// Declare and initialize variables
	WSADATA wsaData;
	int iResult;

	DWORD dwError;
	int i = 0;

	struct hostent *remoteHost;
	char *host_name;
	struct in_addr addr;

	char **pAlias;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) 
	{
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	host_name = computer_name;

	printf("Calling gethostbyname with %s\n", host_name);
	remoteHost = gethostbyname(host_name);

	if (remoteHost == NULL) 
	{
		dwError = WSAGetLastError();
		if (dwError != 0) 
		{
			if (dwError == WSAHOST_NOT_FOUND) 
			{
				printf("Host not found\n");
				return 1;
			}
			else if (dwError == WSANO_DATA) 
			{
				printf("No data record found\n");
				return 1;
			}
			else 
			{
				printf("Function failed with error: %ld\n", dwError);
				return 1;
			}
		}
	}
	else 
	{
		printf("Function returned:\n");
		printf("\tOfficial name: %s\n", remoteHost->h_name);
		for (pAlias = remoteHost->h_aliases; *pAlias != 0; pAlias++) 
		{
			printf("\tAlternate name #%d: %s\n", ++i, *pAlias);
		}
		printf("\tAddress type: ");
		switch (remoteHost->h_addrtype) 
		{
		case AF_INET:
			printf("AF_INET\n");
			break;
		case AF_NETBIOS:
			printf("AF_NETBIOS\n");
			break;
		default:
			printf(" %d\n", remoteHost->h_addrtype);
			break;
		}
		printf("\tAddress length: %d\n", remoteHost->h_length);

		i = 0;
		if (remoteHost->h_addrtype == AF_INET)
		{
			while (remoteHost->h_addr_list[i] != 0) 
			{
				addr.s_addr = *(u_long *)remoteHost->h_addr_list[i++];
				printf("\tIP Address #%d: %s\n", i, inet_ntoa(addr));
			}
		}
		else if (remoteHost->h_addrtype == AF_NETBIOS)
		{
			printf("NETBIOS address was returned\n");
		}
	}
	return 0;
}

int ComputerFinder::BuildNameIPPair()
{
	return 0;
}

int ComputerFinder::GetNameIPPair()
{
	return 0;
}

int ComputerFinder::GetSelfName()
{
	char buffer[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD len = MAX_COMPUTERNAME_LENGTH + 1;
	if (GetComputerName(buffer, &len))
	{
		cout << buffer << endl;
	}
	return 0;
}

