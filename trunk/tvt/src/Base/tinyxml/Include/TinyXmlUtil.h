/*H***************************************************************************
File            : TinyXmlUtil.h
Subsystem       : 
Function Name(s): TinyXmlUtil

Description     : TinyXml�򵥵ķ�װ����
Author          : Yiqiu
Date            : 2009/1/15
Revision        : 1.1
History
1.0  �ṩTinyXmlһЩ�򵥵ķ�װ��������Щ����ΪXML�������ļ�����д��
1.1  ��XML Text��Attribute ������һ���ļ���ʽ���壬
     ��������ļ����µ�ֻ��ȡ�ڵ�ʹ����ڵ�
-------
XML like
1.              2.          3. 
<1>             <1>         <1>
  <2>             <1.1 />     <Name ID="1"/>
    <3> <3/>      <1.2 />     <Name ID="2"/>
  </2>            <1.3 />     <Name ID="3"/>
</1>            </1>        </1>
-------
Copyright (c) DOWSHU Electronica (China) Ltd.
***************************************************************************H*/
#ifndef _TINYXMLUTIL_H_2009_1_
#define _TINYXMLUTIL_H_2009_1_


#include "tinyxml.h"


// �Ժ�д����Ϊ���������CreateChildAndSetChildText���档
// ����һ��Get��Create�����������ǣ�get���child���ھͻ�ʧ�ܣ�Create��ᴴ��

namespace TinyXmlUtil
{ 
    // Create Elemnet
    //{
    // return last Child, if Fail return NULL
    TiXmlElement* CreateChildEle(
        TiXmlDocument* pDoc,
        const char* pChildEleName );

    // return last Child, if Fail return NULL
    TiXmlElement* CreateChildEle(
        TiXmlElement* pEle,
        const char* pChildEleName );

    // Create like 1, return last Child, if Fail return NULL
    TiXmlElement* __cdecl CreateChildChildEle(
        TiXmlDocument* pDoc,
        unsigned short nNum,
        ... );

    // Create like 1, return last Child, if Fail return NULL
    TiXmlElement* __cdecl CreateChildChildEle(
        TiXmlElement* pEle,
        unsigned short nNum,
        ... );

    // Create like 2, return last Child, if Fail return NULL
    TiXmlElement* __cdecl CreateChildEle(
        TiXmlDocument* pDoc,
        unsigned short nNum,
        ... );

    // Create like 2, return last Child, if Fail return NULL
    TiXmlElement* __cdecl CreateChildEle(
        TiXmlElement* pEle,
        unsigned short nNum,
        ... );

    // Create like 3, return last Child, if Fail return NULL
    TiXmlElement* CreateTwinChildEle(
        TiXmlDocument* pDoc,
        unsigned short nNum,
        const char* pChildEleName,
        const char* pAttributeName );

    // Create like 3, return last Child, if Fail return NULL
    TiXmlElement* CreateTwinChildEle(
        TiXmlElement* pEle,
        unsigned short nNum,
        const char* pChildEleName,
        const char* pAttributeName );
    // }

    // Get Element
    // {
    // Get like 1, return last Child, if Fail return NULL
    TiXmlElement* __cdecl GetChildChildEle(
        TiXmlDocument* pDoc,
        unsigned short nNum,
        ...);

    // Get like 1, return last Child, if Fail return NULL
    TiXmlElement* __cdecl GetChildChildEle(
        TiXmlElement* pEle,
        unsigned short nNum,
        ...);
    // }
};


#include "TinyxmlTextUtil.h"
#include "TinyxmlAttiUtil.h"
#include "TinyXmlUtil_VC6.h"



#endif


// End of file


