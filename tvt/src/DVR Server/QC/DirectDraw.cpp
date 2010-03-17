// DirectDraw.cpp: implementation of the CDirectDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "QC.h"
#include "DirectDraw.h" 
//#include "qcdlg.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
extern int g_card_id;
DDPIXELFORMAT g_ddpfOverlayFormats[] = 
{
	{sizeof(DDPIXELFORMAT), DDPF_FOURCC,MAKEFOURCC('U','Y','V','Y'),0,0,0,0,0},
	{sizeof(DDPIXELFORMAT), DDPF_FOURCC,MAKEFOURCC('Y','U','Y','2'),0,0,0,0,0},	
	{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 16, 0xF800, 0x07e0, 0x001F, 0}
};
//////////////////////////////////////////////////////////////////////
#define CHANNELNUMBER 4
#define XOFFSET 20
#define YOFFSET 50
#define RWIDTH 320
#define RHEIGHT 240
#define DRAWWIDTH 180
#define DRAWHEIGHT 130
//#define INSTANCE 2
CRect USB_RECT[] =
{
	CRect(DRAWWIDTH*0+XOFFSET, DRAWHEIGHT*0+YOFFSET, DRAWWIDTH*2+XOFFSET, DRAWHEIGHT*2+YOFFSET),
	CRect(DRAWWIDTH*2+XOFFSET, DRAWHEIGHT*0+YOFFSET, DRAWWIDTH*4+XOFFSET, DRAWHEIGHT*2+YOFFSET),
	CRect(DRAWWIDTH*0+XOFFSET, DRAWHEIGHT*2+YOFFSET, DRAWWIDTH*2+XOFFSET, DRAWHEIGHT*4+YOFFSET),
	CRect(DRAWWIDTH*2+XOFFSET, DRAWHEIGHT*2+YOFFSET, DRAWWIDTH*4+XOFFSET, DRAWHEIGHT*4+YOFFSET),

	CRect(DRAWWIDTH*0+XOFFSET, DRAWHEIGHT*0+YOFFSET, DRAWWIDTH*4+XOFFSET, DRAWHEIGHT*4+YOFFSET),
};

CRect channel_rect[]=
{
	    CRect (DRAWWIDTH*0+XOFFSET,DRAWHEIGHT*0+YOFFSET,DRAWWIDTH*1+XOFFSET,DRAWHEIGHT*1+YOFFSET),
		CRect (DRAWWIDTH*1+XOFFSET,DRAWHEIGHT*0+YOFFSET,DRAWWIDTH*2+XOFFSET,DRAWHEIGHT*1+YOFFSET),
		CRect (DRAWWIDTH*2+XOFFSET,DRAWHEIGHT*0+YOFFSET,DRAWWIDTH*3+XOFFSET,DRAWHEIGHT*1+YOFFSET),
		CRect (DRAWWIDTH*3+XOFFSET,DRAWHEIGHT*0+YOFFSET,DRAWWIDTH*4+XOFFSET,DRAWHEIGHT*1+YOFFSET),

		CRect (DRAWWIDTH*0+XOFFSET,DRAWHEIGHT*1+YOFFSET,DRAWWIDTH*1+XOFFSET,DRAWHEIGHT*2+YOFFSET),
		CRect (DRAWWIDTH*1+XOFFSET,DRAWHEIGHT*1+YOFFSET,DRAWWIDTH*2+XOFFSET,DRAWHEIGHT*2+YOFFSET),
		CRect (DRAWWIDTH*2+XOFFSET,DRAWHEIGHT*1+YOFFSET,DRAWWIDTH*3+XOFFSET,DRAWHEIGHT*2+YOFFSET),
		CRect (DRAWWIDTH*3+XOFFSET,DRAWHEIGHT*1+YOFFSET,DRAWWIDTH*4+XOFFSET,DRAWHEIGHT*2+YOFFSET),

		CRect (DRAWWIDTH*0+XOFFSET,DRAWHEIGHT*2+YOFFSET,DRAWWIDTH*1+XOFFSET,DRAWHEIGHT*3+YOFFSET),
	    CRect (DRAWWIDTH*1+XOFFSET,DRAWHEIGHT*2+YOFFSET,DRAWWIDTH*2+XOFFSET,DRAWHEIGHT*3+YOFFSET),
		CRect (DRAWWIDTH*2+XOFFSET,DRAWHEIGHT*2+YOFFSET,DRAWWIDTH*3+XOFFSET,DRAWHEIGHT*3+YOFFSET),
		CRect (DRAWWIDTH*3+XOFFSET,DRAWHEIGHT*2+YOFFSET,DRAWWIDTH*4+XOFFSET,DRAWHEIGHT*3+YOFFSET),

		CRect (DRAWWIDTH*0+XOFFSET,DRAWHEIGHT*3+YOFFSET,DRAWWIDTH*1+XOFFSET,DRAWHEIGHT*4+YOFFSET),
		CRect (DRAWWIDTH*1+XOFFSET,DRAWHEIGHT*3+YOFFSET,DRAWWIDTH*2+XOFFSET,DRAWHEIGHT*4+YOFFSET),
		CRect (DRAWWIDTH*2+XOFFSET,DRAWHEIGHT*3+YOFFSET,DRAWWIDTH*3+XOFFSET,DRAWHEIGHT*4+YOFFSET),
		CRect (DRAWWIDTH*3+XOFFSET,DRAWHEIGHT*3+YOFFSET,DRAWWIDTH*4+XOFFSET,DRAWHEIGHT*4+YOFFSET),

		CRect (DRAWWIDTH*0+XOFFSET-5,DRAWHEIGHT*0+YOFFSET-5,DRAWWIDTH*4+XOFFSET+5,DRAWHEIGHT*4+YOFFSET+5),
};

/*
功能说明: 将YUV422平面数据转换为打包数据
		  分两种情况实现 : 如果CPU支持MMX指令集，则用MMX汇编实现，否则使用C语言实现
参数说明: pDst[OUT]: 转换目标地址--即转换后的数据存放位置
		  pSrc[IN] : 转换源地址----即需要转换的数据存放位置
		  nWidth[IN]:图像宽度(PIXEL)
		  nHeight[IN]:图像高度(PIXEL)
		  nPitch[IN]:每行像素占用的字节数，如果是DDRAW的表面就传递DDSURFACEDESC::lPitch；
											如果是普通转换，则传递nWidth*2(因为每个象素两个字节)
修改记录:
			修改人		修改时间		修改内容
--------------------------------------------------------------------------
			zhangzhen	2007/02/02		整理YUV422平面数据打包功能为一个函数
*/
void YUV422Pack(void *pDst, void *pSrc, unsigned int nWidth, unsigned int nHeight, unsigned int nPitch)
{
	BYTE *pY = (BYTE*)pSrc;
	BYTE *pU = (BYTE*)pSrc + nWidth * nHeight;
	BYTE *pV = (BYTE*)pSrc + nWidth * nHeight * 3 / 2;

	long iEDXValue=0;
	_asm 
	{
		mov eax,1
			cpuid
			mov iEDXValue,edx
	}

	if((iEDXValue&0x800000) == 0)//CPU不支持MMX,直接转换
	{
		WORD *pwYUV422 = (WORD *)pDst;
						
		for (int i = 0; i < nHeight; i++)
		{
			for (int j = 0; j < nWidth/2; j++)
			{
				*pwYUV422++ = (*pU++) << 8 | *pY++;
				*pwYUV422++ = (*pV++) << 8 | *pY++;
			}
			pwYUV422 += nPitch/2 - /*nPitch*/nWidth;	//Modify By zhangzhen 2007/04/9
		}					
	}
	else//CPU支持MMX，用MMX指令集转换
	{
		unsigned int count = nWidth*2/32;
		unsigned int diff = nPitch -nWidth*2;
		__asm 
		{
			mov edi, pDst;
			mov esi, pY;
			mov eax, pU;
			mov edx, pV;
			mov ebx, nHeight
		LOOP_H:
			mov ecx, count;
		LOOP_W:
			movq mm0, [esi + 0];
			movq mm2, [esi + 8];
			movq mm4, [eax];
			movq mm6, [edx];
			movq mm1, mm0;
			movq mm3, mm2;
			movq mm5, mm4;

			PUNPCKLBW MM4, MM6;
			PUNPCKHBW MM5, MM6;

			PUNPCKLBW MM0, MM4;
			PUNPCKHBW MM1, MM4;
			PUNPCKLBW MM2, MM5;
			PUNPCKHBW MM3, MM5;

			movq [edi +  0], mm0;
			movq [edi +  8], mm1;
			movq [edi + 16], mm2;
			movq [edi + 24], mm3;

			add esi, 16;
			add edi, 32;
			add eax, 8;
			add	edx, 8;
			dec ecx;
			jnz LOOP_W;
			add edi,diff;
			dec ebx;
			jnz LOOP_H;
			emms;
		}
	}
}

/*
功能说明: 将YUV420平面数据转换为YUV422打包数据
参数说明: pDst[OUT]: 转换目标地址--即转换后的数据存放位置
		  pSrc[IN] : 转换源地址----即需要转换的数据存放位置
		  nWidth[IN]:图像宽度(PIXEL)
		  nHeight[IN]:图像高度(PIXEL)
		  nPitch[IN]:每行像素占用的字节数，如果是DDRAW的表面就传递DDSURFACEDESC::lPitch；
											如果是普通转换，则传递nWidth*2(因为每个象素两个字节)
		YUV420数据格式:偶数行(0,2,4...):横向相邻两个像素共享U分量，并与其下一行相邻像素共享V分量
					   奇数行(1,3,5...):横向相邻两个像素共享V分量，并与其上一行相邻像素共享U分量
修改记录:
			修改人		修改时间		修改内容
--------------------------------------------------------------------------
			zhangzhen	2007/03/27		<4108-16>
*/
void YUV420_YUV422Pack(void *pDst, void *pSrc, unsigned int nWidth, unsigned int nHeight, unsigned int nPitch,INT src420Subtype)
{
	if (src420Subtype == 1)
	{
		////2009/02/27---
		int BufWidth;
		if(nWidth == 352)
		{
			BufWidth = 384;
		}
		else if(nWidth == 176)
		{
			BufWidth = 192;
		}
		else
		{
			BufWidth = nWidth;
		}
		
		
		BYTE* pY = (BYTE*)pSrc;
		BYTE* pU = pY+BufWidth/*384*/*nHeight/*288*/;//2009/02/27
		BYTE* pV = pY+BufWidth/*384*/*nHeight/*288*/+BufWidth/*384*/*nHeight/*288*//4;//2009/02/27
		BYTE * pSurf=(BYTE*)pDst;
		for(int i=0;i<nHeight/*288*/;i++)//2009/02/27
		{
			for(int j=0;j<nWidth/*352*//2;j++)
			{
				pSurf[j*4] =	pY[j*2];
				pSurf[j*4+1] =	pU[j*2+1];
				pSurf[j*4+2] =	pY[j*2+1];
				pSurf[j*4+3] =	pU[j*2];
			}
			pSurf += nPitch;//352*2;
			pY += BufWidth/*384*/;
			if(i&1)
			{
				pU += BufWidth/*384*/;
				pV += BufWidth/*384*/;
			}
		}	

	}
	else
	{	
		int	i = 0;
		int j = 0;
		
		BYTE *pY = (BYTE*)pSrc;
		BYTE *pU = (BYTE*)pSrc + nWidth * nHeight;
		BYTE *pV = (BYTE*)pU + nWidth * nHeight * 1 / 4;
		
		long iEDXValue=0;
		_asm 
		{
			mov eax,1
				cpuid
				mov iEDXValue,edx
		}
		
		if((iEDXValue&0x800000) == 0)//CPU不支持MMX,直接转换
		{
			BYTE *pYUV422Pack = (BYTE*)pDst;
			
			for(i = 0; i < nHeight; i++)
			{
				for(j = 0; j < nWidth/2; j++)
				{
					*pYUV422Pack++ = *pY++;
					*pYUV422Pack++ = *pU++;
					*pYUV422Pack++ = *pY++;
					*pYUV422Pack++ = *pV++;
				}
				
				pYUV422Pack += nPitch - nWidth*2;
				
				if(i%2 == 0)	//偶数行，UV分量分别要向前回一行，因为下一个奇数行要共享本行的UV分量
				{
					pU -= nWidth/2;
					pV -= nWidth/2;
				}
			}
		}
		else//CPU支持MMX，用MMX指令集转换
		{
			unsigned int count = nWidth*2/32;
			unsigned int diff = nPitch -nWidth*2;
			unsigned int line_bytes_uv = nWidth/2;
			__asm 
			{
				mov edi, pDst;
				mov esi, pY;
				mov eax, pU;
				mov edx, pV;
				mov ebx, nHeight
LOOP_H:
				mov ecx, count;
LOOP_W:
				movq mm0, [esi + 0];
				movq mm2, [esi + 8];
				movq mm4, [eax];
				movq mm6, [edx];
				movq mm1, mm0;
				movq mm3, mm2;
				movq mm5, mm4;
				
				PUNPCKLBW MM4, MM6;
				PUNPCKHBW MM5, MM6;
				
				PUNPCKLBW MM0, MM4;
				PUNPCKHBW MM1, MM4;
				PUNPCKLBW MM2, MM5;
				PUNPCKHBW MM3, MM5;
				
				movq [edi +  0], mm0;
				movq [edi +  8], mm1;
				movq [edi + 16], mm2;
				movq [edi + 24], mm3;
				
				add esi, 16;
				add edi, 32;
				add eax, 8;
				add	edx, 8;
				dec ecx;
				jnz LOOP_W;
				add edi,diff;
				test ebx, 0x1
					jnz	YU_NEXT_LINE		;奇数行与其前面一偶数行共享UV分量
					sub	eax, line_bytes_uv
					sub edx, line_bytes_uv
YU_NEXT_LINE:
				dec ebx;
				jnz LOOP_H;
				emms;
			}
		}
	}
}

CRect scrRect(0,0,RWIDTH,RHEIGHT);
CDirectDraw::CDirectDraw()
{
	m_OverlayUpdate = NULL;
}

CDirectDraw::CDirectDraw(HWND hwnd)
{
	m_hWnd = hwnd;

	m_OverlayUpdate = NULL;
	for (int i = 0; i < 16; i++)
	{
		m_pDDSBackYUY2[i] = NULL;
	}
}

CDirectDraw::~CDirectDraw()
{

}

BOOL CDirectDraw::IniDirectDraw()
{
	//创建一个DirectDraw对象的实例
	HRESULT hr = DirectDrawCreateEx(NULL, (void **)&m_pDD7, IID_IDirectDraw7, NULL);
	if(FAILED(hr))
    {
        CoInitialize(NULL);
		hr = DirectDrawCreateEx(NULL, (void **)&m_pDD7, IID_IDirectDraw7, NULL);
        if(FAILED(hr))
        {
			AfxMessageBox("DirectDrawCreate Error!");
			return FALSE;
        }
    }
	hr = m_pDD7->SetCooperativeLevel(m_hWnd, DDSCL_NORMAL);
    if(FAILED(hr))
    {
        return FALSE;
    }
	
    DDSURFACEDESC2 ddsd;
    memset(&ddsd, 0, sizeof(ddsd));
	
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;// | DDSCAPS_VIDEOMEMORY;
	
    m_pDD7->CreateSurface(&ddsd, &m_pddsPrimary, NULL);
	hr = m_pDD7->CreateClipper(0, &m_lpClipper, NULL);
	if (hr != DD_OK )
	{
		return FALSE;
	}
	else
		m_hr = m_lpClipper->SetHWnd(0, m_hWnd);

	if (FAILED(hr = IDirectDrawSurface_SetClipper(m_pddsPrimary, m_lpClipper)))
	{
		return FALSE;
	}

	DDPIXELFORMAT pixelformat =	g_ddpfOverlayFormats[1];
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH| DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;//DDSCAPS_SYSTEMMEMORY;//|DDSCAPS_VIDEOMEMORY
	ddsd.ddpfPixelFormat = pixelformat;
	ddsd.dwWidth  = RWIDTH;
	ddsd.dwHeight = RHEIGHT;
	if(m_pDD7->CreateSurface(&ddsd, &this->m_lpddsBack, NULL) != DD_OK)
	{
		this->m_pddsPrimary->Release();
		this->m_pddsPrimary = NULL;
		
		m_pDD7->Release();
		m_pDD7 = NULL;
		return S_FALSE;
	}
	
	return TRUE;
}

BOOL CDirectDraw::DrawBuffer(int index, LPBYTE pBuffer, WORD len)
{
	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	static BOOL	first = TRUE;
	if (first)
	{
		DrawBack();
	}

	first = FALSE;
	HRESULT ddrval = m_lpddsBack->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR , NULL);
	if(ddrval != DD_OK) 
	{
		m_pddsPrimary->Restore();
		m_lpddsBack->Restore();
		return S_OK;
	}

	if (0xff == index)
	{
		memset(ddsd.lpSurface, 0x200, 153600);
		m_lpddsBack->Unlock(NULL);
		ddrval = m_pddsPrimary->Blt(&channel_rect[16], m_lpddsBack, &scrRect, DDBLT_ASYNC, NULL); //| DDBLT_WAIT
	}
	else// if ((index>=0)&&(index<16))
	{
		for (int i = 0; i < RHEIGHT; i++)
		{
			memcpy((BYTE* )ddsd.lpSurface + i*ddsd.lPitch, pBuffer + i * RWIDTH*2, RWIDTH*2);
		}
		//memcpy((BYTE* )ddsd.lpSurface,pBuffer,len);
		m_lpddsBack->Unlock(NULL);
		//DrawBack();
		ddrval = m_pddsPrimary->Blt(&channel_rect[index], m_lpddsBack, &scrRect, DDBLT_ASYNC, NULL); //| DDBLT_WAIT
	}
	
	return TRUE;
}

void CDirectDraw::FreeDirectDraw()
{
	m_OverlayTimeUpdateQuit=TRUE;
	if(NULL != m_OverlayUpdate)
	{
		WaitForSingleObject(m_OverlayUpdate,100);
		DWORD ExitCode;
		if (GetExitCodeThread(m_OverlayUpdate, &ExitCode))
		{
			if (ExitCode == STILL_ACTIVE)
			{
				TerminateThread(m_OverlayUpdate, 1);
			}
		}
		CloseHandle(m_OverlayUpdate);
		m_OverlayUpdate=NULL;
	}
	
	if(m_lpClipper)
	{
		m_lpClipper->Release();
		m_lpClipper=NULL;
	}
	
	if ( m_pDD7 != NULL)
	{
		m_pDD7->Release();
		m_pDD7 = NULL;
	}
	if (m_pddsPrimary != NULL)
	{
		m_pddsPrimary->Release();
		m_pddsPrimary = NULL;
	}	
	if (m_lpddsBack != NULL)
	{
		m_lpddsBack->Release();
		m_lpddsBack = NULL;
	}	
}

void CDirectDraw::DrawOverlayBack()
{

		HDC hDc;
//		CDC *cDc ;
		hDc = GetDC(m_hWnd);
		HBRUSH hBr = CreateSolidBrush(OVERLAY_KEYCOLOR); 
	//	SetBkColor(hDc,(COLORREF)RGB(0,0,0));
	//
	//	Rectangle(hDc,channel_rect[16].left,channel_rect[16].bottom,channel_rect[16].right,channel_rect[16].top );
		::FillRect(hDc,channel_rect[16], hBr);
		DeleteObject(hBr);
		ReleaseDC(m_hWnd,hDc);
/*
//		HDC hDc;
	
		HRESULT ddrval;
		DDSURFACEDESC2 ddsd;
		memset(&ddsd, 0, sizeof(ddsd));
		ddsd.dwSize     = sizeof(ddsd);

		if (m_lpddsBack->IsLost() == DDERR_SURFACELOST) 
			m_lpddsBack->Restore();

		ddrval = m_lpddsBack->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR, NULL);
		WORD *pW = (WORD *)ddsd.lpSurface;
		for(int i = 0; i < RHEIGHT; i++)
		{
			//memset((BYTE* )ddsd.lpSurface + i*ddsd.lPitch, 128, RWIDTH * 2);
			for (LONG j = 0; j < ddsd.lPitch/2; j++)
				*pW++ = 0xFFFF;
		}
		m_lpddsBack->Unlock(NULL);
		ddrval = m_pddsPrimary->Blt(&channel_rect[16], m_lpddsBack, &scrRect, DDBLT_ASYNC, NULL); //| DDBLT_WAIT

*/	
//		CWnd * wnd=CWnd::FromHandle(m_hWnd);
//		CQCDlg *pdlg=(CQCDlg *) wnd;
//		if (pdlg->m_haveAlarmCard)
//		if (0)//pdlg->m_Alarmtest)
//		{
//			hDc=GetDC(m_hWnd);
//			SetBkColor(hDc,(COLORREF)RGB(200,128,2));
//			LPCTSTR	str;
//			str="Alarm Card Testing...";
//			TextOut(hDc,300,300,str,21);
//			ReleaseDC(m_hWnd,hDc);
//		}
		return ;
}

//检测是否支持overlay
BOOL CDirectDraw::AreOverlaysSupported()
{
	DDCAPS  capsDrv;
    HRESULT ddrval;
 
    // Get driver capabilities to determine Overlay support.
    ZeroMemory(&capsDrv, sizeof(capsDrv));
    capsDrv.dwSize = sizeof(capsDrv);
 
    ddrval = m_pDD7->GetCaps(&capsDrv, NULL);
    if (FAILED(ddrval))
        return FALSE;
 
    // Does the driver support overlays in the current mode? 
    // (Currently the DirectDraw emulation layer does not support overlays.
    // Overlay related APIs will fail without hardware support).  
    if (!(capsDrv.dwCaps & DDCAPS_OVERLAY))
        return FALSE;
 
    return TRUE;

}

BOOL CDirectDraw::CreateOverlay(DWORD dwFormat)
{
	HRESULT ddrval;
	DDSURFACEDESC2 ddsdOverlay;
	INIT_DIRECTDRAW_STRUCT(ddsdOverlay);
	ddsdOverlay.ddsCaps.dwCaps		= DDSCAPS_OVERLAY | DDSCAPS_VIDEOMEMORY;
	ddsdOverlay.dwFlags				= DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH| DDSD_PIXELFORMAT;
	ddsdOverlay.dwWidth				= 704;

	if (dwFormat == DVRVIDEO_STANDARD_PAL)
		ddsdOverlay.dwHeight		= 576;
	else
		ddsdOverlay.dwHeight		= 480;

	ddsdOverlay.dwBackBufferCount	= 0;
	ddsdOverlay.ddpfPixelFormat		= g_ddpfOverlayFormats[1];
	ddrval = m_pDD7->CreateSurface(&ddsdOverlay, &m_lpddsOverlay, NULL);
	if (FAILED(ddrval))
	{
		AfxMessageBox("CREATE OVERLAY ERROR");
		return FALSE;
	}

	//add by chenliang 20070518 解决TD3316对显卡的兼容性
	m_OverlayTimeUpdateQuit=FALSE;
	if (m_OverlayUpdate==NULL)
	{
		m_OverlayUpdate=CreateThread(0,0,TimeUpdateOverlay,(LPVOID)this,0,NULL);
	}
	return TRUE;
}

void CDirectDraw::ShowOverlay(BOOL bShow)
{
	HRESULT ddrval;
	DDOVERLAYFX ovfx;
	DWORD dwUpdateFlags;

	if (bShow == TRUE)
		dwUpdateFlags = DDOVER_SHOW | DDOVER_DDFX| DDOVER_KEYDESTOVERRIDE;
	else
		dwUpdateFlags = DDOVER_HIDE;

	INIT_DIRECTDRAW_STRUCT(ovfx);
	ovfx.dckDestColorkey.dwColorSpaceLowValue = OVERLAY_KEYCOLOR;
    ovfx.dckDestColorkey.dwColorSpaceHighValue = OVERLAY_KEYCOLOR;

	ddrval = m_lpddsOverlay->UpdateOverlay(
		NULL, 
		m_pddsPrimary, 
		&channel_rect[16], 
		dwUpdateFlags, 
		&ovfx);

	if (FAILED(ddrval))
		AfxMessageBox("FAIL UPDATEOVERLAY");
}

void CDirectDraw::GetOverlayADDR_PITCH(DWORD *pADDR, DWORD *pPITCH)
{
	HRESULT ddrval;
	DDSURFACEDESC2 ddsd;
	INIT_DIRECTDRAW_STRUCT(ddsd);

	if (m_lpddsOverlay->IsLost() == DDERR_SURFACELOST) 
		m_lpddsOverlay->Restore();

	ddrval = m_lpddsOverlay->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT, NULL);
	if (FAILED(ddrval))
        AfxMessageBox("LOCK ERROR");
    
    WORD * pW = (WORD *)ddsd.lpSurface;
    for (int ii = 0; ii < 576; ii++)
    {
        for (ULONG jj = 0; jj < ddsd.lPitch / 2; jj++)
        {
            *pW++ = 0x8000;
        }
    }
	m_lpddsOverlay->Unlock(NULL);
	*pADDR = (DWORD)ddsd.lpSurface;
	*pPITCH = ddsd.lPitch;
}

void CDirectDraw::DrawBack()
{
	HDC hDc;
	
	hDc = GetDC(m_hWnd);
	HBRUSH hBr = CreateSolidBrush(RGB(128, 128, 128));
    
    ::FillRect(hDc, USB_RECT[4], hBr);
	//::FillRect(hDc, channel_rect[16], hBr);
	DeleteObject(hBr);
	ReleaseDC(m_hWnd,hDc);
}

void CDirectDraw::Init3316Back(DWORD dwFormat)
{
	HRESULT hr;
	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
//DDSBackYUY2
	DDPIXELFORMAT pixelFormat =	
			{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 32,  0xFF0000, 0xFF00, 0xFF, 0}; //modify 
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH| DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	ddsd.ddpfPixelFormat = pixelFormat;

	if (dwFormat == DVRVIDEO_STANDARD_PAL)
	{
		ddsd.dwWidth = 352;
		ddsd.dwHeight = 288;
	}
	else
	{
		ddsd.dwWidth = 352;
		ddsd.dwHeight = 240;
	}
	
// 	if (dwFormat == DVRVIDEO_STANDARD_PAL)
// 	{
// 		ddsd.dwWidth = 704;
// 		ddsd.dwHeight = 576;
// 	}
// 	else
// 	{
// 		ddsd.dwWidth = 704;
// 		ddsd.dwHeight = 480;
// 	}
	for (int i = 0; i < 16; i++)
	{
		hr = m_pDD7->CreateSurface(&ddsd, &m_pDDSBackYUY2[i], NULL);
		if (hr != DD_OK)
		{
			AfxMessageBox("CreateSurface Back Error!");
		}
	}
}

void CDirectDraw::ShowPreview(long lIndex, BYTE *pBuf)
{
	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	
	if (m_pDDSBackYUY2[lIndex]->IsLost() == DDERR_SURFACELOST) 
		m_pDDSBackYUY2[lIndex]->Restore();
	
	HRESULT hr = m_pDDSBackYUY2[lIndex]->Lock(NULL, 
		&ddsd, 
		DDLOCK_SURFACEMEMORYPTR, 
		NULL);
	if (FAILED(hr))
	{
//		AfxMessageBox("ShowPreview Error");
		return;
	}

	PDWORD pRGB = (PDWORD)ddsd.lpSurface;
	for (int i = (int)(ddsd.dwHeight - 1); i >= 0; i--)
	{
		for (int j = 0; j < (int)ddsd.dwWidth; j++)
		{
			pRGB[i * ddsd.lPitch/4 + j] = pBuf[2] << 16 | pBuf[1] << 8 | pBuf[0];
			pBuf += 3;
		}
	}

	hr = m_pDDSBackYUY2[lIndex]->Unlock(NULL);
	if (FAILED(hr))
		return;

	if (m_pddsPrimary->IsLost() == DDERR_SURFACELOST) 
			m_pddsPrimary->Restore();

	m_pddsPrimary->Blt(&channel_rect[lIndex], m_pDDSBackYUY2[lIndex], NULL, DDBLT_ASYNC, NULL);
}

void CDirectDraw::InitUSBBack(DWORD dwFormat, int nChannelNum)
{
	HRESULT hr;
	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
//DDSBackYUY2
	DDPIXELFORMAT pixelFormat =	
		{sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('Y','U','Y','2'), 0, 0, 0, 0, 0};

	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	ddsd.ddpfPixelFormat = pixelFormat;

	if (dwFormat == DVRVIDEO_STANDARD_PAL)
	{
		ddsd.dwWidth = 352;
		ddsd.dwHeight = 288;
	}
	else
	{
		ddsd.dwWidth = 320;
		ddsd.dwHeight = 240;
	}
	
	for (int i = 0; i < nChannelNum; i++)
	{
		hr = m_pDD7->CreateSurface(&ddsd, &m_pDDSBackYUY2[i], NULL);
		if (hr != DD_OK)
		{
			AfxMessageBox("CreateSurface Back Error!");
		}
	}
}

void CDirectDraw::ShowUSB(long lIndex, BYTE *pBuf, int nChannelNum)
{
	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	
	if (m_pDDSBackYUY2[lIndex]->IsLost() == DDERR_SURFACELOST)
		m_pDDSBackYUY2[lIndex]->Restore();
	
	HRESULT hr = m_pDDSBackYUY2[lIndex]->Lock(NULL, 
		&ddsd, 
		DDLOCK_SURFACEMEMORYPTR, 
		NULL);

	if (FAILED(hr))
	{
		return;
	}

	PBYTE pS = (PBYTE)ddsd.lpSurface;
	for (DWORD i = 0; i < ddsd.dwHeight; i++)
	{
		memcpy(pS + ddsd.lPitch * i, pBuf + ddsd.dwWidth * 2 * i, ddsd.dwWidth * 2);
	}

	hr = m_pDDSBackYUY2[lIndex]->Unlock(NULL);
	if (FAILED(hr))
		return;

	if (m_pddsPrimary->IsLost() == DDERR_SURFACELOST)
			m_pddsPrimary->Restore();

	if (nChannelNum == 1)
	{
		m_pddsPrimary->Blt(&USB_RECT[4], m_pDDSBackYUY2[lIndex], NULL, DDBLT_ASYNC, NULL);
	}
	else
	{
		m_pddsPrimary->Blt(&USB_RECT[lIndex], m_pDDSBackYUY2[lIndex], NULL, DDBLT_ASYNC, NULL);
	}
}

void CDirectDraw::FreeUSBBack()
{
	for (int i = 0; i < 16; i++)
	{
		if (m_pDDSBackYUY2[i] != NULL)
		{
			m_pDDSBackYUY2[i]->Release();
			m_pDDSBackYUY2[i] = NULL;
		}
	}
}

void CDirectDraw::InitDSPBack(DWORD dwFormat, int nChanNum)
{
	HRESULT hr;
	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
//DDSBackYUY2
	DDPIXELFORMAT pixelFormat =	
		{sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('Y','U','Y','2'), 0, 0, 0, 0, 0};

	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	ddsd.ddpfPixelFormat = pixelFormat;

	if (dwFormat == DVRVIDEO_STANDARD_PAL)
	{
		ddsd.dwWidth = 352;
		ddsd.dwHeight = 288;
	}
	else
	{
		if (g_card_id == DVR_CARD_TD4408)
		{
			ddsd.dwWidth = 352;
		}
		else
		{
			ddsd.dwWidth = 320;
		}
		
		ddsd.dwHeight = 240;
	}
	
	for (int i = 0; i < nChanNum; i++)
	{
		hr = m_pDD7->CreateSurface(&ddsd, &m_pDDSBackYUY2[i], NULL);
		if (hr != DD_OK)
		{
			AfxMessageBox("CreateSurface Back Error!");
		}
	}
}

//djx 2008/7/22 4108
void CDirectDraw::InitDSPD1Back(DWORD dwFormat, int nChanNum)
{
	HRESULT hr;
	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
//DDSBackYUY2
	DDPIXELFORMAT pixelFormat =	
		{sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('Y','U','Y','2'), 0, 0, 0, 0, 0};

	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	ddsd.ddpfPixelFormat = pixelFormat;

	if (dwFormat == DVRVIDEO_STANDARD_PAL)
	{
		ddsd.dwWidth = 352*2;
		ddsd.dwHeight = 288*2;
	}
	else
	{
		ddsd.dwWidth = 352*2;
		ddsd.dwHeight = 240*2;
	}
	hr = m_pDD7->CreateSurface(&ddsd, &m_pDDSBackYUY2[0], NULL); //第一通道为D1


	if (dwFormat == DVRVIDEO_STANDARD_PAL)
	{
		ddsd.dwWidth = 352;
		ddsd.dwHeight = 288;
	}
	else
	{
		ddsd.dwWidth = 352;
		ddsd.dwHeight = 240;
	}
	for (int i = 1; i < nChanNum; i++)
	{
		hr = m_pDD7->CreateSurface(&ddsd, &m_pDDSBackYUY2[i], NULL);
		if (hr != DD_OK)
		{
			AfxMessageBox("CreateSurface Back Error!");
		}
	}
}

void CDirectDraw::FreeDSPBack()
{
	for (int i = 0; i < 16; i++)
	{
		if (m_pDDSBackYUY2[i] != NULL)
		{
			m_pDDSBackYUY2[i]->Release();
			m_pDDSBackYUY2[i] = NULL;
		}
	}
}
																//djx 2008/7/22 4108
void CDirectDraw::ShowDSP(
    const CRect& rect,
    const FILETIME& time,
    long lIndex, BYTE *pBuf, int nChanNum, DWORD format,INT src420Subtype)
{
	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	
	if (m_pDDSBackYUY2[lIndex]->IsLost() == DDERR_SURFACELOST)
		m_pDDSBackYUY2[lIndex]->Restore();
	
	HRESULT hr = m_pDDSBackYUY2[lIndex]->Lock(NULL, 
		&ddsd, 
		DDLOCK_SURFACEMEMORYPTR, 
		NULL);
	if (FAILED(hr))
	{
		return;
	}

	PBYTE pS = (PBYTE)ddsd.lpSurface;
//	for (DWORD i = 0; i < ddsd.dwHeight; i++)
//	{
		
//		memcpy(pS + ddsd.lPitch * i, pBuf + ddsd.dwWidth * 2 * i, ddsd.dwWidth * 2);
//	}
	if (format == YUV420) //djx 2008/7/22 4108
	{
		YUV420_YUV422Pack(pS, pBuf, ddsd.dwWidth, ddsd.dwHeight, ddsd.lPitch,src420Subtype);
	}
	else if (format == YUV422)
	{
		YUV422Pack(pS, pBuf, ddsd.dwWidth, ddsd.dwHeight, ddsd.lPitch);
	}
	else if (format == YUV422PACK)
	{
		for (int i = 0; i < ddsd.dwHeight; i++)
		{
			memcpy(pS,pBuf,ddsd.dwWidth*2);
			pS += ddsd.lPitch;
			pBuf += ddsd.dwWidth*2;
		}
	}
	
	hr = m_pDDSBackYUY2[lIndex]->Unlock(NULL);
	if (FAILED(hr))
		return;

	if (m_pddsPrimary->IsLost() == DDERR_SURFACELOST)
			m_pddsPrimary->Restore();

	if (nChanNum == 4)
	{
        CPoint p(rect.left, rect.top);
        CRect R = USB_RECT[lIndex] + p;
		m_pddsPrimary->Blt(&R, m_pDDSBackYUY2[lIndex], NULL, DDBLT_ASYNC, NULL);

        // 显示智能矩形框和路径
        
        if ( lIndex ==  0 )
        {
            HDC dc;
            m_pddsPrimary->GetDC(&dc);

            IIVViewer* pViewer = IVLiveFactory::GetLiveViewer();
            pViewer->Paint(lIndex, dc, R, time);

            m_pddsPrimary->ReleaseDC(dc);
        }
 	
    }
	else
	{
		m_pddsPrimary->Blt(&channel_rect[lIndex], m_pDDSBackYUY2[lIndex], NULL, DDBLT_ASYNC, NULL);
	}
}

DWORD WINAPI CDirectDraw::TimeUpdateOverlay(LPVOID lpParameter)
{
	CDirectDraw * my =(CDirectDraw *)lpParameter;
	while (!my->m_OverlayTimeUpdateQuit)
	{
		my->ShowOverlay(TRUE);
		Sleep(40);
	}
	return TRUE;
}

VOID CDirectDraw::Show(long lIndex, BYTE *pBuf, int nChanNum)
{
	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	
	if (m_pDDSBackYUY2[lIndex]->IsLost() == DDERR_SURFACELOST) 
		m_pDDSBackYUY2[lIndex]->Restore();
	
	HRESULT hr = m_pDDSBackYUY2[lIndex]->Lock(NULL, 
		&ddsd, 
		DDLOCK_SURFACEMEMORYPTR, 
		NULL);
	if (FAILED(hr))
	{
		//		AfxMessageBox("ShowPreview Error");
		return;
	}
	

	PBYTE pRGB = (PBYTE)ddsd.lpSurface;
	for (DWORD i = 0; i < ddsd.dwHeight; i++)
	{
		memcpy(pRGB + ddsd.lPitch * (ddsd.dwHeight-i-1), pBuf + ddsd.dwWidth * 4 * i, ddsd.dwWidth * 4);
	}

	
	hr = m_pDDSBackYUY2[lIndex]->Unlock(NULL);
	if (FAILED(hr))
		return;
	
	if (m_pddsPrimary->IsLost() == DDERR_SURFACELOST) 
		m_pddsPrimary->Restore();
	
	if (nChanNum == 4)
	{
		m_pddsPrimary->Blt(&USB_RECT[lIndex], m_pDDSBackYUY2[lIndex], NULL, DDBLT_ASYNC, NULL);
	}
	else
	{
		m_pddsPrimary->Blt(&channel_rect[lIndex], m_pDDSBackYUY2[lIndex], NULL, DDBLT_ASYNC, NULL);
	}
	//m_pddsPrimary->Blt(&channel_rect[lIndex], m_pDDSBackYUY2[lIndex], NULL, DDBLT_ASYNC, NULL);
}

VOID CDirectDraw::InitBack(DWORD dwFormat,BOOL bD1, int nChanNum)
{
	HRESULT hr;
	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	DDPIXELFORMAT pixelFormat =	
	{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 32,  0xFF0000, 0xFF00, 0xFF, 0}; //modify 
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH| DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	ddsd.ddpfPixelFormat = pixelFormat;
	if(bD1)
	{
		if (dwFormat == DVRVIDEO_STANDARD_PAL)
		{
			ddsd.dwWidth = 704;
			ddsd.dwHeight = 576;
		}
		else
		{
			ddsd.dwWidth = 704;
			ddsd.dwHeight = 480;
		}
	}
	else
	{
		if (dwFormat == DVRVIDEO_STANDARD_PAL)
		{
			ddsd.dwWidth = 352;
			ddsd.dwHeight = 288;
		}
		else
		{
			ddsd.dwWidth = 352;
			ddsd.dwHeight = 240;
		}
	}


	for (int i = 0; i < nChanNum; i++)
	{
		hr = m_pDD7->CreateSurface(&ddsd, &m_pDDSBackYUY2[i], NULL);
		if (hr != DD_OK)
		{
			AfxMessageBox("CreateSurface Back Error!");
		}
	}
}

VOID CDirectDraw::InitHalfD1Back(DWORD dwFormat, int nChanNum)
{
	HRESULT hr;
	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	DDPIXELFORMAT pixelFormat =	
	{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 32,  0xFF0000, 0xFF00, 0xFF, 0}; //modify 
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH| DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	ddsd.ddpfPixelFormat = pixelFormat;
	
	if (dwFormat == DVRVIDEO_STANDARD_PAL)
	{
		ddsd.dwWidth = 704;
		ddsd.dwHeight = 288;
	}
	else
	{
		ddsd.dwWidth = 704;
		ddsd.dwHeight = 240;
	}

	for (int i = 0; i < nChanNum; i++)
	{
		hr = m_pDD7->CreateSurface(&ddsd, &m_pDDSBackYUY2[i], NULL);
		if (hr != DD_OK)
		{
			AfxMessageBox("CreateSurface Back Error!");
		}
	}
}
