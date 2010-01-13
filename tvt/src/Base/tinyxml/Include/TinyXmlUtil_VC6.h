/*H***************************************************************************
File            : TinyXmlUtil_VC6.h
Subsystem       : 
Function Name(s):

Description     : vc 版本的编译器对模板支持不好，用模板下列三个函数编译会有问题
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
	
bool GetElementTextData(
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
bool GetElementAttributeData(
	TiXmlElement* pEle,          // User Element
	const char* pChildAttrName,
	char* szValue,
	const char* pDefault = NULL )
{
	const char* pTmpVal = GetElementAttributeData(pEle, pChildAttrName, pDefault);
	if ( NULL == pTmpVal )
	{
		return false;
	}
	
	return (0 == strcpy(szValue, pTmpVal));
}

bool GetChildElementAttributeData(
	TiXmlElement* pEle,          // User Element
	const char* pChildEleName,   // Will Get Data Child Element Name
	const char* pChildAttrName,
	char* szValue,
	const char* pDefault = NULL )
{
	const char* pTmpVal = GetChildElementAttributeData(
		pEle, pChildEleName, pChildAttrName, pDefault);
	if ( NULL == pTmpVal )
	{
		return false;
	}
	
	return (0 == strcpy(szValue, pTmpVal));
}

#else

template<unsigned long nSize>
bool GetElementTextData(
	TiXmlElement* pEle,          // User Element
	char (&szValue)[nSize],
	const char* pDefault = NULL)
{
	const char* pTmpVal = GetElementTextData(pEle, pDefault);
	if ( NULL == pTmpVal )
	{
		return false;
	}
	
	return (0 == strcpy_s<nSize>(szValue, pTmpVal));
}

template<unsigned long nSize>
bool GetChildElementAttributeData(
    TiXmlElement* pEle,          // User Element
    const char* pChildEleName,   // Will Get Data Child Element Name
    const char* pChildAttrName,
    char (&szValue)[nSize],
    const char* pDefault = NULL )
{
    const char* pTmpVal = GetChildElementAttributeData(
        pEle, pChildEleName, pChildAttrName, pDefault);
    if ( NULL == pTmpVal )
    {
        return false;
    }

    return (0 == strcpy_s<nSize>(szValue, pTmpVal));
}

template<unsigned long nSize>
bool GetElementAttributeData(
	TiXmlElement* pEle,          // User Element
	const char* pChildAttrName,
	char (&szValue)[nSize],
	const char* pDefault = NULL )
{
	const char* pTmpVal = GetElementAttributeData(pEle, pChildAttrName, pDefault);
	if ( NULL == pTmpVal )
	{
		return false;
	}
	
	return (0 == strcpy_s<nSize>(szValue, pTmpVal));
}


#endif
}



/**
@purpose          : 设置一个定时器
@param	 timerID  : 定时器ID
@param   interval : 定时器调用间隔
@param   handler  : 处理接口
@param   callTimes: 调用次数,默认调用无穷次
@param   debugInfo: 调试信息
@return		      : 如果设置成功则返回true
*/




 
/**
@purpose          : 设置一个定时器
@param	 timerID  : 定时器ID
@param   interval : 定时器调用间隔
@param   handler  : 处理接口
@param   callTimes: 调用次数,默认调用无穷次
@param   debugInfo: 调试信息
@return		      : 如果设置成功则返回true
*/

static int a(int i , int j);

#endif