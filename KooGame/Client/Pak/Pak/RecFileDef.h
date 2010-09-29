/**H**************************************************************************
 File            : RecFileDef.h
 Subsystem       : 
 Function Name(s): CRecFileDef
 Author          : YiQiu
 Date            : 2010-9-28  
 Time            : 15:33
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _RECFILEDEF_H_2010_9
#define _RECFILEDEF_H_2010_9



namespace RecFile
{

typedef unsigned int DWORD;
typedef unsigned int size_t;
typedef unsigned short WORD; 
typedef unsigned char BYTE;

typedef unsigned long long QWORD;

#define _MakeDWORD(a,b,c,d) ((a<<24)|(b<<16)|(c<<8)|(d))
#define _MakeFileVersion(year,month,day) ((year<<16)|(month<<8)|(day))

enum
{
	File_Format_Flag = _MakeDWORD('A','n','i','m'),
	File_Version_1_0 = _MakeFileVersion(2010,9,28),

};

// Encrypt Algorithm 
enum EncryptAlgo
{
    Raw_E_Algo,
    Xor_E_Algo,
    BlowFish_E_Algo,
};

enum CompressAlgo
{
    Raw_C_Algo,
    LZMA_C_Algo,  // 7z
};

struct DataInfo
{
    DWORD dwDataPos;
    DWORD dwDataLen;
    BYTE nEncryptAlgo;
    BYTE nCompressAlgo;
    BYTE nReserve[2];

    union {
        struct {
            DWORD dwParam1;
            DWORD dwParam2;
        } EncryptParam;

        char cEncryptParam[8];
        BYTE ucEncryptParam[8];
    };
};

struct FileHead
{
    DWORD FormatFlag;
    DWORD Version;
    DWORD nFileCount;
    BYTE nHashBits;    // 16位，32位，64位
                       // 要检验hash碰撞，包增量更新及时处理出一个新最适应的模式
    BYTE nHashPart;
    BYTE nHashKey;
    BYTE nFileName;

    WORD wDataEncryptLen;

    union
    {
        WORD  wHashValue[1];
        DWORD dwHashValue[1];
        QWORD qwHashValue[1];
    };
};


struct DataHead
{
    DWORD dwSize;
    BYTE nEncryptAlgo;
    BYTE nCompressAlgo;
    DWORD dwDataLen;

    union
    {
        struct {
            DWORD dwParam1;
            DWORD dwParam2;
        } EncryptParam;

        char cEncryptParam[8];
        BYTE ucEncryptParam[8];
    };
};



};


#endif  // _RECFILEDEF_H_2010_


 // End of file



