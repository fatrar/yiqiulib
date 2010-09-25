/**CPP*************************************************************************
 File            : File.cpp
 Subsystem       : 
 Function Name(s): CFile
 Author          : YiQiu
 Date            : 2010-9-25  
 Time            : 10:48
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd. Ltd.
**************************************************************************cpp**/
#include "FileSystem.h"
#include <stdio.h>

namespace FileSystem
{
namespace Util
{
    extern FileSystemCode s_LastErrCode;
}


inline void RefreshErrCode(FileSystemCode Code)
{
    if ( Util::s_LastErrCode == NO_Error )
    {
        Util::s_LastErrCode = Code;
    }
}

inline DWORD GetCreateFlags(CFile::CreateFlags nCreateFlags)
{
    DWORD dwCreateFlag;
    if (nCreateFlags & CFile::modeCreate)
    {
        if (nCreateFlags & CFile::modeNoTruncate)
        {
            /**
            *@note 如果文件存在就打开，不存在就创建一个新的
            */
            dwCreateFlag = OPEN_ALWAYS;
        }
        else
        {
            /**
            *@note 创建一个新的空文件，不管文件是否已经存在
            */
            dwCreateFlag = CREATE_ALWAYS;
        }
    }
    else
    {
        /**
        *@note 只打开现有的，当文件不存在就失败
        */
        dwCreateFlag = OPEN_EXISTING;
    }
    return dwCreateFlag;
}

inline DWORD GetAccessFlags(CFile::AccessFlags nAccessFlags)
{
    DWORD dwAccess;
    switch ( nAccessFlags )
    {
    case CFile::modeRead:
        dwAccess = GENERIC_READ;
        break;
    case CFile::modeWrite:
        dwAccess = GENERIC_WRITE;
        break;
    case CFile::modeReadWrite:
        dwAccess = GENERIC_READ | GENERIC_WRITE;
        break;
    default:
        break;
        // ASSERT(FALSE);  // invalid share mode
    }
    return dwAccess;
}

CFile::CFile(
    FString pFileName,
    AccessFlags nAccessFlags,
    CreateFlags nCreateFlags, 
    ShareFlags nShareFlags )
{
    Open(pFileName,nAccessFlags,nCreateFlags,nShareFlags);
}

CFile::~CFile()
{
    Close();
}

BOOL CFile::Open(
    FString pFileName,
    AccessFlags nAccessFlags,
    CFile::CreateFlags nCreateFlags,
    CFile::ShareFlags nShareFlags )
{
    DWORD dwAccess = GetAccessFlags(nAccessFlags);
    DWORD dwShareMode = nShareFlags;
    DWORD dwCreateFlag = GetCreateFlags(nCreateFlags);

    HANDLE hFile = ::CreateFile(
        pFileName, dwAccess, dwShareMode, NULL,
        dwCreateFlag, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        RefreshErrCode();
        return FALSE;
    }

    m_hFile = hFile;
    return TRUE;
}
// ftruncate truncate
// chsize remove
BOOL CFile::OpenByRead(
    FString pFileName,
    CFile::ShareFlags nShareFlags )
{
    DWORD dwAccess = GENERIC_READ;
    DWORD dwShareMode = nShareFlags;
    DWORD dwCreateFlag = OPEN_EXISTING;

    HANDLE hFile = ::CreateFile(
        pFileName, dwAccess, dwShareMode, NULL,
        dwCreateFlag, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        RefreshErrCode();
        return FALSE;
    }

    m_hFile = hFile;
    return TRUE;
}

BOOL CFile::OpenByWrite(
    FString pFileName,
    CFile::CreateFlags nCreateFlags,
    CFile::ShareFlags nShareFlags )
{
    DWORD dwAccess = GENERIC_WRITE;
    DWORD dwShareMode = nShareFlags;
    DWORD dwCreateFlag = GetCreateFlags(nCreateFlags);
    HANDLE hFile = ::CreateFile(
        pFileName, dwAccess, dwShareMode, NULL,
        dwCreateFlag, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        RefreshErrCode();
        return FALSE;
    }

    m_hFile = hFile;
    return TRUE;
}

void CFile::Seek( size_t nOffset, SeekPosition nFrom )
{
    int nRc = fseek(m_hFile, nOffset, nFrom);
    if ( nRc == -1 )
    {
        RefreshErrCode(Seek_Error);
    }
}

void CFile::SetLength( size_t dwNewLen )
{
    Seek(dwNewLen, begin);
    if ( ::SetEndOfFile(m_hFile) )
    {
        return;
    }

    RefreshErrCode();
}

size_t CFile::GetLength() const
{
    ULARGE_INTEGER liSize;
    liSize.LowPart = ::GetFileSize(m_hFile, &liSize.HighPart);
    if (liSize.LowPart != INVALID_FILE_SIZE)
    {
        return liSize.QuadPart;
    }

    RefreshErrCode();
    return 0;
}

size_t CFile::Read( void* pBuf, size_t nCount )
{
    if ( nCount == 0 || pBuf == NULL )
    {
        return 0;
    }

    DWORD dwRead = ::fread(pBuf, 1, nCount, m_hFile);
    if (dwRead != nCount)
    {
        RefreshErrCode(Read_Error);
    }

    return dwRead;
}

void CFile::Write( const void* pBuf, size_t nCount )
{
    if ( nCount == 0 || pBuf == NULL )
    {
        return;
    }

    DWORD nWritten = fwrite(pBuf, 1, nCount, m_hFile);
    if ( nWritten != nCount )
    {
        return RefreshErrCode(Write_Error);
    }
}

void CFile::Flush()
{
    if (-1 == ::fflush(m_hFile))
    {
        return RefreshErrCode();
    }
}

void CFile::Close()
{
    if ( m_hFile == NULL )
    {
        return;
    }

    if ( -1 == ::fclose(m_hFile) )
    {
        RefreshErrCode();
    }
}

BOOL CFile::Rename( FString pOldName, FString pNewName )
{
    if (::MoveFile(pOldName, pNewName))
    {
        return TRUE;
    }

    RefreshErrCode();
    return FALSE;
}

BOOL CFile::Remove( FString pFileName )
{
    if (::DeleteFile((LPTSTR)pFileName))
    {
        return TRUE;
    }

    RefreshErrCode();
    return FALSE;
}

BOOL CFile::IsExist( FString pFileName )
{
    DWORD dwAccess = GENERIC_READ;
    DWORD dwShareMode = FILE_SHARE_WRITE | FILE_SHARE_READ;
    DWORD dwCreateFlag = OPEN_EXISTING;
    HANDLE hFile = ::CreateFile(
        pFileName, dwAccess, dwShareMode, NULL,
        dwCreateFlag, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    CloseHandle(hFile);
    return TRUE;
}

}








// End of file