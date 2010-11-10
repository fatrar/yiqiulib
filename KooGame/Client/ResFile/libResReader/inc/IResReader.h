/**H**************************************************************************
 File            : IRecReader.h
 Subsystem       : 
 Function Name(s): IRecReader && IResUpdater define
 Author          : YiQiu
 Date            : 2010-10-8  
 Time            : 14:30
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _IRECREADER_H_2010_10
#define _IRECREADER_H_2010_10
#include "Common.h"
#include "ResCommon.h"


namespace ResFile
{

/**
*@note IResReader
*@ {
*/

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
    *@note 得到资源包中某个文件的原始数据的长度
    *@param	pFileName 文件名 
    *@return 返回文件的原始数据的长度，如果为0则文件不存在
    */
    virtual size_t GetDataLen(
        const char* pFileName,
        size_t& nPos )=0;

    /**
    *@note 得到资源包中某个文件
    *@param	pFileName 文件名
    *@param UnPackDataInfo 文件的基本信息
    *@return 返回成功失败，0则文件不存在
    */
    virtual bool GetData(
        const char* pFileName,
        CUnPackDataInfo& UnPackDataInfo)=0;

    virtual int GetDataIndex(
        const char* pFileName,
        QWORD& qwHashValue ) = 0;

    virtual bool GetData(
        size_t nPos,
        CUnPackDataInfo& UnPackDataInfo ) = 0;

protected:
    friend CUnPackDataInfo;
    virtual void Release(CUnPackDataInfo* pUnPackDataInfo)=0;

    // 由于C++ friend特权不能继承获得，所以只能在这写个函数实现
    //inline static ResMemType& GetMemType(CUnPackDataInfo* pUnPackDataInfo);
    //inline static BYTE*& GetPtr(CUnPackDataInfo* pUnPackDataInfo);
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
        , m_nNow(0)
        , m_pResReader(NULL)
        , m_nMemType(Mem_Unknown) {}
    inline CUnPackDataInfo(
        BYTE* pData,
        size_t nLen )
        : m_nNow(0)
        , m_pResReader(NULL)
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
    inline ~CUnPackDataInfo(){if(m_pResReader) m_pResReader->Release(this);}

public:
    inline operator const BYTE*()const{return m_pData;}
    inline operator const char*()const{return (char*)m_pData;}
    inline size_t Length(){return m_nLen;}
    inline const void* Ptr(){return (void*)m_pData;}

    inline void Read(void* pBuf, int len)
    {
        memcpy(pBuf, m_pData+m_nNow, len);
        m_nNow += len;
    }

    // 仅IResReader本身访问，其他的最好不要直接访问
public:
    BYTE* m_pData;
    size_t m_nLen;
    size_t m_nNow;
    int m_nMemType;
    IResReader* m_pResReader;
};

API_EXPORT IResReader* CreateResFileReader(const char* pResFilePath);

/**
*@note 从内存数据创建资源包读取对象
*@param	pData 压缩包数据  
*@param nSize 数据长度  
*@param bRef 是否直接引用数据 
*@param bDelete 如果是引用，是否在析构delete pData
*/
API_EXPORT IResReader* CreateMemResReader(
    BYTE* pData, size_t nSize,
    bool bRef, bool bDelete );

/**
*@note 干掉对象且会将pResReader赋空
*/
API_EXPORT void DestroyResReader(IResReader*& pResReader);


API_EXPORT int IsNeedUnpack(const char* pResFilePath);
API_EXPORT bool UnpackFile(const char* pResFilePath);

/** IResReader
*@ } 
*/

// -----------------------------------------------------------

/**
*@note IResUpdater
*@ {
*/

/**
*@note 不能更新文件名
是否支持，更新数据的压缩格式和加密方式
*/
struct IResUpdater
{
    virtual ~IResUpdater(){}

    /**
    *@note 更新，原本想法是可以由用户去选择是节约内存模式和最快模式
    *@param	nMode 更新模式
    *@return 
    */
    virtual bool Update(const char* pFilepath) = 0;
};

/**
*@note 补丁包放在内存中更新某个文件
*@param	pFilepath 需要更新的文件路径 
*@param pPatchFilePath 补丁文件路径
*/
IResUpdater* CreateResUpdater(
    //const char* pFilepath,
    const char* pPatchFilePath );

/**
*@note 补丁包放在内存中更新某个文件
*@param	pFilepath 需要更新的文件路径
*@param pData 放在内存中的补丁文件
*@param nSize 数据长度
*/
IResUpdater* CreateResUpdater(
    //const char* pFilepath,
    BYTE* pData,
    size_t nSize,
    bool bAutoDel = true);

CLASS_EXPORT int Decode7z(
    void* oData, size_t* oSize, 
    const void* zData, size_t* zSize,
    const BYTE* props);

CLASS_EXPORT int Decode7z(
    void* oData, size_t* oSize, 
    const void* zData, size_t* zSize,
    unsigned int nLevel );


/** IResUpdater
*@ } 
*/

}





#endif  // _IRECFILEREADER_H_2010_


 // End of file



