// ResReaderTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


// #define PatchFile  "F:\\yiqiulib\\KooGame\\Client\\ResFile\\release\\Patch.pat"
// #define OldFile  "F:\\yiqiulib\\KooGame\\Client\\ResFile\\release\\data1.pak"

template<typename T, typename T2, T2 T::*P>
struct Operator :
    public T
{
    inline bool operator ==(const T& a) const 
    {
        return /*__super::*/this->*P == a.*P;
    }
   /* inline bool operator > (const T& a) const
    {return qwValue>a.qwValue;}
    inline bool operator < (const T& a) const
    {return qwValue<a.qwValue;}*/
};

struct CTest {
    int i;
};
struct CTestCom
{
    bool operator()(const CTest& _Left, const CTest& _Right) const
    {	// apply operator< to operands
        return (_Left.i < _Right.i);
    }
};

#define NewFile "F:\\yiqiulib\\KooGame\\Client\\ResFile\\release\\Data_New_Version_1_0.pak"
#define OldFile "F:\\yiqiulib\\KooGame\\Client\\ResFile\\release\\Data_Old_Version_1_0.pak"
                                                                  
#include "ResFileUtil.h"
#include "FileSystem.h"

int _tmain(int argc, _TCHAR* argv[])
{
    FileSystem::CFile Old, New;
    Old.OpenByRead(OldFile);
    New.OpenByRead(NewFile);

    ResFile::FileHead0* pOldHead = 
        ResFile::Util::GetFileHead<ResFile::File_Version_1_0>(Old);
    ResFile::FileHead0* pNewHead = 
        ResFile::Util::GetFileHead<ResFile::File_Version_1_0>(New);

   // ResFile::UnpackFile("D:\\PackTest\\anim.pak");

    /*
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
    */

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

