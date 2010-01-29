/*H***************************************************************************
 File            : TinyxmlTextUtil.h
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
#ifndef _TINYXMLTEXTUTIL_H_2010_
#define _TINYXMLTEXTUTIL_H_2010_



class TiXmlElement;
class TiXmlDocument;



namespace TinyXmlUtil
{
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

    bool SetElementTextBinaryData(
        TiXmlElement* pEle,          // User Element
        void* pValue,
        size_t nLen )
    {
        if ( NULL == pEle ||
             NULL == pValue ||
             IsBadReadPtr(pValue, nLen) )
        {
            return false;
        }
    }

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

}












#endif  // _TINYXMLTEXTUTIL_H_2010_


 // End of file



