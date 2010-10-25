// Pak.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <io.h>
#include <fstream>
using namespace std;

template<int i>
int xxx(int a)
{
    char sz[i] = {0};
    a = i;
    return a;
}

#include <Windows.h>

#define TestLen 20
int szBuf[TestLen];

size_t Test2(int FindKey)
{
    size_t nLow = 0, nHigh = TestLen-1;
    size_t nMid;
    while (nLow <= nHigh)
    {
        nMid = (nLow + nHigh)>>1;
        int& TempKey = szBuf[nMid];
        if ( TempKey == FindKey )
        {
            return nMid;
        }
        else if ( TempKey > FindKey )
        {
            nHigh = nMid - 1;
        }
        else
        {
            nLow = nMid + 1;
        }
    }
    return -1;
}
#include <hash_map>
#include <string>
using namespace std;
using namespace stdext;
int _tmain(int argc, _TCHAR* argv[])
{
    stdext::hash_map<string, int> Map;
    Map["aaa"] = 1;
    Map["aaa"] = 1;
    for ( int i =0; i<TestLen; ++i )
    {
        szBuf[i] = i;
    }

    for ( int i =0; i<TestLen; ++i )
    {
        printf("%d\n", Test2(i));
    }


    open("C:\\Info.txt", 0);
    ifstream Reader;
    Reader.open("C:\\Info.txt");
    char szBuf[1024];
    Reader.read(szBuf, 1024);
	return 0;
}

