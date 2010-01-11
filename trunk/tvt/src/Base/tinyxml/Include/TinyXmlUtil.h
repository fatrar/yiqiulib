/*H***************************************************************************
File            : TinyXmlUtil.h
Subsystem       : 
Function Name(s): CTinyXmlUtil

Description     : 
Author          : Yiqiu
Date            : 2009/1/15
Revision        : 

History
-------
XML like
1.              2.          3. 
<1>             <1>         <1>
  <2>             <1.1 />     <Name ID="1"/>
    <3> <3/>      <1.2 />     <Name ID="2"/>
  </2>            <1.3 />     <Name ID="3"/>
</1>            </1>        </1>
Copyright (c) DOWSHU Electronica (China) Ltd.
***************************************************************************H*/
#ifndef _TINYXMLUTIL_H_2009_1_
#define _TINYXMLUTIL_H_2009_1_

class TiXmlElement;
class TiXmlDocument;

#ifdef TINYXML_EXPORTS
//    #define TINYXML_API __declspec(dllexport)
#else
    //#define TINYXML_API __declspec(dllimport)
    #ifndef TINYXML_LINK
        #define TINYXML_LINK
        #ifdef _DEBUG
            #pragma comment(lib, "tinyxml_debug.lib")
            #pragma message("Automatically linking with tinyxml_debug.lib") 
        #else
            #pragma comment(lib, "tinyxml.lib")
            #pragma message("Automatically linking with tinyxml.lib") 
        #endif // _DEBUG
    #endif
#endif


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

    // Get Text Value
    // {
    // Get pUserEle Child Element Name=pChildEleName Text
    const char* GetElementTextData(
        TiXmlElement* pEle,          // User Element
        const char* pDefault = NULL);
	
// look in TinyXmlUtil_VC6.h
//     template<unsigned long nSize>
//     bool GetElementTextData(
//         TiXmlElement* pEle,          // User Element
//         char (&szValue)[nSize],
//         const char* pDefault = NULL)

    bool GetElementTextData(
        TiXmlElement* pEle,          // User Element
        int& nValue,
        int nDefault = 0);

    bool GetElementTextData(
        TiXmlElement* pEle,          // User Element
        bool& bValue,
        bool bDefault = false);

    const char* GetChildElementTextData(
        TiXmlElement* pEle,          // User Element
        const char* pChildEleName,   // Will Get Data Child Element Name
        const char* pDefault = NULL);

    bool GetChildElementTextData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element N
        int& nValue,
        int nDefault = 0 );

    bool GetChildElementTextData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element N
        bool& bValue,
        bool bDefault = false );
    // }

    // Set Text Value
    // {
    // Get pUserEle Child Element Name=pChildEleName Text
    bool SetElementTextData(
        TiXmlElement* pEle,          // User Element
         const char* pValue );

    bool SetElementTextData(
        TiXmlElement* pEle,          // User Element
        int nValue );

    bool SetElementTextData(
        TiXmlElement* pEle,          // User Element
        bool bValue );

    bool SetChildElementTextData(
        TiXmlElement* pEle,          // User Element
        const char* pChildEleName,   // Will Get Data Child Element Name
        const char* pValue );

    bool SetChildElementTextData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element N
        int nValue );

    bool SetChildElementTextData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element N
        bool bValue );
    // }

    // Get Attribute
    // {
    const char* GetElementAttributeData(
        TiXmlElement* pEle,          // User Element
        const char* pChildAttrName,
        const char* pDefault = NULL );

// look in TinyXmlUtil_VC6.h
//     template<unsigned long nSize>
//     bool GetElementAttributeData(
//         TiXmlElement* pEle,          // User Element
//         const char* pChildAttrName,
//         char (&szValue)[nSize],
//         const char* pDefault = NULL )

    bool GetElementAttributeData(
        TiXmlElement* pEle,         // User Element
        const char* pChildAttrName,
        int& nValue,
        int nDefault = 0 );

    bool GetElementAttributeData(
        TiXmlElement* pEle,         // User Element
        const char* pChildAttrName,
        bool& bValue,
        bool bDefault = false );

    const char* GetChildElementAttributeData(
        TiXmlElement* pEle,          // User Element
        const char* pChildEleName,   // Will Get Data Child Element Name
        const char* pChildAttrName,
        const char* pDefault = NULL );

//  look in  TinyXmlUtil_VC6.h
//     bool GetChildElementAttributeData(
//         TiXmlElement* pEle,          // User Element
//         const char* pChildEleName,   // Will Get Data Child Element Name
//         const char* pChildAttrName,
//         char (&szValue)[nSize],
//         const char* pDefault = NULL );

    bool GetChildElementAttributeData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element Name
        const char* pChildAttrName,
        int& nValue,
        int nDefault = 0 );

    bool GetChildElementAttributeData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element Name
        const char* pChildAttrName,
        bool& bValue,
        bool bDefault = false );
    // }

    // Set Attribute ( if ChildElement not exist, Create a new.) 
    // {  
    bool SetChildElementAttributeData(
        TiXmlElement* pEle,          // User Element
        const char* pChildEleName,   // Will Get Data Child Element Name
        const char* pChildAttrName,
        const char* pValue = "");

    bool SetChildElementAttributeData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element Name
        const char* pChildAttrName,
        int nValue = 0 );

    bool SetChildElementAttributeData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element Name
        const char* pChildAttrName,
        bool bValue = false);
    // }

   /* TiXmlElement* GetSpecifyElement(
        const char* pElementName );

    __cdecl TiXmlElement* GetSpecifyElement(
        u_short nNum,
        ... );*/
};



#include "TinyXmlUtil_VC6.h"



#endif


// End of file


