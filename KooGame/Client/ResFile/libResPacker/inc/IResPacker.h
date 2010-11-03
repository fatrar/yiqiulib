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
    *@note 打包,该函数如果遇到错误会抛异常throw (const char*)
    *@param	pPackFilePath 资源包存放路径
    *@param eFileNamePos 资源文件名的存放方式
    */
    virtual void MakeFile(
        const char* pPackFilePath,
        size_t nVolumeSize,
        bool bIsExistFileName ) = 0;
};

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
    eEncryptAlgo eAlgo,
    BYTE (&szKey)[8],
    eCompressAlgo cAlgo = Lzma_C_Algo );

void DestroyResPacker(IResPacker*& pResPacker);

}















#endif  // _IRESPACKER_H_2010_


 // End of file



