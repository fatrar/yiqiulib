/*H***************************************************************************
 File            : TinyxmlTextUtil.h
 Subsystem       : 
 Function Name(s): TinyxmlUtil
 Author          : YiQiu
 Date            : 2010-1-29  
 Time            : 9:18
 Description     : 

 Revision        : 1.1
 History
 -------
 1.0  ������һ���ļ���ʽ����XML Text�Ĳ���
 1.1  ���ӶԶ��������ݵĶ�д�� GetElementTextBinaryData SetElementTextBinaryData

 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _TINYXMLTEXTUTIL_H_2010_
#define _TINYXMLTEXTUTIL_H_2010_


class TiXmlElement;
class TiXmlDocument;



//
// ע��������base64 ���� �ǵ����� '\0' ���⡣
// ��������ַ��������ȼǵ�+1

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

    bool GetElementTextData(
        TiXmlElement* pEle,          // User Element
        void* pValue,                // buf
        size_t& nLen );

    template<typename T>
    bool GetElementTextData(
        TiXmlElement* pEle,          // User Element
        T& Value )
    {
        size_t nLen = sizeof(T);
        return GetElementTextData(pEle, (void*)&Value, nLen);
    }

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

    bool GetChildElementTextData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element N
        void* pValue,               // buf
        size_t& nLen );

    template<typename T>
    bool GetChildElementTextData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element N
        T& Value )
    {
        size_t nLen = sizeof(T);
        return GetChildElementTextData(pEle, pChildEleName, (void*)&Value, nLen);
    }
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

    bool SetElementTextData(
        TiXmlElement* pEle,          // User Element
        const void* pValue,
        size_t nLen );

    template<typename T>
    bool SetElementTextData(
        TiXmlElement* pEle,          // User Element
        const T& Value)
    {
        return SetElementTextData(pEle, (void*)&Value, sizeof(T));
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

    bool SetChildElementTextData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element N
        const void* pValue,
        size_t nLen );

    template<typename T>
    bool SetChildElementTextData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element N
        const T& Value )
    {
        return SetChildElementTextData(pEle, pChildEleName, (void*)&Value, sizeof(T));
    }
    // }

}












#endif  // _TINYXMLTEXTUTIL_H_2010_


 // End of file



