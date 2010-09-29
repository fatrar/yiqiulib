// Pak.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <io.h>
#include <fstream>
using namespace std;

//#include <Windows.h>

#include <WinSock2.h>
#define _WINSOCK2API_
#include <Iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "Ws2_32.lib")

void IP()
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);


    PIP_ADAPTER_ADDRESSES pAddresses;
    pAddresses = (IP_ADAPTER_ADDRESSES*) malloc(sizeof(IP_ADAPTER_ADDRESSES));
    ULONG outBufLen = 0;
    DWORD dwRetVal = 0;

    // Make an initial call to GetAdaptersAddresses to get the 
    // size needed into the outBufLen variable
    if ( GetAdaptersAddresses(AF_INET, 
            0, 
            NULL, 
            pAddresses, 
            &outBufLen) == ERROR_BUFFER_OVERFLOW )
    {
            free(pAddresses);
            pAddresses = (IP_ADAPTER_ADDRESSES*) malloc(outBufLen);
    }

    // Make a second call to GetAdapters Addresses to get the
    // actual data we want
    if ( (dwRetVal = GetAdaptersAddresses(AF_INET, 
                        0, 
                        NULL, 
                        pAddresses, 
                        &outBufLen)) == NO_ERROR )
    {
        char szMacBuf[128] = {0};
        // If successful, output some information from the data we received
        while (pAddresses) {
            printf("\tFriendly name: %S\n", pAddresses->FriendlyName);
            printf("\tDescription: %S\n", pAddresses->Description);
            sprintf(szMacBuf,
                "%x-%x-%x-%x-%x-%x-%x-%x",
                pAddresses->PhysicalAddress[0],
                pAddresses->PhysicalAddress[1],
                pAddresses->PhysicalAddress[2],
                pAddresses->PhysicalAddress[3],
                pAddresses->PhysicalAddress[4],
                pAddresses->PhysicalAddress[5],
                pAddresses->PhysicalAddress[6],
                pAddresses->PhysicalAddress[7] );
            printf("\tMacAddress: %s\n", szMacBuf);
            pAddresses = pAddresses->Next;
        }
    }
    else
    { 
        printf("Call to GetAdaptersAddresses failed.\n");
        LPVOID lpMsgBuf;
        if (FormatMessage( 
            FORMAT_MESSAGE_ALLOCATE_BUFFER | 
            FORMAT_MESSAGE_FROM_SYSTEM | 
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dwRetVal,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
            (LPTSTR) &lpMsgBuf,
            0,
            NULL )) {
                printf("\tError: %s", lpMsgBuf);
        }
        LocalFree( lpMsgBuf );
    }

}
int _tmain(int argc, _TCHAR* argv[])
{
    IP();

    open("C:\\Info.txt", 0);
    ifstream Reader;
    Reader.open("C:\\Info.txt");
    char szBuf[1024];
    Reader.read(szBuf, 1024);
	return 0;
}

