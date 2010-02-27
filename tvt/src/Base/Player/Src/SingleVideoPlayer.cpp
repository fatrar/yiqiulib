/*H***************************************************************************
 File            : SingleVideoPlayer.cpp
 Subsystem       : 
 Function Name(s): CSingleVideoPlayer
 Author          : YiQiu
 Date            : 2010-2-22  
 Time            : 18:11
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "StdAfx.h"
#include "SingleVideoPlayer.h"
#include <ddraw.h>
#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dxguid.lib")
#include <assert.h>

// 这里是为了提高性能，才判断两次
#define CSingleVideoPlayer_OnVideoPlay(a, b, rect, pTime, hwnd) \
    if ( a || b )\
    {\
        HDC dc;\
        m_pddsPrimary->GetDC(&dc);\
        if ( a )\
        {\
        a->OnVideoPlay(dc, rect, pTime, hwnd);\
        }\
        if ( b )\
        {\
        b->OnVideoPlay(dc, rect, pTime, hwnd);\
        }\
        m_pddsPrimary->ReleaseDC(dc);\
    } 

#define YUV420		0
#define YUV422		1
#define YUV422PACK	2
#define OVERLAY_KEYCOLOR (RGB(0, 0, 0))
#define INIT_DIRECTDRAW_STRUCT(x) (ZeroMemory(&x, sizeof(x)), x.dwSize=sizeof(x))
#define SAFE_RELEASE(p)				 {if(p) {(p)->Release(); (p)=NULL; } }

CSingleVideoPlayer::CSingleVideoPlayer()
    : m_hWnd(NULL)
    , m_pDD7(NULL)
    , m_pddsPrimary(NULL)
    , m_lpClipper(NULL)
    , m_pDDSBack(NULL)
    , m_dwBackColour(RGB(128,128,128))
    , m_pIVideoPlayCallBack(NULL) {}

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
void YUV422Pack(
    void *pDst, const void *pSrc,
    unsigned int nWidth, unsigned int nHeight, 
    unsigned int nPitch)
{
    BYTE *pY = (BYTE*)pSrc;
    BYTE *pU = (BYTE*)pSrc + nWidth * nHeight;
    BYTE *pV = (BYTE*)pSrc + nWidth * nHeight * 3 / 2;

    long iEDXValue=0;
    __asm 
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
void YUV420_YUV422Pack(
    void *pDst, const void *pSrc,
    unsigned int nWidth, unsigned int nHeight,
    unsigned int nPitch, INT src420Subtype)
{
    if (src420Subtype == 1)
    {
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
        BYTE* pU = pY+BufWidth*nHeight;//2009/02/27
        BYTE* pV = pY+BufWidth*nHeight+BufWidth*nHeight/4;//2009/02/27
        BYTE * pSurf=(BYTE*)pDst;
        for(int i=0;i<nHeight;i++)//2009/02/27
        {
            for(int j=0;j<nWidth/2;j++)
            {
                pSurf[j*4] =	pY[j*2];
                pSurf[j*4+1] =	pU[j*2+1];
                pSurf[j*4+2] =	pY[j*2+1];
                pSurf[j*4+3] =	pU[j*2];
            }
            pSurf += nPitch;;
            pY += BufWidth;
            if(i&1)
            {
                pU += BufWidth;
                pV += BufWidth;
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

BOOL CSingleVideoPlayer::InitDirectDrawSome(
    HWND hwnd,
    DWORD dwWidth, DWORD dwHeight,
    const RECT* ShowRect)
{
    assert(hwnd);
    m_hWnd = hwnd;
    if ( NULL == ShowRect )
    {
        GetClientRect(hwnd, &m_ShowRect);
    }
    else
    {
        m_ShowRect = *ShowRect;
    }

    //创建一个DirectDraw对象的实例
    CoInitialize(NULL);
    HRESULT hr = DirectDrawCreateEx(NULL, (void**)&m_pDD7, IID_IDirectDraw7, NULL);
    if(FAILED(hr))
    {
        AfxMessageBox(_T("DirectDrawCreate Error!"));
        return FALSE;
    }
    hr = m_pDD7->SetCooperativeLevel(m_hWnd, DDSCL_NORMAL);
    if(FAILED(hr))
    {
        SAFE_RELEASE(m_pDD7);
        return FALSE;
    }

    DDSURFACEDESC2 ddsd;
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;// | DDSCAPS_VIDEOMEMORY;

    m_pDD7->CreateSurface(&ddsd, &m_pddsPrimary, NULL);
    m_pDD7->CreateClipper(0, &m_lpClipper, NULL);
    m_lpClipper->SetHWnd(0, m_hWnd); 
    if (FAILED(hr = m_pddsPrimary->SetClipper(m_lpClipper)))
    {
        UnitDirectDraw();
        return FALSE;
    }
    return TRUE;
}

void CSingleVideoPlayer::UnitDirectDraw()
{
    SAFE_RELEASE(m_pDDSBack);
    SAFE_RELEASE(m_lpClipper);
    SAFE_RELEASE(m_pddsPrimary);
    SAFE_RELEASE(m_pDD7);
    CoUninitialize();
}

void CSingleVideoPlayer::ShowBack()
{
    HDC hDc = GetDC(m_hWnd);
    HBRUSH hBr = CreateSolidBrush(m_dwBackColour);
    ::FillRect(hDc, &m_ShowRect, hBr);
    DeleteObject(hBr);
    ReleaseDC(m_hWnd,hDc);
}

BOOL CSingleVideoPlayer::InitYUVBack(DWORD dwWidth, DWORD dwHeight)
{
    DDSURFACEDESC2 ddsd;
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    //DDSBackYUY2
    DDPIXELFORMAT pixelFormat =	
    {sizeof(DDPIXELFORMAT), DDPF_FOURCC, MAKEFOURCC('Y','U','Y','2'), 0, 0, 0, 0, 0};
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
    ddsd.ddpfPixelFormat = pixelFormat;
    ddsd.dwWidth = dwWidth;
    ddsd.dwHeight = dwHeight;

    HRESULT hr = m_pDD7->CreateSurface(&ddsd, &m_pDDSBack, NULL);
    if (hr != DD_OK)
    {
        TRACE(_T("InitYUVBack CreateSurface Back Error!\n"));
        return FALSE;
    }
    return TRUE;
}

BOOL CSingleVideoPlayer::InitRGBBack(DWORD dwWidth, DWORD dwHeight)
{
    DDSURFACEDESC2 ddsd;
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    DDPIXELFORMAT pixelFormat =	
    {sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 32,  0xFF0000, 0xFF00, 0xFF, 0}; //modify 
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH| DDSD_PIXELFORMAT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
    ddsd.ddpfPixelFormat = pixelFormat;
    ddsd.dwWidth = dwWidth;
    ddsd.dwHeight = dwHeight;

    HRESULT hr = m_pDD7->CreateSurface(&ddsd, &m_pDDSBack, NULL);
    if (hr != DD_OK)
    {
        TRACE(_T("InitRGBBack CreateSurface Back Error!\n"));
        return FALSE;
    }
    return TRUE;
}

// {
// CYUVSingleVideoPlayer
// 
BOOL CYUVSingleVideoPlayer::InitDirectDraw( HWND hwnd, DWORD dwWidth, DWORD dwHeight, const RECT* ShowRect /*= NULL*/ )
{
    return InitDirectDrawSome(hwnd,dwWidth,dwHeight,ShowRect) && InitYUVBack(dwWidth,dwHeight);
}

// YUV
void CYUVSingleVideoPlayer::Show(
    const tagRECT* rect, 
    const BYTE *pBuf,
    DWORD format, 
    int src420Subtype,
    const FILETIME* pTime,
    IVideoPlayCallBack* p )
{
    DDSURFACEDESC2 ddsd;
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);

    if (m_pDDSBack->IsLost() == DDERR_SURFACELOST)
        m_pDDSBack->Restore();

    HRESULT hr = m_pDDSBack->Lock(
        NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR, NULL);
    if (FAILED(hr))
    {
        return;
    }

    PBYTE pS = (PBYTE)ddsd.lpSurface;
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
    else { return; }

    hr = m_pDDSBack->Unlock(NULL);
    if (FAILED(hr))
        return;

    if (m_pddsPrimary->IsLost() == DDERR_SURFACELOST)
        m_pddsPrimary->Restore();
 
    m_pddsPrimary->Blt((tagRECT*)rect, m_pDDSBack, NULL, DDBLT_ASYNC, NULL);

    CSingleVideoPlayer_OnVideoPlay(
        m_pIVideoPlayCallBack, p, rect, pTime, m_hWnd); 
}

// CYUVSingleVideoPlayer
// }


// {
// CRGBSingleVideoPlayer
//
BOOL CRGBSingleVideoPlayer::InitDirectDraw( HWND hwnd, DWORD dwWidth, DWORD dwHeight, const RECT* ShowRect /*= NULL*/ )
{
    return InitDirectDrawSome(hwnd,dwWidth,dwHeight,ShowRect) && InitRGBBack(dwWidth,dwHeight);
}

void CRGBSingleVideoPlayer::Show(
    const tagRECT* rect,
    const BYTE *pBuf,
    const FILETIME* pTime,
    IVideoPlayCallBack* p )
{
    DDSURFACEDESC2 ddsd;
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);

    if (m_pDDSBack->IsLost() == DDERR_SURFACELOST) 
        m_pDDSBack->Restore();

    HRESULT hr = m_pDDSBack->Lock(
        NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR, 
        NULL);
    if (FAILED(hr))
    {
        // AfxMessageBox("ShowPreview Error");
        return;
    }

    PBYTE pRGB = (PBYTE)ddsd.lpSurface;
    for (DWORD i = 0; i < ddsd.dwHeight; i++)
    {
        memcpy(
            pRGB+ddsd.lPitch*(ddsd.dwHeight-i-1),
            pBuf+ddsd.dwWidth*4*i, 
            ddsd.dwWidth*4);
    }

    hr = m_pDDSBack->Unlock(NULL);
    if (FAILED(hr))
        return;

    if (m_pddsPrimary->IsLost() == DDERR_SURFACELOST) 
        m_pddsPrimary->Restore();

    m_pddsPrimary->Blt((tagRECT*)rect, m_pDDSBack, NULL, DDBLT_ASYNC, NULL);

    CSingleVideoPlayer_OnVideoPlay(
        m_pIVideoPlayCallBack, p, rect, pTime, m_hWnd);  
}

 



// End of files


