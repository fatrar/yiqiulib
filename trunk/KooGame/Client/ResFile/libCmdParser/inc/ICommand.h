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


namespace ICommand
{

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
            T_Bool -> bool   T_String -> char*
            T_Int  -> int    T_Uint   -> unsinged int
    *@param pParam2 ��˼ͬpParam1������ָ�ڶ������������ΪNULL�����������û��
    *@return �Ƿ����óɹ����������ֵ��Ӱ��ICmdParser::ParseExec��ִ�У�
             ���������Ƿ������Щ����
    */
    virtual bool OnSetParam(
        UINT dwParam,
        const void* pParam1,
        const void* pParam2 = NULL)=0; // �ڶ�������������ԶΪ�գ���Ԥ��֧�ֶ����

    /**
    *@note ִ�����ICmdParser::ParseExecִ��OnSetParam�����
    */
    virtual bool Run() = 0;
};

struct ICmdParserBase
{
    virtual ~ICmdParserBase(){}

    enum ValueType
    {
        T_Bool,
        T_String,
        T_Int,
        T_Uint,
        T_Value_Type_Max,
    };

    /**
    *@note �����������ظ���Ӳ�ͬ�������ʱ�������Կո�ָ���
    ��ô�ͻ����̨�ضϳɶ�����������������ʱ��Ҫ���ö�Ρ�
    ���磺xx.exe a=565 b=fff ����̨��ڻᴩ�����������
    *@param	pCmdExecor ����ִ���߶���
    */
    virtual void AddCmdString(const char* pCmdString)=0;

    void Release();
};

/**
*@note �����г���::���������
*      ���������û����ȵ���AddParamRule�����������Ȼ���ParseExec
*      �Ϳ���������������ִ�С�
*      ParseExecd�������ǣ����Ȼ�ص�������ִ���ߣ�Ȼ���ICmdExecor::Run
*      ��������ģʽ�ĺô���ִ����֪������Щ���������ˡ�
*/
struct ICmdParser :
    ICmdParserBase
{  
    /**
    *@note ��������������
    *@param	dwParam �û��Զ���������� 
    *@param pParam  �����Զ����ַ�����  
    *@param t       �����ֵ���������� 
    *@return �Ƿ�ɹ���һ�㲻�����������ظ�������Ч�������
    */
    virtual bool AddParamRule(
        UINT dwParam, 
        const char* pParam,
        ValueType t = T_String )=0;

    /**
    *@note �������������򣬸�AddParamRule��ͬ������������������
    *@param	dwParam �û��Զ���������� 
    *@param pParam1,pParam2 �����Զ����ַ�����  
    *@param t1,t2 �����ֵ���������� 
    *@param pCmdExecor ����ִ���߶���,Ĭ��Ϊ�գ����Ϊ�վ���DefaultCmdExecor���
    *@return �Ƿ�ɹ���һ�㲻�����������ظ�������Ч�������
    */
    /*  ��ʱ��֧��
    virtual bool AddParamRuleEx(
        UINT dwParam,
        const char* pParam1,
        const char* pParam2,
        ValueType t1 = T_String,
        ValueType t2 = T_String,
        ICmdExecor* pCmdExecor = NULL)=0;
    */

    /**
    *@note �������ִ��
    *@param bIgnoreErr �Ƿ���Խ����еĴ���͵���ICmdExecor::OnSetParam�Ĵ���
    ���Ծ���������ͼ���ִ��������򱨴�
    *@return 
    */
    virtual bool ParseExec(bool bIgnoreErr = false)=0;
};

/**
*@note �����г���::���������,
*      �Ǳ�����ֻ������Ľ��������ṩ�ӿڹ����ȡֵ
*      ����ģʽ�����Ǽ򵥣������и�ȱ�㣬�û�Ҫ���Լ���Ҫ�������������ѯһ�顣
*      ��֪����Щ�û������ã���Щû������
*/
struct ICmdParser2 :
    ICmdParserBase
{

    /**
    *@note �õ���Ӧ�����ֵ
    *@param pCmdName ������
    *@param t ��Ӧֵ������
    *@param pValue ����ֵ
    *@return �Ƿ��и�ֵ
    */
    virtual bool GetValue(
        const char* pCmdName, 
        ValueType t,
        void*& pValue )=0;
};

ICmdParser* CreateCmdParser(ICmdExecor* pCmdExecor);
ICmdParser2* CreateCmdParser2();


};







#endif  // _ICOMMAND_H_2010_


// End of file




