// BaseTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#define USEBASEENINE_NAMESPACE
#include "..\Base\Include\singleton.h"
using namespace OCI;
#include <iostream>
#include <Windows.h>
#include <map>
#include <deque>
#include <string>
#include <list>
#include <memory>
using namespace std;

struct aa 
{
    int a;
};

static const aa g_a;

//#include "F:\tvt\src\DVR Server\DEV_HC_DM642i\Include\WPG_EventOccurrence.h"
void Test(const aa& a = g_a)
{
    cout << a.a << endl;
}

#define GetChannelName(i, szBuf) \
    char szBuf[32] = {0}; \
    sprintf(szBuf, "%s%d", "aa", i);

int _tmain(int argc, _TCHAR* argv[])
{
    {
        auto_ptr<char> pBuf;
        char* p2 = pBuf.get() = new char[10];
        strcpy(pBuf, "aaawsd");
    }
    

    GetChannelName(0, szBuf2);

    cout << szBuf2 << endl; 
    const int b = 0;
    const int& aa = b;
    list<int> lk;
  
	return 0;
}

