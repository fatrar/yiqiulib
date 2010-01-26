// Alcommon.cpp: implementation of the CAlcommon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Alcommon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
typedef void (Memcpy_SSE2_Type) (LPVOID pvDest, LPVOID pvSrc, int nCount);
extern "C" Memcpy_SSE2_Type Memcpy_SSE2;
Memcpy_SSE2_Type Memcpy_SSE2;

typedef void (YUV411PLTOYUV422PK_SSE2_Type) (int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset);
extern "C" YUV411PLTOYUV422PK_SSE2_Type YUV411PLTOYUV422PK_SSE2;
YUV411PLTOYUV422PK_SSE2_Type YUV411PLTOYUV422PK_SSE2;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAlcommon::CAlcommon()
{

}

CAlcommon::~CAlcommon()
{

}

BOOL CAlcommon::IsSupportMMX()
{
  unsigned long mmx_support;

  _asm 
  {   
		mov     eax, 1
		cpuid           //_emit   0x0f
		//_emit   0xa2
		mov     mmx_support, edx
  }
  return ((mmx_support & (0x00800000)) == (0x00800000));
}

BOOL CAlcommon::DWReadRegistryValue(HKEY hMainKey, char *pszPath, char *szName, DWORD *pdwRet)
{
	HKEY	hKey;
	ULONG	dwData=0, dwType, dwSize; 
	BOOL	bRet = FALSE;
	
	dwType = REG_DWORD;
	dwSize = sizeof(DWORD);
	if(::RegOpenKeyEx(hMainKey, (const char *)pszPath, 0, KEY_ENUMERATE_SUB_KEYS|KEY_READ|KEY_READ, &hKey))
	{
		return bRet; 
	}

	if(::RegQueryValueEx(hKey, szName, NULL, &dwType, (unsigned char *)&dwData, &dwSize) == ERROR_SUCCESS)
		bRet = TRUE;
	::RegCloseKey(hKey); 
	*pdwRet = dwData;
	return bRet;
}

BOOL CAlcommon::DWWriteRegistryValue(HKEY hMainKey, char *pszPath, char *szName, ULONG dwData)
{
	HKEY	hKey;
	ULONG	dwType, dwSize, dwRet; 
	BOOL	bRet = FALSE;
	
	dwType = REG_DWORD;
	dwSize = sizeof(DWORD);
	if(::RegOpenKeyEx(hMainKey, (const char *)pszPath, 0, KEY_ENUMERATE_SUB_KEYS|KEY_READ|KEY_WRITE, &hKey)) 
	{
		::RegCreateKeyEx(hMainKey, (const char *)pszPath, 0, REG_NONE, REG_OPTION_NON_VOLATILE, 
								KEY_ENUMERATE_SUB_KEYS | KEY_WRITE|KEY_READ, NULL, &hKey, &dwRet);  
	}
	if(::RegSetValueEx(hKey, szName, 0, dwType, (unsigned char *)&dwData, dwSize) == ERROR_SUCCESS)
		bRet = TRUE;
	::RegCloseKey(hKey);
	return bRet;
}

BOOL CAlcommon::GetLogoImage(char *szFileName, WORD wWidth, WORD wHeight, BYTE *pBuffer, DWORD dwlPitch)
{
	HDC      hdcImage;
	HBITMAP	hbm;	
	BITMAP	bm;	
	int		x, y;
	BYTE		*pSurf;
	DWORD    dwBytesInRow;
	BYTE     R,G,B, Y0,Y1,U,V;
	COLORREF	color;
	BOOL		bRet = FALSE;
	
	pSurf = (BYTE *)pBuffer; 
	hbm = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, wWidth, wHeight, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	if(!hbm)
	{
		return bRet;
	}
	hdcImage = ::CreateCompatibleDC(NULL);
	::SelectObject(hdcImage, hbm);
	::GetObject(hbm, sizeof(bm), &bm);

	dwBytesInRow = bm.bmWidth*2;

	for(y=0; y<bm.bmHeight; y++)
	{
		for(x=0; x<bm.bmWidth; x+=2)
		{
		   color = GetPixel(hdcImage, x, y);
			R = GetRValue(color);
			G = GetGValue(color);
			B = GetBValue(color);
			Y0= (BYTE)(0.299*R + 0.587*G + 0.114*B);
			U = (BYTE)(128.0 - 0.1687*R - 0.3313*G + 0.5*B);
			
			color = GetPixel(hdcImage, x+1,y);
			R = GetRValue(color);
			G = GetGValue(color);
			B = GetBValue(color);
			Y1= (BYTE)(0.299*R + 0.587*G + 0.114*B);
			V = (BYTE)(128.0 + 0.5*R - 0.4187*G - 0.0813*B);
			
			*(pSurf++) = Y0;
			*(pSurf++) = U;
			*(pSurf++) = Y1;
			*(pSurf++) = V;
		}
		pSurf += (dwlPitch - dwBytesInRow);
	}
	::DeleteDC(hdcImage);
	::DeleteObject(hbm);
	bRet = TRUE;
	return bRet;
}

void CAlcommon::Memcpy_MMX(LPVOID pvDest, LPVOID pvSrc, int nCount)
{
	__asm 
	{
		mov     ecx, nCount
		shr     ecx, 3		
		test    ecx, ecx	
		jz     short _toend
			
		push    edx
			
		mov     ebx, pvSrc
		mov     edx, pvDest
		mov     eax, 0
			
_loop00 :
		movq    mm0, [ebx+eax*8]
		movq    [edx+eax*8], mm0
		inc     eax
		loop    _loop00
			
		emms
		pop     edx		
			
_toend :
	}
}

void CAlcommon::Memcpy_SSE2(LPVOID pvDest, LPVOID pvSrc, int nCount)
{
	Memcpy_SSE2(pvDest, pvSrc, nCount);
}

void CAlcommon::YUV411PLTOYUV422PK_API(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset)
{
	BYTE		y, cb, cr;
	WORD		wData;
	DWORD		dw411YCount = 0, dw422Count = 0;
	WORD		*pwYUV422;
	int		i, j;
	
	pwYUV422 = (WORD *)pYUV422;
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			y  = pImgY[dw411YCount];
			if((dw411YCount%2) == 0)
			{
				cb = pImgU[i*nWidth/4 + j/4];
				wData = y | (cb << 8);
			}
			else
			{
				cr = pImgV[i*nWidth/4 + j/4];
				wData = y | (cr << 8);
			}
			pwYUV422[dw422Count++] = wData;
			dw411YCount++;
		}
		dw422Count += dwlPitchOffset/2;
	}
}

void CAlcommon::YUV411PLTOYUV422PK_MMX(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset)
{
	__asm
	{
		mov				eax, pImgY;				//	load source Y ptr
		mov				ebx, pImgU;				//	load source U ptr
		mov				edx, pImgV;				//	load source V ptr
		mov				edi, pYUV422;			//	load destination ptr
		mov				esi, nHeight;

_loopHeight:
		mov				ecx, nWidth;			// 704 >> 4

_loopWidth:		
		movd				mm5, [ebx];				//	[00][00][00][00][U3][U2][U1][U0]		
		pxor				mm7, mm7;				// [00][00][00][00][00][00][00][00]		//zero mm7
		punpcklbw		mm5, mm7;				// [00][U3][00][U2][00][U1][00][U0]		
		movq				mm1, mm5;				//
		psllw				mm5, 8;					// mm5 << 8;
		paddsw			mm1, mm5;				// [U3][U3][U2][U2][U1][U1][U0][U0]

		movd				mm6, [edx];				//	[00][00][00][00][V3][V2][V1][V0]
		punpcklbw		mm6, mm7;				// [00][V3][00][V2][00][V1][00][V0]
		movq				mm4, mm6;				//
		psllw				mm6, 8;					// mm6, << 8;
		paddsw			mm4, mm6;				// [V3][V3][V2][V2][V1][V1][V0][V0]
		
		movq				mm0, mm1;				
		punpcklbw		mm0, mm4;				// [V1][U1][V1][U1][V0][U0][V0][U0]
		punpckhbw		mm1, mm4;				// [V3][U3][V3][U3][V2][U2][V2][U2]
				
		movq				mm4, [eax + 0];		//	Y7-Y0
		movq				mm5, mm4;				//	Y7-Y0
		punpcklbw		mm4, mm0;				//	result1 : [V0][Y3][U0][Y2][V0][Y1][U0][Y0]
		punpckhbw		mm5, mm0;				//	result2 : [V1][Y7][U1][Y6][V1][Y5][U1][Y4]
		movq				[edi], mm4;				//	saving result1
		movq				[edi + 8], mm5;		//	saving result2
				
		movq				mm4, [eax + 8];		//	YF-Y8
		movq				mm5, mm4;				//	YF-Y8
		punpcklbw		mm4, mm1;				//	result3 : [V2][YB][U2][YA][V2][Y9][U2][Y8]
		punpckhbw		mm5, mm1;				//	result4 : [V3][YF][U3][YE][V3][YD][U3][YC]
		movq				[edi + 16], mm4;		//	saving result3
		movq				[edi + 24], mm5;		//	saving result4
		
		add				eax, 16;					//	Y
		add				ebx, 4;					//	U
		add				edx, 4;					//	V
		add				edi, 32;					//	pbyOut
		loop				_loopWidth;				//
		
		add				edi, dwlPitchOffset;
		dec				esi;						//esi, esi-1
		jnz				_loopHeight;			//0이면 분기
		emms;
	}
}

void CAlcommon::YUV411PLTOYUV422PK_SSE2(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset)
{
	YUV411PLTOYUV422PK_SSE2(nWidth, nHeight, pImgY, pImgU, pImgV, pYUV422, dwlPitchOffset);
}

void CAlcommon::YUV420PLTOYUV422PK_API(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset)
{
	BYTE		y, cb, cr;
	WORD		wData;
	DWORD		dw411YCount = 0, dw422Count = 0;
	WORD		*pwYUV422;
	int		i, j;
	
	pwYUV422 = (WORD *)pYUV422;
	for(i=0; i<nHeight; i++) 
	{
		for(j=0; j<nWidth; j++)
		{
			y  = pImgY[dw411YCount];
			if((dw411YCount%2) == 0)
			{
				cb = pImgU[(i/2)*nWidth/2 + j/2];
				wData = y | (cb << 8);
			}
			else
			{
				cr = pImgV[(i/2)*nWidth/2 + j/2];
				wData = y | (cr << 8);
			}
			pwYUV422[dw422Count++] = wData;
			dw411YCount++;
		}
		dw422Count += dwlPitchOffset/2;
	}
}

void CAlcommon::YUV420PLTOYUV422PK_MMX(int nWidth, int nWidthUV, int nHeightUV, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset)
{
	__asm
	{
		mov				eax, pImgY;				//	load source Y ptr
		mov				ebx, pImgU;				//	load source U ptr
		mov				edx, pImgV;				//	load source V ptr
		mov				edi, pYUV422;			//	load destination ptr
		
		mov				esi, nHeightUV;
_loopHeight:
		
		mov				ecx, nWidth;
_loopWidth1:
		movq				mm0, [ebx];				//	U7-U0
		movq				mm1, mm0;				//	U7-U0
		movq				mm7, [edx];				//	V7-V0
		
		punpcklbw		mm0, mm7;				//	[V3][U3][V2][U2][V1][U1][V0][U0]	 [V1][U1][V1][U1][V0][U0][V0][U0]
		punpckhbw		mm1, mm7;				//	[V7][U7][V6][U6][V5][U5][V4][U4]
		
		
		movq				mm2, [eax];				//	Y7-Y0
		movq				mm3, mm2;				//	Y7-Y0
		punpcklbw		mm2, mm0;				//	result1 : [V1][Y3][U1][Y2][V0][Y1][U0][Y0]
		punpckhbw		mm3, mm0;				//	result2 : [V3][Y7][U3][Y6][V2][Y5][U2][Y4]
		movq				[edi], mm2;				//	saving result1
		movq				[edi + 8], mm3;		//	saving result2
		
		
		movq				mm4, [eax + 8];		//	YF-Y8
		movq				mm5, mm4;				//	YF-Y8
		punpcklbw		mm4, mm1;				//	result3 : [V5][YB][U5][YA][V4][Y9][U4][Y8]
		punpckhbw		mm5, mm1;				//	result4 : [V7][YF][U7][YE][V6][YD][U6][YC]
		movq				[edi + 16], mm4;		//	saving result3
		movq				[edi + 24], mm5;		//	saving result4
		
		add				eax, 16;					//	Y
		add				ebx, 8;					//	U
		add				edx, 8;					//	V
		add				edi, 32;					//	pbyOut
		
		loop				_loopWidth1;
		
		add				edi, dwlPitchOffset;
		sub				ebx, nWidthUV;
		sub				edx, nWidthUV;
		
		mov				ecx, nWidth;
_loopWidth2:
		
		movq				mm0, [ebx];				//	U7-U0
		movq				mm1, mm0;				//	U7-U0
		movq				mm7, [edx];				//	V7-V0
		
		punpcklbw		mm0, mm7;				//	[V3][U3][V2][U2][V1][U1][V0][U0]
		punpckhbw		mm1, mm7;				//	[V7][U7][V6][U6][V5][U5][V4][U4]
		
		
		movq				mm2, [eax];				//	Y7-Y0
		movq				mm3, mm2;				//	Y7-Y0
		punpcklbw		mm2, mm0;				//	result1 : [V1][Y3][U1][Y2][V0][Y1][U0][Y0]
		punpckhbw		mm3, mm0;				//	result2 : [V3][Y7][U3][Y6][V2][Y5][U2][Y4]
		movq				[edi], mm2;				//	saving result1
		movq				[edi + 8], mm3;		//	saving result2
				
		movq				mm4, [eax + 8];		//	YF-Y8
		movq				mm5, mm4;				//	YF-Y8
		punpcklbw		mm4, mm1;				//	result3 : [V5][YB][U5][YA][V4][Y9][U4][Y8]
		punpckhbw		mm5, mm1;				//	result4 : [V7][YF][U7][YE][V6][YD][U6][YC]
		movq				[edi + 16], mm4;		//	saving result3
		movq				[edi + 24], mm5;		//	saving result4
				
		add				eax, 16;					//	Y
		add				ebx, 8;					//	U
		add				edx, 8;					//	V
		add				edi, 32;					//	pbyOut
		
		loop				_loopWidth2;
		
		add				edi, dwlPitchOffset;
		dec				esi;
		jnz				_loopHeight;
		
		emms;
	}
}

void CAlcommon::YUV420PLTOYUV422PK_SSE2(int nWidth, int nWidthUV, int nHeightUV, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset)
{
	//YUV420PLTOYUV422PK_SSE2(nWidth, nWidthUV, nHeightUV, pImgY, pImgU, pImgV, pYUV422, dwlPitchOffset);
}

void CAlcommon::YUV411PLTOYUV422PK_704X480_API(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitch, DWORD dwlPitchOffset)
{
	BYTE		y, cb, cr;
	WORD		wData;
	DWORD		dw411YCount = 0, dw422Count = 0;
	WORD		*pwYUV422;
	int		i, j;

	pwYUV422 = (WORD *)pYUV422;
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			y  = pImgY[dw411YCount];
			if((dw411YCount%2) == 0)
			{
				cb = pImgU[i*nWidth/4 + j/4];
				wData = y | (cb << 8);
			}
			else
			{
				cr = pImgV[i*nWidth/4 + j/4];
				wData = y | (cr << 8);
			}
			pwYUV422[dw422Count++] = wData;
			dw411YCount++;
		}
		dw422Count += dwlPitchOffset/2;
		dw422Count += (dwlPitch)/2;
	}
}

void CAlcommon::YUV411PLTOYUV422PK_704X480_MMX(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitch, DWORD dwlPitchOffset)
{
	__asm
	{
		mov				eax, pImgY;			//	load source Y ptr
		mov				ebx, pImgU;				//	load source U ptr
		mov				edx, pImgV;				//	load source V ptr
		mov				edi, pYUV422;
		mov				esi, nHeight;		

_loopHeight:
		mov				ecx, nWidth;		

_loopWidth:		
		movd				mm5, [ebx];				//	[00][00][00][00][U3][U2][U1][U0]		
		pxor				mm7, mm7;				// [00][00][00][00][00][00][00][00]		//zero mm7
		punpcklbw		mm5, mm7;				// [00][U3][00][U2][00][U1][00][U0]		
		movq				mm1, mm5;				//
		psllw				mm5, 8;					// mm5 << 8;
		paddsw			mm1, mm5;				// [U3][U3][U2][U2][U1][U1][U0][U0]

		movd				mm6, [edx];				//	[00][00][00][00][V3][V2][V1][V0]
		punpcklbw		mm6, mm7;				// [00][V3][00][V2][00][V1][00][V0]
		movq				mm4, mm6;				//
		psllw				mm6, 8;					// mm6 << 8;
		paddsw			mm4, mm6;				// [V3][V3][V2][V2][V1][V1][V0][V0]
		
		movq				mm0, mm1;				
		punpcklbw		mm0, mm4;				// [V1][U1][V1][U1][V0][U0][V0][U0]
		punpckhbw		mm1, mm4;				// [V3][U3][V3][U3][V2][U2][V2][U2]
				
		movq				mm4, [eax + 0];		//	Y7-Y0
		movq				mm5, mm4;				//	Y7-Y0
		punpcklbw		mm4, mm0;				//	result1 : [V0][Y3][U0][Y2][V0][Y1][U0][Y0]
		punpckhbw		mm5, mm0;				//	result2 : [V1][Y7][U1][Y6][V1][Y5][U1][Y4]
		movq				[edi], mm4;				//	saving result1
		movq				[edi + 8], mm5;		//	saving result2
				
		movq				mm4, [eax + 8];		//	YF-Y8
		movq				mm5, mm4;				//	YF-Y8
		punpcklbw		mm4, mm1;				//	result3 : [V2][YB][U2][YA][V2][Y9][U2][Y8]
		punpckhbw		mm5, mm1;				//	result4 : [V3][YF][U3][YE][V3][YD][U3][YC]
		movq				[edi + 16], mm4;		//	saving result3
		movq				[edi + 24], mm5;		//	saving result4
		
		add				eax, 16;					//	Y
		add				ebx, 4;					//	U
		add				edx, 4;					//	V
		add				edi, 32;					//	pbyOut
		loop				_loopWidth;				//
		
		add				edi, dwlPitchOffset;
		add				edi, dwlPitch;
		dec				esi;						//esi, esi-1
		jnz				_loopHeight;			//0이면 분기
		emms;
	}
}

void CAlcommon::YUV411PLTOYUV422PK_704X480_SSE2(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitch, DWORD dwlPitchOffset)
{
	//YUV411PLTOYUV422PK_704X480_SSE2(nWidth, nHeight, pImgY, pImgU, pImgV, pYUV422, dwlPitch, dwlPitchOffset);
}

void CAlcommon::YUV411PLTOYUV422PK_352X240_API(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset, DWORD dwYOffset, DWORD dwUVOffset)
{
	BYTE		y, cb, cr;
	WORD		wData;
	DWORD		dw411YCount = 0, dw422Count = 0;
	WORD		*pwYUV422;
	int		i, j;
	
	pwYUV422 = (WORD *)pYUV422;
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			y  = pImgY[dw411YCount + i*dwYOffset];
			if((dw411YCount%2) == 0)
			{
				cb = pImgU[i*nWidth/4 + j/4 + i*dwUVOffset];
				wData = y | (cb << 8);
			}
			else
			{
				cr = pImgV[i*nWidth/4 + j/4 + i*dwUVOffset];
				wData = y | (cr << 8);
			}
			pwYUV422[dw422Count++] = wData;
			dw411YCount++;
		}
		dw422Count += dwlPitchOffset/2;
	}
}

void CAlcommon::YUV411PLTOYUV422PK_352X240_MMX(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset, DWORD dwYOffset, DWORD dwUVOffset)
{
	__asm
	{
		mov				eax, pImgY;				//	load source Y ptr
		mov				ebx, pImgU;				//	load source U ptr
		mov				edx, pImgV;				//	load source V ptr
		mov				edi, pYUV422;			//	load destination ptr
		mov				esi, nHeight;

_loopHeight:
		mov				ecx, nWidth;		

_loopWidth:		
		movd				mm5, [ebx];				//	[00][00][00][00][U3][U2][U1][U0]		
		pxor				mm7, mm7;				// [00][00][00][00][00][00][00][00]		//zero mm7
		punpcklbw		mm5, mm7;				// [00][U3][00][U2][00][U1][00][U0]		
		movq				mm1, mm5;				//
		psllw				mm5, 8;					// mm5 << 8;
		paddsw			mm1, mm5;				// [U3][U3][U2][U2][U1][U1][U0][U0]

		movd				mm6, [edx];				//	[00][00][00][00][V3][V2][V1][V0]
		punpcklbw		mm6, mm7;				// [00][V3][00][V2][00][V1][00][V0]
		movq				mm4, mm6;				//
		psllw				mm6, 8;					// mm6 << 8;
		paddsw			mm4, mm6;				// [V3][V3][V2][V2][V1][V1][V0][V0]
		
		movq				mm0, mm1;				
		punpcklbw		mm0, mm4;				// [V1][U1][V1][U1][V0][U0][V0][U0]
		punpckhbw		mm1, mm4;				// [V3][U3][V3][U3][V2][U2][V2][U2]
				
		movq				mm4, [eax + 0];		//	Y7-Y0
		movq				mm5, mm4;				//	Y7-Y0
		punpcklbw		mm4, mm0;				//	result1 : [V0][Y3][U0][Y2][V0][Y1][U0][Y0]
		punpckhbw		mm5, mm0;				//	result2 : [V1][Y7][U1][Y6][V1][Y5][U1][Y4]
		movq				[edi], mm4;				//	saving result1
		movq				[edi + 8], mm5;		//	saving result2
				
		movq				mm4, [eax + 8];		//	YF-Y8
		movq				mm5, mm4;				//	YF-Y8
		punpcklbw		mm4, mm1;				//	result3 : [V2][YB][U2][YA][V2][Y9][U2][Y8]
		punpckhbw		mm5, mm1;				//	result4 : [V3][YF][U3][YE][V3][YD][U3][YC]
		movq				[edi + 16], mm4;		//	saving result3
		movq				[edi + 24], mm5;		//	saving result4
		
		add				eax, 16;					//	Y
		add				ebx, 4;					//	U
		add				edx, 4;					//	V
		add				edi, 32;					//	pbyOut
		loop				_loopWidth;				//
				
		add				edi, dwlPitchOffset;
		add				eax, dwYOffset;
		add				ebx, dwUVOffset;
		add				edx, dwUVOffset;
		dec				esi;						//esi, esi-1
		jnz				_loopHeight;			//0이면 분기
		emms;
	}
}

void CAlcommon::YUV411PLTOYUV422PK_352X240_SSE2(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset, DWORD dwYOffset, DWORD dwUVOffset)
{
//YUV411PLTOYUV422PK_352X240_SSE2(nWidth, nHeight, pImgY, pImgU, pImgV, pYUV422, dwlPitchOffset, dwYOffset, dwUVOffset);
}

void CAlcommon::YUV420PLTOYUV422PK_704X480_API(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitch, DWORD dwlPitchOffset)
{
	BYTE		y, cb, cr;
	WORD		wData;
	DWORD		dw411YCount = 0, dw422Count = 0;
	WORD		*pwYUV422;
	int		i, j;

	pwYUV422 = (WORD *)pYUV422;
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			y  = pImgY[dw411YCount];
			if((dw411YCount%2) == 0)
			{
				cb = pImgU[i*nWidth/2 + j/2];
				wData = y | (cb << 8);
			}
			else
			{
				cr = pImgV[i*nWidth/2 + j/2];
				wData = y | (cr << 8);
			}
			pwYUV422[dw422Count++] = wData;
			dw411YCount++;
		}
		dw422Count += dwlPitchOffset/2;
		dw422Count += (dwlPitch)/2;
	}
}

void CAlcommon::YUV420PLTOYUV422PK_704X480_MMX(int nWidth, int nWidthUV, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitch, DWORD dwlPitchOffset)
{
	__asm
	{
		mov				eax, pImgY;			//	load source Y ptr
		mov				ebx, pImgU;				//	load source U ptr
		mov				edx, pImgV;				//	load source V ptr
		mov				edi, pYUV422;			//	load destination ptr
		
		mov				esi, nHeight;
_loopHeight:
		
		mov				ecx, nWidth;
_loopWidth1:
		movq				mm0, [ebx];				//	U7-U0
		movq				mm1, mm0;				//	U7-U0
		movq				mm7, [edx];				//	V7-V0
		
		punpcklbw		mm0, mm7;				//	[V3][U3][V2][U2][V1][U1][V0][U0]	 [V1][U1][V1][U1][V0][U0][V0][U0]
		punpckhbw		mm1, mm7;				//	[V7][U7][V6][U6][V5][U5][V4][U4]
		
		
		movq				mm2, [eax];				//	Y7-Y0
		movq				mm3, mm2;				//	Y7-Y0
		punpcklbw		mm2, mm0;				//	result1 : [V1][Y3][U1][Y2][V0][Y1][U0][Y0]
		punpckhbw		mm3, mm0;				//	result2 : [V3][Y7][U3][Y6][V2][Y5][U2][Y4]
		movq				[edi], mm2;				//	saving result1
		movq				[edi + 8], mm3;		//	saving result2
		
		
		movq				mm4, [eax + 8];		//	YF-Y8
		movq				mm5, mm4;				//	YF-Y8
		punpcklbw		mm4, mm1;				//	result3 : [V5][YB][U5][YA][V4][Y9][U4][Y8]
		punpckhbw		mm5, mm1;				//	result4 : [V7][YF][U7][YE][V6][YD][U6][YC]
		movq				[edi + 16], mm4;		//	saving result3
		movq				[edi + 24], mm5;		//	saving result4
		
		add				eax, 16;					//	Y
		add				ebx, 8;					//	U
		add				edx, 8;					//	V
		add				edi, 32;					//	pbyOut
		
		loop				_loopWidth1;
		
		add				edi, dwlPitchOffset;
		add				edi, dwlPitch;
		sub				ebx, nWidthUV;
		sub				edx, nWidthUV;
		
		mov				ecx, nWidth;
_loopWidth2:
		
		movq				mm0, [ebx];				//	U7-U0
		movq				mm1, mm0;				//	U7-U0
		movq				mm7, [edx];				//	V7-V0
		
		punpcklbw		mm0, mm7;				//	[V3][U3][V2][U2][V1][U1][V0][U0]
		punpckhbw		mm1, mm7;				//	[V7][U7][V6][U6][V5][U5][V4][U4]
		
		
		movq				mm2, [eax];				//	Y7-Y0
		movq				mm3, mm2;				//	Y7-Y0
		punpcklbw		mm2, mm0;				//	result1 : [V1][Y3][U1][Y2][V0][Y1][U0][Y0]
		punpckhbw		mm3, mm0;				//	result2 : [V3][Y7][U3][Y6][V2][Y5][U2][Y4]
		movq				[edi], mm2;				//	saving result1
		movq				[edi + 8], mm3;		//	saving result2
				
		movq				mm4, [eax + 8];		//	YF-Y8
		movq				mm5, mm4;				//	YF-Y8
		punpcklbw		mm4, mm1;				//	result3 : [V5][YB][U5][YA][V4][Y9][U4][Y8]
		punpckhbw		mm5, mm1;				//	result4 : [V7][YF][U7][YE][V6][YD][U6][YC]
		movq				[edi + 16], mm4;		//	saving result3
		movq				[edi + 24], mm5;		//	saving result4
				
		add				eax, 16;					//	Y
		add				ebx, 8;					//	U
		add				edx, 8;					//	V
		add				edi, 32;					//	pbyOut
		
		loop				_loopWidth2;
		
		add				edi, dwlPitchOffset;
		add				edi, dwlPitch;
		dec				esi;
		jnz				_loopHeight;
		
		emms;
	}
}

void CAlcommon::YUV420PLTOYUV422PK_704X480_SSE2(int nWidth, int nWidthUV, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitch, DWORD dwlPitchOffset)
{
//YUV420PLTOYUV422PK_704X480_SSE2(nWidth, nWidthUV, nHeight, pImgY, pImgU, pImgV, pYUV422, dwlPitch, dwlPitchOffset);
}

void CAlcommon::YUV420PLTOYUV422PK_352X240_API(int nWidth, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset, DWORD dwYOffset, DWORD dwUVOffset)
{
	BYTE		y, cb, cr;
	WORD		wData;
	DWORD		dw411YCount = 0, dw422Count = 0;
	WORD		*pwYUV422;
	int		i, j;
	
	pwYUV422 = (WORD *)pYUV422;
	for(i=0; i<nHeight; i++) 
	{
		for(j=0; j<nWidth; j++)
		{
			y  = pImgY[dw411YCount + i*dwYOffset];
			if((dw411YCount%2) == 0)
			{
				cb = pImgU[(i/2)*nWidth/2 + j/2 + i*dwUVOffset];
				wData = y | (cb << 8);
			}
			else
			{
				cr = pImgV[(i/2)*nWidth/2 + j/2 + i*dwUVOffset];
				wData = y | (cr << 8);
			}
			pwYUV422[dw422Count++] = wData;
			dw411YCount++;
		}
		dw422Count += dwlPitchOffset/2;
	}
}

void CAlcommon::YUV420PLTOYUV422PK_352X240_MMX(int nWidth, int nWidthUV, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset, DWORD dwYOffset, DWORD dwUVOffset)
{
	__asm
	{
		mov				eax, pImgY;				//	load source Y ptr
		mov				ebx, pImgU;				//	load source U ptr
		mov				edx, pImgV;				//	load source V ptr
		mov				edi, pYUV422;			//	load destination ptr

		mov				esi, nHeight;
_loopHeight:
		
		mov				ecx, nWidth;
_loopWidth1:
		movq				mm0, [ebx];				//	U7-U0
		movq				mm1, mm0;				//	U7-U0
		movq				mm7, [edx];				//	V7-V0
		
		punpcklbw		mm0, mm7;				//	[V3][U3][V2][U2][V1][U1][V0][U0]	 [V1][U1][V1][U1][V0][U0][V0][U0]
		punpckhbw		mm1, mm7;				//	[V7][U7][V6][U6][V5][U5][V4][U4]
				
		movq				mm2, [eax];				//	Y7-Y0
		movq				mm3, mm2;				//	Y7-Y0
		punpcklbw		mm2, mm0;				//	result1 : [V1][Y3][U1][Y2][V0][Y1][U0][Y0]
		punpckhbw		mm3, mm0;				//	result2 : [V3][Y7][U3][Y6][V2][Y5][U2][Y4]
		movq				[edi], mm2;				//	saving result1
		movq				[edi + 8], mm3;		//	saving result2
		
		
		movq				mm4, [eax + 8];		//	YF-Y8
		movq				mm5, mm4;				//	YF-Y8
		punpcklbw		mm4, mm1;				//	result3 : [V5][YB][U5][YA][V4][Y9][U4][Y8]
		punpckhbw		mm5, mm1;				//	result4 : [V7][YF][U7][YE][V6][YD][U6][YC]
		movq				[edi + 16], mm4;		//	saving result3
		movq				[edi + 24], mm5;		//	saving result4
		
		add				eax, 16;					//	Y
		add				ebx, 8;					//	U
		add				edx, 8;					//	V
		add				edi, 32;					//	pbyOut
		
		loop				_loopWidth1;

		add				edi, dwlPitchOffset;
		add				eax, dwYOffset;
		sub				ebx, nWidthUV;
		sub				edx, nWidthUV;

		mov				ecx, nWidth;
_loopWidth2:
		
		movq				mm0, [ebx];				//	U7-U0
		movq				mm1, mm0;				//	U7-U0
		movq				mm7, [edx];				//	V7-V0
		
		punpcklbw		mm0, mm7;				//	[V3][U3][V2][U2][V1][U1][V0][U0]
		punpckhbw		mm1, mm7;				//	[V7][U7][V6][U6][V5][U5][V4][U4]
		
		
		movq				mm2, [eax];				//	Y7-Y0
		movq				mm3, mm2;				//	Y7-Y0
		punpcklbw		mm2, mm0;				//	result1 : [V1][Y3][U1][Y2][V0][Y1][U0][Y0]
		punpckhbw		mm3, mm0;				//	result2 : [V3][Y7][U3][Y6][V2][Y5][U2][Y4]
		movq				[edi], mm2;				//	saving result1
		movq				[edi + 8], mm3;		//	saving result2
				
		movq				mm4, [eax + 8];		//	YF-Y8
		movq				mm5, mm4;				//	YF-Y8
		punpcklbw		mm4, mm1;				//	result3 : [V5][YB][U5][YA][V4][Y9][U4][Y8]
		punpckhbw		mm5, mm1;				//	result4 : [V7][YF][U7][YE][V6][YD][U6][YC]
		movq				[edi + 16], mm4;		//	saving result3
		movq				[edi + 24], mm5;		//	saving result4
				
		add				eax, 16;					//	Y
		add				ebx, 8;					//	U
		add				edx, 8;					//	V
		add				edi, 32;					//	pbyOut
		
		loop				_loopWidth2;
		
		add				edi, dwlPitchOffset;
		add				eax, dwYOffset;
		
		add				ebx, dwUVOffset;
		add				edx, dwUVOffset;

		dec				esi;
		jnz				_loopHeight;
		
		emms;
	}
}

void CAlcommon::YUV420PLTOYUV422PK_352X240_SSE2(int nWidth, int nWidthUV, int nHeight, BYTE *pImgY, BYTE *pImgU, BYTE *pImgV, BYTE *pYUV422, DWORD dwlPitchOffset, DWORD dwYOffset, DWORD dwUVOffset)
{
	//YUV420PLTOYUV422PK_352X240_SSE2(nWidth, nWidthUV, nHeight, pImgY, pImgU, pImgV, pYUV422, dwlPitchOffset, dwYOffset, dwUVOffset);
}
