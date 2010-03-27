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



// 这里是为了提高性能，才判断两次
#define CSingleVideoPlayer_OnVideoPlay(a, b, rect, pTime, hwnd) \
    if ( a || b )\
    {\
        HDC dc;\
        m_pddsPrimary->GetDC(&dc);\
        if ( a )\
        {\
            a->OnVideoPlay(dc, rect, pTime, hwnd, 1, m_dwUserData);\
        }\
        if ( b )\
        {\
            b->OnVideoPlay(dc, rect, pTime, hwnd, 0, dwUserData);\
        }\
        m_pddsPrimary->ReleaseDC(dc);\
    } 


void CSingleVideoPlayer::OnVideoPlay(
    const tagRECT* rect,
    const FILETIME* pTime,
    IVideoPlayCallBack* p,
    DWORD dwUserData )
{
    if ( p || m_pIVideoPlayCallBack )
    {
        HDC dc;
        m_pddsPrimary->GetDC(&dc);
        if ( m_pIVideoPlayCallBack )
        {
            m_pIVideoPlayCallBack->OnVideoPlay(
                dc, rect, pTime, m_hWnd, 1, m_dwUserData);
        }
        if ( p )
        {
            p->OnVideoPlay(
                dc, rect, pTime, m_hWnd, 0, dwUserData);
        }
        m_pddsPrimary->ReleaseDC(dc);
    } 
}
#define OVERLAY_KEYCOLOR (RGB(0, 0, 0))

CSingleVideoPlayer::CSingleVideoPlayer()
    : m_hWnd(NULL)
    , m_pDD7(NULL)
    , m_pddsPrimary(NULL)
    , m_lpClipper(NULL)
    , m_pDDSBack(NULL)
    , m_dwBackColour(RGB(128,128,128))
    , m_pIVideoPlayCallBack(NULL)
    , m_dwUserData(0) {}

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
        OutputDebugString(_T("DirectDrawCreate Error!"));
        return FALSE;
    }
    hr = m_pDD7->SetCooperativeLevel(m_hWnd, DDSCL_NORMAL);
    if(FAILED(hr))
    {
        safeRelease(m_pDD7);
        return FALSE;
    }

    DDSURFACEDESC2 ddsd = {sizeof(DDSURFACEDESC2)};
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
    safeRelease(m_pDDSBack);
    safeRelease(m_lpClipper);
    safeRelease(m_pddsPrimary);
    safeRelease(m_pDD7);
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
    m_dwWidth = dwWidth;
    m_dwHeight = dwHeight;

    DDSURFACEDESC2 ddsd = {sizeof(DDSURFACEDESC2)};
    
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
        OutputDebugString(_T("InitYUVBack CreateSurface Back Error!\n"));
        return FALSE;
    }
    return TRUE;
}

BOOL CSingleVideoPlayer::InitRGBBack(DWORD dwWidth, DWORD dwHeight)
{
    m_dwWidth = dwWidth;
    m_dwHeight = dwHeight;

    DDSURFACEDESC2 ddsd = {sizeof(DDSURFACEDESC2)};
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
        OutputDebugString(_T("InitRGBBack CreateSurface Back Error!\n"));
        return FALSE;
    }
    return TRUE;
}


BOOL CSingleVideoPlayer::ReInitYUVBack( DWORD dwWidth, DWORD dwHeight )
{
    safeRelease(m_pDDSBack);
    return InitYUVBack(dwWidth, dwHeight);
}

BOOL CSingleVideoPlayer::ReInitRGBBack( DWORD dwWidth, DWORD dwHeight )
{
    safeRelease(m_pDDSBack);
    return InitRGBBack(dwWidth, dwHeight);
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
    IVideoPlayCallBack* p,
    DWORD dwUserData )
{
    if (m_pDDSBack->IsLost() == DDERR_SURFACELOST)
        m_pDDSBack->Restore();

    DDSURFACEDESC2 ddsd = {sizeof(DDSURFACEDESC2)};
    HRESULT hr = m_pDDSBack->Lock(
        NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR, NULL);
    if (FAILED(hr))
    {
        return;
    }

    PBYTE pS = (PBYTE)ddsd.lpSurface;
    if (format == YUV420) //djx 2008/7/22 4108
    {
        CPicTransformUtil::YUV420_YUV422Pack(pS, pBuf, ddsd.dwWidth, ddsd.dwHeight, ddsd.lPitch,src420Subtype);
    }
    else if (format == YUV422)
    {
        CPicTransformUtil::YUV422Pack(pS, pBuf, ddsd.dwWidth, ddsd.dwHeight, ddsd.lPitch);
    }
    else if (format == YUV422PACK)
    {
        for (DWORD i = 0; i < ddsd.dwHeight; i++)
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

    OnVideoPlay(rect, pTime, p, dwUserData); 
}

// CYUVSingleVideoPlayer
// }

// {
// CYUVSingleVideoPlayerEx
// 
// YUV
void CYUVSingleVideoPlayerEx::Show(
    const tagRECT* rect, 
    const BYTE *pBuf,
    DWORD dwWidth, DWORD dwHeight,
    DWORD format, 
    int src420Subtype,
    const FILETIME* pTime,
    IVideoPlayCallBack* p,
    DWORD dwUserData )
{
    if ( dwWidth!= m_dwWidth || dwHeight != m_dwHeight )
    {
        ReInitYUVBack(dwWidth, dwHeight);
    }
    return CYUVSingleVideoPlayer::Show(
        rect, pBuf,
        format, src420Subtype,
        pTime, p, dwUserData );
}
// CYUVSingleVideoPlayerEx
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
    IVideoPlayCallBack* p,
    DWORD dwUserData )
{
    if (m_pDDSBack->IsLost() == DDERR_SURFACELOST) 
        m_pDDSBack->Restore();

    DDSURFACEDESC2 ddsd = { sizeof(DDSURFACEDESC2) };
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

    OnVideoPlay(rect, pTime, p, dwUserData); 
}
// CRGBSingleVideoPlayer
// }

// {
// CRGBSingleVideoPlayer
//

void CRGBSingleVideoPlayerEx::Show(
    const tagRECT* rect,
    const BYTE *pBuf,
    DWORD dwWidth, 
    DWORD dwHeight, 
    const FILETIME* pTime /*= NULL*/, 
    IVideoPlayCallBack* p /*= NULL*/,
    DWORD dwUserData )
{
    if ( dwWidth!= m_dwWidth || dwHeight != m_dwHeight )
    {
        ReInitRGBBack(dwWidth, dwHeight);
    }
    return CRGBSingleVideoPlayer::Show(
        rect, pBuf, pTime, p, dwUserData );
}

// CRGBSingleVideoPlayerEx
// }

// End of files


