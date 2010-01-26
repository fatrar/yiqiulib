#include "stdafx.h"
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

//=========================================================================================================================//
// Windows Header Files:
#include <windows.h>
#include <mmsystem.h>
#define INITGUID
#include <objbase.h>
#include <initguid.h>
#include "ddraw.h"
#include <stdio.h>
#include "DXOverlay.h"
#include "Resource.h"
#include "./head/DefineBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//=========================================================================================================================//
LPDIRECTDRAW7        g_pDD          = NULL;
LPDIRECTDRAWSURFACE7 g_pDDSPrimary  = NULL;
LPDIRECTDRAWSURFACE7 g_pDDSOverlay  = NULL;
LPDIRECTDRAWSURFACE7 g_pDDSCapture[MAX_VIDEO_CHANNEL];
LPDIRECTDRAWSURFACE7 g_pDDSLogo		= NULL;
DDCAPS               g_ddcaps;
DDOVERLAYFX          g_OverlayFX;
DWORD                g_dwOverlayFlags     = 0;
HWND						g_hWnd;

WORD						g_wOverlayStartX;
WORD						g_wOverlayStartY;
RECT						g_rsOverlay, g_rdOverlay;
DEVMODE					g_FirstDM;

//=========================================================================================================================//
DDPIXELFORMAT g_ddpfOverlayFormats[] = 
{
	{sizeof(DDPIXELFORMAT), DDPF_FOURCC,MAKEFOURCC('U','Y','V','Y'),0,0,0,0,0},
	{sizeof(DDPIXELFORMAT), DDPF_FOURCC,MAKEFOURCC('Y','U','Y','2'),0,0,0,0,0},	
	{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16,  0xF800, 0x07e0, 0x001F, 0}
};

//=========================================================================================================================//
// Overlay Control Variable
extern char	szDebug[256];

//=========================================================================================================================//
void FreeDirectDraw(void)
{	
	int i;
	
	for(i=0; i<MAX_VIDEO_CHANNEL; i++)
		SAFE_RELEASE(g_pDDSCapture[i]);
	SAFE_RELEASE(g_pDDSOverlay);
	SAFE_RELEASE(g_pDDSLogo);
	SAFE_RELEASE(g_pDDSPrimary);
	SAFE_RELEASE(g_pDD);
}

//=========================================================================================================================//
HRESULT CreatePrimarySurface()
{
	DDSURFACEDESC2 ddsd;
	HRESULT        hr;
	
	if(!g_pDD)
		return E_FAIL;
	
	INIT_DIRECTDRAW_STRUCT(ddsd);
	ddsd.dwFlags           = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE;
	hr = g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, NULL);	
	if(FAILED(hr))
	{
		OutputDebugString("g_pDD->CreateSurface g_pDDSPrimary fail\n");
		return hr;
	}	
	return hr;
}

//=========================================================================================================================//
BOOL InitDirectDraw(HWND hWnd)
{
	HRESULT         hr;
	
	hr = DirectDrawCreateEx(NULL, (void**)&g_pDD, IID_IDirectDraw7, NULL);
	if(FAILED(hr))
	{
		OutputDebugString("DirectDrawCreateEx fail\n");
		goto ErrorOut;
	}
	
	hr = g_pDD->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
	if(FAILED(hr))
	{
		OutputDebugString("g_pDD->SetCooperativeLevel fail1\n");
		goto ErrorOut;	
	}

	ZeroMemory(&g_ddcaps, sizeof(g_ddcaps));
   g_ddcaps.dwSize = sizeof(g_ddcaps);
   if(FAILED(hr = g_pDD->GetCaps(&g_ddcaps, NULL)))
	{
		OutputDebugString("g_pDD->GetCaps fail\n");
		goto ErrorOut;
	}

	hr = CreatePrimarySurface();
	if(FAILED(hr))
	{
		OutputDebugString("CreatePrimarySurface() fail\n");
		goto ErrorOut;	
	}
		
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &g_FirstDM);
	g_hWnd = hWnd;
	return TRUE;

ErrorOut:
	FreeDirectDraw();
	return FALSE;
}

//g_pDDSPrimary->Blt(pRcDst, g_pDDSCapture[byChannel], pRcSrc, DDBLT_WAIT, NULL);
//=========================================================================================================================//
void HideOverlay()
{
	if(g_pDDSOverlay)
		g_pDDSOverlay->UpdateOverlay(NULL, g_pDDSPrimary, NULL, DDOVER_HIDE, NULL);
}

//=========================================================================================================================//
void ShowOverlay()
{
	if(g_pDDSOverlay)
	{
		//g_rdOverlay.bottom+=20;
	//	g_rdOverlay.right +=20;
		HRESULT hr = g_pDDSOverlay->UpdateOverlay(&g_rsOverlay, g_pDDSPrimary, &g_rdOverlay, g_dwOverlayFlags, &g_OverlayFX);
	}
}

//=========================================================================================================================//
BOOL IsOverlaysSupported()
{
	DDCAPS  capsDrv;
	HRESULT hr;
	
	INIT_DIRECTDRAW_STRUCT(capsDrv);
	hr = g_pDD->GetCaps(&capsDrv, NULL);
	if(FAILED(hr))
	{
		AfxMessageBox("Direct Overary Init Error! FAILED(hr) \n");
		return FALSE;
	}
	if(!(capsDrv.dwCaps & DDCAPS_OVERLAY) || !(capsDrv.dwCaps & DDCAPS_OVERLAYSTRETCH))
	{
		AfxMessageBox("Direct Overary Init Error! !(capsDrv.dwCaps & DDCAPS_OVERLAY) \n");
		return FALSE;
	}	
	return TRUE;
}

//=========================================================================================================================//
BOOL DxT_Init(HWND hwnd)
{
	if(!InitDirectDraw(hwnd))
	{
		AfxMessageBox("Direct Overary Init Error! !InitDirectDraw() \n");
		return FALSE;
	}	
	if(!IsOverlaysSupported())
	{
		AfxMessageBox("Direct Overary Init Error! !IsOverlaysSupported() \n");
		return FALSE;
	}	
	return TRUE;
}

//=========================================================================================================================//
void DxT_Cleanup(BOOL bOverlay)
{
	int i;

	if(bOverlay == VIDEO_LIVE)
	{
		HideOverlay();
		SAFE_RELEASE(g_pDDSOverlay);
	}
	else if(bOverlay == VIDEO_CAPTURE)
	{
		for(i=0; i<MAX_VIDEO_CHANNEL; i++)
			SAFE_RELEASE(g_pDDSCapture[i]);	
	}
}

//=========================================================================================================================//
BOOL DxT_CreateLogo()
{
	DDSURFACEDESC2	ddsd;
	HRESULT        hr;
	BOOL				bRet = FALSE;

	if (!g_pDD || !g_pDDSPrimary)
		return bRet;
	
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize          = sizeof(ddsd);
	ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	ddsd.dwBackBufferCount = 0;
	ddsd.dwWidth         = LOGO_WIDTH;
	ddsd.dwHeight        = LOGO_HEIGHT;
	ddsd.ddpfPixelFormat = g_ddpfOverlayFormats[1];
	if(FAILED(hr = g_pDD->CreateSurface(&ddsd, &g_pDDSLogo, NULL))) 
	{
		OutputDebugString("g_pDD->CreateSurface g_pDDSLogo fail\n");
		return bRet;
	}

	LPDIRECTDRAWCLIPPER pClipper = NULL;
	if(FAILED(hr = g_pDD->CreateClipper(0, &pClipper, NULL)))
	{
		OutputDebugString("g_pDD->CreateClipper fail\n");
		return bRet;
	}
	
	if(FAILED(hr = pClipper->SetHWnd(0, g_hWnd)))
	{
		OutputDebugString("pClipper->SetHWnd fail\n");
		return bRet;
	}
	
	if(FAILED(hr = g_pDDSPrimary->SetClipper(pClipper)))
	{
		OutputDebugString("g_pDDSPrimary->SetClipper fail\n");
		return bRet;			
	}
	SAFE_RELEASE(pClipper);
	bRet = TRUE;
	return bRet;
}

//=========================================================================================================================//
BOOL DxT_CreateSurface(BYTE byLiveOverlay, 
					   BYTE byDisplayFormat, 
					   BYTE *pbyImageSize, 
					   WORD wOverlayStartX, 
					   WORD wOverlayStartY, 
					   WORD wOverlayWidth, 
					   WORD wOverlayHeight, 
					   WORD *pwImageWidth, 
					   WORD *pwImageHeight)
{
	DDSURFACEDESC2 ddsdOverlay;
	DDSURFACEDESC2	ddsd;
	HRESULT        hr;
	BOOL				bRet = FALSE;
	int				i;

	if (!g_pDD || !g_pDDSPrimary)
		return FALSE;
	
	if(byLiveOverlay == DX_OFFSCREEN)
	{
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize          = sizeof(ddsd);
		ddsd.dwFlags        = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
		ddsd.dwBackBufferCount = 0;
		ddsd.ddpfPixelFormat = g_ddpfOverlayFormats[byDisplayFormat];
		for(i=0; i<MAX_VIDEO_CHANNEL; i++)
		{
			ddsd.dwWidth      = pwImageWidth[pbyImageSize[i]];
			ddsd.dwHeight		= pwImageHeight[pbyImageSize[i]];
			if(FAILED(hr = g_pDD->CreateSurface(&ddsd, &g_pDDSCapture[i], NULL))) 
			{
				OutputDebugString("g_pDD->CreateSurface g_pDDSCapture fail\n");
				return FALSE;
			}		
		}
		bRet = TRUE;
	}
	else if(byLiveOverlay == DX_OVERLAY)
	{
		INIT_DIRECTDRAW_STRUCT(ddsdOverlay);
		ddsdOverlay.ddsCaps.dwCaps		= DDSCAPS_OVERLAY | DDSCAPS_VIDEOMEMORY;
		ddsdOverlay.dwFlags				= DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH| DDSD_PIXELFORMAT;
		ddsdOverlay.dwWidth				= pwImageWidth[0];
		ddsdOverlay.dwHeight				= pwImageHeight[0];
		ddsdOverlay.dwBackBufferCount	= 0;
		ddsdOverlay.ddpfPixelFormat	= g_ddpfOverlayFormats[byDisplayFormat];

		hr = g_pDD->CreateSurface(&ddsdOverlay, &g_pDDSOverlay, NULL);
		if(FAILED(hr))
		{
			OutputDebugString("g_pDD->CreateSurface fail\n");
			return FALSE;
		}
		if(!g_pDDSOverlay)
		{
			OutputDebugString("g_pDDSOverlay == NULL\n");
			return FALSE;
		}	
			
		DxT_OverlaySetting(wOverlayStartX, wOverlayStartY, wOverlayWidth, wOverlayHeight, pwImageWidth[0], pwImageHeight[0]);
		bRet = TRUE;
	}
	return bRet;
}

//=========================================================================================================================//
BOOL GetOverlayAddress(DWORD *pdwAddress, DWORD *pdwlPitch)
{
	HRESULT				hr;
	DDSURFACEDESC2		ddsd;

	INIT_DIRECTDRAW_STRUCT(ddsd);
	hr = g_pDDSOverlay->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT, NULL);
	if(FAILED(hr))
	{
		AfxMessageBox("g_pDDSOverlay->Lock fail");
		return FALSE;
	}

	g_pDDSOverlay->Unlock(NULL);
	*pdwAddress = (DWORD)ddsd.lpSurface;
	*pdwlPitch = ddsd.lPitch;
	return TRUE;
}

//=========================================================================================================================//
BOOL DxT_OverlaySetting(WORD wOverlayStartX, WORD wOverlayStartY, WORD wOverlayWidth, WORD wOverlayHeight, WORD wImageWidth, WORD wImageHeight)
{
	HRESULT        hr;	
	DDCAPS         capsDrv;
	
	if(!g_pDD || !g_pDDSPrimary || !g_pDDSOverlay)
		return FALSE;
	
	INIT_DIRECTDRAW_STRUCT(capsDrv);
	hr = g_pDD->GetCaps(&capsDrv, NULL);
	if (FAILED(hr))
		return FALSE;
	
	ZeroMemory(&g_OverlayFX, sizeof(g_OverlayFX));
	g_OverlayFX.dwSize = sizeof(g_OverlayFX);
	if(g_FirstDM.dmBitsPerPel == 32)
	{
		g_OverlayFX.dckDestColorkey.dwColorSpaceHighValue = BK_OVERLAY_COLORKEY_32;
		g_OverlayFX.dckDestColorkey.dwColorSpaceLowValue  = BK_OVERLAY_COLORKEY_32;
	}
	else if(g_FirstDM.dmBitsPerPel == 16)
	{
		g_OverlayFX.dckDestColorkey.dwColorSpaceHighValue = BK_OVERLAY_COLORKEY_16;
		g_OverlayFX.dckDestColorkey.dwColorSpaceLowValue  = BK_OVERLAY_COLORKEY_16;
	}
	g_dwOverlayFlags = DDOVER_DDFX | DDOVER_SHOW | DDOVER_KEYDESTOVERRIDE;
	
	g_rsOverlay.left = 0;
	g_rsOverlay.top = 0; // position 0,0 is already position (boundary aligned)
	g_rsOverlay.right = wImageWidth;
	g_rsOverlay.bottom = wImageHeight;
	
	g_rdOverlay.left = wOverlayStartX;
	g_rdOverlay.top = wOverlayStartY;
	g_rdOverlay.right = wOverlayWidth+5; //g_rdOverlay.left+ wOverlayWidth;
	g_rdOverlay.bottom = wOverlayHeight+5; //g_rdOverlay.top+ wOverlayHeight;
	return TRUE;
}

//=========================================================================================================================//
BYTE *SetCaptureSurfaceLock(BYTE byChannel, DWORD *pdwlPitch)
{
	DDSURFACEDESC2	ddsd;
	HRESULT			hr;
	BYTE				*pAddress;	

	INIT_DIRECTDRAW_STRUCT(ddsd);
	hr = g_pDDSCapture[byChannel]->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT, NULL);	
	
	pAddress = (BYTE *)ddsd.lpSurface;
	*pdwlPitch = ddsd.lPitch;
	return pAddress;
}

//=========================================================================================================================//
void SetCaptureSurfaceUnlock(BYTE byChannel)
{
	g_pDDSCapture[byChannel]->Unlock(NULL);
}

//=========================================================================================================================//
BYTE *SetLogoSurfaceLock(DWORD *pdwlPitch)
{
	DDSURFACEDESC2	ddsd;
	BYTE				*pAddress;
	HRESULT			hr;
	
	INIT_DIRECTDRAW_STRUCT(ddsd);
	hr = g_pDDSLogo->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT, NULL);	
	pAddress = (BYTE *)ddsd.lpSurface;
	*pdwlPitch = ddsd.lPitch;
	return pAddress;
}

//=========================================================================================================================//
void SetLogoSurfaceUnlock()
{
	g_pDDSLogo->Unlock(NULL);
}

//=========================================================================================================================//
BOOL SetCaptureDisplay(BYTE byChannel, RECT *pRcDst, RECT *pRcSrc, BOOL bLogo)
{
	HRESULT	hr;

	if(bLogo == FALSE)
		hr = g_pDDSPrimary->Blt(pRcDst, g_pDDSCapture[byChannel], pRcSrc, DDBLT_WAIT, NULL);
	else
		hr = g_pDDSPrimary->Blt(pRcDst, g_pDDSLogo, pRcSrc, DDBLT_WAIT, NULL);
	if(FAILED(hr))
	{
		if(hr == DDERR_SURFACELOST)
		{
			if(FAILED(hr = g_pDD->RestoreAllSurfaces()))
			{
				OutputDebugString("g_pDD->RestoreAllSurfaces fail \n");
			}
		}
		OutputDebugString("g_pDDSPrimary->Blt fail \n");
		return FALSE;
	}
	return TRUE;
}