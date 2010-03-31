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


typedef CWinTraits<
      WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN| 
      WS_CLIPSIBLINGS|WS_BORDER|
      WS_EX_DLGMODALFRAME, 0> CMyControlWinTraits;


class CSnapShotWnd
    : public CWindowImpl<CSnapShotWnd, CWindow, CMyControlWinTraits>
    , public ISnapShotSender
{
public:
    CSnapShotWnd(void);
    ~CSnapShotWnd(void);

protected:
    enum
    {
        PicCtrl_X_Start = 10,
        PicCtrl_Y_Start = 10,
        PicCtrl_X_Offset = 20,
        PicCtrl_Y_Offset = 20,

        PicCtrl_Width = 352,
        PicCtrl_Height = 288,

        Bt_Height = 20,


        WM_REDRAWWINDOW = WM_USER + 100,
        IDC_CLICKBT = WM_USER + 200,
    };

public:
    DECLARE_WND_CLASS(_T("Yiqiu SnapShotWnd Window Class"))

    BEGIN_MSG_MAP(CSnapShotWnd)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_REDRAWWINDOW, OnRedrawWindow)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)
        MSG_WM_PAINT(OnPaint)
        MSG_WM_INITDIALOG(OnInitDialog)
        COMMAND_ID_HANDLER_EX(IDC_CLICKBT, OnClickBT)
        //COMMAND_CODE_HANDLER_EX(BN_CLICKED, OnBnClicked)
    END_MSG_MAP()

public:
    HWND Create(
        HWND hWndParent,
        int nWidth, 
        SnapShotWnd::SnapShotPos Pos, 
        int nTop);

     // ISnapShotSender
protected:
    virtual void OnSnapShotSend(
        int nChannelID, 
        DWORD dwRuleID,
        FILETIME* pTime,
        BYTE* pData,
        size_t nLen);

protected:
    LRESULT OnRedrawWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnEraseBkgnd(HDC hdc);
    LRESULT OnPaint(HDC hdc);
    LRESULT OnInitDialog(HWND hwndFocus, LPARAM lParam);
    void OnClickBT(UINT uCode, int nID, HWND hwndCtrl);

private:
    CImage m_Image[Max_SnapShot_Pic_Count];
    HGLOBAL m_hGlobal[Max_SnapShot_Pic_Count];
    CPicCtrl m_PicCtrl[Max_SnapShot_Pic_Count];

    size_t m_nIndex;

    CRect m_MinRect;
    CRect m_MaxRect;
    CButton m_ShowBt;
    BOOL n_bState;
};








#endif  // _SNAPSHOTWND_H_2010_


// End of file



