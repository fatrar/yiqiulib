/**CPP*************************************************************************
 File            : ResPacker.cpp
 Subsystem       : 
 Function Name(s): CResPacker
 Author          : YiQiu
 Date            : 2010-10-13  
 Time            : 10:47
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
//template class CResPacker<File_Version_1_0>;

//template class CResPacker<File_Version_1_0>;
//template class CResPacker<File_Version_1_1>;
//template class CResPacker<File_Version_1_2>;
//template class CResPacker<File_Version_1_3>;

string g_strErr = "";

template<typename T>
inline void CopyParam(T& TParam, void* pParam)
{
    TParam = *((T*)pParam);
}

IResPacker* CreateResPacker( 
    DWORD Version,
    const char* pResFlodPath, 
    eEncryptAlgo eAlgo,
    BYTE (&szKey)[8],
    eCompressAlgo cAlgo )
{
    if ( NULL == pResFlodPath )
    {
        pResFlodPath = "";
    }
    
    switch ( Version )
    {
    case File_Version_1_0:
        return new CResPacker<File_Version_1_0>(
            pResFlodPath, 
            eAlgo,szKey, cAlgo );
    case File_Version_1_1:
        return new CResPacker<File_Version_1_1>(
            pResFlodPath, 
            eAlgo,szKey, cAlgo );
    case File_Version_1_2:
        return new CResPacker<File_Version_1_2>(
            pResFlodPath, 
            eAlgo,szKey, cAlgo );
    case File_Version_1_3:
        return new CResPacker<File_Version_1_3>(
            pResFlodPath, 
            eAlgo,szKey, cAlgo );
    default:
    	return NULL;
    }
}

void DestroyResPacker( IResPacker*& pResPacker )
{
    safeDelete(pResPacker);
}

template<DWORD Version>
CResPacker<Version>::CResPacker(
    const char* pResFlodPath,
    eEncryptAlgo eAlgo,
    BYTE (&szKey)[8],
    eCompressAlgo cAlgo )
    : m_strResFold(pResFlodPath)
    , m_eAlgo(eAlgo)
    , m_cAlgo(cAlgo)
    , m_nFileAllDataSize(0)
    , m_nFileNameSize(0)
    , m_pFileNameBuf(NULL)
    , m_nIndexCount(0)
    , m_pResCrypto(NULL)
{
    memcpy(m_szKey, szKey, 8);
}

template<DWORD Version>
void CResPacker<Version>::AddFile(
    const char* pFileName )
{
    if ( !isValidString(pFileName) )
    {
        return;
    }

    FileInfo Info;
    Info.strFileName = pFileName;
    m_nFileNameSize += Info.strFileName.length() + 1;
    m_FileInfoList.push_back(Info);
}

template<DWORD Version>
void CResPacker<Version>::MakeFile(
    const char* pPackFilePath,
    size_t nVolumeSize,
    bool bIsExistFileName )
{
    m_nVolumeSize = nVolumeSize;
    m_bIsExistFileName = bIsExistFileName;
    /**
    *@note �������̣������̶߳��ļ���д�����⿪һ���߳�ר�������ݵ�ת��
    *      �������̶߳�һС�����ļ��ͷ���һ���¼�������һ���߳�ת����ת���߳�ת����д��Res�ļ�����
    *      �����߳�ȫ�������ᷢһ��������¼������ʱ��ת���߳�����������е�ת������˳�
    *      ���̷߳������¼����ȴ�ת���߳��˳���Ȼ��Res�ļ�����һ����д���ļ�
    */
    Init();
    DoRead();
    WaitForSingleObject(m_hTransThread, INFINITE);
    DoWrite(pPackFilePath);
    ShowLog();
    Unit();
}

template<DWORD Version>
unsigned int CResPacker<Version>::DataTransform()
{
    MSG msg;
    PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    while(1)
    {     
        BOOL bRc = GetMessage(
            &msg, HWND(-1), 
            Msg_Read_Some, Msg_Read_Finsih);
        if ( !bRc )
        {
            DWORD dwError = GetLastError();
            goto end; // �Ƿ�WM_QUIT
        }
   
        switch(msg.message)
        { 
        case Msg_Read_Some:
            TransformOne(*(FileInfo*)msg.wParam);
            break;
        case Msg_Just_Test:
            break;   
        case Msg_Read_Finsih:
            goto end;
        }
    }

end:
    return 0;
}

template<DWORD Version>
void CResPacker<Version>::Init()
{
    m_CompressFn[Raw_C_Algo] = &CResPacker<Version>::RawCompress;
    m_CompressFn[Zip_C_Algo] = &CResPacker<Version>::ZipCompress;
    m_CompressFn[Lzma_C_Algo] = &CResPacker<Version>::LzmaCompress;

    m_pResCrypto = Util::IResCrypto::CreateResCrypto(m_eAlgo, m_szKey);

    m_pRawFileBuf = new BYTE[Raw_File_Buf];
    m_nRawFileBufUse = 0;
    m_pResFileBuf = new BYTE[Res_File_Buf];
    m_nResFileBufRemain = Res_File_Buf;
    m_pResFileBufNow = m_pResFileBuf;

    if ( m_bIsExistFileName )
    {
        InitFileName();
    }

    m_hTransThread = (HANDLE)_beginthreadex(
        NULL, 0, 
        &CResPacker<Version>::DataTransform, this,
        0, &m_nThreadID );

    while (1)
    {
        if ( PostThreadMessage(m_nThreadID, Msg_Just_Test, 0, 0) )
        {
            break;
        }

        Sleep(1);
    }
}

template<DWORD Version>
void CResPacker<Version>::Unit()
{
    m_pResCrypto->Release();
    safeDeleteArray(m_pRawFileBuf);
    safeDeleteArray(m_pResFileBuf);
    safeDeleteArray(m_pFileNameBuf);

    CloseHandle(m_hTransThread);
}

template<DWORD Version>
void CResPacker<Version>::MakePath(
    const string& strFileName,
    string& strFilePath)
{
    if ( m_strResFold.size() == 0 )
    {
        strFilePath = strFileName;
    }
    else if ( m_strResFold[m_strResFold.size()-1] == '\\' ||
        m_strResFold[m_strResFold.size()-1] == '/' )
    {
        strFilePath = m_strResFold + strFileName;
    }
    else
    {
        strFilePath = m_strResFold + "/" + strFileName;
    }
}

template<DWORD Version>
int CResPacker<Version>::LzmaCompress(
    void* pIn, size_t nIn,
    void* pOut, size_t& nOut )
{
    return LzmaUtil::LzmaCompress(
        (unsigned char*)pOut, &nOut,
        (const unsigned char*)pIn, nIn, Compress_Normal);
}


}

#include "ResPacker_1_1.cpp"
#include "ResPacker_1_0.cpp"


// End of file