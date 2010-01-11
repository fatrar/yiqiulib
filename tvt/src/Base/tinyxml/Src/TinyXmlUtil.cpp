/*CPP*************************************************************************
File            : TinyXmlUtil.cpp
Subsystem       : 
Function Name(s): CTinyXmlUtil

Description     : 
Author          : Yiqiu
Date            : 2009/1/15
Revision        : 

History
-------


Copyright (c) DOWSHU Electronica (China) Ltd.
*************************************************************************CPP*/
#include "StdAfx.h"
#include "Tinyxml.h"
#include "TinyXmlUtil.h"
#include "..\..\Base\Include\Common.h"
//#include <commondefine.h>

TiXmlElement* TinyXmlUtil::CreateChildEle(
    TiXmlDocument* pDoc,
    const char* pChildEleName )
{
    if ( NULL == pDoc ||
         !isValidString(pChildEleName) )
    {
        return NULL;
    }

    TiXmlElement* pDocRootEle = pDoc->FirstChildElement(pChildEleName);
    if ( NULL == pDocRootEle )
    {
        pDocRootEle = new TiXmlElement(pChildEleName);
        pDoc->LinkEndChild(pDocRootEle);
    }

    return pDocRootEle;
}

TiXmlElement* TinyXmlUtil::CreateChildEle(
    TiXmlElement* pEle,
    const char* pChildEleName )
{
    if ( NULL == pEle ||
         !isValidString(pChildEleName) )
    {
        return NULL;
    }

    TiXmlElement* pDocRootEle = pEle->FirstChildElement(pChildEleName);
    if ( NULL == pDocRootEle )
    {
        pDocRootEle = new TiXmlElement(pChildEleName);
        pEle->LinkEndChild(pDocRootEle);
    }

    return pDocRootEle;
}

TiXmlElement* TinyXmlUtil::CreateChildChildEle(
    TiXmlDocument* pDoc,
    u_short nNum, ... )
{
    if ( NULL == pDoc ||
         0 == nNum )
    {
        return NULL;
    }

    TiXmlElement* pTmpEle = NULL;
    va_list args;
    va_start(args, nNum);
    for (u_short i = 0; i<nNum; ++i)
    {
        const char* pChildEleName = va_arg(args, const char*);
        if ( i == 0 )
        {
            pTmpEle = CreateChildEle(pDoc, pChildEleName);
        }
        else
        {
            pTmpEle = CreateChildEle(pTmpEle, pChildEleName);
        }

        if ( NULL == pTmpEle)
        {
            break;
        }
    }
    va_end(args);

    return pTmpEle;
}

TiXmlElement* TinyXmlUtil::CreateChildChildEle(
    TiXmlElement* pEle,
    u_short nNum, ... )
{
    if ( NULL == pEle ||
         0 == nNum )
    {
        return NULL;
    }

    TiXmlElement* pTmpEle = pEle;
    va_list args;
    va_start(args, nNum);
    for (u_short i = 0; i<nNum; ++i)
    {
        const char* pChildEleName = va_arg(args, const char*);
        pTmpEle = CreateChildEle(pTmpEle, pChildEleName); 
        if ( NULL == pTmpEle )
        {
            break;
        }
    }
    va_end(args);

    return pTmpEle;
}

TiXmlElement* TinyXmlUtil::CreateChildEle(
    TiXmlDocument* pDoc,
    unsigned short nNum, ... )
{
    if ( NULL == pDoc ||
        0 == nNum )
    {
        return NULL;
    }

    TiXmlElement* pTmpEle = NULL;
    va_list args;
    va_start(args, nNum);
    for (u_short i = 0; i<nNum; ++i)
    {
        const char* pChildEleName = va_arg(args, const char*);
        pTmpEle = CreateChildEle(pDoc, pChildEleName);
        if ( NULL == pTmpEle)
        {
            break;
        }
    }
    va_end(args);

    return pTmpEle;
}

TiXmlElement* TinyXmlUtil::CreateChildEle(
    TiXmlElement* pEle,
    unsigned short nNum, ... )
{
    if ( NULL == pEle ||
         0 == nNum )
    {
        return NULL;
    }

    TiXmlElement* pTmpEle = pEle;
    va_list args;
    va_start(args, nNum);
    for (u_short i = 0; i<nNum; ++i)
    {
        const char* pChildEleName = va_arg(args, const char*);
        pTmpEle = CreateChildEle(pEle, pChildEleName); 
        if ( NULL == pTmpEle )
        {
            break;
        }
    }
    va_end(args);

    return pTmpEle;
}

TiXmlElement* TinyXmlUtil::CreateTwinChildEle(
    TiXmlDocument* pDoc,
    unsigned short nNum,
    const char* pChildEleName,
    const char* pAttributeName )
{
    if ( NULL == pDoc ||
         0 == nNum ||
         !isValidString(pChildEleName) )
    {
        return NULL;
    }

    TiXmlElement* pTmpEle = NULL;
    for (u_short i=0; i< nNum; ++i)
    {
        bool bFound = false;
        for ( pTmpEle = pDoc->FirstChildElement(pChildEleName);
              pTmpEle!= NULL;
              pTmpEle = pTmpEle->NextSiblingElement(pChildEleName) )
        {
            int j = -1;
            pTmpEle->Attribute(pAttributeName, &j);
            if ( j == i )  // if Found break, goto next i
            {
                bFound = true;
                break;
            }
        }

        if ( bFound )
        {
            continue;
        }
       
        pTmpEle = new TiXmlElement(pChildEleName);
        pTmpEle->SetAttribute(pAttributeName, i);
        pDoc->LinkEndChild(pTmpEle);
    }

    return pTmpEle;
}

TiXmlElement* TinyXmlUtil::CreateTwinChildEle(
    TiXmlElement* pEle,
    unsigned short nNum,
    const char* pChildEleName,
    const char* pAttributeName )
{
    if ( NULL == pEle ||
        0 == nNum ||
        !isValidString(pChildEleName) )
    {
        return NULL;
    }

    TiXmlElement* pTmpEle = NULL;
    for (u_short i=0; i< nNum; ++i)
    {
        bool bFound = false;
        for ( pTmpEle = pEle->FirstChildElement(pChildEleName);
            pTmpEle!= NULL;
            pTmpEle = pTmpEle->NextSiblingElement(pChildEleName) )
        {
            int j = -1;
            const char* pTmp = pTmpEle->Attribute(pAttributeName, &j);
            if ( j == i && pTmp != NULL)  // if Found break, goto next i
            {
                bFound = true;
                break;
            }
        }

        if ( bFound )
        {
            continue;
        }

        pTmpEle = new TiXmlElement(pChildEleName);
        pTmpEle->SetAttribute(pAttributeName, i);
        pEle->LinkEndChild(pTmpEle);
    }

    return pTmpEle;
}

//
// ***************** Get Element ******************
//
TiXmlElement* TinyXmlUtil::GetChildChildEle( 
    TiXmlDocument* pDoc,
    unsigned short nNum, ... )
{
    if ( NULL == pDoc ||
         0 == nNum )
    {
        return NULL;
    }

    TiXmlElement* pTmpEle = NULL;
    va_list args;
    va_start(args, nNum);
    for (u_short i = 0; i<nNum; ++i)
    {
        const char* pChildEleName = va_arg(args, const char*);
        if ( i == 0 )
        {
             pTmpEle = pDoc->FirstChildElement(pChildEleName); 
        }
        else
        {
            pTmpEle = pTmpEle->FirstChildElement(pChildEleName);
        }
       
        if ( NULL == pTmpEle )
        {
            break;
        }
    }
    va_end(args);

    return pTmpEle;
}

TiXmlElement* TinyXmlUtil::GetChildChildEle(
    TiXmlElement* pEle,
    unsigned short nNum, ... )
{
    if ( NULL == pEle ||
         0 == nNum )
    {
        return NULL;
    }

    TiXmlElement* pTmpEle = pEle;
    va_list args;
    va_start(args, nNum);
    for (u_short i = 0; i<nNum; ++i)
    {
        const char* pChildEleName = va_arg(args, const char*);
        pTmpEle = pTmpEle->FirstChildElement(pChildEleName); 
        if ( NULL == pTmpEle )
        {
            break;
        }
    }
    va_end(args);

    return pTmpEle;
}

//
// ******************** Get Text Value *********************
//
const char* TinyXmlUtil::GetElementTextData(
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

bool TinyXmlUtil::GetElementTextData(
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

bool TinyXmlUtil::GetElementTextData(
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

const char* TinyXmlUtil::GetChildElementTextData(
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

    return GetElementTextData(pChildEle, pDefault);
}

bool TinyXmlUtil::GetChildElementTextData(
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

    return GetElementTextData(pChildEle, nValue, nDefault);
}

bool TinyXmlUtil::GetChildElementTextData(
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

    return GetElementTextData(pChildEle, bValue, bDefault);
}


//
// ****************** Set Text Value *********************
// 
bool TinyXmlUtil::SetElementTextData(
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

bool TinyXmlUtil::SetElementTextData(
    TiXmlElement* pEle, /* User Element */ 
    int nValue )
{
    char szBuf[20] = {0};
    _itoa(nValue, szBuf, 10);
    return SetElementTextData(pEle, szBuf);
}

bool TinyXmlUtil::SetElementTextData( 
    TiXmlElement* pEle, /* User Element */
    bool bValue )
{
    return SetElementTextData(pEle, (int)bValue);
}

bool TinyXmlUtil::SetChildElementTextData(
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

    return SetElementTextData(
        pEle->FirstChildElement(pChildEleName),
        pValue );
}

bool TinyXmlUtil::SetChildElementTextData( 
    TiXmlElement* pEle, /* User Element */ 
    const char* pChildEleName, /* Will Get Data Child Element N */ 
    int nValue )
{
    char szBuf[20] = {0};
    _itoa(nValue, szBuf, 10);
    return SetChildElementTextData(
        pEle,
        pChildEleName,
        szBuf );
}

bool TinyXmlUtil::SetChildElementTextData( 
    TiXmlElement* pEle, /* User Element */
    const char* pChildEleName, /* Will Get Data Child Element N */ 
    bool bValue )
{
    return SetChildElementTextData(
        pEle,
        pChildEleName,
        (int)bValue );
}

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


//
//TiXmlElement* TinyXmlUtil::GetSpecifyElement(
//    const char* pUserName)
//{
//    if ( NULL == pUserName || 0 == strlen(pUserName)  )
//    {
//        return NULL;
//    }
//
//    TiXmlElement* pTmpEle = NULL;
//    for ( pTmpEle = pFatherEle->FirstChildElement(c_szUser);
//          pTmpEle != NULL;
//          pTmpEle = pTmpEle->NextSiblingElement(c_szUser) )
//    {
//        const char* pUser = GetChildElementData(pTmpEle, c_szUserName);
//        if ( NULL == pUser ||
//             0 == strlen(pUser) )
//        {
//            // Log 
//            continue;
//        }
//
//       if ( strDecryptUserName == string(pUser) )
//       {
//           pUserEle = pTmpEle;
//           return USERMGR_NO_ERR;
//       }
//    }
//
//    return USERMGR_OTHER_ERR;
//}
//
//
//
//
//TinyXmlUtil::GetSpecifyElement()
//{
//
//}




// End of file





