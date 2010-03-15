/**H**************************************************************************
 File            : PicCtrl.h
 Subsystem       : 
 Function Name(s): CPicCtrl
 Author          : YiQiu
 Date            : 2010-3-15  
 Time            : 17:34
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _PICCTRL_H_2010_3
#define _PICCTRL_H_2010_3
#include <atlcrack.h>
#pragma once



class CPicCtrl : 
    public CWindowImpl<CPicCtrl, CWindow, CControlWinTraits>
{
public:
    CPicCtrl(void);
    ~CPicCtrl(void);

public:
    DECLARE_WND_CLASS(_T("My CPicCtrl"))

    BEGIN_MSG_MAP(CPicCtrl)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)
        MSG_WM_PAINT(OnPaint)
    END_MSG_MAP()

public:
    void SetImage(CImage* pImage){m_pImage=pImage;}

protected:
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnEraseBkgnd(HDC hdc);
    LRESULT OnPaint(HDC hdc);

protected:
    
    enum
    {

    };
protected:
    CImage* m_pImage;
};









#endif  // _PICCTRL_H_2010_


// End of file



