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
#include "..\..\Common\inc\Common.h"

namespace ICommand
{

void* Str2Bool(const char* pStr)
{
    return (void*)(pStr[0] != 0);
}

void* Str2Str(const char* pStr){return (void*)pStr;}
void* Str2Int(const char* pStr)
{
    return (void*)atoi(pStr);
}

void* Str2Uint(const char* pStr)
{
    return (void*)strtoul(pStr,NULL,10);
  //   _Value = strtoul(strTmp.c_str(), NULL, 10);
}


void CCmdParser::AddCmdString(
    const char* pCmdString )
{
    if ( isValidString(pCmdString) )
    {
        return;
    }

    m_CmdList.push_back(pCmdString);
}

bool CCmdParser::AddParamRule(
    UINT dwParam, const char* pParam, 
    ValueType t /*= T_String*/, 
    ICmdExecor* pCmdExecor /*= NULL*/ )
{
    if ( isValidString(pParam) )
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
    Info.pCmdExecor = pCmdExecor == NULL ? m_pDefCmdExecor : pCmdExecor;
    m_CmdParmMap[strParam] = Info;
}

bool CCmdParser::ParseExec(
    bool bIgnoreErr /*= false*/ )
{
    CmdParmMap::iterator Mapiter;
    CmdList::iterator iter = m_CmdList.begin();
    for (; iter!= m_CmdList.end(); ++iter)
    {
        string& strCmd = *iter;
        size_t nPos = strCmd.find('=');
        if ( nPos == string::npos )
        {
            if (bIgnoreErr) continue;
            string strErr = "Cmd Error at Commad -->";
            strErr += strCmd;
            throw strErr.c_str();
        }

        string strParam = strCmd.substr(nPos);
        string strValue = strCmd.substr(nPos+1, string::npos);

        Mapiter = m_CmdParmMap.find(strParam);
        if ( Mapiter != m_CmdParmMap.end() )
        {
            if (bIgnoreErr) continue;
            string strErr = "Param Rule not find -->";
            strErr += strParam;
            throw strErr.c_str();
        }

        ParamInfo& Info = Mapiter->second;
        if ( 1 )
        {
        }
        //Mapiter->second.strValue = strValue;
    
    }
}

}


// End of file