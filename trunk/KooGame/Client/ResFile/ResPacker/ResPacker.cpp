// ResPacker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

union CA
{
    int a;
    char b;
};

class CB :
    public CA
{

};

int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

