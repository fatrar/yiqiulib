/**H**************************************************************************
 File            : CmdParser.h
 Subsystem       : 
 Function Name(s): CCmdParser
 Author          : YiQiu
 Date            : 2010-10-15  
 Time            : 18:20
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _CMDPARSER_H_2010_10
#define _CMDPARSER_H_2010_10

#include "ICommand.h"
#include <string>
#include <list>
#include <map>
using namespace std;


namespace ICommand
{

typedef void* (*Str2XXFn)(const char*);

class CCmdParser :
    public ICmdParser
{
public:
    CCmdParser(ICmdExecor* pCmdExecor);
       // : m_pDefCmdExecor(pCmdExecor){}
public:
    virtual void AddCmdString(const char* pCmdString);
    virtual bool AddParamRule(
        UINT dwParam, 
        const char* pParam,
        ValueType t = T_String );

    virtual bool ParseExec(bool bIgnoreErr = false);

protected:
    void ConvertMyType(const char* pValue, void* );

protected:
    struct ParamInfo
    {   
        UINT dwParam;
        ValueType t;
        string strValue;
    };

    typedef map<string, ParamInfo> CmdParmMap;
    typedef list<string> CmdList;
private:
    ICmdExecor* m_pCmdExecor;
    CmdParmMap m_CmdParmMap;
    CmdList m_CmdList;
    Str2XXFn m_Str2XXFn[T_Value_Type_Max];
};


}






#endif  // _CMDPARSER_H_2010_


// End of file

