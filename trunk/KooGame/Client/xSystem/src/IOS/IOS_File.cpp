/**CPP*************************************************************************
 File            : File.cpp
 Subsystem       : 
 Function Name(s): CFile
 Author          : YiQiu
 Date            : 2010-9-25  
 Time            : 10:48
 Description     : CFile IOS 实现

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd. Ltd.
**************************************************************************cpp**/
#include "FileSystem.h"
//#include <stdio.h>
#include <io.h>
#include <sys\stat.h>
namespace FileSystem
{
namespace Util
{
    extern FileSystemCode s_LastErrCode;
}
typedef unsigned long DWORD; 

inline void RefreshErrCode(FileSystemCode Code)
{
    if ( Util::s_LastErrCode == NO_Error )
    {
        Util::s_LastErrCode = Code;
    }
}

inline DWORD GetShareFlags(CFile::ShareFlags nShareFlags)
{
    DWORD dwShareFlags = 0;
    if ( nShareFlags & CFile::shareDenyWrite )
    {
        dwShareFlags |= S_IWRITE;
    }
    if ( nShareFlags & CFile::shareDenyRead )
    {
        dwShareFlags |= S_IREAD;
    }
    return dwShareFlags;
}

inline DWORD GetCreateFlags(
    CFile::CreateFlags nCreateFlags )
{
    DWORD dwOpenFlags = 0;
    if (nCreateFlags & CFile::modeCreate)
    {
        if (nCreateFlags & CFile::modeNoTruncate)
        {
            /**
            *@note 如果文件存在就打开，不存在就创建一个新的
            */
            dwOpenFlags |= O_CREAT;
        }
        else
        {
            /**
            *@note 创建一个新的空文件，不管文件是否已经存在
            */
            dwOpenFlags |= O_CREAT | O_TRUNC;
        }
    }
    else
    {
        /**
        *@note 只打开现有的，当文件不存在就失败
        */
        ;// nothing to do.
    }
    return dwOpenFlags;
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
    DWORD dwOpenFlags = O_BINARY | GetCreateFlags(nCreateFlags, nShareFlags);
    if ( nAccessFlags == modeRead )
    {
        dwOpenFlags |= O_RDONLY;
    }
    else if ( nAccessFlags == modeWrite )
    {
        dwOpenFlags |= O_WRONLY;
    }
    else
    {
        dwOpenFlags |= O_RDWR;
    }
    DWORD dwShareFlags = GetShareFlags(nShareFlags);
    int nFile = open(pFileName, dwOpenFlags, dwShareFlags);
    if (nFile == -1)
    {
        RefreshErrCode(Open_Error);
        return FALSE;
    }

    m_hFile = nFile;
    return TRUE;
}
// ftruncate truncate
// chsize remove
BOOL CFile::OpenByRead(
    FString pFileName,
    CFile::ShareFlags nShareFlags )
{  
    DWORD dwOpenFlags = O_BINARY | O_RDONLY;
    DWORD dwShareFlags = GetShareFlags(nShareFlags);
    int nFile = open(pFileName, dwOpenFlags, dwShareFlags);
    if (nFile == -1)
    {
        RefreshErrCode(Open_Error);
        return FALSE;
    }

    m_hFile = nFile;
    return TRUE;
}

BOOL CFile::OpenByWrite(
    FString pFileName,
    CFile::CreateFlags nCreateFlags,
    CFile::ShareFlags nShareFlags )
{
    DWORD dwOpenFlags = O_BINARY | O_WRONLY;
    dwOpenFlags |= GetCreateFlags(nCreateFlags);
    DWORD dwShareFlags = GetShareFlags(nShareFlags);
    int nFile = open(pFileName, dwOpenFlags, dwShareFlags);
    if (nFile == -1)
    {
        RefreshErrCode(Open_Error);
        return FALSE;
    }

    m_hFile = nFile;
    return TRUE;
}

void CFile::Seek( size_t nOffset, SeekPosition nFrom )
{
    int nRc = lseek(m_hFile, nOffset, nFrom);
    if ( nRc == -1 )
    {
        RefreshErrCode(Seek_Error);
    }
}

void CFile::SetLength( size_t dwNewLen )
{
    if ( -1 != ::chsize(m_hFile, dwNewLen) )
    {
        return;
    }

    RefreshErrCode(File_SetLengh_Error);
}

size_t CFile::GetLength() const
{
    long nLength = ::filelength(m_hFile);
    if (nLength != -1)
    {
        return nLength;
    }

    RefreshErrCode(File_GetLengh_Error);
    return 0;
}

size_t CFile::Read( void* pBuf, size_t nCount )
{
    if ( nCount == 0 || pBuf == NULL )
    {
        return 0;
    }

    DWORD dwRead = ::read(m_hFile, pBuf, nCount);
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

    DWORD nWritten = ::write(m_hFile, pBuf, nCount);
    if ( nWritten != nCount )
    {
        return RefreshErrCode(Write_Error);
    }
}

void CFile::Flush()
{
    if (-1 == ::_commit(m_hFile))
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

    if ( -1 == ::close(m_hFile) )
    {
        RefreshErrCode(Close_Error);
    }
}

BOOL CFile::Rename( FString pOldName, FString pNewName )
{
    if (-1 != ::rename(pOldName, pNewName))
    {
        return TRUE;
    }

    RefreshErrCode(File_Rename_Error);
    return FALSE;
}

BOOL CFile::Remove( FString pFileName )
{
    if (-1 != ::remove(pFileName))
    {
        return TRUE;
    }

    RefreshErrCode(File_Remove_Error);
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