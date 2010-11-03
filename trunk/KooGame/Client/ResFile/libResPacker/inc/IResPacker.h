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
    *@note ���Ҫ�������Դ�����ļ�����Ĭ�ϴ����ѹ�����ܲ���
    *@param	pFileName �ļ��Ķ���������ȫ·����Ҫ���ļ�����Ҫ����ǰĿ¼+���
    */
    virtual void AddFile(
        const char* pFileName ) = 0;

    /**
    *@note ���,�ú������������������쳣throw (const char*)
    *@param	pPackFilePath ��Դ�����·��
    *@param eFileNamePos ��Դ�ļ����Ĵ�ŷ�ʽ
    */
    virtual void MakeFile(
        const char* pPackFilePath,
        size_t nVolumeSize,
        bool bIsExistFileName ) = 0;
};

/**
*@note ������Դ������� 
*@param	pResFlodPath ��Դ�ļ�Ŀ¼  
*@param cAlgo   Ĭ��ѹ���㷨����   
*@param pcParam Ĭ��ѹ���㷨����
*@param eAlgo   Ĭ�ϼ����㷨���� 
*@param ecParam Ĭ�ϼ����㷨����
*/
IResPacker* CreateResPacker(
    const char* pResFlodPath, 
    eEncryptAlgo eAlgo,
    BYTE (&szKey)[8],
    eCompressAlgo cAlgo = Lzma_C_Algo );

void DestroyResPacker(IResPacker*& pResPacker);

}















#endif  // _IRESPACKER_H_2010_


 // End of file



