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


struct TDataMemInfo
{
    enum {Max_Num = 12};
    static const BYTE s_RawPercent[Max_Num];      // x%��ԭʼ�������ڴ�xxByte���£�
                                                  // 100Ϊ���������ļ���С��0����С
    static const BYTE s_CompressPercent[Max_Num]; // x%��ѹ���������ڴ�xxByte����
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
        BYTE szKey[8];  // �����Կ8���ֽ�
        QWORD dwKey;    // �����XOR���������
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
    BlowFish_E_Algo, // ����㷨�죬������������任���룬������Կ����˷�ʱ��
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
//    // In_Out,  ��ʱ��֧�����
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
    // �������ԭ�ȴ�����ͻ�������ѹ�ڴ�أ���ʱûʵ�֣�Ԥ��
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
*@note �ⲿ�ֱ����Է����ļ�ͷ��TDataIndex������Ϊ�ļ�ͷһ���ǳ�פ���ڴ�
*      �ⲿ�����ݲ��Ƕ�ȡ���ݱ���ģ����ǽ�Լ�ڴ�ͷ�����
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
    DWORD dwRealFileCount;  // ������ְ����ļ��ĸ�����dwFileCountΪ�־�֮��ľ���
    DWORD dwMaxVolumeSize;  // �־���������ֵ
    //DWORD nCompressLevel:3;

    struct TDataIndex
    {
        DWORD dwOffset, dwLen;
        DWORD dwRawVolumeDataLen;
    }DataIndex[1];
};

/**
*@note ���ͷ��ָ1���У�ÿ������ǰ��ͷ������һ���������û��ͷ��
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

}

#pragma pack(pop)

#endif  // _RESFILEDEF_H_2010_


 // End of file



