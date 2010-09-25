/**CPP*************************************************************************
 File            : File.cpp
 Subsystem       : 
 Function Name(s): CFile
 Author          : YiQiu
 Date            : 2010-9-25  
 Time            : 10:48
 Description     : Windows CFile实现

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd. Ltd.
**************************************************************************cpp**/
#include "FileSystem.h"
#include <Windows.h>

namespace FileSystem
{
namespace Util
{
    extern FileSystemCode s_LastErrCode;
}


inline void RefreshErrCode()
{
    if ( Util::s_LastErrCode == NO_Error )
    {
        Util::s_LastErrCode = (FileSystemCode)::GetLastError();
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
    LARGE_INTEGER liOff;
    liOff.QuadPart = nOffset;
    liOff.LowPart = ::SetFilePointer(
        m_hFile, liOff.LowPart, &liOff.HighPart,
        (DWORD)nFrom);
    if (liOff.LowPart  != (DWORD)-1)
    {    
        return;
    }

    RefreshErrCode();
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

    DWORD dwRead = 0;
    if (!::ReadFile(m_hFile, pBuf, nCount, &dwRead, NULL))
    {
        RefreshErrCode();
    }

    return dwRead;
}

void CFile::Write( const void* pBuf, size_t nCount )
{
    if ( nCount == 0 || pBuf == NULL )
    {
        return;
    }

    DWORD nWritten;
    if (!::WriteFile(m_hFile, pBuf, nCount, &nWritten, NULL))
    {
        return RefreshErrCode();
    }

    // Win95 will not return an error all the time (usually DISK_FULL)
    if (nWritten != nCount)
    {
        return RefreshErrCode();
    }
}

void CFile::Flush()
{
    if (!::FlushFileBuffers(m_hFile))
    {
        return RefreshErrCode();
    }
}

void CFile::Close()
{
    if ( m_hFile == INVALID_HANDLE_VALUE &&
         m_hFile == NULL )
    {
        return;
    }

    if ( !::CloseHandle(m_hFile) )
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