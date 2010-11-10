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
*@note ��Ӧ����IResReader���棬����Ϊ�˷���CUnPackDataInfoʹ�÷�������
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
    *@note �õ���Դ����ĳ���ļ���ԭʼ���ݵĳ���
    *@param	pFileName �ļ��� 
    *@return �����ļ���ԭʼ���ݵĳ��ȣ����Ϊ0���ļ�������
    */
    virtual size_t GetDataLen(
        const char* pFileName,
        size_t& nPos )=0;

    /**
    *@note �õ���Դ����ĳ���ļ�
    *@param	pFileName �ļ���
    *@param UnPackDataInfo �ļ��Ļ�����Ϣ
    *@return ���سɹ�ʧ�ܣ�0���ļ�������
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

    // ����C++ friend��Ȩ���ܼ̳л�ã�����ֻ������д������ʵ��
    //inline static ResMemType& GetMemType(CUnPackDataInfo* pUnPackDataInfo);
    //inline static BYTE*& GetPtr(CUnPackDataInfo* pUnPackDataInfo);
};

/**
*@note �����ѹ���ļ�����Ϣ�����Եõ����ݼ����ݳ���.
*      �����֪������ļ��Ĵ�С����֪������ļ�����С�������ڹ����������
*      һ���ڴ滺�漰�����С��IRecReader�������ֵ���������ֵ��������   
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

    // ��IResReader������ʣ���������ò�Ҫֱ�ӷ���
public:
    BYTE* m_pData;
    size_t m_nLen;
    size_t m_nNow;
    int m_nMemType;
    IResReader* m_pResReader;
};

API_EXPORT IResReader* CreateResFileReader(const char* pResFilePath);

/**
*@note ���ڴ����ݴ�����Դ����ȡ����
*@param	pData ѹ��������  
*@param nSize ���ݳ���  
*@param bRef �Ƿ�ֱ���������� 
*@param bDelete ��������ã��Ƿ�������delete pData
*/
API_EXPORT IResReader* CreateMemResReader(
    BYTE* pData, size_t nSize,
    bool bRef, bool bDelete );

/**
*@note �ɵ������һὫpResReader����
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
*@note ���ܸ����ļ���
�Ƿ�֧�֣��������ݵ�ѹ����ʽ�ͼ��ܷ�ʽ
*/
struct IResUpdater
{
    virtual ~IResUpdater(){}

    /**
    *@note ���£�ԭ���뷨�ǿ������û�ȥѡ���ǽ�Լ�ڴ�ģʽ�����ģʽ
    *@param	nMode ����ģʽ
    *@return 
    */
    virtual bool Update(const char* pFilepath) = 0;
};

/**
*@note �����������ڴ��и���ĳ���ļ�
*@param	pFilepath ��Ҫ���µ��ļ�·�� 
*@param pPatchFilePath �����ļ�·��
*/
IResUpdater* CreateResUpdater(
    //const char* pFilepath,
    const char* pPatchFilePath );

/**
*@note �����������ڴ��и���ĳ���ļ�
*@param	pFilepath ��Ҫ���µ��ļ�·��
*@param pData �����ڴ��еĲ����ļ�
*@param nSize ���ݳ���
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



