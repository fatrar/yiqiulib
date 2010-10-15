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

class CCmdParser :
    public ICmdParser
{
public:
    CCmdParser(ICmdExecor* pCmdExecor)
        : m_pDefCmdExecor(pCmdExecor){}
public:
    virtual void AddCmdString(const char* pCmdString);
    virtual bool AddParamRule(
        UINT dwParam, 
        const char* pParam,
        ValueType t = T_String, 
        ICmdExecor* pCmdExecor = NULL);

    virtual bool ParseExec(bool bIgnoreErr = false);

protected:
    void ConvertMyType(const char* pValue, void* );

protected:
    struct ParamInfo
    {   
        UINT dwParam;
        ValueType t;
        ICmdExecor* pCmdExecor;
        string strValue;
    };

    typedef map<string, ParamInfo> CmdParmMap;
    typedef list<string> CmdList;
private:
    ICmdExecor* m_pDefCmdExecor;
    CmdParmMap m_CmdParmMap;
    CmdList m_CmdList;
};


}






#endif  // _CMDPARSER_H_2010_


// End of file

