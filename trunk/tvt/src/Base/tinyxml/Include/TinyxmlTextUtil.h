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
    const char* GetTextData(
        TiXmlElement* pEle,          // User Element
        const char* pDefault = NULL);

    // look in TinyXmlUtil_VC6.h
    //     template<unsigned long nSize>
    //     bool GetElementTextData(
    //         TiXmlElement* pEle,          // User Element
    //         char (&szValue)[nSize],
    //         const char* pDefault = NULL)

    bool GetTextData(
        TiXmlElement* pEle,          // User Element
        int& nValue,
        int nDefault = 0);

    bool GetTextData(
        TiXmlElement* pEle,          // User Element
        bool& bValue,
        bool bDefault = false);

    bool GetBinaryTextData(
        TiXmlElement* pEle,          // User Element
        void* pValue,                // buf
        size_t& nLen );

    template<typename T>
    bool GetBinaryTextData(
        TiXmlElement* pEle,          // User Element
        T& Value )
    {
        size_t nLen = sizeof(T);
        return GetBinaryTextData(pEle, (void*)&Value, nLen);
    }

    const char* GetChildTextData(
        TiXmlElement* pEle,          // User Element
        const char* pChildEleName,   // Will Get Data Child Element Name
        const char* pDefault = NULL);

    bool GetChildTextData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element N
        int& nValue,
        int nDefault = 0 );

    bool GetChildTextData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element N
        bool& bValue,
        bool bDefault = false );

    bool GetChildBinaryTextData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element N
        void* pValue,               // buf
        size_t& nLen );

    template<typename T>
    bool GetChildBinaryTextData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element N
        T& Value )
    {
        size_t nLen = sizeof(T);
        return GetChildBinaryTextData(pEle, pChildEleName, (void*)&Value, nLen);
    }
    // }

    // Set Text Value
    // {
    // Get pUserEle Child Element Name=pChildEleName Text
    bool SetTextData(
        TiXmlElement* pEle,          // User Element
        const char* pValue );

    bool SetTextData(
        TiXmlElement* pEle,          // User Element
        int nValue );

    bool SetTextData(
        TiXmlElement* pEle,          // User Element
        bool bValue );

    bool SetBinaryTextData(
        TiXmlElement* pEle,          // User Element
        const void* pValue,
        size_t nLen );

    template<typename T>
    bool SetBinaryTextData(
        TiXmlElement* pEle,          // User Element
        const T& Value)
    {
        return SetBinaryTextData(pEle, (void*)&Value, sizeof(T));
    }

    bool SetChildTextData(
        TiXmlElement* pEle,          // User Element
        const char* pChildEleName,   // Will Get Data Child Element Name
        const char* pValue );

    bool SetChildTextData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element N
        int nValue );

    bool SetChildTextData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element N
        bool bValue );

    bool SetChildBinaryTextData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element N
        const void* pValue,
        size_t nLen );

    template<typename T>
    bool SetChildBinaryTextData(
        TiXmlElement* pEle,         // User Element
        const char* pChildEleName,  // Will Get Data Child Element N
        const T& Value )
    {
        return SetChildBinaryTextData(pEle, pChildEleName, (void*)&Value, sizeof(T));
    }
    // }

}












#endif  // _TINYXMLTEXTUTIL_H_2010_


 // End of file



