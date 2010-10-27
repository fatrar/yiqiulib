/*CPP*************************************************************************
File            : ResPatchGenerator.cpp
Subsystem       : 
Function Name(s): CResPatchGenerator

Description     : 
Author          : Yiqiu
Date            : 2010/10/20
Revision        : 

History
-------


Copyright (c) Shenzhen KooGame Tech Co.,Ltd.
*************************************************************************CPP*/
#include "stdafx.h"
#include "ResPatchGenerator.h"
#include "ResFileDef.h"


namespace ResFile
{

namespace ResPatchGenerator
{
typedef TFileHead<File_Version_1_0> FileHead;
typedef FileHead::TDataIndex DataIndex;
typedef TDataInfo<File_Version_1_0> DataInfo;

FileHead* GetFileHead(FileSystem::CFile& File)
{
    TFileHeadBase HeadBase;
    FileSystem::size_t nRead = File.Read(
        &HeadBase, sizeof(TFileHeadBase));
    if ( nRead != sizeof(TFileHeadBase) )
    {
        return NULL;
    }

    if ( HeadBase.FormatFlag != Res_File_Format_Flag ||
         HeadBase.Version != File_Version_1_0 ||
         HeadBase.dwFileCount == 0 ||
         HeadBase.dwSize != Util::GetFileHeadSize<File_Version_1_0>(HeadBase.dwFileCount) )
    {
        return NULL;
    }

    char* pHead = new char[HeadBase.dwSize];
    memcpy(pHead, &HeadBase, sizeof(HeadBase));
    char* pBuf = pHead + sizeof(TFileHeadBase);
    File.Read(pBuf, HeadBase.dwSize-sizeof(TFileHeadBase));
    return (FileHead*)pHead;
}

void DestroyFileHead(FileHead*& Head)
{
    if ( Head )
    {
        delete[] ((char*)Head);
        Head = NULL;
    } 
}   

/**
*@note 将A中元素，B中没有的，放进DiffBuf
*/
void FillDiff(
    const DataIndex* A, size_t nACount,  // 需要查找的出不同的队列
    const DataIndex* B, size_t nBCount,  // 与源数据比较的数据
    DataIndex* DiffBuf, size_t& nDiffCount )
{
    /**
    *@note 大体算法：将老的在新的里面找有没有出现，没有那么就是需要移除的
    *      在具体实现上，我们利用两个比较队列都是升序排列，我们记录上一次的位置
    */
    size_t& nDiffPos = nDiffCount = 0;
    for (size_t i = 0, nPos = 0; i<nACount; ++i)
    {
        while (1)
        {
            if ( A[i] < B[nPos] )
            {
                DiffBuf[nDiffPos++] = A[i];
                break;
            }
            //else if ( A[i] == B[nPos] )
            //{
            //    ++nPos;
            //}
            //else {} //  OldDataIndex[i] > NewDataIndex[nPos] 
            
            if ( ++nPos < nBCount )
            {
                continue;
            }
            
            /**
            *@note 当老的数据全部大于新的，那么老的剩下没有比较的都是需要移除的
            */
            void* pDst = DiffBuf + nDiffPos;
            const void* pSrc = A + i;
            size_t nCopyNumber = nACount-i-1;
            memcpy(pDst, pSrc, sizeof(DataIndex)*nCopyNumber);
            nDiffPos += nCopyNumber;
            return;
        }
    }
}   

#define FillRemoveDataIndex(OldDataIndex,nOldCount,NewDataIndex,nNewCount,RemoveDataIndex,nRemoveCount) \
    FillDiff(OldDataIndex,nOldCount,NewDataIndex,nNewCount,RemoveDataIndex,nRemoveCount)

#define FillAddDataIndex(NewDataIndex,nNewCount,OldDataIndex,nOldCount,AddDataIndex,nAddCount)\
    FillDiff(NewDataIndex,nNewCount,OldDataIndex,nOldCount,AddDataIndex,nAddCount)

void MakePatchData(
    FileSystem::CFile& File, DWORD dwFileHeadSize,
    DataIndex* AddDataIndex, size_t nIndexCount,
    void*& pPatchData, size_t& nPatchDataSize )
{
    nPatchDataSize = 0;
    for (size_t i=0;i<nIndexCount;++i)
    {
        nPatchDataSize += AddDataIndex[i].Info.dwDataLen;
    }
    pPatchData = new char[nPatchDataSize];
    
    DWORD dwOffsetNow = dwFileHeadSize;
    char* pBuf = (char*)pPatchData;
    for (size_t i=0;i<nIndexCount;++i)
    {
        size_t nRead = AddDataIndex[i].Info.dwDataLen;
        DWORD& dwDataOffset = AddDataIndex[i].Info.dwDataOffset;
        File.Seek(dwDataOffset);
        if ( nRead != File.Read(pBuf, nRead) )
        {
            throw "MakePatchData() Read File Failed!";
        }
        
        /**
        *@note 偏移以后的补丁文件数据索引和文件读取Buf
        */
        dwDataOffset = dwOffsetNow;
        dwOffsetNow += nRead;  // 计算出下一个数据的位置
        pBuf += nRead;
    }
}

void DestroyPatchData(void*& pPatchData)
{
    if ( pPatchData )
    {
        delete[] ((char*)pPatchData);
        pPatchData = NULL;
    } 
}

void Generate(
    const char* pOld,
    const char* pNew, 
    const char* pPatchPath )
{
    /**
    *@note 1. Check 
    */
    if ( !isValidString(pOld) ||
         !isValidString(pNew) ||
         !isValidString(pPatchPath) )
    {
        throw "Input Param Error!";
    }

    FileSystem::CFile OldFile, NewFile, PatchFile;
    if ( !OldFile.OpenByRead(pOld) ||
         !NewFile.OpenByRead(pNew) )
    {
        throw "Can`t Open Your Compare File!";
    }

    bool bRc = false;
    FileHead* pOldFileHead = NULL, *pNewFileHead = NULL;
    DataIndex* AddDataIndex = NULL, *RemoveDataIndex = NULL;
   
    if ( NULL == (pOldFileHead=GetFileHead(OldFile)) || 
         NULL == (pNewFileHead=GetFileHead(NewFile)) )
    {
        throw "File is not correct: is Not Our Res File!";
    }

    /**
    *@note 2. 得到需要删除和添加的数据信息
    */
    const DataIndex* OldDataIndex = pOldFileHead->DataIndex;
    const DataIndex* NewDataIndex = pNewFileHead->DataIndex;
    size_t nOldCount = pOldFileHead->dwFileCount;
    size_t nNewCount = pNewFileHead->dwFileCount;
    AddDataIndex = new DataIndex[nNewCount];   // 最大将新的最加上
    RemoveDataIndex = new DataIndex[nOldCount]; // 最大将老的全部移除

    size_t nRemoveCount = 0, nAddCount = 0;
    FillRemoveDataIndex(
        OldDataIndex, nOldCount,
        NewDataIndex, nNewCount,
        RemoveDataIndex, nRemoveCount);
    FillAddDataIndex(
        NewDataIndex, nNewCount,
        OldDataIndex, nOldCount,
        AddDataIndex, nAddCount );

    /**
    *@note 3. open Patch file and Write Base Head
    */
    if ( !PatchFile.OpenByWrite(pPatchPath) )
    {
        throw "Can`t Open Patch File!";
    }
 
    TResPatchFileHead PatchFileHead;
    PatchFileHead.FormatFlag = Patch_File_Format_Flag;
    PatchFileHead.Version = File_Version_1_0;
    PatchFileHead.dwAddFileCount = nAddCount;
    PatchFileHead.dwRemoveFileCount = nRemoveCount;
    PatchFile.Write(&PatchFileHead, sizeof(TResPatchFileHead));

    /**
    *@note 4. Write Remove Data Index(Only Hash)
    */
    for ( size_t i = 0; i< nRemoveCount; ++i )
    {
        UHashValue& HashValue = RemoveDataIndex[i].HashValue;
        PatchFile.Write(&HashValue, sizeof(UHashValue));
    }

    /**
    *@note 5. 将新的数据从新文件读出当今Buf，并建立好索引
    */
#define PatchFileHeadSize(nAdd,nRemove) \
(sizeof(TResPatchFileHead)+sizeof(UHashValue)*nRemove+sizeof(DataIndex)*nAdd)

    void* pPatchData = NULL;
    size_t nPatchDataSize = 0;
    MakePatchData(
        NewFile, PatchFileHeadSize(nAddCount,nRemoveCount),
        AddDataIndex, nAddCount, 
        pPatchData, nPatchDataSize );

    /**
    *@note 6. Write Add Data Index(Hash+Datasize+DataOffset)
              and Write New Add Data
    */
    PatchFile.Write(AddDataIndex, sizeof(DataIndex)*nAddCount);
    PatchFile.Write(pPatchData, nPatchDataSize);
    DestroyPatchData(pPatchData);
    
    DestroyFileHead(pOldFileHead);
    DestroyFileHead(pNewFileHead);
    safeDeleteArray(AddDataIndex);
    safeDeleteArray(RemoveDataIndex);
}

}  // namespace ResPatchGenerator
}