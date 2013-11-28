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
#pragma pack(push)
#pragma pack(4)
#include "ResCommon.h"



namespace ResFile
{

template<DWORD Version> struct TFileHead;
template<DWORD Version> struct TDataHead;


namespace TDataMemInfo
{
    enum {Max_Num = 12};
    //static const BYTE s_RawPercent[Max_Num];      // x%的原始数据是内存xxByte以下，
                                                  // 100为最大的数据文件大小，0则最小
    //static const BYTE s_CompressPercent[Max_Num]; // x%的压缩数据是内存xxByte以下
    
    static const BYTE s_RawPercent[] = 
    {
        100,95,90,85,80,75,
        70,65,60,55,50,0
    };
    static const BYTE s_CompressPercent[] = 
    {
        100,95,90,85,80,75,
        70,65,60,55,50,0
    };
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
    DWORD dwVolumeCount;
    DWORD dwMaxVolumeSize;
};

union UHashValue
{
    DWORD dwValue[2];
    QWORD qwValue;

    inline bool operator ==(const UHashValue& a) const {return qwValue==a.qwValue;}
    inline bool operator > (const UHashValue& a) const {return qwValue>a.qwValue;}
    inline bool operator < (const UHashValue& a) const {return qwValue<a.qwValue;}

    inline explicit UHashValue(const QWORD& V){qwValue = V;}
    inline explicit UHashValue(const UHashValue& V){qwValue = V.qwValue;}
    inline UHashValue(){qwValue=0;}
    
    // arm or gcc4.1 64 operator bug
    inline UHashValue& operator=(const QWORD& V){qwValue = V; return *this;}
    inline UHashValue& operator=(const UHashValue& V)
    {dwValue[0] = V.dwValue[0];dwValue[1] = V.dwValue[1]; return *this;}
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
    CompressParam_Count,
};

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
    DWORD dwRawDataLen:25;  // 64MB-1
    DWORD nCompressAlgo:3;  // 0-7
    DWORD nCompressLevel:3; // 0-7
    DWORD nIsDecrypt:1;
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
    //DWORD nCompressLevel:3;

    struct TDataIndex
    {
        DWORD dwOffset, dwLen;
        DWORD dwRawVolumeLen;
    }DataIndex[1];
};

/**
*@note 这个头是指1卷中，每个数据前的头，本身一卷的数据是没有头的
*/
template<>
struct TDataHead<File_Version_1_1>
{
     UHashValue HashValue;
     DWORD dwRawDataLen;
};
/** File Version 1.1 Define
*@ } 
*/


/**
*@note File Version 1.2 Define
*@ {
*/
template<>
struct TFileHead<File_Version_1_2> :
    TFileHead<File_Version_1_0>{};
template<>
struct TDataHead<File_Version_1_2> :
    TDataHead<File_Version_1_0>{};

static const BYTE g_Res12Key[] = {0xc3, 0x49, 0x8A, 0xE6};

/** File Version 1.2 Define
*@ } 
*/

/**
*@note File Version 1.3 Define
*@ {
*/
template<>
struct TFileHead<File_Version_1_3> :
    TFileHead<File_Version_1_0>{};
template<>
struct TDataHead<File_Version_1_3> :
    TDataHead<File_Version_1_0>{};

static const BYTE g_Res13Key[] = {0xd4, 0x59, 0x8a, 0x76};

/** File Version 1.3 Define
*@ } 
*/

typedef TFileHead<File_Version_1_0> FileHead0;
typedef TFileHead<File_Version_1_1> FileHead1;
typedef FileHead0::TDataIndex DataIndex0;
typedef FileHead1::TDataIndex DataIndex1;
typedef TDataHead<File_Version_1_0> DataHead0;
typedef TDataHead<File_Version_1_1> DataHead1;

}

#pragma pack(pop)

#endif  // _RESFILEDEF_H_2010_


 // End of file



