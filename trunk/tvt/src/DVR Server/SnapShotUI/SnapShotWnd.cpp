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
{
    for (int i=0; i<Max_SnapShot_Pic_Count; ++i)
    {
        m_hGlobal[i] = GlobalAlloc(GMEM_MOVEABLE, Max_SnapShot_Pic_Size);
    }

    m_Image[0].Load(_T("E:\\1.jpg"));
    m_Image[1].Load(_T("E:\\2.jpg"));
    m_PicCtrl[0].SetImage(&m_Image[0]);
    m_PicCtrl[1].SetImage(&m_Image[1]);
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

}

HWND CSnapShotWnd::Create( HWND hWndParent, int nWidth )
{
    int nPicCtrlWidth = int((nWidth-4*PicCtrl_X_Start)/3.0);
    int nPicCtrlHeight = int(nPicCtrlWidth*288/352.0);
    int nAllHeight = nPicCtrlHeight + 2*PicCtrl_Y_Start;
    
    RECT rect;
    rect.top = rect.left = 0;
    rect.bottom = nAllHeight;
    rect.right = nWidth;
    __super::Create(hWndParent, _U_RECT(rect));
    ShowWindow(SW_SHOW);
    
    RECT Tmprect;
    for (int i=0; i<Max_SnapShot_Pic_Count; ++i)
    {
        Tmprect.left = PicCtrl_X_Start + i*(PicCtrl_X_Start+nPicCtrlWidth);
        Tmprect.top = PicCtrl_Y_Start;
        Tmprect.right = PicCtrl_X_Start + nPicCtrlWidth + i*(PicCtrl_X_Start+nPicCtrlWidth);
        Tmprect.bottom = PicCtrl_Y_Start + nPicCtrlHeight; 
        m_PicCtrl[i].Create(*this, _U_RECT(Tmprect));
        m_PicCtrl[i].ShowWindow(SW_SHOW);
    }

    return m_hWnd;
}


#include "ISnapShot.h"

static CSnapShotWnd g_Wnd;

HWND CreateSnapShotWnd( HWND hWndParent, int nWidth )
{
    return g_Wnd.Create(hWndParent, nWidth); 
}

BOOL DestroySnapShotWnd()
{
    return g_Wnd.DestroyWindow();
}


// End of file
