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
#include <map>
using namespace std;


namespace ICommand
{

class CCmdParser :
    public ICmdParser
{
public:
    CCmdParser(ICmdExecor* pCmdExecor);
    ~CCmdParser(void);
public:
    virtual void AddCmdString(const char* pCmdString);
    virtual bool AddParamRule(
        UINT dwParam, 
        const char* pParam,
        ValueType t = T_String, 
        ICmdExecor* pCmdExecor = NULL);

    virtual bool ParseExec(bool bIgnoreErr = false);

protected:
    struct ParamInfo
    {   
        UINT dwParam;
        ValueType t;
        ICmdExecor* pCmdExecor;
    };

    typedef map<string, ParamInfo> CmdParmMap;
private:
    ICmdExecor* m_pDefCmdExecor;
    CmdParmMap m_CmdParmMap;
};


}






#endif  // _CMDPARSER_H_2010_


// End of file

