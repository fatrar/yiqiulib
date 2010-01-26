// Overlay.h: interface for the COverlay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OVERLAY_H__92CDA106_D31F_450C_AC3B_F40B61B23EF5__INCLUDED_)
#define AFX_OVERLAY_H__92CDA106_D31F_450C_AC3B_F40B61B23EF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COverlay  
{
public:
	BOOL HideOverlay();
	BOOL DistroyOverlay();
	BOOL CreateOverlay();
	BOOL Initial(HWND hwnd);
	COverlay();
	virtual ~COverlay();
//private:
	HWND		m_hWnd;
	HDC		m_hDC;
	HFONT		m_hFont;
	HBRUSH	m_hOverlayBrush;
	HBRUSH	m_hCaptureBrush;	
	HPEN		m_hLinePen;
	HPEN		m_hFocusPen;
	HPEN		m_hOverlayPen;
	RECT	m_rcOverlayRect;
	WORD	m_wOverlayWidth;
	WORD	m_wOverlayHeight;
	WORD	m_wOverlayPixel;
	DWORD	m_dwOverlaySize;
	DWORD	m_dwR3OverlayAddress;
	BYTE	*m_pbyOverlayAddress;
	BYTE	*m_pbyLogoAddress;
	DWORD	m_dwOverlayPitch;

};

#endif // !defined(AFX_OVERLAY_H__92CDA106_D31F_450C_AC3B_F40B61B23EF5__INCLUDED_)
