/**H**************************************************************************
 File            : ResFileUtil.h
 Subsystem       : 
 Function Name(s): CResFileUtil
 Author          : YiQiu
 Date            : 2010-10-28  
 Time            : 10:16
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _RESFILEUTIL_H_2010_10
#define _RESFILEUTIL_H_2010_10
#include "Common.h"
#include "ResFileDef.h"
#include "FileSystem.h"


namespace ResFile
{

namespace Util
{

template<DWORD Version>
inline size_t GetFileHeadSize(size_t nFileNum);

template<DWORD Version>
inline TFileHead<Version>* GetFileHead(
    FileSystem::CFile& File)
{
    TFileHeadBase HeadBase;
    FileSystem::size_t nRead = File.Read(
        &HeadBase, sizeof(TFileHeadBase) );
    if ( nRead != sizeof(TFileHeadBase) )
    {
        return NULL;
    }

    if ( HeadBase.FormatFlag != Res_File_Format_Flag ||
         HeadBase.Version != Version ||
         HeadBase.dwFileCount == 0  )
    {
        return NULL;
    }

    char* pHead = new char[HeadBase.dwSize];
    memcpy(pHead, &HeadBase, sizeof(HeadBase));
    char* pBuf = pHead + sizeof(TFileHeadBase);
    File.Read(pBuf, HeadBase.dwSize-sizeof(TFileHeadBase));
    return (TFileHead<Version>*)pHead;
}

ResCodeDef GetFileBaseHead(
    const char* pResFilePath,
    TFileHeadBase& HeadBase,
    FileSystem::CFile& File );


inline ResCodeDef GetFileBaseHead(
    const char* pResFilePath,
    TFileHeadBase& HeadBase )
{
    FileSystem::CFile File;
    return GetFileBaseHead(pResFilePath, HeadBase, File);
}

template<DWORD Version>
inline void DestroyFileHead(TFileHead<Version>*& Head)
{
    if ( Head )
    {
        delete[] (char*)Head;
        Head = NULL;
    }
}

template<DWORD Version>
inline size_t WriteBaseHead(
    FileSystem::CFile& File,
    size_t nFileCount,
    bool bIsExistFileName,
    DWORD eAlgo,
    BYTE (&szKey)[8] )
{
    TFileHeadBase HeadBase;
    HeadBase.dwSize = GetFileHeadSize<Version>(nFileCount);
    HeadBase.FormatFlag = Res_File_Format_Flag;
    HeadBase.Version = Version;
    HeadBase.dwFileCount = nFileCount;
    HeadBase.bIsExistFileName = bIsExistFileName;
    HeadBase.eAlgo = eAlgo;
    memcpy(HeadBase.szKey, szKey, 8);
    File.Write(&HeadBase, sizeof(TFileHeadBase));
    return HeadBase.dwSize;
}

template<DWORD Version>
inline size_t GetFileHeadSize<Version>(DWORD dwFileCount)
{
    typedef TFileHead<Version> FileHead;
    return sizeof(FileHead) + sizeof(FileHead::TDataIndex)*(dwFileCount-1);
}

typedef void (*DataReadCallBackFn)(void*,DataHead1*,BYTE*);

bool UpackFileData(
    FileSystem::CFile& File,
    const FileHead1* pHead,
    DataReadCallBackFn DataReadCallBack,
    void* pParam );

template<typename T, typename S>
inline void TryResetBuf(T* pBuf, S nBufSize, S nNewSize)
{
    if (nNewSize > nBufSize)
    {
        delete[] pBuf;
        nBufSize = nNewSize;
        pBuf = new T[nBufSize];
    }
}


} // Util



} // ResFile







#endif  // _RESFILEUTIL_H_2010_


 // End of file



