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
    *@note �������ݣ���������
    *@return �ɹ�ʧ��
    */
    virtual bool Parse()=0;

    /**
    *@note �õ���Դ����ĳ���ļ���ԭʼ���ݵĳ���
    *@param	pFileName �ļ��� 
    *@return �����ļ���ԭʼ���ݵĳ��ȣ����Ϊ0���ļ�������
    */
    virtual size_t GetDataLen(const char* pFileName)=0;

    /**
    *@note �õ���Դ����ĳ���ļ�
    *@param	pFileName �ļ���
    *@param UnPackDataInfo �ļ��Ļ�����Ϣ
    *@return ���سɹ�ʧ�ܣ�0���ļ�������
    */
    virtual bool GetData(
        const char* pFileName,
        CUnPackDataInfo& UnPackDataInfo)=0;

protected:
    friend CUnPackDataInfo;
    virtual void Release(CUnPackDataInfo* pUnPackDataInfo)=0;

    // ����C++ friend��Ȩ���ܼ̳л�ã�����ֻ������д������ʵ��
    inline static int GetMemType(CUnPackDataInfo* pUnPackDataInfo);
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
*@note �ɵ������һὫpResReader����
*/
void DestroyResReader(IResReader*& pResReader);

}






#endif  // _IRECFILEREADER_H_2010_


 // End of file



