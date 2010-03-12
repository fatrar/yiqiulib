/*H***************************************************************************
File            : TinyXmlUtil_VC6.h
Subsystem       : 
Function Name(s):

Description     : vc �汾�ı�������ģ��֧�ֲ��ã���ģ�������������������������
Author          : Yiqiu
Date            : 2009/1/15
Revision        : 

History
-------

Copyright (c) xx Ltd.
***************************************************************************H*/
#ifndef _TINYXMLUTIL_VC6_H_2009_1_
#define _TINYXMLUTIL_VC6_H_2009_1_



namespace TinyXmlUtil
{

#if defined(_MSC_VER) && (_MSC_VER <= 1200 )  // vc6 and old version use
	
bool GetTextData(
    TiXmlElement* pEle,          // User Element
    char* szValue,
    const char* pDefault = NULL)
{
    const char* pTmpVal = GetElementTextData(pEle, pDefault);
    if ( NULL == pTmpVal )
    {
        return false;
    }
	
    return (0 == strcpy(szValue, pTmpVal));
}

template<unsigned long nSize>
bool GetAttributeData(
	TiXmlElement* pEle,          // User Element
	const char* pChildAttrName,
	char* szValue,
	const char* pDefault = NULL )
{
	const char* pTmpVal = GetAttributeData(pEle, pChildAttrName, pDefault);
	if ( NULL == pTmpVal )
	{
		return false;
	}
	
	return (0 == strcpy(szValue, pTmpVal));
}

bool GetChildAttributeData(
	TiXmlElement* pEle,          // User Element
	const char* pChildEleName,   // Will Get Data Child Element Name
	const char* pChildAttrName,
	char* szValue,
	const char* pDefault = NULL )
{
	const char* pTmpVal = GetChildAttributeData(
		pEle, pChildEleName, pChildAttrName, pDefault);
	if ( NULL == pTmpVal )
	{
		return false;
	}
	
	return (0 == strcpy(szValue, pTmpVal));
}

#else

template<unsigned long nSize>
bool GetTextData(
	TiXmlElement* pEle,          // User Element
	char (&szValue)[nSize],
	const char* pDefault = NULL)
{
	const char* pTmpVal = GetTextData(pEle, pDefault);
	if ( NULL == pTmpVal )
	{
		return false;
	}
	
	return (0 == strcpy_s<nSize>(szValue, pTmpVal));
}

template<unsigned long nSize>
bool GetChildAttributeData(
    TiXmlElement* pEle,          // User Element
    const char* pChildEleName,   // Will Get Data Child Element Name
    const char* pChildAttrName,
    char (&szValue)[nSize],
    const char* pDefault = NULL )
{
    const char* pTmpVal = GetChildAttributeData(
        pEle, pChildEleName, pChildAttrName, pDefault);
    if ( NULL == pTmpVal )
    {
        return false;
    }

    return (0 == strcpy_s<nSize>(szValue, pTmpVal));
}

template<unsigned long nSize>
bool GetAttributeData(
	TiXmlElement* pEle,          // User Element
	const char* pChildAttrName,
	char (&szValue)[nSize],
	const char* pDefault = NULL )
{
	const char* pTmpVal = GetAttributeData(pEle, pChildAttrName, pDefault);
	if ( NULL == pTmpVal )
	{
		return false;
	}
	
	return (0 == strcpy_s<nSize>(szValue, pTmpVal));
}


#endif
}



/**
@purpose          : ����һ����ʱ��
@param	 timerID  : ��ʱ��ID
@param   interval : ��ʱ�����ü��
@param   handler  : ����ӿ�
@param   callTimes: ���ô���,Ĭ�ϵ��������
@param   debugInfo: ������Ϣ
@return		      : ������óɹ��򷵻�true
*/




 
/**
@purpose          : ����һ����ʱ��
@param	 timerID  : ��ʱ��ID
@param   interval : ��ʱ�����ü��
@param   handler  : ����ӿ�
@param   callTimes: ���ô���,Ĭ�ϵ��������
@param   debugInfo: ������Ϣ
@return		      : ������óɹ��򷵻�true
*/

static int a(int i , int j);

#endif