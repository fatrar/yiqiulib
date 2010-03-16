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
{
    for (int i=0; i<Max_SnapShot_Pic_Count; ++i)
    {
        m_hGlobal[i] = GlobalAlloc(GMEM_MOVEABLE, Max_SnapShot_Pic_Size);
    }

   // m_Image[0].Load(_T("E:\\1.jpg"));
   // m_Image[1].Load(_T("E:\\2.jpg"));
   // m_PicCtrl[0] = m_Image[0];
   // m_PicCtrl[1] = m_Image[1];
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


    return TRUE; // background is erased
}

LRESULT CSnapShotWnd::OnPaint(HDC hdc)
{
//TRACE(_T("CSnapShotWnd::OnPaint\n"));
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
    BYTE* pData,
    size_t nLen )
{
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
    //RedrawWindow();
    //Invalidate();
    //PostMessage(WM_PAINT);
    PostMessage(WM_REDRAWWINDOW);
}

HWND CSnapShotWnd::Create(
    HWND hWndParent,
    int nWidth, 
    SnapShotWnd::SnapShotPos Pos, 
    int nTop )
{
    int nPicCtrlWidth = int((nWidth-4*PicCtrl_X_Start)/3.0);
    int nPicCtrlHeight = int(nPicCtrlWidth*288/352.0);
    int nAllHeight = nPicCtrlHeight + 2*PicCtrl_Y_Start;
    
    CRect rect(0,0,nWidth,nAllHeight);
    if (Pos == SnapShotWnd::SnapShot_Top)
    {}
    else if (Pos == SnapShotWnd::SnapShot_Bottom)
    {
        CRect Tmprect;
        ::GetClientRect(hWndParent, &Tmprect);
        rect.MoveToY(Tmprect.Height()-nAllHeight);
    }
    else
    {
        rect.MoveToY(nTop);
    }
    __super::Create(hWndParent, _U_RECT(rect));
    ShowWindow(SW_SHOW);
    
    RECT Tmprect;
    Tmprect.top = PicCtrl_Y_Start;
    Tmprect.bottom = PicCtrl_Y_Start + nPicCtrlHeight; 
    for (int i=0; i<Max_SnapShot_Pic_Count; ++i)
    {
        Tmprect.left = PicCtrl_X_Start + i*(PicCtrl_X_Start+nPicCtrlWidth);   
        Tmprect.right = PicCtrl_X_Start + nPicCtrlWidth + i*(PicCtrl_X_Start+nPicCtrlWidth);       
        m_PicCtrl[i].Create(*this, _U_RECT(Tmprect));
        m_PicCtrl[i].ShowWindow(SW_SHOW);
    }

    return m_hWnd;
}

LRESULT CSnapShotWnd::OnRedrawWindow(
    UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    RedrawWindow();
    return TRUE;
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

}

// End of file
