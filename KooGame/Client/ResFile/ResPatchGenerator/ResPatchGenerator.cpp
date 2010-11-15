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


namespace ResFile
{

#define GetFileHead(f) Util::GetFileHead<File_Version_1_1>(f)
#define DestroyFileHead(h) Util::DestroyFileHead(h)


void CResPatchGenerator::Generate(
    const char* pOld,
    const char* pNew, 
    const char* pPatch,
    size_t nMaxVolumeSize )
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
    WritePatchFile(pPatch, nMaxVolumeSize);

    /**
    *@note 5. Do end
    */
    
}

void CResPatchGenerator::DataTransfrom()
{
    /**
    *@note pack
    */
    BYTE *dest = m_pPatchData+m_nPatchNow;
    size_t destLen = Res_File_Buf-m_nPatchNow;
    int nRc = LzmaUtil::LzmaCompress(
        dest, &destLen, m_pVolume, m_nVolumeNow, Compress_High);
    if ( nRc != 0 )
    {
        throw "Compress Failed!";
    }
 
    DataIndex1& Index = m_pAddIndex[m_nIndexNow++];
    Index.dwLen = destLen;
    Index.dwOffset = m_nPatchNow;
    Index.dwRawVolumeLen = m_nVolumeNow;

    m_nPatchNow += destLen;
    m_nVolumeNow = 0;
    /**
    *@note Encrypt
    */
    // ���ڲ�����һ���û��ò������Ͳ�������
}

void CResPatchGenerator::FillData(
    BYTE*& pBuf, BYTE* pData,
    const TDataIndex0& Index  )
{
    DataHead1* pDataHead = (DataHead1*)pBuf;
    pDataHead->HashValue = Index.HashValue;
    pDataHead->dwRawDataLen = Index.dwLen;
    pBuf += sizeof(DataHead1);
    memcpy(pBuf, pData, Index.dwLen);
    pBuf += Index.dwLen;
}

void CResPatchGenerator::MakePatchData(
    size_t nMaxVolumeSize )
{
#define PatchFileHeadSize(nAdd,nRemove) \
    (sizeof(TResPatchFileHead)+sizeof(UHashValue)*nRemove+sizeof(DataIndex)*nAdd)

    /**
    *@note �������մ��ļ�������Buf
    */
    size_t nPatchDataSize = Res_File_Buf;
    m_pPatchData = new BYTE[nPatchDataSize];

    /**
    *@note ��������1��ԭʼ���ݾ��Buf
    */
    m_nVolumeSize = nMaxVolumeSize ;
    m_pVolume = new BYTE[m_nVolumeSize];

    /**
    *@note ������������Buf����������ƫ�������ļ�Buf��ƫ�ƣ������մ��ļ�ʱ����
    */
    m_pAddIndex = new DataIndex1[m_NewData.size()];
    
    /**
    *@note ��ʱ����ԭʼ�������ָ��
    */
    BYTE* pRawBuf = m_pVolume;
    UnapckDataMap::iterator iter = m_NewData.begin();
    const TDataIndex0& Index = iter->first;
    size_t nDataSize = sizeof(DataHead1) + Index.dwLen;
    if ( nDataSize > nMaxVolumeSize )
    {
        // ��һ�����ݴ��ھ����ֵ����ô��ֱ�Ӵ򿪾��Ҵ���þ�
        Util::TryResetBuf(m_pVolume, m_nVolumeSize, nDataSize);
        pRawBuf = m_pVolume;
        FillData(pRawBuf, iter->second, Index);

        // pack And Encrypt
        m_nVolumeNow = nDataSize;
        DataTransfrom();
        pRawBuf = m_pVolume;
    }
    else
    {
        FillData(pRawBuf, iter->second, Index);
        m_nVolumeNow = nDataSize;
    }

    for ( ++iter; iter!= m_NewData.end(); ++iter )
    {
        const TDataIndex0& Index = iter->first;
        nDataSize = sizeof(DataHead1) + Index.dwLen;
        size_t nTmpSize = m_nVolumeNow + nDataSize;
        if ( nTmpSize > nMaxVolumeSize )
        {
            // pack And Encrypt
            DataTransfrom();

            // ��һ�����ݴ��ھ����ֵ����ô��ֱ�Ӵ򿪾��Ҵ���þ�
            Util::TryResetBuf(m_pVolume, m_nVolumeSize, nDataSize);
            pRawBuf = m_pVolume;
            FillData(pRawBuf, iter->second, Index);
            m_nVolumeNow = nTmpSize;
        }
        else
        {
            FillData(pRawBuf, iter->second, Index);
            m_nVolumeNow = nTmpSize;
        }
    }

    /**
    *@note �������һ������
    */
    if ( pRawBuf != m_pVolume )
    {
        DataTransfrom();
    }
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

void CResPatchGenerator::WritePatchFile(
    const char* pPatch,
    size_t nMaxVolumeSize )
{
    /**
    *@note 1. open Patch file 
    */
    if ( !m_PatchFile.OpenByWrite(pPatch) )
    {
        throw "Can`t Open Patch File!";
    }
 
    /**
    *@note 2. ���µ����ݴ����ļ���������Buf��������������
    */
    MakePatchData(nMaxVolumeSize);

    /**
    *@note 3. Write Base Head
    */
    TPatchFileHeadBase PatchFileHeadBase;
    PatchFileHeadBase.FormatFlag = Patch_File_Format_Flag;
    PatchFileHeadBase.Version = File_Version_1_0;
    PatchFileHeadBase.dwAddFileCount = m_NewData.size();
    PatchFileHeadBase.dwRemoveFileCount = m_Remove.size();
    PatchFileHeadBase.dwVolumeCount = m_nIndexNow;
    PatchFileHeadBase.dwMaxVolumeSize = nMaxVolumeSize;
    m_PatchFile.Write(&PatchFileHeadBase, sizeof(TPatchFileHeadBase));

    /**
    *@note 4. Write Remove Data Index(Only Hash)
    */
    UnapckDataMap::iterator RemoveIter = m_Remove.begin();
    for ( ; RemoveIter!= m_Remove.end(); ++RemoveIter )
    {
        const UHashValue& HashValue = RemoveIter->first.HashValue;
        m_PatchFile.Write(&HashValue, sizeof(UHashValue));
    }

    /**
    *@note 5. Write Add Data Index(Hash+Datasize+DataOffset)
              and Write New Add Data
    */
    m_PatchFile.Write(m_pAddIndex, sizeof(DataIndex1)*m_nIndexNow);
    safeDeleteArray(m_pAddIndex);
    m_PatchFile.Write(m_pPatchData, m_nPatchNow);
    DestroyPatchData(m_pPatchData);
}

void CResPatchGenerator::UnpackFile()
{
    if ( !Util::UpackFileData(m_OldFile, m_pOldFileHead, OnDataReadCallBack, &m_OldData) )
    {
        throw "Unpack Old File Failed!";
    }
    if ( !Util::UpackFileData(m_NewFile, m_pNewFileHead, OnDataReadCallBack, &m_NewData) )
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
    BYTE* pTmp = new BYTE[Index.dwLen];
    memcpy(pTmp, pData, Index.dwLen);
    Data[Index] = pTmp;
}

void CResPatchGenerator::Parse()
{
    /**
    *@note �����㷨Ϊ��
    *  �������ļ����������ݣ����µ�������. ���û�ҵ���
    *  ��ô�ϵ��Ǹ�����Ҫ���������浽m_Remove��.
    *  ����ҵ�����ô�ͱȽ����ݣ��������ݵĴ�СӦ��һ����Ȼ��Ƚ�ʵ������
    *  �����������Ҳ��һ���ģ�m_NewDataɾ��ͬ���ġ���һ���ͽ��ϵ��Ǹ����浽m_Reserve��
    *  ����ѭ���Ľ���ǣ�m_Remove���汣����Ҫɾ�������ݣ�m_NewData�����¼ӵ�����
    */
    UnapckDataMap::iterator OldIter = m_OldData.begin();
    for ( ; OldIter!= m_OldData.end(); ++OldIter )
    {
        const TDataIndex0& TmpKey = OldIter->first;
        UnapckDataMap::iterator NewIter = m_NewData.find(TmpKey);
        BYTE* pOldData = OldIter->second;
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
        BYTE* pNewData = NewIter->second;
        if ( memcmp(pOldData, pNewData, NewDataLen) != 0 )
        {
            m_Remove[TmpKey] = pOldData;
            continue;
        }
       
        delete[] pNewData;
        m_NewData.erase(NewIter);
    }
}

CResPatchGenerator::CResPatchGenerator()
    : m_pPatchData(NULL)
    , m_nPatchNow(0)
    , m_pVolume(NULL)
    , m_nVolumeSize(0)
    , m_nIndexNow(0)
    , m_pAddIndex(NULL)
    , m_nVolumeNow(0)
{
}

CResPatchGenerator::~CResPatchGenerator()
{
    safeDeleteArray(m_pPatchData);
    safeDeleteArray(m_pVolume);
    safeDeleteArray(m_pAddIndex);
    DestroyFileHead(m_pOldFileHead);
    DestroyFileHead(m_pNewFileHead);
    StlHelper::STLDeleteAssociate(m_OldData);
    StlHelper::STLDeleteAssociate(m_NewData);
}

}  