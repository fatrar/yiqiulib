// ResReaderTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>

int _tmain(int argc, _TCHAR* argv[])
{
    __int64 i = 0x5546;
    __int64 j = 5264;

    j ^= i;
    j ^= i;


    ResFile::IResReader* pResReader = 
        ResFile::CreateResFileReader("F:\\yiqiulib\\KooGame\\Client\\ResFile\\ResPacker\\ResPacker.pak");
    
    //pResReader->Parse();

    ResFile::CUnPackDataInfo DataInfo;
    pResReader->GetData("1.jpg", DataInfo);

    std::ofstream Writer("1.jpg");
    Writer.write(DataInfo, DataInfo.Length());
    Writer.close();
	return 0;
}
