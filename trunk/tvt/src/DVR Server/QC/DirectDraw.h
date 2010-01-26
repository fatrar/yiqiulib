// DirectDraw.h: interface for the CDirectDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRECTDRAW_H__39309A6B_BCF6_4B86_9DF8_BBDDB756D59A__INCLUDED_)
#define AFX_DIRECTDRAW_H__39309A6B_BCF6_4B86_9DF8_BBDDB756D59A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
#define YUV420		0
#define YUV422		1
#define YUV422PACK	2
#define OVERLAY_KEYCOLOR (RGB(0, 0, 0))
#define INIT_DIRECTDRAW_STRUCT(x) (ZeroMemory(&x, sizeof(x)), x.dwSize=sizeof(x))
#define SAFE_RELEASE(p)				 {if(p) {(p)->Release(); (p)=NULL; } }
//////////////////////////////////////////////////////////////////////
class CDirectDraw  
{
public:
	LPDIRECTDRAWSURFACE7 m_pDDSBackYUY2[16];
	BOOL IniDirectDraw();
	CDirectDraw(HWND hwnd);
	CDirectDraw();
	virtual ~CDirectDraw();
public :
	VOID InitHalfD1Back(DWORD dwFormat,int nChanNum);
	VOID InitBack(DWORD dwFormat,BOOL bD1, int nChanNum);//zld 2009/4/27 4408
	VOID Show(long lIndex, BYTE *pBuf, int nChanNum);//zld 2009/4/27 4408
	//add by chenliang 20070518 解决TD3316对显卡的兼容性
	BOOL m_OverlayTimeUpdateQuit;
	HANDLE m_OverlayUpdate;
	static DWORD WINAPI TimeUpdateOverlay(LPVOID lpParameter);

	void ShowDSP(long lIndex, BYTE *pBuf, int nChanNum, DWORD format,INT src420Subtype=0);//djx 2008/7/22 4108
	void FreeDSPBack();
	void InitDSPBack(DWORD dwFormat, int nChanNum);
	void InitDSPD1Back(DWORD dwFormat, int nChanNum);//djx 2008/7/22 4108
	void FreeUSBBack();
	void ShowUSB(long lIndex, BYTE *pBuf, int nChannelNum);
	void InitUSBBack(DWORD dwFormat, int nChannelNum);
	void ShowPreview(long lIndex, BYTE *pBuf);
	void Init3316Back(DWORD dwFormat);
	void DrawBack();
	void GetOverlayADDR_PITCH(DWORD *dwADDR, DWORD *dwPITCH);
	void ShowOverlay(BOOL bShow);
	LPDIRECTDRAWSURFACE7 m_lpddsOverlay;
	BOOL CreateOverlay(DWORD dwFormat);
	BOOL AreOverlaysSupported();
	void DrawOverlayBack();
	void FreeDirectDraw();
	HWND m_hWnd;
	BOOL DrawBuffer(int index,LPBYTE pBuffer,WORD len);
    IDirectDraw7 *m_pDD7;
    IDirectDrawSurface7 *m_pddsPrimary;
    HRESULT m_hr;
	LPDIRECTDRAWCLIPPER m_lpClipper;// Pointer to the clipper 
	LPDIRECTDRAWSURFACE7 m_lpddsBack;
};

#endif // !defined(AFX_DIRECTDRAW_H__39309A6B_BCF6_4B86_9DF8_BBDDB756D59A__INCLUDED_)
