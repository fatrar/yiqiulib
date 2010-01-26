// Overlay.cpp: implementation of the COverlay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QC.h"
#include "Overlay.h"
#include "dxoverlay.h"
#include "./head/DefineBase.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
COverlay::COverlay()
{

}

COverlay::~COverlay()
{

}

BOOL COverlay::Initial(HWND hwnd)
{
	if(DxT_Init(hwnd) == FALSE)
	{
		OutputDebugString("DxT_Init fail \n");
		return FALSE;
	}
	if (FALSE==DxT_CreateLogo())
	{
		return FALSE;
	}
	DxT_Cleanup(VIDEO_LIVE);
	return TRUE;
}

//DEL BOOL COverlay::DxT_CreateLogo()
//DEL {
//DEL 	return TRUE;
//DEL 
//DEL }

//DEL BOOL COverlay::DxT_Cleanup(VIDEO_LIVE)
//DEL {
//DEL 	return TRUE;
//DEL 
//DEL }

BOOL COverlay::CreateOverlay()
{
	if(DxT_CreateSurface(DX_OVERLAY, 1, NULL, (USHORT)m_rcOverlayRect.left, (USHORT)m_rcOverlayRect.top,(USHORT)m_rcOverlayRect.right, (USHORT)m_rcOverlayRect.bottom, &m_wOverlayWidth, &m_wOverlayHeight) == FALSE)
	{
		AfxMessageBox("DxT_CreateSurface fail");
		return FALSE;
	}
	if(GetOverlayAddress(&m_dwR3OverlayAddress, &m_dwOverlayPitch) == TRUE)
	{
		ShowOverlay();
	}
	return TRUE;
}

BOOL COverlay::DistroyOverlay()
{
	FreeDirectDraw();
	return TRUE;
}
BOOL COverlay::HideOverlay()
{
	return FALSE;
}
