/**H**************************************************************************
 File            : ICommand.h
 Subsystem       : 
 Function Name(s): ICommand
 Author          : YiQiu
 Date            : 2010-9-26  
 Time            : 17:22
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _ICOMMAND_H_2010_9
#define _ICOMMAND_H_2010_9


typedef unsigned int UINT;

/**
*@note 命令行程序::命令执行者
*/
struct ICmdExecor
{
    /**
    *@note  设置参数给命令执行者，默认传一个参数值
    *@param	dwParam 用户自定义的命令码  
    *@param pParam1 命令解析者解析的之后命令参数
            参数类型取决于用在AddParamRule的ValueType，他遵循下面规则
            T_Bool -> bool*   T_String -> char*
            T_Int  -> int*    T_Uint   -> unsinged int*
    *@param pParam2 意思同pParam1，不过指第二个参数，如果为NULL代表这个参数没有
    *@return 是否设置成功，这个返回值会影响ICmdParser::ParseExec的执行，
             他会以它是否忽略这些错误
    */
    virtual bool OnSetParam(
        UINT dwParam,
        const void* pParam1,
        const void* pParam2 = NULL)=0;

    /**
    *@note 执行命令，ICmdParser::ParseExec执行OnSetParam后调用
    */
    virtual bool Run();
};

/**
*@note 命令行程序::命令解析者
*/
struct ICmdParser
{
    enum ValueType
    {
        T_Bool,
        T_String,
        T_Int,
        T_Uint,
        T_Unknown,
    };

    virtual ~ICmdParser(void){};
    
    /**
    *@note 设置默认的命令执行者，AddParamRule[ex]如果pCmdExecor为空以他为默认
    *@param	pCmdExecor 命令执行者对象
    */
    virtual void SetDefaultCmdExecor(ICmdExecor* pCmdExecor)=0;

    /**
    *@note 添加命令，可以重复添加不同的命令，有时候命令以空格分隔，
           那么就会控制台截断成多个命令参数过来。这时需要设置多次。
           比如：xx.exe a=565 b=fff 控制台入口会穿两个命令参数
    *@param	pCmdExecor 命令执行者对象
    */
    virtual void AddCmdString(const char* pCmdString)=0;

    /**
    *@note 添加命令解析规则
    *@param	dwParam 用户自定义的命令码 
    *@param pParam  命令自定义字符串码  
    *@param t       命令对值的数据类型 
    *@param pCmdExecor 命令执行者对象,默认为空，如果为空就以DefaultCmdExecor替代
    *@return 是否成功，一般不错，估计设置重复或者无效规则参数
    */
    virtual bool AddParamRule(
        UINT dwParam, 
        const char* pParam,
        ValueType t = T_String, 
        ICmdExecor* pCmdExecor = NULL)=0;

    /**
    *@note 添加命令解析规则，跟AddParamRule不同的是能设置两个参数
    *@param	dwParam 用户自定义的命令码 
    *@param pParam1,pParam2 命令自定义字符串码  
    *@param t1,t2 命令对值的数据类型 
    *@param pCmdExecor 命令执行者对象,默认为空，如果为空就以DefaultCmdExecor替代
    *@return 是否成功，一般不错，估计设置重复或者无效规则参数
    */
    virtual bool AddParamRuleEx(
        UINT dwParam,
        const char* pParam1,
        const char* pParam2,
        ValueType t1 = T_String,
        ValueType t2 = T_String,
        ICmdExecor* pCmdExecor = NULL)=0;

    /**
    *@note 解析命令并执行
    *@param bIgnoreErr 是否忽略解析中的错误和调用ICmdExecor::OnSetParam的错误，
                       忽略就遇到错误就继续执行命令，否则报错
    *@return 
    */
    virtual bool ParseExec(bool bIgnoreErr = false)=0;
};










#endif  // _ICOMMAND_H_2010_


// End of file




