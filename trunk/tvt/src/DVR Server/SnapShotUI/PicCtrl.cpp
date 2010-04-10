/**CPP*************************************************************************
 File            : PicCtrl.cpp
 Subsystem       : 
 Function Name(s): CPicCtrl
 Author          : YiQiu
 Date            : 2010-3-15  
 Time            : 17:35
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "StdAfx.h"
#include "PicCtrl.h"



CPicCtrl::CPicCtrl(void)
    : m_pImage(NULL)
{
}

CPicCtrl::~CPicCtrl(void)
{
}

LRESULT CPicCtrl::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DestroyWindow();
    return 0;
}

LRESULT CPicCtrl::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    PostQuitMessage(0);
    return 0;
}

LRESULT CPicCtrl::OnEraseBkgnd(HDC hdc)
{
    // heliang fix
    return FALSE; // background is erased
}

LRESULT CPicCtrl::OnPaint(HDC hdc)
{
    CPaintDC dc2(m_hWnd);
    return 0;
    if ( m_pImage )
    {
        CDC dc(GetDC());
        RECT rect;
        GetClientRect(&rect);
        int nOld = dc.SetStretchBltMode(STRETCH_HALFTONE);
        m_pImage->StretchBlt(
            dc, 0, 0, rect.right, rect.bottom);
        dc.SetStretchBltMode(nOld);
    }
    return 0;
}



// End of file