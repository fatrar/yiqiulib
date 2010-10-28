// ResReaderTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <Windows.h>

#include <iostream>
using namespace std;

class A
{
public:
    int a;
public:
    A()
    {
        a = 1;
    }
    ~A()
    {
        cout << a << endl;
        //if (a) {delete a; a = NULL;}
    }
};

int _tmain(int argc, _TCHAR* argv[])
{
    int i = 1<< 2;
    A* a = new A[3];
    delete a;


/*
    ResFile::IResReader* pResReader = 
        ResFile::CreateResFileReader("F:\\yiqiulib\\KooGame\\Client\\ResFile\\ResPacker\\ResPacker.pak");
    
    //pResReader->Parse();

    ResFile::CUnPackDataInfo DataInfo;
    pResReader->GetData("1.jpg", DataInfo);

    std::ofstream Writer("1.jpg");
    Writer.write(DataInfo, DataInfo.Length());
    Writer.close();
    */
	return 0;
}

