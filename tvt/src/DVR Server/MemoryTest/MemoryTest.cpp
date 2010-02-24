// MemoryTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#define new[] new(_NORMAL_BLOCK, __FILE__,__LINE__)

int _tmain(int argc, _TCHAR* argv[])
{
    memory_check.Init();
    //DebugOut("aaa");
    char* p = new char;
    char* ptest = new char[1000];
	return 0;
}

