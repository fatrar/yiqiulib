/**H**************************************************************************
 File            : IResPacker.h
 Subsystem       : 
 Function Name(s): IResPacker
 Author          : YiQiu
 Date            : 2010-10-12  
 Time            : 15:50
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _IRESPACKER_H_2010_10
#define _IRESPACKER_H_2010_10
#include "ResFileDef.h"

namespace ResFile
{

/**
*@note ��Դ����߽ӿڶ���
*/
struct IResPacker
{
    /**
    *@note ����Ĭ�ϼ��ܷ�ʽ  
    *@param eAlgo �����㷨����
    *@param ecParam �����㷨����
    */
    virtual void SetDefaultEncryptParam(
        EncryptAlgo eAlgo,
        void* peParam = NULL ) = 0;

    /**
    *@note ����Ĭ��ѹ������
    *@param cAlgo ѹ���㷨���� 
    *@param pcParam ѹ���㷨����  
    */
    virtual void SetDefaultCompressParam(
        CompressAlgo cAlgo,
        void* pcParam = NULL) = 0;

    /**
    *@note ������ԴĬ�ϸ�Ŀ¼ 
    *@param	pPath ��Դ�ļ�Ŀ¼  
    *@return 
    */
    virtual void SetCurrentPath(const char* pPath);

    /**
    *@note ���Ҫ�������Դ�����ļ�����Ĭ�ϴ����ѹ�����ܲ���
    *@param	pFileName �ļ��Ķ���������ȫ·����Ҫ���ļ�����Ҫ����ǰĿ¼+���
    */
    virtual void AddFile(
        const char* pFileName ) = 0;

    /**
    *@note ���Ҫ�������Դ�����ļ�����ָ�������ѹ�����ܲ���
    *@param	pFileName ͬ��  
    *@param cAlgo ѹ���㷨���� 
    *@param pcParam ѹ���㷨���� 
    *@param eAlgo �����㷨����
    *@param ecParam �����㷨����
    */
    virtual void AddFile(
        const char* pFileName,
        CompressAlgo cAlgo,
        void* pcParam,
        EncryptAlgo eAlgo,    
        void* peParam ) = 0;

    /**
    *@note ���
    *@param	pPackFilePath ��Դ�����·��
    *@param eFileNamePos ��Դ�ļ����Ĵ�ŷ�ʽ
    *@return �ɹ�ʧ��
    */
    virtual bool MakeFile(
        const char* pPackFilePath,
        FileNamePos eFileNamePos) = 0;
};

IResPacker* CreateResPacker();
void DestroyResPacker(IResPacker*& pResPacker);

}















#endif  // _IRESPACKER_H_2010_


 // End of file



