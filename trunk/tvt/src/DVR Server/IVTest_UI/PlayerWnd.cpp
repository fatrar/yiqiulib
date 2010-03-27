/**CPP*************************************************************************
 File            : PlayerWnd.cpp
 Subsystem       : 
 Function Name(s): CPlayerWnd
 Author          : YiQiu
 Date            : 2010-3-26  
 Time            : 14:50
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "StdAfx.h"
#include "PlayerWnd.h"


CPlayerWnd::CPlayerWnd(void)
{
}

CPlayerWnd::~CPlayerWnd(void)
{
}

LRESULT CPlayerWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_Player.UnitDirectDraw();
    DestroyWindow();
    return 0;
}

LRESULT CPlayerWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    PostQuitMessage(0);
    return 0;
}

LRESULT CPlayerWnd::OnEraseBkgnd(HDC hdc)
{
    return FALSE;
    //return TRUE; // background is erased
}

LRESULT CPlayerWnd::OnPaint(HDC hdc)
{
    CPaintDC dc(m_hWnd);
    m_Player.ShowBack();
    return 0;
}

LRESULT CPlayerWnd::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
    
    return TRUE; // set focus to default control
}

void CPlayerWnd::Init(
    HWND hwnd, CRect& rect,
    DWORD dwWidth, DWORD dwHeight)
{
    Create(hwnd, rect);
    ShowWindow(SW_SHOW);
    m_Player.InitDirectDraw(
        *this, dwWidth, dwHeight );
    //m_Player.SetVideoPlayCallback(this, -1);
}

BOOL CPlayerWnd::OnVideoPlay(
    HDC dc,
    const tagRECT* rect,    // 针对屏幕坐标
    const FILETIME* pTime,
    HWND hwnd, int nFlag,
    DWORD dwUserData )
{
    static IIVViewer* pViewer = IVLiveFactory::GetLiveViewer();
    pViewer->Paint(dwUserData, dc, *rect, *pTime);
    return TRUE;
}

void CPlayerWnd::VideoComing(
    DWORD dwVideoFormat,
    FRAMEBUFSTRUCT *bufs )
{
    if ( !IsWindow() )
    {
        return;
    }
    // 为了支持多分辨显示，及CIF变为D1等，不用static
    /*static*/ DWORD s_dwWidth = g_pFormat[dwVideoFormat][0];
    /*static*/ DWORD s_dwHeight = g_pFormat[dwVideoFormat][1];

    int nChannel = bufs->ChannelIndex;
    CRect Rect;
    GetClientRect(&Rect);
    ClientToScreen(&Rect);

    FILETIME* pTime = (FILETIME*)&bufs->FrameTime;

    m_Player.Show(
        Rect, bufs->pBuf,
        s_dwWidth, s_dwHeight,
        CSingleVideoPlayer::YUV422,
        0, pTime,
        this, bufs->ChannelIndex );
}


// End of file