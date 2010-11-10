/**CPP*************************************************************************
 File            : ResPacker_1_0.cpp
 Subsystem       : 
 Function Name(s): CResPacker<File_Version_1_0
 Author          : YiQiu
 Date            : 2010-11-3  
 Time            : 16:02
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
**************************************************************************cpp**/
#include "StdAfx.h"
#include "ResPacker.h"

namespace ResFile
{

template<>
void CResPacker<File_Version_1_0>::InitFileName()
{
    m_pFileNameBuf = new BYTE[m_nFileNameSize];
    FileInfoList::iterator iter = m_FileInfoList.begin();
    BYTE* pFileNameBuf = m_pFileNameBuf;
    for ( ; iter!= m_FileInfoList.end(); ++iter )
    {
        FileInfo& TmpFileInfo = *iter;
        string& strTmpPath = TmpFileInfo.strFileName;
        memcpy(pFileNameBuf, strTmpPath.c_str(), strTmpPath.length()+1);
        pFileNameBuf += strTmpPath.length()+1;
    }
    FileInfo TmpFileInfo;
    TmpFileInfo.pRawDataBuf = pFileNameBuf;
    TmpFileInfo.nRawDataSize = m_nFileNameSize;
    m_FileInfoList.push_front(TmpFileInfo);
}

template<>
void CResPacker<File_Version_1_0>::DoRead()
{
    BYTE* pRawFileBufNow = m_pRawFileBuf;
    size_t nRawFileBufRemain = Raw_File_Buf;
    string strFilePath;

    FileInfoList::iterator iter = m_FileInfoList.begin();
    if ( m_bIsExistFileName )
    {
        ++iter; // 第一个为已经读好数据的文件名
    }
    for ( ; iter!= m_FileInfoList.end(); ++iter )
    {
        FileInfo& TmpFileInfo = *iter;

        /**
        *@note 1. Get Raw Res File Path
        */
        string& strFileName = TmpFileInfo.strFileName;
        MakePath(strFileName, strFilePath);
        
        /**
        *@note 2. Open File And Test File Exist And Buffer is enough
        */
        FileSystem::CFile Reader;
        if ( ! Reader.OpenByRead(strFilePath.c_str()) )
        {
            g_strErr =  "Can`t Open File:";
            g_strErr += strFilePath;
            throw g_strErr.c_str();
        }
        FileSystem::size_t nFileSize = Reader.GetLength();
        if ( nFileSize == 0 ||
             nFileSize > nRawFileBufRemain )
        {
            throw "Total File Size above 64MB or File Size is 0";
        }

        /**
        *@note 3. Read All File Data To Buffer 
        */
        //FileSystem::size_t nFileBufSize = nRawFileBufRemain;
        if ( nFileSize != Reader.Read((void*)pRawFileBufNow, nFileSize) )
        {
            g_strErr = "Read File Failed at File " + strFilePath;
            throw g_strErr.c_str();
        }

        /**
        *@note 4. Set Info for Data Compress and Save To File
        */
        TmpFileInfo.pRawDataBuf = pRawFileBufNow;
        TmpFileInfo.nRawDataSize = nFileSize;

        /**
        *@note 5. Refresh Buffer Point and Buffer Remain size
        */
        pRawFileBufNow += nFileSize;
        nRawFileBufRemain -= nFileSize;

        /**
        *@note 6. Test is Notify DataTransform Thread of Compress Data
        */
        PostThreadMessage(
            m_nThreadID, Msg_Read_Some, 
            (WPARAM)&TmpFileInfo, 0); 
    }

    /**
    *@note Notify DataTransform Thread of Raw Res File Data Read Finish
    */
    PostThreadMessage(
        m_nThreadID, Msg_Read_Finsih, 0, 0); 
}

template<>
void CResPacker<File_Version_1_0>::DoWrite(
    const char* pPackFilePath )
{
    /**
    *@note 1. Create File
    */
    FileSystem::CFile Writer;
    Writer.OpenByWrite(pPackFilePath);
    if ( !Writer.IsOpen() )
    {
        g_strErr = string("Can`t Open ") + pPackFilePath;
        throw g_strErr.c_str();
    }

    /**
    *@note 2. Write File Head 
    */
    size_t nHeadSize = Util::WriteBaseHead<File_Version_1_0>(
        Writer, m_FileInfoList.size(),
        m_bIsExistFileName, m_eAlgo, m_szKey );
       
    // 这个部分我暂时不用
    DWORD dwRawDataMem[TDataMemInfo::Max_Num] = {0};
    DWORD dwCompressDataMem[TDataMemInfo::Max_Num] = {0};
    Writer.Write((void*)dwRawDataMem, sizeof(dwRawDataMem));
    Writer.Write((void*)dwCompressDataMem, sizeof(dwCompressDataMem));

    typedef TFileHead<File_Version_1_0>::TDataIndex DataIndex;
    DataIndexList<File_Version_1_0>::iterator iter;
    for ( iter = m_DataIndexList.begin();
          iter!= m_DataIndexList.end();
          ++iter )
    {
        /**
        *@note 原先记录只是数据本身之间的，实际的需要加文件头
        */
        DataIndex& Index = *iter;
        Index.dwOffset += nHeadSize;
        Writer.Write((void*)&Index, sizeof(DataIndex));
    }

    /**
    *@note 3. Write File Data
    */
    size_t nResDataSize = Res_File_Buf - m_nResFileBufRemain;
    Writer.Write(m_pResFileBuf, nResDataSize);

    Writer.Close();
}

template<>
void CResPacker<File_Version_1_0>::TransformOne(
    FileInfo& Info )
{
    typedef TDataHead<File_Version_1_0> DataHead;
    DWORD dwDataOffset = m_pResFileBufNow - m_pResFileBuf;
    
    /**
    *@note 1. 填数据头
    */
    DataHead* pDataHead = (DataHead*)m_pResFileBufNow;
    pDataHead->dwRawDataLen = Info.nRawDataSize;
    pDataHead->nCompressAlgo = m_cAlgo;
    pDataHead->nCompressLevel = Compress_High;
    pDataHead->nIsDecrypt = 0;
    m_pResFileBufNow += sizeof(DataHead);
    m_nResFileBufRemain -= sizeof(DataHead);

    /**
    *@note 2. 压缩
    */
    CompressFn pCompressFn = m_CompressFn[m_cAlgo];
    size_t nPackLen = m_nResFileBufRemain;
    int nRc = (this->*pCompressFn)(
        Info.pRawDataBuf, Info.nRawDataSize,
        (void*)m_pResFileBufNow, nPackLen);
    if ( nRc != 0 )
    {
        throw "Compress Failed!";
    }

    void* pRawEncryptBuf = m_pResFileBufNow;
    m_pResFileBufNow += nPackLen;
    m_nResFileBufRemain -= nPackLen;
    m_nFileAllDataSize += nPackLen;
    Info.nPackDataSize = nPackLen;

    /**
    *@note 3. 加密
    */
    EncryptFn pEncryptFn = m_EncryptFn[m_eAlgo];
    (this->*pEncryptFn)(pRawEncryptBuf, nPackLen);
 
    /**
    *@note 4. 添加数据索引
    */
    TFileHead<File_Version_1_0>::TDataIndex Index;
    Index.HashValue = OCI::HashStringEx(Info.strFileName.c_str());
    Index.dwOffset = dwDataOffset;
    Index.dwLen = nPackLen + sizeof(DataHead);
    m_DataIndexList.insert(Index);
}

template<>
void CResPacker<File_Version_1_0>::ShowLog()
{
    FileInfoList::iterator iter = m_FileInfoList.begin();
    for ( int i = 1; iter!= m_FileInfoList.end(); ++iter, ++i )
    {
        const FileInfo& Info = *iter;
        cout << '[' << i << "] " 
            << Info.strFileName.c_str() << " : "
            << Info.nRawDataSize << "-->"
            << Info.nPackDataSize << "   Ratio=" 
            << Info.nPackDataSize*100.0/Info.nRawDataSize
            << '%' << endl;
            
    }
    cout << "File Head size=" 
        << Util::GetFileHeadSize<File_Version_1_0>(m_DataIndexList.size())
        << "\n All Data size = " << DWORD(m_pResFileBufNow- m_pResFileBuf)
        << "\n Data size = " << m_nFileAllDataSize << endl;  
}

}

// End of file