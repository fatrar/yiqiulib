/*H***************************************************************************
 File            : TinyxmlAttiUtil.cpp
 Subsystem       : 
 Function Name(s): CTinyxmlAttiUtil
 Author          : YiQiu
 Date            : 2010-1-30  
 Time            : 16:56
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "StdAfx.h"
#include "Tinyxml.h"
#include "TinyXmlUtil.h"
#include "..\..\Base\Include\Common.h"


//
// ********************* Get Attribute *************************
//
const char* TinyXmlUtil::GetElementAttributeData(
    TiXmlElement* pEle, /* User Element */
    const char* pChildAttrName, 
    const char* pDefault /*= NULL */ )
{
    if ( NULL == pEle ||
        !isValidString(pChildAttrName) )
    {
        return pDefault;
    }

    const char* pData = pEle->Attribute(pChildAttrName);
    if ( NULL == pData )
    {
        return pDefault;
    }

    return pData;
}

bool TinyXmlUtil::GetElementAttributeData(
    TiXmlElement* pEle, /* User Element */ 
    const char* pChildAttrName,
    int& nValue,
    int nDefault /*= 0 */ )
{
    if ( NULL == pEle ||
        !isValidString(pChildAttrName) )
    {
        nValue = nDefault; 
        return false;
    }

    const char* pData = pEle->Attribute(pChildAttrName, &nValue);
    if ( NULL == pData )
    {
        nValue = nDefault; 
        return false;
    }

    return true;
}

bool TinyXmlUtil::GetElementAttributeData(
    TiXmlElement* pEle, /* User Element */
    const char* pChildAttrName,
    bool& bValue, 
    bool bDefault /*= false */ )
{
    if ( NULL == pEle ||
        !isValidString(pChildAttrName) )
    {
        bValue = bDefault; 
        return false;
    }

    int nValue;
    const char* pData = pEle->Attribute(pChildAttrName, &nValue);
    if ( NULL == pData )
    {
        bValue = bDefault; 
        return false;
    }

    bValue = (nValue!=0);
    return true;
}

const char* TinyXmlUtil::GetChildElementAttributeData(
    TiXmlElement* pEle, /* User Element */
    const char* pChildEleName,
    const char* pChildAttrName,
    const char* pDefault )
{
    if ( NULL == pEle ||
        !isValidString(pChildEleName) ||
        !isValidString(pChildAttrName) )
    {
        return pDefault;
    }

    TiXmlElement* pChildEle = pEle->FirstChildElement(pChildEleName);
    if ( NULL == pChildEleName )
    {
        return pDefault;
    }

    return GetElementAttributeData(
        pChildEle,
        pChildAttrName,
        pDefault );
}

bool TinyXmlUtil::GetChildElementAttributeData(
    TiXmlElement* pEle,        /* User Element */
    const char* pChildEleName, /* Will Get Data Child Element Name */
    const char* pChildAttrName,
    int& nValue,
    int nDefault )
{
    if ( NULL == pEle ||
        !isValidString(pChildEleName) ||
        !isValidString(pChildAttrName) )
    {
        nValue = nDefault;
        return false;
    }

    TiXmlElement* pChildEle = pEle->FirstChildElement(pChildEleName);
    if ( NULL == pChildEleName )
    {
        nValue = nDefault;
        return false;
    }

    return GetElementAttributeData(
        pChildEle,
        pChildAttrName,
        nValue,
        nDefault );
}

bool TinyXmlUtil::GetChildElementAttributeData(
    TiXmlElement* pEle,        /* User Element */
    const char* pChildEleName, /* Will Get Data Child Element Name */ 
    const char* pChildAttrName,
    bool& bValue,
    bool bDefault )
{
    if ( NULL == pEle ||
        !isValidString(pChildEleName) ||
        !isValidString(pChildAttrName) )
    {
        bValue = bDefault;
        return false;
    }

    TiXmlElement* pChildEle = pEle->FirstChildElement(pChildEleName);
    if ( NULL == pChildEleName )
    {
        bValue = bDefault;
        return NULL;
    }

    return GetElementAttributeData(
        pChildEle,
        pChildAttrName,
        bValue,
        bDefault );
}

bool TinyXmlUtil::SetChildElementAttributeData(
    TiXmlElement* pEle, /* User Element */ 
    const char* pChildEleName, /* Will Get Data Child Element Name */
    const char* pChildAttrName,
    const char* pValue /*= ""*/ )
{
    if ( NULL == pEle ||
        !isValidString(pChildEleName) ||
        !isValidString(pChildAttrName) ||
        NULL == pValue )
    {
        return false;
    }

    TiXmlElement* pChildEle = CreateChildEle(pEle, pChildEleName);
    if ( NULL == pChildEle )
    {
        return false;
    }

    pChildEle->SetAttribute(pChildAttrName, pValue);
    return true;
}

bool TinyXmlUtil::SetChildElementAttributeData(
    TiXmlElement* pEle, /* User Element */ 
    const char* pChildEleName, /* Will Get Data Child Element Name */
    const char* pChildAttrName, 
    int nValue /*= 0 */ )
{
    if ( NULL == pEle ||
        !isValidString(pChildEleName) ||
        !isValidString(pChildAttrName) )
    {
        return false;
    }

    TiXmlElement* pChildEle = CreateChildEle(pEle, pChildEleName);
    if ( NULL == pChildEle )
    {
        return false;
    }

    pChildEle->SetAttribute(pChildAttrName, nValue);
    return true;
}

bool TinyXmlUtil::SetChildElementAttributeData(
    TiXmlElement* pEle, /* User Element */ 
    const char* pChildEleName, /* Will Get Data Child Element Name */
    const char* pChildAttrName,
    bool bValue /*= false*/ )
{
    if ( NULL == pEle ||
         !isValidString(pChildEleName) ||
         !isValidString(pChildAttrName) )
    {
        return false;
    }

    TiXmlElement* pChildEle = CreateChildEle(pEle, pChildEleName);
    if ( NULL == pChildEle )
    {
        return false;
    }

    pChildEle->SetAttribute(pChildAttrName, bValue);
    return true;
}

bool TinyXmlUtil::SetAttributeData(
    TiXmlElement* pEle,
    const char* pChildAttrName,
    const void* pValue,
    size_t nLen )
{
    if ( NULL == pEle ||
         !isValidString(pChildAttrName) ||
         IsBadReadPtr(pValue, nLen) )
    {
        return false;
    }

    BYTE* pBase64Data = NULL;
    size_t nOutLen;
    Base64Encode(
        (const BYTE*)pValue, 
        nLen, 
        (BYTE*&)pBase64Data,
        nOutLen);
    if ( NULL == pBase64Data )
    {
        return false;
    }

    //
    // 为什么要拷贝一份？
    // 因为Base64Encode返回的是二进制，他并不一定是'\0'结尾。
    // 根据Base64的算法，当刚好是3的倍数时，结尾一般不是'\0'
    //
    char* pBuf = new char[nOutLen+1];
    memcpy(pBuf, pBase64Data, nLen);
    pBuf[nLen] = '\0';
    delete[] pBase64Data;

    pEle->SetAttribute(pChildAttrName, pBuf);
    delete[] pBuf;
    return true;
}
















// End of file