/**H**************************************************************************
 File            : IRecFileReader.h
 Subsystem       : 
 Function Name(s): CIRecFileReader
 Author          : YiQiu
 Date            : 2010-10-8  
 Time            : 14:30
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _IRECFILEREADER_H_2010_10
#define _IRECFILEREADER_H_2010_10

#include "ResCommon.h"

namespace ResFile
{

class CUnPackDataInfo;

/**
*@note 本应放在IResReader里面，但是为了方便CUnPackDataInfo使用放在外面
*/
enum ResMemType
{
    Mem_Unknown,
    User_Allocate,
    Reader_Static_Buf,
    Reader_Allocate,
};

struct IResReader
{
    virtual ~IResReader() {};

    /**
    *@note 解析数据，建立索引
    *@return 成功失败
    */
    virtual bool Parse()=0;

    /**
    *@note 得到资源包中某个文件的原始数据的长度
    *@param	pFileName 文件名 
    *@return 返回文件的原始数据的长度，如果为0则文件不存在
    */
    virtual size_t GetDataLen(const char* pFileName)=0;

    /**
    *@note 得到资源包中某个文件
    *@param	pFileName 文件名
    *@param UnPackDataInfo 文件的基本信息
    *@return 返回成功失败，0则文件不存在
    */
    virtual bool GetData(
        const char* pFileName,
        CUnPackDataInfo& UnPackDataInfo)=0;

protected:
    friend CUnPackDataInfo;
    virtual void Release(CUnPackDataInfo* pUnPackDataInfo)=0;

    // 由于C++ friend特权不能继承获得，所以只能在这写个函数实现
    inline static int GetMemType(CUnPackDataInfo* pUnPackDataInfo);
};

/**
*@note 保存解压后文件的信息，可以得到数据及数据长度.
*      如果你知道这个文件的大小或者知道这个文件最大大小，可以在构造可以输入
*      一个内存缓存及缓存大小，IRecReader发现这个值会利用这个值拷贝数据   
*/
class CUnPackDataInfo 
{
    friend IResReader;
public:
    inline CUnPackDataInfo()
        : m_pData(NULL)
        , m_nLen(0)
        , m_nMemType(Mem_Unknown) {}
    inline CUnPackDataInfo(
        BYTE* pData,
        size_t nLen )
    {
        if ( pData == NULL )
        {
            CUnPackDataInfo();
        }
        else
        {
            m_pData = pData;
            m_nLen  = nLen;
            m_nMemType = User_Allocate;
        }
    }
    inline ~CUnPackDataInfo(){m_pResReader->Release(this);}

public:
    inline operator const BYTE*()const{return m_pData;}
    inline operator const char*()const{return (char*)m_pData;}
    inline size_t Length(){return m_nLen;}
    inline const BYTE* Ptr(){return m_pData;}

protected:
    BYTE* m_pData;
    size_t m_nLen;
    int m_nMemType;
    IResReader* m_pResReader;
};

int IResReader::GetMemType(CUnPackDataInfo* pUnPackDataInfo)
{
    return pUnPackDataInfo->m_nMemType;
}

IResReader* CreateResFileReader(const char* pResFilePath);
IResReader* CreateMemResReader(BYTE* pData, size_t nSize);

/**
*@note 干掉对象且会将pResReader赋空
*/
void DestroyResReader(IResReader*& pResReader);

}






#endif  // _IRECFILEREADER_H_2010_


 // End of file



