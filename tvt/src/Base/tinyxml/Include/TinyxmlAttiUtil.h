/*H***************************************************************************
 File            : TinyxmlAttiUtil.h
 Subsystem       : 
 Function Name(s): TinyxmlUtil
 Author          : YiQiu
 Date            : 2010-1-29  
 Time            : 9:18
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _TINYXMLATTIUTIL_H_2010_
#define _TINYXMLATTIUTIL_H_2010_


class TiXmlElement;
class TiXmlDocument;

namespace TinyXmlUtil
{
    // Get Attribute
    // {
    const char* GetElementAttributeData(
        TiXmlElement* pEle,          // User Element
        const char* pAttrName,
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
        const char* pAttrName,
        int& nValue,
        int nDefault = 0 );

    bool GetElementAttributeData(
        TiXmlElement* pEle,         // User Element
        const char* pAttrName,
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
    bool SetAttributeData(
        TiXmlElement* pEle,
        const char* pChildAttrName,
        const void* pValue, 
        size_t nLen );

    template<typename T>
    bool SetAttributeData(
        TiXmlElement* pEle,
        const char* pChildAttrName,
        const T& Value )
    {
        return SetAttributeData(pEle, pChildAttrName, (void*)&Value, sizeof(T));
    }

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
}


#endif  // _TINYXMLATTIUTIL_H_2010_


 // End of file



