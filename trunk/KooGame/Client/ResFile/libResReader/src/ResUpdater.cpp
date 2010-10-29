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
//using namespace FileSystem;

IResUpdater* CreateResUpdater( 
//    const char* pFilepath,
    BYTE* pData, size_t nSize,
    bool bAutoDel )
{
    return new CResUpdaterByPatchData(pData, nSize, bAutoDel);
}

IResUpdater* CreateResUpdater(
//    const char* pFilepath,
    const char* pPatchFilePath )
{
    return new CResUpdaterByPatchFile(pPatchFilePath);
}

/**
*@{ ******************** CResUpdater ********************
*/

bool CResUpdater::Update(
    const char* pFilepath )
{
    m_OldResFile.OpenByRead(pFilepath);
    char szNewPath[260] = {0};
    sprintf(szNewPath, "%s.bak", pFilepath);
    m_NewResFile.OpenByWrite(szNewPath);

    if ( !m_OldResFile.IsOpen() )
    {
        return false;
    }

    FileHead* OldFileHead = Util::GetFileHead<File_Version_1_0>(m_OldResFile);
    if ( NULL == OldFileHead )
    {
        return false;
    }

    TResPatchFileHead* pPatchFileHead = GetPatchFileHead();
    DWORD dwRemoveFileCount = pPatchFileHead->dwRemoveFileCount;
    DWORD dwAddFileCount = pPatchFileHead->dwAddFileCount;
    UHashValue* pRemoveList = (UHashValue*)(pPatchFileHead + 1);
    DataIndex* pAddDataIndex = (DataIndex*)(pRemoveList + dwRemoveFileCount);

    /**
    *@note �����ļ��еı��������ݴ���ļ�����������µ�����
    */
    DWORD dwFileCount = OldFileHead->dwFileCount + dwAddFileCount - dwRemoveFileCount;
    bool bRc = WriteOldData(
        OldFileHead, dwFileCount, pRemoveList, dwRemoveFileCount);
    m_OldResFile.Close();
    Util::DestroyFileHead(OldFileHead);
    if ( !bRc )
    {
        DestroyPatchFileHead(pPatchFileHead);
        return false;
    }

    /**
    *@note ���¼ӵ�����д�����ļ�
    */
    bRc = !!WriteNewData(pAddDataIndex, dwAddFileCount);
    DestroyPatchFileHead(pPatchFileHead);
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

void CResUpdater::GetReserveDataIndexFromOldFile(
    FileHead* OldFileHead,
    const UHashValue* pRemoveList, 
    DWORD dwRemoveFileCount )
{
    const DataIndex* OldDataIndex = OldFileHead->DataIndex;
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
                
                StlHelper::Array2STL(
                    (DataIndex*)(OldDataIndex+i+1),
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
                    (DataIndex*)(OldDataIndex+i+1),
                    dwOldFileCount-i-1,
                    m_NewFileDataIndex );
                return;     
            }
        }
    }
}

bool CResUpdater::WriteOldData(
    FileHead* OldFileHead,
    DWORD dwFileCount,
    const UHashValue* pRemoveList,
    DWORD dwRemoveFileCount )
{
    /**
    *@note ����Ԥ��һ���ļ�ͷ
    */
    DWORD dwFileHeadSize = Util::GetFileHeadSize<File_Version_1_0>(dwFileCount);
    m_NewResFile.Seek(dwFileHeadSize);
    m_dwPosNow = dwFileHeadSize;

    /**
    *@note �����ļ��б��������������Ž�m_NewFileDataIndex
    */
    GetReserveDataIndexFromOldFile(
        OldFileHead, pRemoveList, dwRemoveFileCount);

    /**
    *@note ����Ҫ���������ݴ����ļ��ж��������������ļ�
    */
    bool bRc = true;
    char* pBuf = NULL;
    size_t nBufSize = 0;
    set<DataIndex>::iterator iter = m_NewFileDataIndex.begin();
    for (; iter!=m_NewFileDataIndex.end(); ++iter)
    {
        DataIndex& TmpDataIndex = *iter;
        const DWORD& dwDataLen = TmpDataIndex.Info.dwDataLen;
        DWORD& dwDataOffset = TmpDataIndex.Info.dwDataOffset;

        /**
        *@note ���ϴε�buf�Ƿ�������ļ�������������newһ��
        *      ������Ƶ�Ŀ�ģ����ٶ��ڴ��new��delete�����Ч��
        */
        if ( nBufSize < dwDataLen )
        {
           safeDeleteArray(pBuf);
           pBuf = new char[dwDataLen];
           nBufSize = dwDataLen;
        }

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

    //m_NewResFile.Flush();
    safeDeleteArray(pBuf);
    return bRc;
}


void CResUpdater::WriteFileHead()
{
    m_NewResFile.Seek(0);

    DWORD nHeadSize = Util::WriteBaseHead<File_Version_1_0>(
        m_NewResFile, m_NewFileDataIndex.size(), Not_Exist);

    typedef TDataMemInfo<File_Version_1_0> DataMemInfo;
    DWORD dwRawDataMem[DataMemInfo::Max_Num] = {0};          
    DWORD dwCompressDataMem[DataMemInfo::Max_Num] = {0};
    m_NewResFile.Write((void*)dwRawDataMem, sizeof(dwRawDataMem));
    m_NewResFile.Write((void*)dwCompressDataMem, sizeof(dwCompressDataMem));

    set<DataIndex>::iterator iter = m_NewFileDataIndex.begin();
    for ( ; iter != m_NewFileDataIndex.end(); ++iter )
    {
        DataIndex& Index = *iter;
        m_NewResFile.Write((void*)&Index, sizeof(DataIndex));
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

bool CResUpdaterByPatchData::WriteNewData(
    DataIndex* pAddDataIndex,
    DWORD dwAddFileCount )
{
    for (DWORD i=0; i<dwAddFileCount; ++i)
    {
        DataIndex& TmpDataIndex = pAddDataIndex[i];
        DWORD& dwDataOffset = TmpDataIndex.Info.dwDataOffset;
        const DWORD& dwDataLen = TmpDataIndex.Info.dwDataLen;
        
        /**
        *@note ȡ���ݵ�ָ�룬��д�����ļ�
        */
        void* pBuf = m_pPatchData + dwDataOffset;
        m_NewResFile.Write(pBuf, dwDataLen);

        /**
        *@note ��д�µ��ļ�������ˢ���ļ�λ�ã���������������С�
        */
        dwDataOffset = m_dwPosNow; //  �����趨���������ļ���λ��
        m_dwPosNow += dwDataLen;
        m_NewFileDataIndex.insert(TmpDataIndex);
    }
    //m_NewResFile.Flush();
    return true;
}

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

TResPatchFileHead* CResUpdaterByPatchFile::GetPatchFileHead()
{
    if ( !m_PatchFile.IsOpen() )
    {
        return NULL;
    }

    TResPatchFileHead Head;
    m_PatchFile.Read(&Head, sizeof(Head));

    if ( Head.FormatFlag != Patch_File_Format_Flag ||
         Head.Version != File_Version_1_0 )
    {
        return NULL;
    }

#define GetPatchFileHeadSize() (sizeof(Head)+ \
    Head.dwRemoveFileCount*sizeof(UHashValue) + Head.dwAddFileCount*sizeof(DataIndex)) 

    size_t nPatchFileHeadSize = GetPatchFileHeadSize();
    char* pBuf = new char[nPatchFileHeadSize];
    memcpy(pBuf, &Head, sizeof(Head));
    m_PatchFile.Read(pBuf+sizeof(Head), nPatchFileHeadSize-sizeof(Head));

    return (TResPatchFileHead*)pBuf;
}

void CResUpdaterByPatchFile::DestroyPatchFileHead( TResPatchFileHead*& Head )
{
    if ( Head )
    {
        delete[] (char*)Head; 
        Head  = NULL;
    }
}

bool CResUpdaterByPatchFile::WriteNewData(
    DataIndex* pAddDataIndex,
    DWORD dwAddFileCount )
{
    bool bRc = true;
    char* pBuf = NULL;
    size_t nBufSize = 0;
    for (DWORD i=0; i<dwAddFileCount; ++i)
    {
        DataIndex& TmpDataIndex = pAddDataIndex[i];
        DWORD& dwDataOffset = TmpDataIndex.Info.dwDataOffset;
        const DWORD& dwDataLen = TmpDataIndex.Info.dwDataLen;
        
        /**
        *@note ���ϴε�buf�Ƿ�������ļ�������������newһ��
        *      ������Ƶ�Ŀ�ģ����ٶ��ڴ��new��delete�����Ч��
        */
        if ( nBufSize < dwDataLen )
        {
            safeDeleteArray(pBuf);
            pBuf = new char[dwDataLen];
            nBufSize = dwDataLen;
        }

        /**
        *@note �����ļ������ݣ���д�����ļ�
        */
        m_PatchFile.Seek(dwDataOffset);
        if ( dwDataLen != m_PatchFile.Read(pBuf, dwDataLen) )
        {
            bRc = false;
            break;
        }
        m_NewResFile.Write(pBuf, dwDataLen);

        /**
        *@note ��д�µ��ļ�������ˢ���ļ�λ�ã���������������С�
        */
        dwDataOffset = m_dwPosNow; //  �����趨���������ļ���λ��
        m_dwPosNow += dwDataLen;
        m_NewFileDataIndex.insert(TmpDataIndex);
    }
    //m_NewResFile.Flush();
    safeDeleteArray(pBuf);
    return bRc;
}

/** CResUpdaterByPatchFile
*@ }
*/

}





// End of file