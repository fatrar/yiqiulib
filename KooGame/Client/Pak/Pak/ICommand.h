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
*@note �����г���::����ִ����
*/
struct ICmdExecor
{
    /**
    *@note  ���ò���������ִ���ߣ�Ĭ�ϴ�һ������ֵ
    *@param	dwParam �û��Զ����������  
    *@param pParam1 ��������߽�����֮���������
            ��������ȡ��������AddParamRule��ValueType������ѭ�������
            T_Bool -> bool*   T_String -> char*
            T_Int  -> int*    T_Uint   -> unsinged int*
    *@param pParam2 ��˼ͬpParam1������ָ�ڶ������������ΪNULL�����������û��
    *@return �Ƿ����óɹ����������ֵ��Ӱ��ICmdParser::ParseExec��ִ�У�
             ���������Ƿ������Щ����
    */
    virtual bool OnSetParam(
        UINT dwParam,
        const void* pParam1,
        const void* pParam2 = NULL)=0;

    /**
    *@note ִ�����ICmdParser::ParseExecִ��OnSetParam�����
    */
    virtual bool Run();
};

/**
*@note �����г���::���������
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
    *@note ����Ĭ�ϵ�����ִ���ߣ�AddParamRule[ex]���pCmdExecorΪ������ΪĬ��
    *@param	pCmdExecor ����ִ���߶���
    */
    virtual void SetDefaultCmdExecor(ICmdExecor* pCmdExecor)=0;

    /**
    *@note �����������ظ���Ӳ�ͬ�������ʱ�������Կո�ָ���
           ��ô�ͻ����̨�ضϳɶ�����������������ʱ��Ҫ���ö�Ρ�
           ���磺xx.exe a=565 b=fff ����̨��ڻᴩ�����������
    *@param	pCmdExecor ����ִ���߶���
    */
    virtual void AddCmdString(const char* pCmdString)=0;

    /**
    *@note ��������������
    *@param	dwParam �û��Զ���������� 
    *@param pParam  �����Զ����ַ�����  
    *@param t       �����ֵ���������� 
    *@param pCmdExecor ����ִ���߶���,Ĭ��Ϊ�գ����Ϊ�վ���DefaultCmdExecor���
    *@return �Ƿ�ɹ���һ�㲻�����������ظ�������Ч�������
    */
    virtual bool AddParamRule(
        UINT dwParam, 
        const char* pParam,
        ValueType t = T_String, 
        ICmdExecor* pCmdExecor = NULL)=0;

    /**
    *@note �������������򣬸�AddParamRule��ͬ������������������
    *@param	dwParam �û��Զ���������� 
    *@param pParam1,pParam2 �����Զ����ַ�����  
    *@param t1,t2 �����ֵ���������� 
    *@param pCmdExecor ����ִ���߶���,Ĭ��Ϊ�գ����Ϊ�վ���DefaultCmdExecor���
    *@return �Ƿ�ɹ���һ�㲻�����������ظ�������Ч�������
    */
    virtual bool AddParamRuleEx(
        UINT dwParam,
        const char* pParam1,
        const char* pParam2,
        ValueType t1 = T_String,
        ValueType t2 = T_String,
        ICmdExecor* pCmdExecor = NULL)=0;

    /**
    *@note �������ִ��
    *@param bIgnoreErr �Ƿ���Խ����еĴ���͵���ICmdExecor::OnSetParam�Ĵ���
                       ���Ծ���������ͼ���ִ��������򱨴�
    *@return 
    */
    virtual bool ParseExec(bool bIgnoreErr = false)=0;
};










#endif  // _ICOMMAND_H_2010_


// End of file




