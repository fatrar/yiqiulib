/**H**************************************************************************
 File            : PlayerWnd.h
 Subsystem       : 
 Function Name(s): CPlayerWnd
 Author          : YiQiu
 Date            : 2010-3-26  
 Time            : 11:16
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _PLAYERWND_H_2010_3
#define _PLAYERWND_H_2010_3
#include <atlcrack.h>
#pragma once


#define YUV_Play

class CPlayerWnd :
    public CWindowImpl<CPlayerWnd, CWindow, CControlWinTraits>, 
    public IVideoPlayCallBack
{
public:
    CPlayerWnd(void);
    ~CPlayerWnd(void);

public:
    DECLARE_WND_CLASS(_T("Yiqiu CPlayerWnd Window Class"))

    BEGIN_MSG_MAP(CPlayerWnd)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)
        MSG_WM_PAINT(OnPaint)
        MSG_WM_INITDIALOG(OnInitDialog)
    END_MSG_MAP()

public:
    void Init(
        HWND hwnd, CRect& rect,
        DWORD dwWidth, DWORD dwHeight);

    void VideoComing(
        DWORD dwVideoFormat, 
        FRAMEBUFSTRUCT *bufs );

protected:
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnEraseBkgnd(HDC hdc);
    LRESULT OnPaint(HDC hdc);
    LRESULT OnInitDialog(HWND hwndFocus, LPARAM lParam);

    // IVideoPlayCallBack
protected:
    virtual BOOL OnVideoPlay(
        HDC dc,
        const tagRECT* rect,    // Õë¶ÔÆÁÄ»×ø±ê
        const FILETIME* pTime,
        HWND hwnd,
        int nFlag,
        DWORD dwUserData );

private:

#ifdef YUV_Play
    CYUVSingleVideoPlayerEx m_Player;
#else
    CRGBSingleVideoPlayerEx m_Player;
    BYTE* m_pRGBBuf;
#endif // YUV_Play
    
    
};








#endif  // _PLAYERWND_H_2010_


// End of file



