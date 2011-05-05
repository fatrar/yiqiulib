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

template<typename T>
class CCmdParserBase :
    public T
{
public:
    CCmdParserBase();
    virtual void AddCmdString(const char* pCmdString);

    // must Impl Interface
public:
    virtual void ParsePass(
        const char* pCmdName,
        const char* pCmdValue,
        bool bIgnoreErr  ) = 0;

protected:
    void Parse(bool bIgnoreErr = false);
    
protected:
    typedef list<string> CmdList;
    CmdList m_CmdList;
    Str2XXFn m_Str2XXFn[T_Value_Type_Max];
    string m_strErr;
};

struct StringCmp
{
    bool operator()(const string& a, const string& b) const
    {
        return stricmp(a.c_str(), b.c_str()) < 0;
    }
};

class CCmdParser :
    public CCmdParserBase<ICmdParser>
{
public:
    CCmdParser(ICmdExecor* pCmdExecor)
        : m_pCmdExecor(pCmdExecor){}

    // ICmdParser
public:
    virtual bool AddParamRule(
        UINT dwParam, 
        const char* pParam,
        ValueType t = T_String );

    virtual bool ParseExec(bool bIgnoreErr = false);
 
    // CCmdParserBase
protected:
    virtual void ParsePass(
        const char* pCmdName,
        const char* pCmdValue,
        bool bIgnoreErr  );

protected:
    struct ParamInfo
    {   
        UINT dwParam;
        ValueType t;
        string strValue;
    };
  
private:
    typedef map<string, ParamInfo, StringCmp> CmdParmMap;
    CmdParmMap m_CmdParmMap; 
    ICmdExecor* m_pCmdExecor;
};

class CCmdParser2 :
    public CCmdParserBase<ICmdParser2>
{
    // CCmdParserBase
public:
    virtual void ParsePass(
        const char* pCmdName,
        const char* pCmdValue,
        bool bIgnoreErr );

    // ICmdParser2
public:
    virtual void Parse(bool bIgnoreErr = false)
    {
        CCmdParserBase<ICmdParser2>::Parse(bIgnoreErr);
    }

    virtual bool GetValue(
        const char* pCmdName, 
        ValueType t,
        void*& pValue );

private:
    typedef map<string, string, StringCmp> CmdParmMap;
    CmdParmMap m_CmdParmMap;
};

}






#endif  // _CMDPARSER_H_2010_


// End of file

