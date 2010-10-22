/**CPP*************************************************************************
 File            : CmdParser.cpp
 Subsystem       : 
 Function Name(s): CCmdParser
 Author          : YiQiu
 Date            : 2010-10-15  
 Time            : 18:30
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
**************************************************************************cpp**/
#include "StdAfx.h"
#include "CmdParser.h"
#include "Common.h"




namespace ICommand
{

ICmdParser* CreateCmdParser(ICmdExecor* pCmdExecor)
{
    if ( NULL == pCmdExecor )
    {
        return NULL;
    }

    return new CCmdParser(pCmdExecor);
}

ICmdParser2* CreateCmdParser2()
{
    new CCmdParser2();
}


void ICmdParserBase::Release()
{
    delete this;
}
//void DestroyCmdParser(ICmdParserBase*& pCmdParser)
//{
//    safeDelete(pCmdParser);
//}

void* Str2Bool(const char* pStr){return (void*)(pStr[0] != 0);}
void* Str2Str(const char* pStr){return (void*)pStr;}
void* Str2Int(const char* pStr){return (void*)atoi(pStr);}
void* Str2Uint(const char* pStr){return (void*)strtoul(pStr,NULL,10);}

template class CCmdParserBase<ICmdParser>;
template class CCmdParserBase<ICmdParser2>;

template<typename T>
CCmdParserBase<T>::CCmdParserBase()
{
    m_Str2XXFn[T_Bool] = &Str2Bool;
    m_Str2XXFn[T_String] = &Str2Str;
    m_Str2XXFn[T_Int] = &Str2Int;
    m_Str2XXFn[T_Uint] = &Str2Uint;
}

template<typename T>
void CCmdParserBase<T>::AddCmdString(
    const char* pCmdString )
{
    if ( !isValidString(pCmdString) )
    {
        return;
    }
   
    m_CmdList.push_back(pCmdString);
}

template<typename T>
void CCmdParserBase<T>::Parse(
    bool bIgnoreErr /*= false*/ )
{
#define TryThrow(bFlag) \
    if ( (bFlag) )\
    {\
        if (bIgnoreErr) continue;\
        m_strErr = "Cmd Error at Commad -->";\
        m_strErr += strCmd;\
        throw m_strErr.c_str();\
    }
    
    CmdList::iterator iter = m_CmdList.begin();
    const string& strCmd = *iter;
    string strCmdName, strValue;
    for (; iter!= m_CmdList.end(); ++iter)
    {
        size_t nPos = strCmd.find('=');
        TryThrow ( nPos == string::npos || 0 == nPos );  //防止没有=或者=在第一个
  
        strCmdName = strCmd.substr(0, nPos);
        //
        // [] 需要处理路径带""的异常
        //
        strValue = strCmd.substr(nPos+1, string::npos);
        std::size_t nSize = strValue.size();
        TryThrow( nSize == 0 );

        if ( (strValue[0] == '\"' && strValue[nSize-1] == '\"')||
             (strValue[0] == '\'' && strValue[nSize-1] == '\'') )
        {
            strValue = strValue.substr(1, nSize-1);
            TryThrow( strValue.size() == 0 );
        }

        ParsePass(strCmdName.c_str(), strValue.c_str(), bIgnoreErr);
    }

}

bool CCmdParser::AddParamRule(
    UINT dwParam, const char* pParam, 
    ValueType t /*= T_String*/)
{
    if ( !isValidString(pParam) )
    {
        return false;
    }

    string strParam = pParam;
    CmdParmMap::iterator iter = m_CmdParmMap.find(strParam);
    if ( iter != m_CmdParmMap.end() )
    {
        return false;
    }

    ParamInfo Info;
    Info.dwParam = dwParam;
    Info.t = t;
    m_CmdParmMap[strParam] = Info;
    return true;
}

bool CCmdParser::ParseExec(
    bool bIgnoreErr /*= false*/ )
{
    Parse(bIgnoreErr);
    return m_pCmdExecor->Run();
}

void CCmdParser::ParsePass(
    const char* pCmdName, 
    const char* pCmdValue,
    bool bIgnoreErr )
{
    CmdParmMap::iterator Mapiter = m_CmdParmMap.find(pCmdName);
    if ( Mapiter == m_CmdParmMap.end() )
    {
        if (bIgnoreErr) return;
        m_strErr = "Param Rule not find -->";
        m_strErr += pCmdName;
        throw m_strErr.c_str();
    }

    ParamInfo& Info = Mapiter->second;
    void* pValue = m_Str2XXFn[Info.t](pCmdValue);
    bool bRc = m_pCmdExecor->OnSetParam(Info.dwParam, pValue);
    if ( !bRc && !bIgnoreErr )
    {
        static string strErr = "Command Value is Valid, command Error at ";
        strErr += pCmdValue;
        throw strErr.c_str();
    }
}

void CCmdParser2::ParsePass(
    const char* pCmdName,
    const char* pCmdValue,
    bool bIgnoreErr )
{
    m_CmdParmMap[pCmdName] = pCmdValue;
}

bool CCmdParser2::GetValue(
    const char* pCmdName, 
    ValueType t,
    void*& pValue )
{
    CmdParmMap::iterator Mapiter = m_CmdParmMap.find(pCmdName);
    if ( Mapiter == m_CmdParmMap.end() )
    {
        return false;
    }

    const string& strValue = Mapiter->second;
    pValue = m_Str2XXFn[t](strValue.c_str());
    return true;
}

}


// End of file