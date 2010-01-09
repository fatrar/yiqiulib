// TiCoffFile.h: interface for the CTiCoffFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TICOFFFILE_H__9C6253DB_61D3_4240_9A70_F45E30BFE373__INCLUDED_)
#define AFX_TICOFFFILE_H__9C6253DB_61D3_4240_9A70_F45E30BFE373__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <process.h>

#define PCI64_HDCR           0x01C1FFF4UL
#define HDCR_PCIBOOT         0x00000004UL
#define HDCR_WARMRESET       0x00000001UL
#define HDCR_DSPINT          0x00000002UL
#define STRUCELEN 8
#pragma pack(1) 

typedef struct _COFF_FILE_HEADER
{
	unsigned short  usVersion;
	unsigned short  usSectionNumber;
	unsigned long   ulTimeStamp;
	unsigned long   ulSymbolTableAddr;
	unsigned long   ulSymbolEntriesNumber;
	unsigned short  usOptionalHeader;
	unsigned short  usFlag;
	unsigned short  usTargetID;
}COFF_FILE_HEADER,*PCOFF_FILE_HEADER;

typedef struct _COFF_FILE_OPTION_HEADER
{
	unsigned short  usMagic;
	unsigned short  usVerStamp;
	unsigned long   ulExecCodeLen;
	unsigned long   ulInitDataLen;
	unsigned long   ulUninitDataLen;
	unsigned long   ulEntryPoint;
	unsigned long   ulExecBeginAddr;
	unsigned long   ulInitDataAddr;
}COFF_FILE_OPTION_HEADER, *PCOFF_FILE_OPTION_HEADER;

typedef struct _COFF_FILE_SECTION_HEADER 
{
	char            cName[8];
	unsigned long   ulPhyAddr;
	unsigned long   ulVirAddr;
	unsigned long   ulSectionSize;
	unsigned long   ulRawDataPtr;
	unsigned long   ulRelocationPtr;
	unsigned long   ulLinnumPtr;
	unsigned long   ulRelocationEnts;
	unsigned long   ulLinnums;
	unsigned long   ulFlags;
	unsigned short  usResv;
	unsigned short  usPage;
}COFF_FILE_SECTION_HEADER, *PCOFF_FILE_SECTION_HEADER;

#pragma pack()


class CTiCoffFile  
{
public:
	CTiCoffFile(HANDLE hDevice);
	virtual ~CTiCoffFile();
public:
	int LoadCoffFile(LPCTSTR lpFileName);
	int OpenCoffFile();
	static UINT __stdcall ThreadFun(LPVOID pBuf);
	BOOL Wait();
private:
	COFF_FILE_HEADER m_stCoffFileHeader;
	COFF_FILE_OPTION_HEADER m_stCoffFileOptionHeader;
	PCOFF_FILE_SECTION_HEADER m_pSectionHeader;

//	CDm642Drv* m_pDm642Drv;
private:
	/*LPCTSTR*/CString m_FileName;	//保存文件名，降低耦合度	zhangzhen	2007/04/05
	UINT ThreadId;
	HANDLE m_hThread;
	void InitDSP();
	void StartDSP();
protected:
	ULONG WriteCoff(unsigned long ulVirtualAddr, int WriteSize, char* pBuf);
	HANDLE m_hDevice;
	ULONG Read8MMemVal(ULONG ulOffset, ULONG &ulValue);
	ULONG Write8MMemVal(ULONG ulOffset, ULONG ulValue);
};

#endif // !defined(AFX_TICOFFFILE_H__9C6253DB_61D3_4240_9A70_F45E30BFE373__INCLUDED_)
