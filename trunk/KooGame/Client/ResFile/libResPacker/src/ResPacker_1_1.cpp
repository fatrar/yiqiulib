/**CPP*************************************************************************
 File            : ResPacker_1_1.cpp
 Subsystem       : 
 Function Name(s): CResPacker<File_Version_1_1>
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
void CResPacker<File_Version_1_1>::InitFileName()
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

    //  [] How ?
    FileInfo TmpFileInfo;
    TmpFileInfo.pRawDataBuf = pFileNameBuf;
    TmpFileInfo.nRawDataSize = m_nFileNameSize;
    TransformOne(TmpFileInfo);
}

template<>
void CResPacker<File_Version_1_1>::DoRead()
{
    // RawFileBufNow���յ����ݸ�ʽΪ һ��ͷһ������������ѭ��
    BYTE* pRawFileBufNow = m_pRawFileBuf;
    size_t nRawFileBufRemain = Raw_File_Buf;

    string strFilePath;
   
    //BYTE* pVolume = pRawFileBufNow;  // ��¼һ���ͷָ��
    //bool bFirstData = true;

    FileInfoList::iterator iter = m_FileInfoList.begin();
    FileInfo* pSendInfo = NULL;
    size_t* pnVolumeSizeNow = NULL; // &pSendInfo->nRawDataSize; // ��¼һ���ʵ�ʴ�С
    //pSendInfo->pRawDataBuf = pRawFileBufNow;
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
        if ( !Reader.OpenByRead(strFilePath.c_str()) )
        {
            g_strErr = "Can`t Open File: " + strFilePath;
            throw g_strErr.c_str();
        }
        FileSystem::size_t nFileSize = Reader.GetLength();
        size_t nGroupSize = nFileSize + sizeof(DataHead); // Head+Data
        size_t nDataHeadLen = sizeof(TDataHead<File_Version_1_1>);
        if ( nFileSize == 0 ||
             nGroupSize > nRawFileBufRemain )
        {
            printf("File = %s\r\n", strFilePath.c_str());
            throw "Total File Size above 64MB or File Size is 0";
        }

        /**
        *@note 3. Do Volume something
        */
        // �����û������һ�����ݣ��Ͳ������С�ж�
        if ( pSendInfo == 0 )
        {
            pSendInfo = &TmpFileInfo;
            pSendInfo->pRawDataBuf = pRawFileBufNow;
            pnVolumeSizeNow = &pSendInfo->nRawDataSize;
            *pnVolumeSizeNow = nGroupSize;
        }
        else
        {
            size_t nNewVolumeSize = *pnVolumeSizeNow + nGroupSize;
            if ( nNewVolumeSize > m_nVolumeSize ) // ���ھ��С����Ҫ����
            {
                PostThreadMessage(
                    m_nThreadID, Msg_Read_Some,(WPARAM)pSendInfo, 0);
                
                pSendInfo = &TmpFileInfo;
                pSendInfo->pRawDataBuf = pRawFileBufNow;
                pnVolumeSizeNow = &pSendInfo->nRawDataSize;
                *pnVolumeSizeNow = nGroupSize;
            }
            else
            {
                // ����Iter������Ϊ��Ч�����ۼӵ�ǰ���С
                TmpFileInfo.pRawDataBuf = (BYTE*)Invaild_Pointer; 
                *pnVolumeSizeNow = nNewVolumeSize;
            }
        }

        /**
        *@note ������ͷ����Bufƫ��
        */
        DataHead* pDataHead = (DataHead*)pRawFileBufNow;
        pDataHead->HashValue = OCI::HashStringEx(strFileName.c_str());
        pDataHead->dwRawDataLen = nFileSize;
        pRawFileBufNow = (BYTE*)(pDataHead + 1);
        nRawFileBufRemain -= sizeof(DataHead);

        /**
        *@note 4. Read All File Data To Buffer�������� 
        */
        //FileSystem::size_t nFileBufSize = nRawFileBufRemain;
        if ( nFileSize != Reader.Read((void*)pRawFileBufNow, nFileSize) )
        {
            g_strErr = "Read File Failed at File " + strFilePath;
            throw g_strErr.c_str();
        }

        /**
        *@note 5. Refresh Buffer Point and Buffer Remain size
        */
        pRawFileBufNow += nFileSize;
        nRawFileBufRemain -= nFileSize;     
    }

    PostThreadMessage(
        m_nThreadID, Msg_Read_Some, (WPARAM)pSendInfo, 0);

    /**
    *@note Notify DataTransform Thread of Raw Res File Data Read Finish
    */
    PostThreadMessage(
        m_nThreadID, Msg_Read_Finsih, 0, 0);
}

template<>
void CResPacker<File_Version_1_1>::DoWrite(
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
    size_t nHeadSize = Util::WriteBaseHead<File_Version_1_1>(
        Writer, m_nIndexCount, m_bIsExistFileName, 
        m_eAlgo, m_szKey );
    size_t nRealFileCount  = m_FileInfoList.size();
    Writer.Write(&nRealFileCount, sizeof(size_t));
    Writer.Write(&m_nVolumeSize, sizeof(size_t));
       
    typedef TFileHead<File_Version_1_1>::TDataIndex DataIndex;
    DataIndexList<File_Version_1_1>::iterator iter;
    for ( iter = m_DataIndexList.begin();
          iter!= m_DataIndexList.end();
          ++iter )
    {
        /**
        *@note ԭ�ȼ�¼ֻ�����ݱ���֮��ģ�ʵ�ʵ���Ҫ���ļ�ͷ
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
void CResPacker<File_Version_1_1>::TransformOne(
    FileInfo& Info )
{
    if ( Info.pRawDataBuf == (BYTE*)Invaild_Pointer )
    {
        return;
    }

    ++m_nIndexCount;
    // ���������Ѿ�д���Ǹ�λ��
    DWORD dwDataOffset = m_pResFileBufNow - m_pResFileBuf;

    /**
    *@note 1. ֱ��ѹ��
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

    /**
    *@note 2. ˢ������
    */
    void* pRawEncryptBuf = m_pResFileBufNow;
    m_pResFileBufNow += nPackLen;    // ƫ����Դ�ļ�Bufָ��
    m_nResFileBufRemain -= nPackLen; // ���»�ʣ������Դ�ļ�Buf
    m_nFileAllDataSize += nPackLen;  // �ۼ��ж������ݣ�����Log����ʾ�ļ��ܹ�����ô�����ݣ�������ͷ
    Info.nPackDataSize = nPackLen;   // ��¼�ÿ��ѹ�����д�С������Log

    /**
    *@note 3. ����
    */
    //EncryptFn pEncryptFn = m_EncryptFn[m_eAlgo];
    //(this->*pEncryptFn)(pRawEncryptBuf, nPackLen);
    m_pResCrypto->Encrypt(pRawEncryptBuf, nPackLen);
 
    /**
    *@note 4. �����������
    */
    TFileHead<File_Version_1_1>::TDataIndex Index;
    Index.dwOffset = dwDataOffset;
    Index.dwLen = nPackLen;
    Index.dwRawVolumeLen = Info.nRawDataSize;
    m_DataIndexList.push_back(Index);
}

template<>
void CResPacker<File_Version_1_1>::ShowLog()
{
    FileInfoList::iterator Iter = m_FileInfoList.begin();
    bool bFlag = true;
    size_t nGroupSize = 0;
    for ( int i = 0; Iter!= m_FileInfoList.end(); ++Iter )
    {
        const FileInfo& Info = *Iter;
        if ( Info.pRawDataBuf && Info.pRawDataBuf != (BYTE*)Invaild_Pointer )
        { 
            ++i;
            if ( i != 1 )
            {
                cout << "}\nGroupSize = " 
                    << nGroupSize + Info.nRawDataSize << endl;
                nGroupSize = 0;
            }

            nGroupSize = Info.nRawDataSize;
            cout << '[' << i << "] = {" 
                 << Info.strFileName.c_str() << "; ";
           
            bFlag = true;
        }
        else
        {
            nGroupSize += Info.nRawDataSize;
            cout << Info.strFileName.c_str() << "; ";
        }
    }
    cout << "}\nGroupSize = " << nGroupSize << endl;

    typedef FileHead::TDataIndex DataIndex;
    DataIndexList<File_Version_1_1>::iterator iter = m_DataIndexList.begin();
    for ( int i = 1; iter!= m_DataIndexList.end(); ++iter,++i )
    {
        const DataIndex& Index = *iter;
        cout << '[' << i << "] " 
             << Index.dwRawVolumeLen << "-->"
             << Index.dwLen << "   Ratio=" 
             << Index.dwLen*100.0/Index.dwRawVolumeLen
             << '%' << endl;
        
    }
    cout << "File Head size=" 
         << Util::GetFileHeadSize<File_Version_1_1>(m_DataIndexList.size())
         << "\n All Data size = " << DWORD(m_pResFileBufNow- m_pResFileBuf)
         << "\n Data size = " << m_nFileAllDataSize << endl;  
}




}

// End of file