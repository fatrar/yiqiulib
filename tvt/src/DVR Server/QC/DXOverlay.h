#if !defined(AFX_DXOVERLAY10_H__0BA204B2_CBCC_11D2_A35B_88742F5F02D8__INCLUDED_)
#define AFX_DXOVERLAY10_H__0BA204B2_CBCC_11D2_A35B_88742F5F02D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ddraw.h>

#define OVERLAY_KEYCOLOR (RGB(254,1,254))
#define INIT_DIRECTDRAW_STRUCT(x) (ZeroMemory(&x, sizeof(x)), x.dwSize=sizeof(x))
#define SAFE_RELEASE(p)				 {if(p) {(p)->Release(); (p)=NULL; } }

/****************************************************************************************
	DirectX Overlay Surface Create Function
****************************************************************************************/
BOOL InitDirectDraw(HWND hwnd);
void FreeDirectDraw(void);
HRESULT CreatePrimarySurface(void);
void HideOverlay();
void ShowOverlay();
BOOL isOverlaysSupported();

BOOL	DxT_Init(HWND hwnd);
void	DxT_Cleanup(BOOL bOverlay);
BOOL	DxT_CreateLogo();

BOOL	DxT_CreateSurface(BYTE byLiveOverlay, BYTE byDisplayFormat, BYTE *pbyImageSize, WORD wOverlayStartX, WORD wOverlayStartY, WORD wOverlayWidth, WORD wOverlayHeight, WORD *pwImageWidth, WORD *pwImageHeight);
BOOL	DxT_OverlaySetting(WORD wOverlayStartX, WORD wOverlayStartY, WORD wOverlayWidth, WORD wOverlayHeight, WORD wImageWidth, WORD wImageHeight);
BOOL	GetOverlayAddress(DWORD *pdwAddress, DWORD *pdwlPitch);
BYTE	*SetCaptureSurfaceLock(BYTE byChannel, DWORD *pdwlPitch);
void	SetCaptureSurfaceUnlock(BYTE byChannel);
BYTE	*SetLogoSurfaceLock(DWORD *pdwlPitch);
void	SetLogoSurfaceUnlock();
BOOL	SetCaptureDisplay(BYTE byChannel, RECT *pRcDst, RECT *pRcSrc, BOOL bLogo);

#endif // !defined(AFX_DXOVERLAY10_H__0BA204B2_CBCC_11D2_A35B_88742F5F02D8__INCLUDED_)
