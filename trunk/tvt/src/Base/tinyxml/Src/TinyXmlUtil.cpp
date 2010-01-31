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





