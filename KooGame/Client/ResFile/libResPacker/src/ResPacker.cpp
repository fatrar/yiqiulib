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
#include <assert.h>

namespace ResFile
{
//template class CResPacker<File_Version_1_0>;

template<typename T>
inline void CopyParam(T& TParam, void* pParam)
{
    TParam = *((T*)pParam);
}

IResPacker* CreateResPacker( 
    const char* pResFlodPath,
    eCompressAlgo cAlgo /*= LZMA2_C_Algo*/, 
    void* pcParam /*= (void*)&g_DefeParam*/, 
    eEncryptAlgo eAlgo /*= Raw_E_Algo*/,
    void* peParam /*= NULL */ )
{
    if ( isValidString(pResFlodPath) )
    {
        pResFlodPath = "";
    }

    typedef TEncryptParam<File_Version_1_0> EncryptParam;
    typedef TCompressParam<File_Version_1_0> CompressParam;

    EncryptParam eParam;
    CompressParam cParam = g_DefcParam;
    if ( eAlgo != Raw_E_Algo )
    {
        if ( NULL != peParam )
        {
            CopyParam(eParam, peParam);
        }
        else
        {
            eAlgo = Raw_E_Algo;
        }
    }

    if ( NULL == pcParam )
    {
        CopyParam(cParam, pcParam);
        cAlgo = LZMA2_C_Algo;
    }
    else
    {
        CopyParam(cParam, pcParam);
    }
    
    return new CResPacker<File_Version_1_0>(
        pResFlodPath,
        cAlgo, cParam,
        eAlgo, eParam );
}

void DestroyResPacker( IResPacker*& pResPacker )
{
    safeDelete(pResPacker);
}

template<>
CResPacker<File_Version_1_0>::CResPacker(
    const char* pResFlodPath, 
    eCompressAlgo cAlgo,
    const CompressParam& pcParam,
    eEncryptAlgo eAlgo,
    const EncryptParam& peParam )
    : m_strResFold(pResFlodPath)
    , m_DefeAlgo(eAlgo)
    , m_DefcAlgo(cAlgo)
    , m_Thread(NULL)
    , m_pFileBuf(NULL)
    , m_nNowPos(0)
    , m_DefcParam(pcParam)
    , m_DefeParam(peParam)
{}

template<>
CResPacker<File_Version_1_0>::~CResPacker(void)
{
}

// template<>
// void CResPacker<File_Version_1_0>::SetDefaultEncryptParam(
//     EncryptAlgo eAlgo,
//     void* peParam /*= NULL */ )
// {
//     typedef TEncryptParam<File_Version_1_0> EncryptParam;
//     if ( eAlgo != Raw_E_Algo )
//     {
//         if ( NULL == peParam )
//         {
//             return;
//         }
//         
//         CopyParam(m_DefeParam, peParam);
//         //m_DefeParam = *((EncryptParam*)peParam);
//     }
//     m_DefeAlgo = eAlgo;
// }
// 
// template<>
// void CResPacker<File_Version_1_0>::SetDefaultCompressParam(
//     CompressAlgo cAlgo, 
//     void* pcParam /*= NULL*/ )
// {
//     typedef TCompressParam<File_Version_1_0> CompressParam;
//     if ( pcParam == NULL )
//     {
//         return;
//     }
//     m_DefcAlgo = cAlgo;
//     CopyParam(m_DefcParam, pcParam);
//     //m_DefcParam = *((CompressParam*)pcParam);
// }
// 
// template<>
// void CResPacker<File_Version_1_0>::SetCurrentPath(
//     const char* pPath )
// {
//     if ( isValidString(pPath) )
//     {
//         return;
//     }
// 
//     m_strResFold = pPath;
// }

template<>
void CResPacker<File_Version_1_0>::AddFile(
    const char* pFileName )
{
    if ( isValidString(pFileName) )
    {
        return;
    }

    FileInfo Info;
    Info.strFileName = pFileName;
    Info.eAlgo = m_DefeAlgo;
    Info.eParam = m_DefeParam;
    Info.cAlgo = m_DefcAlgo;
    Info.cParam  = m_DefcParam;
    m_FileInfoList.push_back(Info);
}

template<>
void CResPacker<File_Version_1_0>::AddFile(
    const char* pFileName,
    eCompressAlgo cAlgo, void* pcParam,
    eEncryptAlgo eAlgo, void* peParam )
{
    if ( isValidString(pFileName) )
    {
        return;
    }

    FileInfo Info;
    Info.strFileName = pFileName;
    
    /**
    *@note 如果参数为0，则用默认的
    */
    if ( pcParam == NULL )
    {
        Info.cAlgo = m_DefcAlgo;
        Info.cParam = m_DefcParam;
    }
    else
    {
        Info.cAlgo = cAlgo;
        CopyParam(Info.cParam, pcParam);
        //Info.cParam  = *((CompressParam*)pcParam);
    }

    if ( eAlgo != Raw_E_Algo )
    {
        if ( NULL == peParam )
        {
            CopyParam(Info.eParam, peParam);
            Info.eAlgo = m_DefeAlgo;
        }
        else
        {
            CopyParam(Info.eParam, peParam);
            Info.eAlgo = m_DefeAlgo;
        }
    }
    
    m_FileInfoList.push_back(Info);
}

template<>
bool CResPacker<File_Version_1_0>::MakeFile(
    const char* pPackFilePath,
    eFileNamePos eFileNamePos )
{
    return true;
}

// Thread

template<>
DWORD CResPacker<File_Version_1_0>::FileReadWrite()
{
    FileInfoList::iterator iter;
    for ( iter = m_FileInfoList.begin();
          iter!= m_FileInfoList.end();
          ++iter )
    {
    }

    return 0;
}

}



//namespace GG
//{
//    typedef unsigned int size_t; 
//
//    typedef unsigned char size_t; 
//}
//
//class JJ
//{
//
//};






// End of file