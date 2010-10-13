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

#define Res_Verion File_Version_1_0

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
    *@note ���Ҫ�������Դ�����ļ�����ָ�������ѹ�����ܲ���
    *@param	pFileName ͬ��  
    *@param cAlgo ѹ���㷨���� 
    *@param pcParam ѹ���㷨���� 
    *@param eAlgo �����㷨����
    *@param ecParam �����㷨����
    */
    virtual void AddFile(
        const char* pFileName,
        eCompressAlgo cAlgo,
        void* pcParam,
        eEncryptAlgo eAlgo,    
        void* peParam ) = 0;

    /**
    *@note ���
    *@param	pPackFilePath ��Դ�����·��
    *@param eFileNamePos ��Դ�ļ����Ĵ�ŷ�ʽ
    *@return �ɹ�ʧ��
    */
    virtual bool MakeFile(
        const char* pPackFilePath,
        eFileNamePos eFileNamePos) = 0;
};

enum ECompressParam
{
    Compress_Normal,
    Unpack_Fast,
    Compress_High,
};

template<DWORD Version> struct TEncryptParam;
template<DWORD Version> struct TCompressParam;

template<> class TEncryptParam<File_Version_1_0> :
    public TDataHead<File_Version_1_0>::TEncryptParam {};

template<> struct TCompressParam<File_Version_1_0>
{   
    TCompressParam(ECompressParam e = Compress_Normal):cParam(e){}
    typedef TCompressParam<File_Version_1_0> CompressParam;
    void operator = (const CompressParam& a){cParam = a.cParam;}
    ECompressParam cParam;
};

#if Res_Verion == File_Version_1_0
    static TCompressParam<File_Version_1_0> g_DefcParam;
    //TEncryptParam<File_Version_1_0> g_DefeParam;
#else
#endif

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
    eCompressAlgo cAlgo = LZMA2_C_Algo,
    void* pcParam = (void*)&g_DefcParam,
    eEncryptAlgo eAlgo = Raw_E_Algo,
    void* peParam  = NULL );

void DestroyResPacker(IResPacker*& pResPacker);

}















#endif  // _IRESPACKER_H_2010_


 // End of file



