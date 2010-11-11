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
#include "ResFileUtil.h"
#include "StlHelper.h"

namespace ResFile
{


#define GetFileHead(f) Util::GetFileHead<File_Version_1_1>(f)
#define DestroyFileHead(h) Util::DestroyFileHead<File_Version_1_1>(h)

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

void CResPatchGenerator::Generate(
    const char* pOld,
    const char* pNew, 
    const char* pPatch )
{
    /**
    *@note 1. Check
    */
    Check(pOld, pNew, pPatch);

    /**
    *@note 2. Unpack Compare File
    */
    UnpackFile();

    /**
    *@note 3. Parse, Get Reserse Data And new Add Data Info
    */
    Parse();

    /**
    *@note 4. Write Patch File
    */
    WritePatchFile(pPatch);

    /**
    *@note 5. Do end
    */
    
}

void CResPatchGenerator::Check( 
    const char* pOld,
    const char* pNew, 
    const char* pPatchPath )
{
    /**
    *@note 1. Input param
    */
    if ( !isValidString(pOld) ||
         !isValidString(pNew) ||
         !isValidString(pPatchPath) )
    {
        throw "Input Param Error!";
    }

    /**
    *@note 2. File Can Open? 
    */
    
    if ( !m_OldFile.OpenByRead(pOld) ||
         !m_NewFile.OpenByRead(pNew) )
    {
        throw "Can`t Open Your Compare File!";
    }
   
    /**
    *@note 3. File Format Correct? 
    */
    if ( NULL == (m_pOldFileHead=GetFileHead(m_OldFile)) || 
         NULL == (m_pNewFileHead=GetFileHead(m_NewFile)) )
    {
        throw "File is not correct, Is Not Our Res File Format!";
    }
}

void CResPatchGenerator::WritePatchFile(const char* pPatch)
{
    /**
    *@note 1. open Patch file 
    */
    if ( !m_PatchFile.OpenByWrite(pPatch) )
    {
        throw "Can`t Open Patch File!";
    }
 
    /**
    *@note 2. Write Base Head
    */
    TPatchFileHeadBase PatchFileHeadBase;
    PatchFileHeadBase.FormatFlag = Patch_File_Format_Flag;
    PatchFileHeadBase.Version = File_Version_1_0;
    PatchFileHeadBase.dwAddFileCount = m_NewData.size();
    PatchFileHeadBase.dwRemoveFileCount = m_Remove.size();
    m_PatchFile.Write(&PatchFileHeadBase, sizeof(TPatchFileHeadBase));

    /**
    *@note 3. Write Remove Data Index(Only Hash)
    */
    UnapckDataMap::iterator RemoveIter = m_Remove.begin();
    for ( ; RemoveIter!= m_Remove.end(); ++RemoveIter )
    {
        UHashValue& HashValue = RemoveIter->first.HashValue;
        m_PatchFile.Write(&HashValue, sizeof(UHashValue));
    }

    /**
    *@note 4. 将新的数据从新文件读出当今Buf，并建立好索引
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
    *@note 5. Write Add Data Index(Hash+Datasize+DataOffset)
              and Write New Add Data
    */
    m_PatchFile.Write(AddDataIndex, sizeof(DataIndex)*nAddCount);
    m_PatchFile.Write(pPatchData, nPatchDataSize);
    DestroyPatchData(pPatchData);
}

void CResPatchGenerator::UnpackFile()
{
    if ( !UpackFileData(m_OldFile, m_pOldFileHead, OnDataReadCallBack, &m_OldData) )
    {
        throw "Unpack Old File Failed!";
    }
    if ( !UpackFileData(m_NewFile, m_pNewFileHead, OnDataReadCallBack, &m_NewData) )
    {
        throw "Unpack New File Failed!";
    }
}

void CResPatchGenerator::OnDataReadCallBack(
    void* pParam,
    DataHead1* pHead,
    BYTE* pData )
{
    UnapckDataMap& Data = *(UnapckDataMap*)pParam;
    TDataIndex0 Index;
    Index.dwLen = pHead->dwRawDataLen;
    Index.HashValue = pHead->HashValue;
    char* pTmp = new char[Index.dwLen];
    memcpy(pTmp, pData, Index.dwLen);
    Data[Index] = pTmp;
}

void CResPatchGenerator::Parse()
{
    /**
    *@note 具体算法为：
    *  遍历老文件的所有数据，在新的里面找. 如果没找到，
    *  那么老的那个是需要丢弃，保存到m_Remove中.
    *  如果找到，那么就比较数据，首先数据的大小应该一样，然后比较实际数据
    *  如果发现数据也是一样的，m_NewData删除同样的。不一样就将老的那个保存到m_Reserve中
    *  这样循环的结果是：m_Remove里面保存了要删除的数据，m_NewData保存新加的数据
    */
    UnapckDataMap::iterator OldIter = m_OldData.begin();
    for ( ; OldIter!= m_OldData.end(); ++OldIter )
    {
        const TDataIndex0& TmpKey = OldIter->first;
        UnapckDataMap::iterator NewIter = m_NewData.find(TmpKey);
        if ( NewIter == m_NewData.end() )
        {
            m_Remove[TmpKey] = pOldData;
            continue;
        }

        const DWORD& NewDataLen = NewIter->first.dwLen;
        if ( TmpKey.dwLen != NewDataLen )
        {
            m_Remove[TmpKey] = pOldData;
            continue;
        }

        char* pOldData = OldIter->second;
        char* pNewData = NewIter->second;
        if ( memcmp(pOldData, pNewData, NewDataLen) != 0 )
        {
            m_Remove[TmpKey] = pOldData;
            continue;
        }
       
        delete[] pNewData;
        m_NewData.erase(NewIter);
    }
}

CResPatchGenerator::~CResPatchGenerator()
{
    DestroyFileHead(pOldFileHead);
    DestroyFileHead(pNewFileHead);
    StlHelper::STLDeleteAssociate(m_OldData);
    StlHelper::STLDeleteAssociate(m_NewData);
}

}