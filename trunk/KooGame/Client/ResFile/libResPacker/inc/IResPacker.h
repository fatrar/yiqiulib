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
*@note 资源打包者接口定义
*/
struct IResPacker
{
    /**
    *@note 添加要打包进资源包的文件，用默认打包的压缩加密参数
    *@param	pFileName 文件的短名，不是全路径。要读文件则需要将当前目录+这个
    */
    virtual void AddFile(
        const char* pFileName ) = 0;

    /**
    *@note 添加要打包进资源包的文件，能指定打包的压缩加密参数
    *@param	pFileName 同上  
    *@param cAlgo 压缩算法类型 
    *@param pcParam 压缩算法参数 
    *@param eAlgo 加密算法类型
    *@param ecParam 加密算法参数
    */
    virtual void AddFile(
        const char* pFileName,
        eCompressAlgo cAlgo,
        void* pcParam,
        eEncryptAlgo eAlgo,    
        void* peParam ) = 0;

    /**
    *@note 打包
    *@param	pPackFilePath 资源包存放路径
    *@param eFileNamePos 资源文件名的存放方式
    *@return 成功失败
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
*@note 创建资源打包对象 
*@param	pResFlodPath 资源文件目录  
*@param cAlgo   默认压缩算法类型   
*@param pcParam 默认压缩算法参数
*@param eAlgo   默认加密算法类型 
*@param ecParam 默认加密算法参数
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



