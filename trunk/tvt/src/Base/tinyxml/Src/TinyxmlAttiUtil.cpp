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
const char* TinyXmlUtil::GetAttributeData(
    TiXmlElement* pEle, /* User Element */
    const char* pAttrName, 
    const char* pDefault /*= NULL */ )
{
    if ( NULL == pEle ||
        !isValidString(pAttrName) )
    {
        return pDefault;
    }

    const char* pData = pEle->Attribute(pAttrName);
    if ( NULL == pData )
    {
        return pDefault;
    }

    return pData;
}

bool TinyXmlUtil::GetAttributeData(
    TiXmlElement* pEle, /* User Element */ 
    const char* pAttrName,
    int& nValue,
    int nDefault /*= 0 */ )
{
    if ( NULL == pEle ||
        !isValidString(pAttrName) )
    {
        nValue = nDefault; 
        return false;
    }

    const char* pData = pEle->Attribute(pAttrName, &nValue);
    if ( NULL == pData )
    {
        nValue = nDefault; 
        return false;
    }

    return true;
}

bool TinyXmlUtil::GetAttributeData(
    TiXmlElement* pEle, /* User Element */
    const char* pAttrName,
    bool& bValue, 
    bool bDefault /*= false */ )
{
    if ( NULL == pEle ||
        !isValidString(pAttrName) )
    {
        bValue = bDefault; 
        return false;
    }

    int nValue;
    const char* pData = pEle->Attribute(pAttrName, &nValue);
    if ( NULL == pData )
    {
        bValue = bDefault; 
        return false;
    }

    bValue = (nValue!=0);
    return true;
}

bool TinyXmlUtil::GetBinaryAttributeData(
    TiXmlElement* pEle, 
    const char* pAttrName,
    void* pValue, 
    size_t& nLen )
{
    if ( NULL == pEle ||
         !isValidString(pAttrName) )
    {
        return false;
    }

    const char* pBase64Value = pEle->Attribute(pAttrName);
    if ( NULL == pBase64Value )
    {
        return false;
    }

    BYTE* pBase64Buf = NULL;
    size_t nTrueLen = 0;
    Base64Decode(
        (const BYTE*)pBase64Value, 
        strlen(pBase64Value), 
        pBase64Buf,
        nTrueLen );
    if ( NULL == pBase64Buf ) 
    {
        nLen = nTrueLen;
        return false;
    }
    if (nTrueLen > nLen)
    {
        delete[] pBase64Buf;
        nLen = nTrueLen;
        return false;
    }

    memcpy(pValue, pBase64Buf, nTrueLen);
    nLen = nTrueLen;
    delete[] pBase64Buf;
    return true;
}

const char* TinyXmlUtil::GetChildAttributeData(
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

    return GetAttributeData(
        pChildEle,
        pChildAttrName,
        pDefault );
}

bool TinyXmlUtil::GetChildAttributeData(
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

    return GetAttributeData(
        pChildEle,
        pChildAttrName,
        nValue,
        nDefault );
}

bool TinyXmlUtil::GetChildAttributeData(
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

    return GetAttributeData(
        pChildEle,
        pChildAttrName,
        bValue,
        bDefault );
}

bool TinyXmlUtil::SetChildAttributeData(
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

bool TinyXmlUtil::SetChildAttributeData(
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

bool TinyXmlUtil::SetChildAttributeData(
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

bool TinyXmlUtil::SetBinaryAttributeData(
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