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
    *@note 设置默认加密方式  
    *@param eAlgo 加密算法类型
    *@param ecParam 加密算法参数
    */
    void SetDefaultEncryptParam(
        EncryptAlgo eAlgo,
        void* peParam = NULL ) = 0;

    /**
    *@note 设置默认压缩类型
    *@param cAlgo 压缩算法类型 
    *@param pcParam 压缩算法参数  
    */
    void SetDefaultCompressParam(
        CompressAlgo cAlgo,
        void* pcParam = NULL) = 0;

    /**
    *@note 设置资源默认父目录 
    *@param	pPath 资源文件目录  
    *@return 
    */
    void SetCurrentPath(const char* pPath);

    /**
    *@note 添加要打包进资源包的文件，用默认打包的压缩加密参数
    *@param	pFileName 文件的短名，不是全路径。要读文件则需要将当前目录+这个
    */
    void AddFile(
        const char* pFileName ) = 0;

    /**
    *@note 添加要打包进资源包的文件，能指定打包的压缩加密参数
    *@param	pFileName 同上  
    *@param cAlgo 压缩算法类型 
    *@param pcParam 压缩算法参数 
    *@param eAlgo 加密算法类型
    *@param ecParam 加密算法参数
    */
    void AddFile(
        const char* pFileName,
        CompressAlgo cAlgo,
        void* pcParam,
        EncryptAlgo eAlgo,    
        void* peParam ) = 0;

    /**
    *@note 打包
    *@param	pPackFilePath 资源包存放路径
    *@param eFileNamePos 资源文件名的存放方式
    *@return 成功失败
    */
    bool MakeFile(
        const char* pPackFilePath,
        FileNamePos eFileNamePos) = 0
};

IResPacker* CreateResPacker();
void DestroyResPacker(IResPacker*& pResPacker);

}















#endif  // _IRESPACKER_H_2010_


 // End of file



