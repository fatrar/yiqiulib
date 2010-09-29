// Pak.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <io.h>
#include <fstream>
using namespace std;

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

