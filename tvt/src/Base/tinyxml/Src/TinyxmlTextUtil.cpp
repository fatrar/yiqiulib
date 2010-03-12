/*H***************************************************************************
 File            : TinyxmlTextUtil.cpp
 Subsystem       : 
 Function Name(s): CTinyxmlTextUtil
 Author          : YiQiu
 Date            : 2010-1-30  
 Time            : 16:23
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
// ******************** Get Text Value *********************
//
const char* TinyXmlUtil::GetTextData(
    TiXmlElement* pEle,          // User Element
    const char* pDefault )
{
    if ( NULL == pEle )
    {
        return pDefault;
    }

    const char* pValue = NULL;
    pValue = pEle->GetText();
    if ( NULL == pValue )
    {
        return pDefault;
    }

    return pValue;
}

bool TinyXmlUtil::GetTextData(
    TiXmlElement* pEle,
    int& nValue,
    int nDefault )
{
    nValue = nDefault;
    if ( NULL == pEle )
    {   
        return false;
    }

    const char* pValue = pEle->GetText();
    if ( NULL == pValue )
    {
        return false;
    }

    nValue = atoi(pValue);
    return true;
}

bool TinyXmlUtil::GetTextData(
    TiXmlElement* pEle,
    bool& bValue,
    bool bDefault )
{
    const char* pValue = pEle->GetText();
    if ( NULL == pValue )
    {
        bValue = bDefault;
        return false;
    }

    bValue = (atoi(pValue) != 0);
    return true;
}

bool TinyXmlUtil::GetBinaryTextData( 
    TiXmlElement* pEle, /* User Element */ 
    void* pValue, /* buf */ 
    size_t& nLen )
{
    if ( NULL == pEle ||
         NULL == pValue ||
         IsBadWritePtr(pValue, nLen) )
    {
        return false;
    }

    const char* pBase64Value = pEle->GetText();
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
    else if (nTrueLen > nLen)
    {
        delete[] pBase64Buf;
        nLen = nTrueLen;
        return false;
    }

    memcpy(pValue, pBase64Buf, nTrueLen);
    delete[] pBase64Buf;
    return true;
}

const char* TinyXmlUtil::GetChildTextData(
    TiXmlElement* pEle,
    const char* pChildEleName,
    const char* pDefault )
{   
    if ( NULL == pEle ||
         !isValidString(pChildEleName) )
    {
        return pDefault;
    }

    TiXmlElement* pChildEle = pEle->FirstChildElement(pChildEleName);
    if ( NULL == pChildEle )
    {
        return pDefault;
    }

    return GetTextData(pChildEle, pDefault);
}

bool TinyXmlUtil::GetChildTextData(
    TiXmlElement* pEle, /* User Element */ 
    const char* pChildEleName, /* Will Get Data Child Element N */
    int& nValue,
    int nDefault )
{
    nValue = nDefault;
    if ( NULL == pEle ||
         !isValidString(pChildEleName) )
    {
        return false;
    }

    TiXmlElement* pChildEle = pEle->FirstChildElement(pChildEleName);
    if ( NULL == pChildEle )
    {
        return false;
    }

    return GetTextData(pChildEle, nValue, nDefault);
}

bool TinyXmlUtil::GetChildTextData(
    TiXmlElement* pEle, /* User Element */
    const char* pChildEleName, /* Will Get Data Child Element N */
    bool& bValue,
    bool bDefault )
{
    bValue = bDefault;
    if ( NULL == pEle ||
         !isValidString(pChildEleName) )
    {
        return false;
    }

    TiXmlElement* pChildEle = pEle->FirstChildElement(pChildEleName);
    if ( NULL == pChildEle )
    {
        return false;
    }

    return GetTextData(pChildEle, bValue, bDefault);
}

bool TinyXmlUtil::GetChildBinaryTextData(
    TiXmlElement* pEle, /* User Element */
    const char* pChildEleName, /* Will Get Data Child Element N */ 
    void* pValue, /* buf */ 
    size_t& nLen )
{
    if ( NULL == pEle ||
         !isValidString(pChildEleName) )
    {
        return false;
    }

    TiXmlElement* pChildEle = pEle->FirstChildElement(pChildEleName);
    if ( NULL == pChildEle )
    {
        return false;
    }

    return GetBinaryTextData(pChildEle, pValue, nLen);
}

//
// ****************** Set Text Value *********************
// 
bool TinyXmlUtil::SetTextData(
    TiXmlElement* pEle, /* User Element */
    const char* pValue )
{
    if ( NULL == pEle ||
         !isValidString(pValue) )
    {
        return false;
    }

    TiXmlNode* pNode = pEle->FirstChild();
    if ( NULL == pNode )
    {
        pEle->LinkEndChild( new TiXmlText(pValue) );
        return true;
    }

    TiXmlText* pText = pNode->ToText();
    if ( NULL == pText )
    {
        pEle->LinkEndChild( new TiXmlText(pValue) );
        return true;
    }

    pText->SetValue(pValue);
    return true;
}

bool TinyXmlUtil::SetTextData(
    TiXmlElement* pEle, /* User Element */ 
    int nValue )
{
    char szBuf[20] = {0};
    _itoa(nValue, szBuf, 10);
    return SetTextData(pEle, szBuf);
}

bool TinyXmlUtil::SetTextData( 
    TiXmlElement* pEle, /* User Element */
    bool bValue )
{
    return SetTextData(pEle, (int)bValue);
}

bool TinyXmlUtil::SetBinaryTextData(
    TiXmlElement* pEle, /* User Element */
    const void* pValue, 
    size_t nLen )
{
    if ( NULL == pEle ||
         NULL == pValue ||
         IsBadReadPtr(pValue, nLen) )
    {
        return false;
    }

    BYTE* pBase64Data = NULL;
    size_t nOutLen;
    Base64Encode(
        (const BYTE*)pValue, 
        nLen, 
        pBase64Data,
        nOutLen);
    if ( NULL == pBase64Data )
    {
        return false;
    }

    char* pBuf = new char[nOutLen+1];
    memcpy(pBuf, pBase64Data, nOutLen);
    pBuf[nOutLen] = '\0';
    delete[] pBase64Data;

    bool bRc = SetTextData(pEle, pBuf);
    delete[] pBuf;

    return bRc;
}

bool TinyXmlUtil::SetChildTextData(
    TiXmlElement* pEle, /* User Element */ 
    const char* pChildEleName, /* Will Get Data Child Element Name */ 
    const char* pValue )
{
    if ( NULL == pEle ||
        !isValidString(pValue) || 
        !isValidString(pChildEleName) )
    {
        return false;
    }

    return SetTextData(
        CreateChildEle(pEle, pChildEleName),
        pValue );
}

bool TinyXmlUtil::SetChildTextData( 
    TiXmlElement* pEle, /* User Element */ 
    const char* pChildEleName, /* Will Get Data Child Element N */ 
    int nValue )
{
    char szBuf[20] = {0};
    _itoa(nValue, szBuf, 10);
    return SetChildTextData(
        pEle, pChildEleName, szBuf );
}

bool TinyXmlUtil::SetChildTextData( 
    TiXmlElement* pEle, /* User Element */
    const char* pChildEleName, /* Will Get Data Child Element N */ 
    bool bValue )
{
    return SetChildTextData(
        pEle, pChildEleName,
        (int)bValue );
}

bool TinyXmlUtil::SetChildBinaryTextData(
    TiXmlElement* pEle, /* User Element */ 
    const char* pChildEleName, /* Will Get Data Child Element N */ 
    const void* pValue, 
    size_t nLen )
{
    if ( NULL == pEle ||
         NULL == pValue ||
         NULL == pChildEleName ||
         IsBadReadPtr(pValue, nLen) )
    {
        return false;
    }

    BYTE* pBase64Data = NULL;
    size_t nOutLen;
    Base64Encode(
        (const BYTE*)pValue, 
        nLen, 
        pBase64Data,
        nOutLen);
    if ( NULL == pBase64Data )
    {
        return false;
    }

    char* pBuf = new char[nOutLen+1];
    memcpy(pBuf, pBase64Data, nOutLen);
    pBuf[nOutLen] = '\0';
    delete[] pBase64Data;

    bool bRc = SetChildTextData(pEle, pChildEleName, pBuf);
    delete[] pBuf;
    return bRc;
}


// End of file