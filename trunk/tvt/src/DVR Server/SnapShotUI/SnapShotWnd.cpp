/**CPP*************************************************************************
 File            : SnapShotWnd.cpp
 Subsystem       : 
 Function Name(s): CSnapShotWnd
 Author          : YiQiu
 Date            : 2010-3-15  
 Time            : 17:17
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "StdAfx.h"
#include "SnapShotWnd.h"

CSnapShotWnd::CSnapShotWnd(void)
    : m_nIndex(NULL)
    , n_bState(TRUE)
{
    for (int i=0; i<Max_SnapShot_Pic_Count; ++i)
    {
        m_hGlobal[i] = GlobalAlloc(GMEM_MOVEABLE, Max_SnapShot_Pic_Size);
    }

   //m_Image[0].Load(_T("E:\\1.jpg"));
   //m_Image[1].Load(_T("E:\\2.jpg"));
   //m_PicCtrl[0] = m_Image[0];
   //m_PicCtrl[1] = m_Image[1];
}

CSnapShotWnd::~CSnapShotWnd(void)
{
    for (int i=0; i<Max_SnapShot_Pic_Count; ++i)
    {
        GlobalFree(m_hGlobal[i]);
        m_hGlobal[i] = NULL;
    }
}

LRESULT CSnapShotWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DestroyWindow();
    return 0;
}

LRESULT CSnapShotWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    PostQuitMessage(0);
    return 0;
}

LRESULT CSnapShotWnd::OnEraseBkgnd(HDC hdc)
{
    //return FALSE;
    CDCHandle dc(hdc);
    //HBRUSH hOldBrush = dc.SelectStockBrush(BLACK_BRUSH);
    RECT rect;
    GetClientRect(&rect);
    dc.FillSolidRect(&rect, RGB(0,0,0));
    
    //this->SetMsgHandled(FALSE);// SetHandle(FALSE)£»
    return TRUE; // background is erased
}

LRESULT CSnapShotWnd::OnPaint(HDC hdc)
{
    CPaintDC dc(m_hWnd);
    return 0;
}
 
LRESULT CSnapShotWnd::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
    return TRUE; // set focus to default control
}

void CSnapShotWnd::OnSnapShotSend(
    int nChannelID,
    DWORD dwRuleID, 
    FILETIME* pTime,
    BYTE* pData,
    size_t nLen )
{
    return;

    bool bOver = m_nIndex >= Max_SnapShot_Pic_Count;
    size_t nIndex = m_nIndex%Max_SnapShot_Pic_Count;
    if ( bOver )
    {
        m_Image[nIndex].Destroy();
    }
 
    HGLOBAL hGlobal = m_hGlobal[nIndex];
    LPVOID pvData = GlobalLock(hGlobal); 
    if (pvData == NULL)//Ëø¶¨·ÖÅäÄÚ´æ¿é 
    { 
        TRACE(_T("CSnapShotWnd::OnSnapShotSend GlobalLock = NULL\n"));
        assert(false);     
        return;
    }
    memcpy(pvData, pData, nLen); 
    GlobalUnlock(hGlobal); 

        
    CComQIPtr<IStream> spStm;
    HRESULT hr = CreateStreamOnHGlobal(
        hGlobal, FALSE, &spStm);
    if(FAILED(hr)) 
    {
        TRACE(_T("CSnapShotWnd::OnSnapShotSend CreateStreamOnHGlobal Failed\n"));
        assert(false); 
        return;
    }

    m_Image[nIndex].Load(spStm);
    if ( bOver )
    {     
        for (size_t i=0; i<Max_SnapShot_Pic_Count-1; ++i)
        {
            m_PicCtrl[i].Attach(m_PicCtrl[i+1]);
        }
        m_PicCtrl[Max_SnapShot_Pic_Count-1] = m_Image[nIndex];
    }
    else
    {
        m_PicCtrl[nIndex] = m_Image[nIndex];
    }

    ++m_nIndex;

    PostMessage(WM_REDRAWWINDOW);
}

HWND CSnapShotWnd::Create(
    HWND hWndParent,
    int nWidth, 
    SnapShotWnd::SnapShotPos Pos, 
    int nTop )
{
    CRect Parentrect;
    ::GetClientRect(hWndParent, &Parentrect);
    if (nWidth == 0)
    {
        nWidth = Parentrect.Width();
    }

    int nPicCtrlWidth = int((nWidth-(Max_SnapShot_Pic_Count+1)*PicCtrl_X_Start)*1.0/Max_SnapShot_Pic_Count);
    int nPicCtrlHeight = int(nPicCtrlWidth*1.0*PicCtrl_Height/PicCtrl_Width);
    int nAllHeight = nPicCtrlHeight + 2*PicCtrl_Y_Start + Bt_Height;
   
    m_MaxRect.SetRect(0,0,nWidth,nAllHeight);
    if (Pos == SnapShotWnd::SnapShot_Top)
    {}
    else if (Pos == SnapShotWnd::SnapShot_Bottom)
    {
        m_MaxRect.MoveToY(Parentrect.Height()-nAllHeight);
    }
    else
    {
        m_MaxRect.MoveToY(nTop);
    }
    __super::Create(hWndParent, m_MaxRect);
    ShowWindow(SW_SHOW);
    
    m_MinRect.SetRect(0, Parentrect.Height()-Bt_Height, nWidth, Parentrect.Height());

    CRect BTRect(0,0,nWidth,Bt_Height);
    m_ShowBt.Create(
        *this, BTRect,
        _T("Close"), WS_CHILD|WS_VISIBLE,0,
        IDC_CLICKBT );
    m_ShowBt.ShowWindow(SW_SHOW);

    RECT Tmprect;
    Tmprect.top = PicCtrl_Y_Start + Bt_Height;
    Tmprect.bottom = Tmprect.top + nPicCtrlHeight; 
    for (int i=0; i<Max_SnapShot_Pic_Count; ++i)
    {
        Tmprect.left = PicCtrl_X_Start + i*(PicCtrl_X_Start+nPicCtrlWidth);   
        Tmprect.right = PicCtrl_X_Start + nPicCtrlWidth + i*(PicCtrl_X_Start+nPicCtrlWidth);       
        m_PicCtrl[i].Create(*this, Tmprect);
        m_PicCtrl[i].ShowWindow(SW_SHOW);
    }

    return m_hWnd;
}

LRESULT CSnapShotWnd::OnRedrawWindow(
    UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    for (int i = 0; i< Max_SnapShot_Pic_Count; ++i)
        m_PicCtrl[i].RedrawWindow();
    return TRUE;
}

void CSnapShotWnd::OnClickBT(
    UINT uCode, int nID, HWND hwndCtrl )
{
    TRACE(_T("One Click!!!\n"));
    n_bState = !n_bState;
    if ( n_bState )
    {
        MoveWindow(&m_MaxRect);
    }
    else
    {
        MoveWindow(&m_MinRect);
    }
    //return TRUE;
}


namespace SnapShotWnd
{

static CSnapShotWnd g_Wnd;

ISnapShotSender* CreateSnapShotWnd(
    HWND hWnd, int nWidth,
    SnapShotPos Pos,
    int nTop )
{
    /*hWnd = */g_Wnd.Create(hWnd, nWidth, Pos, nTop);
    return &g_Wnd;
}

BOOL DestroySnapShotWnd()
{
    return g_Wnd.DestroyWindow();
}

BOOL ShowSnapShotWnd( int nCmdShow )
{
    return g_Wnd.ShowWindow(nCmdShow);
}

}

// End of file