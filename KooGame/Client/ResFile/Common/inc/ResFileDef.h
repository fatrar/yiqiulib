/**H**************************************************************************
 File            : ResFileDef.h
 Subsystem       : 
 Function Name(s): CResFileDef
 Author          : YiQiu
 Date            : 2010-9-28  
 Time            : 15:33
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _RESFILEDEF_H_2010_9
#define _RESFILEDEF_H_2010_9
#include "ResCommon.h"

namespace ResFile
{

#define _MakeDWORD(a,b,c,d) ((a<<24)|(b<<16)|(c<<8)|(d))
#define _MakeFileVersion(year,month,day) ((year<<16)|(month<<8)|(day))

enum
{
	Res_File_Format_Flag = _MakeDWORD('R','e','c','P'),
    Patch_File_Format_Flag = _MakeDWORD('R','P','a','t'),
	File_Version_1_0 = _MakeFileVersion(2010,9,28),  // 手机原生支持的格式
    File_Version_1_1 = _MakeFileVersion(2010,11,1),  // 加入数据分卷方式，主要用于程序，
                                                     // 在安装到手机后，手机需要转成File_Version_1_0
   
    // example:
    // File_Version_2_0 = _MakeFileVersion(2011,9,28),

    Invaild_Pos = -1,
    Default_Encrypt_Len = 32,
};

template<DWORD Version> struct TFileHead;
template<DWORD Version> struct TDataHead;


struct TDataMemInfo
{
    enum {Max_Num = 12};
    static const BYTE s_RawPercent[Max_Num];      // x%的原始数据是内存xxByte以下，
                                                  // 100为最大的数据文件大小，0则最小
    static const BYTE s_CompressPercent[Max_Num]; // x%的压缩数据是内存xxByte以下
};
__declspec(selectany) const BYTE TDataMemInfo::s_RawPercent[] = 
{
    100,95,90,85,80,75,
     70,65,60,55,50,0
};
__declspec(selectany) const BYTE TDataMemInfo::s_CompressPercent[] = 
{
    100,95,90,85,80,75,
     70,65,60,55,50,0
};

struct TFileHeadBase
{
    DWORD dwSize;
    DWORD FormatFlag;
    DWORD Version;
    DWORD dwFileCount:28;
    DWORD bIsExistFileName:1;
    DWORD eAlgo:3;

    union
    {
        BYTE szKey[8];  // 最大密钥8个字节
        QWORD dwKey;    // 如果是XOR加密用这个
    };
};

struct TPatchFileHeadBase
{
    DWORD FormatFlag;
    DWORD Version;
    DWORD dwAddFileCount:16;
    DWORD dwRemoveFileCount:16;
};

union UHashValue
{
    DWORD dwValue[2];
    QWORD qwValue;

    inline bool operator ==(const UHashValue& a) const {return qwValue==a.qwValue;}
    inline bool operator > (const UHashValue& a) const {return qwValue>a.qwValue;}
    inline bool operator < (const UHashValue& a) const {return qwValue<a.qwValue;}
    inline void operator = (const QWORD& V){qwValue = V;}
    UHashValue(QWORD V = 0){qwValue = V;}
};

// Encrypt Algorithm 
enum eEncryptAlgo
{
    Raw_E_Algo,
    Xor_E_Algo,
    BlowFish_E_Algo, // 这个算法快，但是如果经常变换密码，生成密钥表很浪费时间
    Encrypt_Count,
};

enum eCompressAlgo
{
    Raw_C_Algo,
    Zip_C_Algo,   // Zip
    Lzma_C_Algo,  // 7z
    //LZMA2_C_Algo,
    Compress_Count,
};

enum eCompressParam
{
    Unpack_Fast,
    Compress_Normal,
    Compress_High,
    Compress_Auto,
    CompressParam_Count,
};

//enum eFileNamePos
//{
//    Not_Exist,
//    Bebind_Head,
//    File_Tail,
//    // In_Out,  暂时不支持这个
//};

//#define _FileName_FileExt ".Name"

/**
*@note File Version 1.0 Define
*@ {
*/
template<>
struct TFileHead<File_Version_1_0> :
    TFileHeadBase
{
    // 这个部分原先打算给客户端做解压内存池，暂时没实现，预留
    DWORD dwRawDataMem[TDataMemInfo::Max_Num];          
    DWORD dwCompressDataMem[TDataMemInfo::Max_Num];

    struct TDataIndex
    {
        bool operator < (const TDataIndex& a) const {return HashValue < a.HashValue;}
        bool operator > (const TDataIndex& a) const {return HashValue > a.HashValue;}
        bool operator ==(const TDataIndex& a) const {return HashValue ==a.HashValue;}
        UHashValue HashValue;
        DWORD dwOffset, dwLen;
    }DataIndex[1];
};

/**
*@note 这部分本可以放入文件头的TDataIndex，但因为文件头一般是常驻与内存
*      这部分数据不是读取数据必须的，考虑节约内存就放在这
*/
template<>
struct TDataHead<File_Version_1_0>
{
    DWORD dwRawDataLen:26;  // 64MB-1
    DWORD nCompressAlgo:3;  // 0-7
    DWORD nCompressLevel:3; // 0-7
};
/** File Version 1.0 Define
*@ } 
*/

/**
*@note File Version 1.1 Define
*@ {
*/
template<>
struct TFileHead<File_Version_1_1> :
    TFileHeadBase
{
    DWORD dwRealFileCount;  // 如果不分包，文件的个数，dwFileCount为分卷之后的卷数
    DWORD dwMaxVolumeSize;  // 分卷，单卷的最大值

    struct TDataIndex
    {
        DWORD dwOffset, dwLen;
        DWORD dwRawVolumeDataLen;
    }DataIndex[1];
};

template<>
struct TDataHead<File_Version_1_1>
{
     UHashValue HashValue;
     DWORD dwRawDataLen;
};
/** File Version 1.1 Define
*@ } 
*/

}


#endif  // _RESFILEDEF_H_2010_


 // End of file



