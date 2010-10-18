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

void DestroyCmdParser(ICmdParser*& pCmdParser)
{
    safeDelete(pCmdParser);
}


void* Str2Bool(const char* pStr){return (void*)(pStr[0] != 0);}
void* Str2Str(const char* pStr){return (void*)pStr;}
void* Str2Int(const char* pStr){return (void*)atoi(pStr);}
void* Str2Uint(const char* pStr){return (void*)strtoul(pStr,NULL,10);}

CCmdParser::CCmdParser( ICmdExecor* pCmdExecor )
    : m_pCmdExecor(pCmdExecor)
{
    m_Str2XXFn[T_Bool] = &Str2Bool;
    m_Str2XXFn[T_String] = &Str2Str;
    m_Str2XXFn[T_Int] = &Str2Int;
    m_Str2XXFn[T_Uint] = &Str2Uint;
}

void CCmdParser::AddCmdString(
    const char* pCmdString )
{
    if ( !isValidString(pCmdString) )
    {
        return;
    }

    m_CmdList.push_back(pCmdString);
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
    CmdParmMap::iterator Mapiter;
    CmdList::iterator iter = m_CmdList.begin();
    for (; iter!= m_CmdList.end(); ++iter)
    {
        const string& strCmd = *iter;
        size_t nPos = strCmd.find('=');
        if ( nPos == string::npos || 0 == nPos )  //防止没有=或者=在第一个
        {
            if (bIgnoreErr) continue;
            string strErr = "Cmd Error at Commad -->";
            strErr += strCmd;
            throw strErr.c_str();
        }

        string strParam = strCmd.substr(0, nPos);
        //
        // [] 需要处理路径带""的异常
        //
        string strValue = strCmd.substr(nPos+1, string::npos);

        Mapiter = m_CmdParmMap.find(strParam);
        if ( Mapiter == m_CmdParmMap.end() )
        {
            if (bIgnoreErr) continue;
            string strErr = "Param Rule not find -->";
            strErr += strParam;
            throw strErr.c_str();
        }

        ParamInfo& Info = Mapiter->second;
        void* pValue = m_Str2XXFn[Info.t](strValue.c_str());
        bool bRc = m_pCmdExecor->OnSetParam(Info.dwParam, pValue);
        if ( !bRc && !bIgnoreErr )
        {
            string strErr = "Command Value is Valid, command Error at ";
            strErr += strParam;
            throw strErr.c_str();
        }
    }

    return m_pCmdExecor->Run();
}



}


// End of file