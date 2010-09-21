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



namespace FileSystem
{
	
class CFile
{
public:
	// Flag values
	enum OpenFlags
	{
		modeRead =         (int) 0x00000,
		modeWrite =        (int) 0x00001,
		modeReadWrite =    (int) 0x00002,
		//shareCompat =      (int) 0x00000,
		//shareExclusive =   (int) 0x00010,
		shareDenyWrite =   (int) 0x00020,
		shareDenyRead =    (int) 0x00030,
		//shareDenyNone =    (int) 0x00040,
		//modeNoInherit =    (int) 0x00080,
		modeCreate =       (int) 0x01000,
		modeNoTruncate =   (int) 0x02000,
		//typeText =         (int) 0x04000, // typeText and typeBinary are
		//typeBinary =       (int) 0x08000, // used in derived classes only
		//osNoBuffer =       (int) 0x10000,
		//osWriteThrough =   (int) 0x20000,
		//osRandomAccess =   (int) 0x40000,
		//osSequentialScan = (int) 0x80000,
	};
	
	enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };
	
	// Constructors
	CFile();
	CFile(HANDLE hFile);
	CFile(LPCTSTR lpszFileName, UINT nOpenFlags);
	
	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags);
	
	// Overridables
	ULONGLONG Seek(LONGLONG lOff, UINT nFrom);
	
	// 文件截断
	void SetLength(ULONGLONG dwNewLen);
	
	// 取文件大小
	ULONGLONG GetLength() const;
	
	UINT Read(void* lpBuf, UINT nCount);
	void Write(const void* lpBuf, UINT nCount);
	
	void Flush();
	void Close();
	
	// 重命名
	static void Rename(LPCTSTR lpszOldName,
		LPCTSTR lpszNewName);
	// 删除
	static void Remove(LPCTSTR lpszFileName);
	
	virtual ~CFile();
	
protected:
	HANDLE m_hFile;
};

	
	
	

};








#endif  // _FILESYSTEM_H_2010_


// End of file
