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
    /**
    *@note { GetElement Attribute 
    */
    const char* GetAttributeData(
        TiXmlElement* pEle,          // User Element
        const char* pAttrName,
        const char* pDefault = NULL );

// look in TinyXmlUtil_VC6.h
//     template<unsigned long nSize>
//     bool GetAttributeData(
//         TiXmlElement* pEle,          // User Element
//         const char* pChildAttrName,
//         char (&szValue)[nSize],
//         const char* pDefault = NULL )

    bool GetAttributeData(
        TiXmlElement* pEle,         // User Element
        const char* pAttrName,
        int& nValue,
        int nDefault = 0 );

    bool GetAttributeData(
        TiXmlElement* pEle,         // User Element
        const char* pAttrName,
        bool& bValue,
        bool bDefault = false );

    bool GetBinaryAttributeData(
        TiXmlElement* pEle,
        const char* pAttrName,
        void* pValue, 
        size_t& nLen );

    template<typename T>
    bool GetBinaryAttributeData(
        TiXmlElement* pEle,
        const char* pAttrName,
        const T& Value )
    {
        return GetBinaryAttributeData(pEle, pAttrName, (void*)&Value, sizeof(T));
    }
    /**
    *@note Get Child Element Attribute }
    */

    /**
    *@note { Get Child Element Attribute
    */
    const char* GetChildAttributeData(
        TiXmlElement* pEle,          // User Element
        const char* pChildEleName,   // Will Get Data Child Element Name
        const char* pChildAttrName,
        const char* pDefault = NULL );

//  move to TinyXmlUtil_VC6.h
//     bool GetChildAttributeData(
//         TiXmlElement* pEle,          // User Element
//         const char* pChildEleName,   // Will Get Data Child Element Name
//         const char* pChildAttrName,
//         char (&szValue)[nSize],
//         const char* pDefault = NULL );

    bool GetChildAttributeData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element Name
        const char* pChildAttrName,
        int& nValue,
        int nDefault = 0 );

    bool GetChildAttributeData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element Name
        const char* pChildAttrName,
        bool& bValue,
        bool bDefault = false );
    /**
    *@note  Get Child Element Attribute }
    */

    /**
    *@note { Set Element Attribute 
    */   
    bool SetBinaryAttributeData(
        TiXmlElement* pEle,
        const char* pChildAttrName,
        const void* pValue, 
        size_t nLen );

    template<typename T>
    bool SetBinaryAttributeData(
        TiXmlElement* pEle,
        const char* pChildAttrName,
        const T& Value )
    {
        return SetBinaryAttributeData(pEle, pChildAttrName, (void*)&Value, sizeof(T));
    }
    /**
    *@note Set Element Attribute }
    */

    /**
    *@note { Set Element Attribute 
    *      if ChildElement not exist, Create a new. 
    */  
    bool SetChildAttributeData(
        TiXmlElement* pEle,          // User Element
        const char* pChildEleName,   // Will Get Data Child Element Name
        const char* pChildAttrName,
        const char* pValue = "");

    bool SetChildAttributeData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element Name
        const char* pChildAttrName,
        int nValue = 0 );

    bool SetChildAttributeData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element Name
        const char* pChildAttrName,
        bool bValue = false);
    /**
    *@note Set Element Attribute } 
    */ 
}


#endif  // _TINYXMLATTIUTIL_H_2010_


 // End of file



