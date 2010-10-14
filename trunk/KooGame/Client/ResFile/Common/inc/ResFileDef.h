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
	File_Format_Flag = _MakeDWORD('R','e','c','P'),
	File_Version_1_0 = _MakeFileVersion(2010,9,28),
   
    // example:
    // File_Version_2_0 = _MakeFileVersion(2011,9,28),

    Max_File_Head_1_0 = 1 << 20,  // 1MB

    Invaild_Pos = -1,
};

template<DWORD Version>
struct TDataInfo;

template<DWORD Version>
struct TFileHead;

template<DWORD Version>
struct TDataHead;

template<DWORD Version>
struct TDataMemInfo;

struct TFileHeadBase
{
    DWORD dwSize;
    DWORD FormatFlag;
    DWORD Version;
    DWORD dwFileCount:24;
    DWORD nFileNameFlag:8;  // 可以在外部，也可以在文件最后面，或者在文件头后
};

union UHashValue
{
    struct {
        DWORD dwValue[2];
    } TValue;

    QWORD qwValue;

    inline bool operator ==(const UHashValue& a) const {return qwValue==a.qwValue;}
    inline bool operator > (const UHashValue& a) const {return qwValue>a.qwValue;}
    inline bool operator < (const UHashValue& a) const {return qwValue<a.qwValue;}
    inline void operator = (const QWORD& V){qwValue = V;}
};

// Encrypt Algorithm 
enum eEncryptAlgo
{
    Raw_E_Algo,
    Xor_E_Algo,
    BlowFish_E_Algo,
    Encrypt_Count,
};

enum eCompressAlgo
{
    Raw_C_Algo,
    LZMA_C_Algo,  // 7z
    LZMA2_C_Algo,
    Zip_C_Algo,   // Zip
    Compress_Count,
};

enum eFileNamePos
{
    Not_Exist,
    In_Out,
    Bebind_Head,
    File_Tail,
};

namespace Util
{
template<DWORD Version>
inline size_t GetFileHeadSize(size_t nFileNum);
}

/**
*@note File Version 1.0 Define
*@ {
*/

template<>
struct TDataInfo<File_Version_1_0>
{
    DWORD dwDataPos;
    DWORD dwDataLen;
};

template<>
struct TDataMemInfo<File_Version_1_0>
{
    enum {Max_Num = 12};
    static const BYTE s_RawPercent[Max_Num];
    static const BYTE s_CompressPercent[Max_Num];
};
__declspec(selectany) const BYTE TDataMemInfo<File_Version_1_0>::s_RawPercent[] = 
{
    100,95,90,85,80,
     75,70,65,60,55,
     50,0
};
__declspec(selectany) const BYTE TDataMemInfo<File_Version_1_0>::s_CompressPercent[] = 
{
    100,95,90,85,80,
     75,70,65,60,55,
     50,0
};

template<>
struct TFileHead<File_Version_1_0> :
    public TFileHeadBase
{
    typedef TDataMemInfo<File_Version_1_0> DataMemInfo;
    //BYTE nHashBits;    // 16位，32位，64位
                         // 要检验hash碰撞，包增量更新及时处理出一个新最适应的模式
    //BYTE nHashPart;
    //BYTE nHashKey;
    DWORD dwRawDataMem[DataMemInfo::Max_Num];          
    DWORD dwCompressDataMem[DataMemInfo::Max_Num];

    struct TDataIndex
    {
        bool operator < (const TDataIndex& a) const {return HashValue < a.HashValue;}
        bool operator > (const TDataIndex& a) const {return HashValue > a.HashValue;}
        bool operator ==(const TDataIndex& a) const {return HashValue ==a.HashValue;}
        UHashValue HashValue;
        TDataInfo<File_Version_1_0> DataHead;
    } DataIndex[1];
};

template<>
struct TDataHead<File_Version_1_0>
{
    DWORD dwRawDataLen;
    DWORD nEncryptAlgo:3;     // 0-7
    DWORD nCompressAlgo:3;    // 0-7
    DWORD nDataEncryptLen:26; // Max 64MB-1

    struct TEncryptParam
    {
        void operator = (const TEncryptParam& a){memcpy(this, &a, sizeof(TEncryptParam));}
        union {
            struct {
                DWORD dwParam1;
                DWORD dwParam2;
            } EncryptParam;

            char cEncryptParam[8];
            BYTE ucEncryptParam[8];
        };
    };
};

namespace Util
{

template<>
inline size_t GetFileHeadSize<File_Version_1_0>(DWORD dwFileCount)
{
    //TFileHead<File_Version_1_0> xx;
    typedef TFileHead<File_Version_1_0> FileHead;
    return sizeof(FileHead) + sizeof(FileHead::TDataIndex)*(dwFileCount-1);
}
}
/** File Version 1.0 Define
*@ } 
*/

}


#endif  // _RESFILEDEF_H_2010_


 // End of file



