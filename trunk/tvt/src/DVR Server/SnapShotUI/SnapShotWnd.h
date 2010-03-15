/**H**************************************************************************
 File            : SnapShotWnd.h
 Subsystem       : 
 Function Name(s): CSnapShotWnd
 Author          : YiQiu
 Date            : 2010-3-15  
 Time            : 17:05
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _SNAPSHOTWND_H_2010_3
#define _SNAPSHOTWND_H_2010_3
#include <atlcrack.h>
#pragma once




class CSnapShotWnd
    : public CWindowImpl<CSnapShotWnd, CWindow, CControlWinTraits>
    , public ISnapShotSender

{
public:
    CSnapShotWnd(void);
    ~CSnapShotWnd(void);

public:
    DECLARE_WND_CLASS(_T("My Window Class"))

    BEGIN_MSG_MAP(CSnapShotWnd)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)
        MSG_WM_PAINT(OnPaint)
        MSG_WM_INITDIALOG(OnInitDialog)
    END_MSG_MAP()

public:
    HWND Create(HWND hWndParent, int nWidth);

     // ISnapShotSender
protected:
    virtual void OnSnapShotSend(
        int nChannelID, 
        DWORD dwRuleID,
        BYTE* pData,
        size_t nLen);

protected:
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnEraseBkgnd(HDC hdc);
    LRESULT OnPaint(HDC hdc);
    LRESULT OnInitDialog(HWND hwndFocus, LPARAM lParam);

protected:
    enum
    {
        PicCtrl_X_Start = 10,
        PicCtrl_Y_Start = 10,
        PicCtrl_X_Offset = 20,
        PicCtrl_Y_Offset = 20,

        PicCtrl_Width = 352,
        PicCtrl_Height = 288,
    };
    
private:
    CImage m_Image[Max_SnapShot_Pic_Count];
    HGLOBAL m_hGlobal[Max_SnapShot_Pic_Count];
    CPicCtrl m_PicCtrl[Max_SnapShot_Pic_Count];

public:
    
};








#endif  // _SNAPSHOTWND_H_2010_


// End of file



