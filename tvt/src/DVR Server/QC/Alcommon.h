// Alcommon.h: interface for the CAlcommon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALCOMMON_H__AB716628_5CA4_40F5_A731_7F48102A43A1__INCLUDED_)
#define AFX_ALCOMMON_H__AB716628_5CA4_40F5_A731_7F48102A43A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAlcommon  
{
public:
	void YUV420PLTOYUV422PK_352X240_SSE2(int nWidth, int nWidthUV, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset, DWORD dwYOffset, DWORD dwUVOffset);
	void YUV420PLTOYUV422PK_352X240_MMX(int nWidth, int nWidthUV, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset, DWORD dwYOffset, DWORD dwUVOffset);
	void YUV420PLTOYUV422PK_352X240_API(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset, DWORD dwYOffset, DWORD dwUVOffset);
	void YUV420PLTOYUV422PK_704X480_SSE2(int nWidth, int nWidthUV, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitch, DWORD dwlPitchOffset);
	void YUV420PLTOYUV422PK_704X480_MMX(int nWidth, int nWidthUV, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitch, DWORD dwlPitchOffset);
	void YUV420PLTOYUV422PK_704X480_API(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitch, DWORD dwlPitchOffset);
	void YUV411PLTOYUV422PK_352X240_SSE2(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset, DWORD dwYOffset, DWORD dwUVOffset);
	void YUV411PLTOYUV422PK_352X240_MMX(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset, DWORD dwYOffset, DWORD dwUVOffset);
	void YUV411PLTOYUV422PK_352X240_API(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset, DWORD dwYOffset, DWORD dwUVOffset);
	void YUV411PLTOYUV422PK_704X480_SSE2(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitch, DWORD dwlPitchOffset);
	void YUV411PLTOYUV422PK_704X480_MMX(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitch, DWORD dwlPitchOffset);
	void YUV411PLTOYUV422PK_704X480_API(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitch, DWORD dwlPitchOffset);
	void YUV420PLTOYUV422PK_SSE2(int nWidth, int nWidthUV, int nHeightUV, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset);
	void YUV420PLTOYUV422PK_MMX(int nWidth, int nWidthUV, int nHeightUV, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset);
	void YUV420PLTOYUV422PK_API(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset);
	void YUV411PLTOYUV422PK_SSE2(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset);
	void YUV411PLTOYUV422PK_MMX(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset);
	void YUV411PLTOYUV422PK_API(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset);
	void Memcpy_SSE2(LPVOID pvDest, LPVOID pvSrc, int nCount);
	void Memcpy_MMX(LPVOID pvDest, LPVOID pvSrc, int nCount);
	BOOL GetLogoImage(char *szFileName, WORD wWidth, WORD wHeight, BYTE *pBuffer, DWORD dwlPitch);
	BOOL DWWriteRegistryValue(HKEY hMainKey, char *pszPath, char *szName, ULONG dwData);
	BOOL DWReadRegistryValue(HKEY hMainKey, char *pszPath, char *szName, DWORD *pdwRet);
	BOOL IsSupportMMX();
	CAlcommon();
	virtual ~CAlcommon();

};

#endif // !defined(AFX_ALCOMMON_H__AB716628_5CA4_40F5_A731_7F48102A43A1__INCLUDED_)
