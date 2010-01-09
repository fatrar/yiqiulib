// TiCoffFile.cpp: implementation of the CTiCoffFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TiCoffFile.h"
#include <stdlib.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTiCoffFile::CTiCoffFile(HANDLE hDevice)
{
	m_hDevice = hDevice;
	m_hThread = NULL;
	Write8MMemVal(PCI64_HDCR, HDCR_WARMRESET);//PS
}

CTiCoffFile::~CTiCoffFile()
{
	Write8MMemVal(PCI64_HDCR, HDCR_DSPINT);//PS
	::CloseHandle(m_hThread);
}

int CTiCoffFile::OpenCoffFile()
{
	int nRtn;
	int iRet = 0;             //function return value
	unsigned short i;         //internal iteration variable
	char* pReadBuf;
	int iDataLen;
	unsigned long ulSectionSize;
	unsigned long ulVirtualAddr;
	int iReadSize;
	FILE* m_hFile = NULL;      //file handle
	int m_iSectionNumber = 0;
    InitDSP();	
	if((m_hFile = fopen(m_FileName, "rb")) == NULL)
	{
//		AfxMessageBox("fopen file error");
		iRet = 1;
		goto ERROR_EXIT;
	}

	nRtn = fseek( m_hFile, 0, SEEK_SET );
	ASSERT(nRtn == 0);

	memset( &(m_stCoffFileHeader), 0, sizeof(COFF_FILE_HEADER) );
	iReadSize = sizeof(COFF_FILE_HEADER);
	iRet = fread( &(m_stCoffFileHeader), iReadSize, 1, m_hFile );
	if ( iRet != 1 )
	{
//		AfxMessageBox("fread coffile header error");
		iRet = 2;
		goto ERROR_EXIT;
	}
	
	if (m_stCoffFileHeader.usOptionalHeader > 0)
	{
		memset( &(m_stCoffFileOptionHeader), 0, sizeof(COFF_FILE_OPTION_HEADER) );
		iReadSize = sizeof(COFF_FILE_OPTION_HEADER);
		iRet = fread( &(m_stCoffFileOptionHeader), iReadSize, 1, m_hFile );
		if (iRet != 1)
		{
//			AfxMessageBox("fread coffile opention header error");
			iRet = 3;
			goto ERROR_EXIT;
		}
	}

	m_iSectionNumber = (int)(m_stCoffFileHeader.usSectionNumber);
	if (m_iSectionNumber <= 0)
	{
//		AfxMessageBox("m_stCoffFileHeader.usSectionNumber <0");
		iRet = 4;
		goto ERROR_EXIT;
	}

	m_pSectionHeader = (PCOFF_FILE_SECTION_HEADER)malloc( 
							sizeof(COFF_FILE_SECTION_HEADER)*m_iSectionNumber);
	if (!m_pSectionHeader)
	{
//		AfxMessageBox("m_pSectionHeader == NULL");
		iRet = 5;
		goto ERROR_EXIT;
	}
	iRet = sizeof(COFF_FILE_SECTION_HEADER);
	for( i=0; i<m_stCoffFileHeader.usSectionNumber; i++ )
	{
		iRet = fread( &(m_pSectionHeader[i]),  
				sizeof(COFF_FILE_SECTION_HEADER),1, m_hFile );
 		if (iRet != 1 )
 		{
//            AfxMessageBox("fread m_pSectionHeader.usSectionNumber error");
 			iRet = 6;
 			goto ERROR_EXIT;
 		}
//		Sleep(1);
	}
	pReadBuf = (char*)malloc( 128 + STRUCELEN );
	if ( !pReadBuf )
	{
//		AfxMessageBox("pReadBuf malloc error");
		iRet = 7;
		goto ERROR_EXIT;
	}
	
	iDataLen = 128;

	for( i=0; i<m_stCoffFileHeader.usSectionNumber; i++ )
	{	
		if (((m_pSectionHeader[i].ulFlags & 0x00000020) || //.text
			(m_pSectionHeader[i].ulFlags & 0x00000040) ||
			(m_pSectionHeader[i].ulFlags & 0x00000080) ) && 
			!(m_pSectionHeader[i].ulFlags & 0x00000010) &&
			(m_pSectionHeader[i].ulSectionSize > 0) && 
			(m_pSectionHeader[i].ulRawDataPtr > 0 ))
		{
			nRtn = fseek( m_hFile, m_pSectionHeader[i].ulRawDataPtr, SEEK_SET );
			ASSERT(nRtn == 0);
			ulSectionSize = m_pSectionHeader[i].ulSectionSize;
			ulVirtualAddr = m_pSectionHeader[i].ulVirAddr;


			TRACE( "Addr ==== %8x,Len %8d,RawDataPtr %8x,Flag %4x,Page %4d\n", 
				ulVirtualAddr, ulSectionSize, 
				m_pSectionHeader[i].ulRawDataPtr, 
				m_pSectionHeader[i].ulFlags, 
				m_pSectionHeader[i].usPage);

			while( ulSectionSize > 0 )
			{
				memset( pReadBuf, 0, 128 + STRUCELEN );
				
				iReadSize = (int)ulSectionSize > iDataLen ? iDataLen : ulSectionSize;
				iRet = fread( pReadBuf + STRUCELEN, 1, iReadSize, m_hFile );
				if (iRet != iReadSize)
				{
//					AfxMessageBox("iRet != iReadSize");
					free( pReadBuf );
					iRet = 8;
					goto ERROR_EXIT;
				}
				*(DWORD*)pReadBuf= ulVirtualAddr;
				*(((DWORD*)pReadBuf)+1) = (iReadSize>>2);
			    iRet = WriteCoff (ulVirtualAddr,iReadSize+8,(char*)pReadBuf); 
			   // Sleep(10);
		        ulSectionSize -= iReadSize;
			    ulVirtualAddr += iReadSize;
			}
		}
		else //Not Write,only decard it 
		{
				TRACE( "CTSDTicoffFile ==== Addr %8x, Len %8d, RawDataPtr %8x, Flag %8x, Page %8d NOT USED\n", 
					m_pSectionHeader[i].ulVirAddr,
					m_pSectionHeader[i].ulSectionSize, 
					m_pSectionHeader[i].ulRawDataPtr, 
					m_pSectionHeader[i].ulFlags, 
					m_pSectionHeader[i].usPage);
		}
	}

	free( pReadBuf );
	iRet = 0;
//	Sleep(50);
	Write8MMemVal(0x01c1fff4,2);

ERROR_EXIT:
	ULONG ulHDCRVal = 0;
	Read8MMemVal( PCI64_HDCR, ulHDCRVal );
	ulHDCRVal |= HDCR_PCIBOOT;
	Write8MMemVal( PCI64_HDCR, ulHDCRVal );
//	Sleep(2);
	if ( m_pSectionHeader )
	{
		free( m_pSectionHeader );
		m_pSectionHeader = NULL;
	}

	if (m_hFile)
	{
		fclose( m_hFile );
		m_hFile = NULL;
	}
	return iRet;
}

void CTiCoffFile::InitDSP ()
{
	ULONG ulHDCRVal = 0;
	Read8MMemVal( PCI64_HDCR, ulHDCRVal );
	ulHDCRVal |= HDCR_WARMRESET;              
	Write8MMemVal( PCI64_HDCR, ulHDCRVal );

	Write8MMemVal( 0x01800000, 0x00052078);
	Write8MMemVal( 0x01800004, 0x73a28e01);
	Write8MMemVal( 0x01800008, 0xffffffD3);

	Write8MMemVal( 0x01800010, 0x73a28e01);
	Write8MMemVal( 0x01800014, 0x73a28e01);
	Write8MMemVal( 0x01800018, 0x57226000);//  0x57229000
	Write8MMemVal( 0x0180001c, 0x00000446);
	Write8MMemVal( 0x01800020, 0x001fffef);

	Write8MMemVal( 0x01800044, 0x00000002);
	Write8MMemVal( 0x01800048, 0x00000002);

	Write8MMemVal( 0x01800050, 0x00000002);
	Write8MMemVal( 0x01800054, 0x00000073);

	/*
	{
		//set emif reg value
		ULONG ulEmifA[12] = {0x00052078,        // GCTL     - 0x01800000
                         0x73a28e01,            // CE1      - 0x01800004
                         0xffffffD3,            // CE0      - 0x01800008
                         // Address split 3
                         0x73a28e01,        // CE2      - 0x01800010
                         0x73a28e01,        // CE3      - 0x01800014
						// 0x22a28a42,        // CE3      - 0x01800014
                         0x57226000,        // SDRAMCTL - 0x01800018
                         0x00000446,        // SDRAMTIM - 0x0180001c
                         0x001fffef,        // SDRAMEXT - 0x01800020
                         // Address split 9
                         0x00000002,        // CE1ECCTL - 0x01800044 
                         0x00000002,        // CE0ECCTL - 0x01800048
                         // Address split 
                         0x00000002,        // CE2ECCTL - 0x01800050
                         0x00000073,        // CE3ECCTL - 0x01800054
                        };

		{
			int iRegValIndex = 0;
			m_pDm642Drv->Write8MMemVal( 0x01800000, ulEmifA[iRegValIndex++]);
			m_pDm642Drv->Write8MMemVal( 0x01800004, ulEmifA[iRegValIndex++]);
			m_pDm642Drv->Write8MMemVal( 0x01800008, ulEmifA[iRegValIndex++]);

			m_pDm642Drv->Write8MMemVal( 0x01800010, ulEmifA[iRegValIndex++]);
			m_pDm642Drv->Write8MMemVal( 0x01800014, ulEmifA[iRegValIndex++]);
			m_pDm642Drv->Write8MMemVal( 0x01800018, ulEmifA[iRegValIndex++]);
			m_pDm642Drv->Write8MMemVal( 0x0180001c, ulEmifA[iRegValIndex++]);
			m_pDm642Drv->Write8MMemVal( 0x01800020, ulEmifA[iRegValIndex++]);

			m_pDm642Drv->Write8MMemVal( 0x01800044, ulEmifA[iRegValIndex++]);
			m_pDm642Drv->Write8MMemVal( 0x01800048, ulEmifA[iRegValIndex++]);

			m_pDm642Drv->Write8MMemVal( 0x01800050, ulEmifA[iRegValIndex++]);
			m_pDm642Drv->Write8MMemVal( 0x01800054, ulEmifA[iRegValIndex++]);
		}
	}
	*/
}

void CTiCoffFile::StartDSP ()
{
	ULONG ulHDCRVal = 0;
	Read8MMemVal( PCI64_HDCR, ulHDCRVal );
	ulHDCRVal |= HDCR_PCIBOOT;
	Write8MMemVal( PCI64_HDCR, ulHDCRVal );
//	Sleep(2);
	if ( m_pSectionHeader )
	{
		free( m_pSectionHeader );
		m_pSectionHeader = NULL;
	}
}

UINT CTiCoffFile::ThreadFun (LPVOID lpData)
{
	((CTiCoffFile*)lpData) ->OpenCoffFile();
	return 0;
}
int CTiCoffFile::LoadCoffFile (LPCTSTR lpFileName)
{
	m_FileName = lpFileName;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, (LPVOID)this, 0, &ThreadId);
	return 0;
}

BOOL CTiCoffFile::Wait ()
{
    WaitForSingleObject(m_hThread,INFINITE);
    return TRUE;
}

ULONG CTiCoffFile::Write8MMemVal(ULONG ulOffset, ULONG ulValue)
{
	APP_DRIVER_BUFFER_INFO stParam;
	DWORD result = 0;

	memset( &stParam, 0, sizeof(APP_DRIVER_BUFFER_INFO) );
	stParam.m_ulRegOffset = ulOffset;
	stParam.m_ulRegValue = ulValue;
	ULONG ulResult = 0;
	ULONG ulRetu = DeviceIoControl(m_hDevice,
					IOCTL_WIRITE_SECOND_REGISTER, 
					&stParam, sizeof(APP_DRIVER_BUFFER_INFO),
					NULL, 0,
					&ulResult, NULL);
	ASSERT(ulRetu);
	return !ulRetu;
}

ULONG CTiCoffFile::Read8MMemVal(ULONG ulOffset, ULONG &ulValue)
{
	APP_DRIVER_BUFFER_INFO stParam;
	APP_DRIVER_BUFFER_INFO stRetu;
	DWORD result = 0;

	memset( &stParam, 0, sizeof(APP_DRIVER_BUFFER_INFO) );
	memset( &stRetu, 0, sizeof(APP_DRIVER_BUFFER_INFO) );
	stParam.m_ulRegOffset = ulOffset;
	ULONG ulResult = 0;
	ULONG ulRetu = DeviceIoControl(m_hDevice,
					IOCTL_READ_SECOND_REGISTER, 
					&stParam, sizeof(APP_DRIVER_BUFFER_INFO),
					&stRetu, sizeof(APP_DRIVER_BUFFER_INFO),
					&ulResult, NULL);
	ASSERT(ulRetu);
	ulValue = stRetu.m_ulRegValue;
	return !ulRetu;
}

ULONG CTiCoffFile::WriteCoff(unsigned long ulVirtualAddr, int WriteSize, char *pBuf)
{
	ULONG ulResult = 0;
	DWORD ulRetu = DeviceIoControl(m_hDevice,
					IOCTL_WRITE_COFF, 
					pBuf, WriteSize,
					NULL,0,
					&ulResult, NULL);
	ASSERT(ulRetu);
	return !ulResult;
}
