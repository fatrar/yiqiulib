/**CPP*************************************************************************
 File            : Main.cpp
 Subsystem       : 
 Function Name(s): 
 Author          : YiQiu
 Date            : 2010-12-8  
 Time            : 14:29
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
**************************************************************************cpp**/
#include "stdafx.h"
#include "Img2RGB.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    //while(1)
    //{
    //    __asm{int 3};
    //}
    if ( argc != 2 )
    {
        cout << "参数不对";
        return 0;
    }

    ifstream Reader;
    Reader.open(argv[1]);
    if ( !Reader.is_open() )
    {
        cout << "Can`t Open FileList File --> " << argv[1] << endl;
        return 0;
    }

    RGBFile::CImg2RGB Convert;
    string strLine;
    while( getline(Reader, strLine) )
    {    
        if ( strLine.size() == 0 ||
             strLine[0] == '#' )
        {
            continue;
        }
        
        Convert.Convert(strLine.c_str());
    }

    system("pause");
    return 0;
}


// End of file