/**H**************************************************************************
 File            : FileSystem.h
 Subsystem       : 
 Function Name(s): CFileSystem
 Author          : YiQiu
 Date            : 2010-9-21  
 Time            : 10:17
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd. Ltd.
***************************************************************************H*/
#ifndef _FILESYSTEM_H_2010_9
#define _FILESYSTEM_H_2010_9

#ifdef _WIN32
    typedef void* HANDLE;
    typedef HANDLE FHANDLE;
    typedef const char* FString;
#elif defined(__APPLE__)
    typedef int FHANDLE;
    typedef const char* FString;
#elif defined(__SYMBIAN32__)
    struct FILE;
    typedef FILE* FHANDLE;
#else
    #error FileSystem unsupport system
#endif

namespace FileSystem
{
typedef unsigned long size_t;
typedef long BOOL;
#define NULL 0
#define FALSE 0
#define TRUE 1

enum FileSystemCode
{
    NO_Error,
    Open_Error,
    Seek_Error,
    Flush_Error,
    Close_Error,
    Read_Error,
    Write_Error,
    File_Rename_Error,
    File_Remove_Error,
    File_SetLengh_Error,
    File_GetLengh_Error,
};

namespace Util
{
    BOOL IsFolder(FString pName);
    FileSystemCode GetLastError();
};

class CFile
{
public:
	// Flag values
	enum AccessFlags
	{
		modeRead =         (int) 0x00001,
		modeWrite =        (int) 0x00002,
		modeReadWrite =    (int) 0x00003,
    };

    enum ShareFlags
    {
		//shareCompat =      (int) 0x00000,
		//shareExclusive =   (int) 0x00010,
        shareDenyNone  =     (int) 0x00000,
		shareDenyWrite =     (int) 0x00001,
		shareDenyRead  =     (int) 0x00002,
		shareDenyAll   =     (int) 0x00003,
    };

    // Only Use to Write
    enum CreateFlags
    {
		//modeNoInherit =    (int) 0x00080,
		modeCreate =       (int) 0x00001,
		modeNoTruncate =   (int) 0x00002,
        modeOpenAlway  =   (int) 0x00003
    };
	
	enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };

public:	
	// Constructors
	CFile():m_hFile(NULL){};
    CFile(
        FString pFileName,
        AccessFlags nAccessFlags,
        CreateFlags nCreateFlags, 
        ShareFlags nShareFlags = shareDenyAll);
    ~CFile();

public:	
	BOOL Open(
        FString pFileName,
        AccessFlags nAccessFlags,
        CreateFlags nCreateFlags, 
        ShareFlags nShareFlags = shareDenyAll );

    BOOL OpenByRead(
        FString pFileName,
        ShareFlags nShareFlags = shareDenyAll );

    BOOL OpenByWrite(
        FString pFileName,
        CreateFlags nCreateFlags = modeOpenAlway,
        ShareFlags nShareFlags = shareDenyAll );
	
    inline BOOL IsOpen(){return NULL != m_hFile;}
    inline operator FHANDLE()const {return m_hFile;}

	// Overridables
	void Seek(size_t nOffset, SeekPosition nFrom);
	
	// 文件截断
	void SetLength(size_t nNewLen);
	
	// 取文件大小
	size_t GetLength() const;
	
	size_t Read(void* pBuf, size_t nCount);
	void Write(const void* pBuf, size_t nCount);
	
	void Flush();
	void Close();
	
	// 重命名
	static BOOL Rename(FString pOldName, FString pNewName);

	// 删除
	static BOOL Remove(FString pFileName);
	
    static BOOL IsExist(FString pFileName);

protected:
	FHANDLE m_hFile;
};

	
	
	

};








#endif  // _FILESYSTEM_H_2010_


// End of file
