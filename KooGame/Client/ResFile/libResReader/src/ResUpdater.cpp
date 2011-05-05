/**CPP*************************************************************************
 File            : ResUpdater.cpp
 Subsystem       : 
 Function Name(s): CResUpdater
 Author          : YiQiu
 Date            : 2010-10-27  
 Time            : 17:57
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
**************************************************************************cpp**/
//#include "StdAfx.h"
#include "ResUpdater.h"
#include "ResFileUtil.h"
#include "StlHelper.h"


namespace ResFile
{

IResUpdater* CreateResUpdater( 
    BYTE* pData, size_t nSize,
    bool bAutoDel )
{
    return new CResUpdaterByPatchData(pData, nSize, bAutoDel);
}

IResUpdater* CreateResUpdater(
    const char* pPatchFilePath )
{
    return new CResUpdaterByPatchFile(pPatchFilePath);
}

/**
*@{ ******************** CResUpdater ********************
*/
CResUpdater::~CResUpdater()
{
    safeRelease(m_pResCrypto);
    Util::DestroyFileHead(m_pOldFileHead);
    DestroyPatchFileHead(m_pPatchFileHead);
}

void CResUpdater::GetOldFileHead(const char* pFilepath)
{
    m_OldResFile.OpenByRead(pFilepath);
    if ( !m_OldResFile.IsOpen() )
    {
        throw File_Can_Not_Open;
    }

    m_pOldFileHead = Util::GetFileHead<File_Version_1_0>(m_OldResFile);
    if ( NULL == m_pOldFileHead )
    {
        throw File_Format_Error;
    }

    m_pResCrypto = Util::IResCrypto::CreateResCrypto(
        (eEncryptAlgo)m_pOldFileHead->eAlgo,
        m_pOldFileHead->szKey );
}

bool CResUpdater::Update(
    const char* pFilepath )
{
    GetOldFileHead(pFilepath);

    m_pPatchFileHead = GetPatchFileHead();
    DWORD dwRemoveFileCount = m_pPatchFileHead->dwRemoveFileCount;
    DWORD dwAddFileCount = m_pPatchFileHead->dwAddFileCount;
    const DWORD& dwVolumeCount = m_pPatchFileHead->dwVolumeCount;
    UHashValue* pRemoveList = (UHashValue*)(m_pPatchFileHead + 1);
    DataIndex1* pVolumeIndex = (DataIndex1*)(pRemoveList + dwRemoveFileCount);
    
    char szNewPath[260] = {0};
    sprintf(szNewPath, "%s.bak", pFilepath);
    
    /**
    *@note �����ļ��еı��������ݴ���ļ�����������µ�����
    */
    m_NewResFile.OpenByWrite(szNewPath);
    DWORD dwFileCount = m_pOldFileHead->dwFileCount + dwAddFileCount - dwRemoveFileCount;
    bool bRc = WriteOldData(dwFileCount, pRemoveList, dwRemoveFileCount);
    if ( !bRc )
    {
        return false;
    }

    /**
    *@note ���¼ӵ�����д�����ļ�
    */
    BYTE szKey[8];
    bRc = Unpack(
        m_pPatchFileHead->dwMaxVolumeSize,
        m_pPatchFileHead->dwVolumeCount,
        Raw_E_Algo, szKey,
        pVolumeIndex);
    //bRc = !!WriteNewData(pVolumeIndex, dwVolumeCount);
    DestroyPatchFileHead(m_pPatchFileHead);
    if ( !bRc )
    {
        return false;
    }

    WriteFileHead();
    m_NewResFile.Close();

    bRc = !!FileSystem::CFile::Remove(pFilepath);
    if ( !bRc )
    {
        return false;
    }

    bRc = !!FileSystem::CFile::Rename(szNewPath, pFilepath);
    return bRc;
}

void CResUpdater::DataReadCallBack(
    DataHead1* pHead, BYTE* pData)
{
    /**
    *@note д����ͷ������
    *     []��ʱ��ѹ��ѹ������
    */
    const DWORD& dwDataLen = pHead->dwRawDataLen;
    DataHead0 Head0;
    Head0.dwRawDataLen = dwDataLen;
    Head0.nCompressAlgo = Raw_C_Algo;
    Head0.nCompressLevel = Compress_Normal;
    Head0.nIsDecrypt = 0;
    m_NewResFile.Write(&Head0, sizeof(DataHead0));

    /**
    *@note ��������
    */
    m_pResCrypto->Encrypt(pData, dwDataLen);
    m_NewResFile.Write(pData, dwDataLen);

    /**
    *@note ��д�µ��ļ�������ˢ���ļ�λ�ã���������������С�
    */
    DataIndex0 Index;
    Index.dwOffset = m_dwPosNow;
    Index.dwLen = dwDataLen + sizeof(DataHead0);
    Index.HashValue = pHead->HashValue;
    m_NewFileDataIndex.insert(Index);
    m_dwPosNow += dwDataLen + sizeof(DataHead0);
}

void CResUpdater::GetReserveDataIndexFromOldFile(
    FileHead0* OldFileHead,
    const UHashValue* pRemoveList, 
    DWORD dwRemoveFileCount )
{
    const DataIndex0* OldDataIndex = OldFileHead->DataIndex;
    DWORD dwOldFileCount = OldFileHead->dwFileCount;
    for ( DWORD i = 0, j = 0; i<dwOldFileCount; ++i )
    {
        const UHashValue& OldHash = OldDataIndex[i].HashValue;
        while (1)
        {
            if ( OldHash < pRemoveList[j] )
            {
                m_NewFileDataIndex.insert(OldDataIndex[i]);
                break;
            }
            else if ( OldHash == pRemoveList[j] )
            {
                if ( ++j < dwRemoveFileCount )
                {
                    break;
                }
                
                /**
                *@note ��OldDataIndex��һ�������һ���Ž�m_NewFileDataIndex
                */
                //StlHelper::Array2STL(
                //    (DataIndex0*)(OldDataIndex+i+1),
                //    dwOldFileCount-i-1,
                //    m_NewFileDataIndex );
                StlHelper::Array2STL(
                    (DataIndex0*)(OldDataIndex+i+1),
                    dwOldFileCount-i-1,
                    m_NewFileDataIndex );
                return;
            }
            else  // OldHash > pRemoveList[j]
            {
                if ( ++j < dwRemoveFileCount )
                {
                    continue;
                }
   
                StlHelper::Array2STL(
                    (DataIndex0*)(OldDataIndex+i),
                    dwOldFileCount-i,
                    m_NewFileDataIndex );
                //StlHelper::Array2STL(
                //    (DataIndex0*)(OldDataIndex+i+1),
                //    dwOldFileCount-i-1,
                //    m_NewFileDataIndex );
                return;     
            }
        }
    }
}

bool CResUpdater::WriteOldData(
    DWORD dwFileCount,
    const UHashValue* pRemoveList,
    DWORD dwRemoveFileCount )
{
    /**
    *@note 1. ����Ԥ��һ���ļ�ͷ
    */
    DWORD dwFileHeadSize = Util::GetFileHeadSize<File_Version_1_0>(dwFileCount);
    m_NewResFile.Seek(dwFileHeadSize);
    m_dwPosNow = dwFileHeadSize;

    /**
    *@note 2. �����ļ��б��������������Ž�m_NewFileDataIndex
    */
    GetReserveDataIndexFromOldFile(
        m_pOldFileHead, pRemoveList, dwRemoveFileCount);

    /**
    *@note ����Ҫ���������ݴ����ļ��ж��������������ļ�
    */
    bool bRc = true;
    char* pBuf = NULL;
    DWORD nBufSize = 0;
    set<DataIndex0>::iterator iter = m_NewFileDataIndex.begin();
    for (; iter!=m_NewFileDataIndex.end(); ++iter)
    {
        DataIndex0& TmpDataIndex = *iter;
        const DWORD& dwDataLen = TmpDataIndex.dwLen;
        DWORD& dwDataOffset = TmpDataIndex.dwOffset;
 
        /**
        *@note ���ϴε�buf�Ƿ�������ļ�������������newһ��
        *      ������Ƶ�Ŀ�ģ����ٶ��ڴ��new��delete�����Ч��
        */
        Util::TryResetBuf(pBuf, nBufSize, dwDataLen);

        /**
        *@note �����ļ������ݣ���д�����ļ�
        */
        m_OldResFile.Seek(dwDataOffset);
        if ( dwDataLen != m_OldResFile.Read(pBuf, dwDataLen) )
        {
           bRc = false;
           break;
        }
        m_NewResFile.Write(pBuf, dwDataLen);

        /**
        *@note ��д�µ��ļ���������ˢ���ļ�λ��
        */
        dwDataOffset = m_dwPosNow; //  �����趨���������ļ���λ��
        m_dwPosNow += dwDataLen;
    }

    safeDeleteArray(pBuf);
    m_OldResFile.Close();
    return bRc;
}


void CResUpdater::WriteFileHead()
{
    m_NewResFile.Seek(0);

    DWORD nHeadSize = Util::WriteBaseHead<File_Version_1_0>(
        m_NewResFile, (size_t)m_NewFileDataIndex.size(), false,
        m_pOldFileHead->eAlgo, m_pOldFileHead->szKey );
    {
        DWORD dwRawDataMem[TDataMemInfo::Max_Num] = {0};
        DWORD (&dwCompressDataMem)[TDataMemInfo::Max_Num] = dwRawDataMem;
        m_NewResFile.Write((void*)dwRawDataMem, sizeof(dwRawDataMem));
        m_NewResFile.Write((void*)dwCompressDataMem, sizeof(dwRawDataMem));
    }

    set<DataIndex0>::iterator iter = m_NewFileDataIndex.begin();
    for ( ; iter != m_NewFileDataIndex.end(); ++iter )
    {
        DataIndex0& Index = *iter;
        m_NewResFile.Write((void*)&Index, sizeof(DataIndex0));
    }
}

/** CResUpdater
*@ }
*/


/**
*@{ ******************** CResUpdaterByPatchData ********************
*/
CResUpdaterByPatchData::CResUpdaterByPatchData(
    BYTE* pPatchData, size_t nSize, bool bAutoDel )
    : m_bAutoDel(bAutoDel)
    , m_pPatchData(pPatchData)
    , m_nSize(nSize) {}

CResUpdaterByPatchData::~CResUpdaterByPatchData()
{
    if ( m_bAutoDel ){safeDeleteArray(m_pPatchData);}
}

DWORD CResUpdaterByPatchData::Read( 
    DWORD dwOffset, BYTE* pBuf, DWORD dwLen )
{
    BYTE* pNow = m_pPatchData + dwOffset;
    memcpy(pBuf, pNow, dwLen);
    return dwLen;
}
// bool CResUpdaterByPatchData::WriteNewData(
//     DataIndex1* pVolumeIndex,
//     DWORD dwVolumeCount )
// {
//     Unpack();
//
//     for (DWORD i=0; i<dwAddFileCount; ++i)
//     {
//         DataIndex1& TmpDataIndex = pAddDataIndex[i];
//         DWORD& dwDataOffset = TmpDataIndex.dwOffset;
//         const DWORD& dwDataLen = TmpDataIndex.dwLen;
//         
//         /**
//         *@note ȡ���ݵ�ָ�룬��д�����ļ�
//         */
//         void* pBuf = m_pPatchData + dwDataOffset;
//         m_NewResFile.Write(pBuf, dwDataLen);
// 
//         /**
//         *@note ��д�µ��ļ�������ˢ���ļ�λ�ã���������������С�
//         */
//         dwDataOffset = m_dwPosNow; //  �����趨���������ļ���λ��
//         m_dwPosNow += dwDataLen;
//         m_NewFileDataIndex.insert(TmpDataIndex);
//     }
//     return true;
// }

/** CResUpdaterByPatchData
*@ }
*/


/**
*@{ ******************** CResUpdaterByPatchFile ********************
*/
CResUpdaterByPatchFile::CResUpdaterByPatchFile(
    const char* pPatchFilePath )
{
    m_PatchFile.OpenByRead(pPatchFilePath);
}

TPatchFileHeadBase* CResUpdaterByPatchFile::GetPatchFileHead()
{
    if ( !m_PatchFile.IsOpen() )
    {
        return NULL;
    }

    TPatchFileHeadBase Head;
    m_PatchFile.Read(&Head, sizeof(Head));

    if ( Head.FormatFlag != Patch_File_Format_Flag ||
         Head.Version != File_Version_1_0 )
    {
        return NULL;
    }

#define GetPatchFileHeadSize() (sizeof(Head)+ \
    Head.dwRemoveFileCount*sizeof(UHashValue) + Head.dwAddFileCount*sizeof(DataIndex1)) 

    size_t nPatchFileHeadSize = GetPatchFileHeadSize();
    char* pBuf = new char[nPatchFileHeadSize];
    memcpy(pBuf, &Head, sizeof(Head));
    m_PatchFile.Read(pBuf+sizeof(Head), nPatchFileHeadSize-sizeof(Head));

    return (TPatchFileHeadBase*)pBuf;
}

void CResUpdaterByPatchFile::DestroyPatchFileHead( TPatchFileHeadBase*& Head )
{
    Util::DestroyFileHead(Head);
//     if ( Head )
//     {
//         delete[] (char*)Head; 
//         Head  = NULL;
//     }
}

// bool CResUpdaterByPatchFile::WriteNewData(
//     DataIndex1* pVolumeIndex,
//     DWORD dwVolumeCount )
// {
//     bool bRc = true;
//     char* pBuf = NULL;
//     size_t nBufSize = 0;
//     for (DWORD i=0; i<dwAddFileCount; ++i)
//     {
//         DataIndex0& TmpDataIndex = pAddDataIndex[i];
//         DWORD& dwDataOffset = TmpDataIndex.dwOffset;
//         const DWORD& dwDataLen = TmpDataIndex.dwLen;
//         
//         /**
//         *@note ���ϴε�buf�Ƿ�������ļ�������������newһ��
//         *      ������Ƶ�Ŀ�ģ����ٶ��ڴ��new��delete�����Ч��
//         */
//         if ( nBufSize < dwDataLen )
//         {
//             safeDeleteArray(pBuf);
//             pBuf = new char[dwDataLen];
//             nBufSize = dwDataLen;
//         }
// 
//         /**
//         *@note �����ļ������ݣ���д�����ļ�
//         */
//         m_PatchFile.Seek(dwDataOffset);
//         if ( dwDataLen != m_PatchFile.Read(pBuf, dwDataLen) )
//         {
//             bRc = false;
//             break;
//         }
//         m_NewResFile.Write(pBuf, dwDataLen);
// 
//         /**
//         *@note ��д�µ��ļ�������ˢ���ļ�λ�ã���������������С�
//         */
//         dwDataOffset = m_dwPosNow; //  �����趨���������ļ���λ��
//         m_dwPosNow += dwDataLen;
//         m_NewFileDataIndex.insert(TmpDataIndex);
//     }
//     return bRc;
// }

/** CResUpdaterByPatchFile
*@ }
*/

}





// End of file