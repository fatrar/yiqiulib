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

typedef unsigned int DWROD;
typedef unsigned int size_t;
typedef unsigned short WORD;
typedef unsigned char BYTE;

typedef unsigned long long QWROD;

#define _MakeDWORD(a,b,c,d) ((a<<24)|(b<<16)|(c<<8)|(d))
#define _MakeFileVersion(year,month,day) ((year<<16)|(month<<8)|(day))

enum
{
	File_Format_Flag = _MakeDWORD('A','n','i','m'),
	File_Version_1_0 = _MakeFileVersion(2010,9,28),
};


struct 

struct FileHead
{
    DWROD FormatFlag;
    DWROD Version;
    DWROD nFileCount;
    BYTE nHashBits;    // 8位，16位，32位，64位
                       // 要检验hash碰撞，包增量更新及时处理出一个新最适应的模式
    BYTE nHashPart;
    BYTE nHashKey;
    BYTE nFileName;

    union
    {
        BYTE  nHashValue[1];
        WORD  wHashValue[1];
        DWROD dwHashValue[1];
        QWROD qwHashValue[1];
    };

    DWROD dwDataPos[1];
    DWROD nImgGroupCount;
    DWROD nImgGroupDataPos;
    DWROD nPalGroupCount;
    DWROD nPalGroupDataPos;
};

struct DataHead
{
    BYTE nEncryptMode;
    BYTE nCompressMode;
    WORD nEncryptLen;
    size_t nDataLen;

};



};




#endif  // _RECFILEDEF_H_2010_


 // End of file



