/**H**************************************************************************
 File            : ResCommon.h
 Subsystem       : 
 Function Name(s): 
 Author          : YiQiu
 Date            : 2010-10-8  
 Time            : 16:21
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _RECCOMMON_H_2010_10
#define _RECCOMMON_H_2010_10





namespace ResFile
{

typedef unsigned int DWORD;
typedef unsigned int size_t;
typedef unsigned short WORD; 
typedef unsigned char BYTE;

#if defined(_MSC_VER) && (_MSC_VER <= 1200)	
	typedef unsigned __int64 QWORD;
#else
	typedef unsigned long long QWORD;
#endif

#define NULL 0

namespace Util{};

#define _MakeDWORD(a,b,c,d) ((a<<24)|(b<<16)|(c<<8)|(d))
#define _MakeFileVersion(year,month,day) ((year<<16)|(month<<8)|(day))

enum
{
    Res_File_Format_Flag = _MakeDWORD('R','e','c','P'),
    Patch_File_Format_Flag = _MakeDWORD('R','P','a','t'),
    File_Version_1_0 = _MakeFileVersion(2010,9,28),  // 手机原生支持的格式
    File_Version_1_1 = _MakeFileVersion(2010,11,1),  // 加入数据分卷方式，主要用于程序，
                                                     // 在安装到手机后，手机需要转成File_Version_1_0

    File_Version_1_2 = _MakeFileVersion(2011,6,11),  // 在1.0基础上更改
    File_Version_1_3 = _MakeFileVersion(2013,8,23),  // 在1.0基础上更改

    // example:
    // File_Version_2_0 = _MakeFileVersion(2011,9,28),

    Invaild_Pos = -1,
    Default_Encrypt_Len = 32,
};

enum ResCodeDef
{
    No_Error = 0,
    Input_Param_Error,
    File_Can_Not_Open,
    File_Read_Failed,
    File_Format_Error,


    Is_Version_1_0 = File_Version_1_0,
    Is_Version_1_1 = File_Version_1_1,
    Is_Unpacked_File = Is_Version_1_0,
    Is_Packed_File = Is_Version_1_1,
};

};







#endif  // _RECCOMMON_H_2010_


 // End of file



