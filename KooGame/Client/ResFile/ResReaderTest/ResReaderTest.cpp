// ResReaderTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
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

#define PatchFile  "F:\\yiqiulib\\KooGame\\Client\\ResFile\\release\\Patch.pat"
#define OldFile  "F:\\yiqiulib\\KooGame\\Client\\ResFile\\release\\data1.pak"

int _tmain(int argc, _TCHAR* argv[])
{
    ifstream Reader;
    Reader.open("D:\\dev\\KYOL_1.0\\tools\\AnimationEditor\\pak\\list_main.txt");
    if ( !Reader.is_open() )
    {
        return false;
    }

    string strBasePath = "D:\\dev\\KYOL_1.0\\tools\\AnimationEditor\\pak\\";
    string strDstBasePath = "C:\\Documents and Settings\\orochi\\×ÀÃæ\\Test File\\";
    string strLine;  
    while( getline(Reader, strLine) )
    {    
        if ( strLine.size() == 0 ||
             strLine[0] == '#' )
        {
            continue;
        }
        string strSrc = strBasePath + strLine;
        string strDst = strDstBasePath + strLine;
        BOOL bRc = CopyFileA(strSrc.c_str(), strDst.c_str(), FALSE);
        if ( !bRc )
        {
            DWORD dwErr = GetLastError();
            cout << dwErr <<endl;
        }
    }

    ResFile::IResUpdater* pResUpdater = 
        ResFile::CreateResUpdater(PatchFile);
    pResUpdater->Update(OldFile);

    delete pResUpdater;

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

