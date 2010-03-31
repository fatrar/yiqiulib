/**H**************************************************************************
 File            : MainDlg.h
 Subsystem       : 
 Function Name(s): CMainDlg
 Author          : YiQiu
 Date            : 2010-3-25  
 Time            : 19:00
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _MAINDLG_H_2010_3
#define _MAINDLG_H_2010_3
#include <atlcrack.h>
#include "resource.h"
#include "IVCfgDlg.h"
#pragma once

class CMainDlg :
    public CDialogImpl<CMainDlg>, 
    public CUpdateUI<CMainDlg>,
    public CMessageFilter,
    public CIdleHandler
{
public:
    CMainDlg();

	enum { IDD = IDD_MAINDLG };

    // CUpdateUI
public:
    BEGIN_UPDATE_UI_MAP(CMainDlg)
    END_UPDATE_UI_MAP()

    // CMessageFilter
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    
    // CIdleHandler
protected:
	virtual BOOL OnIdle(){return FALSE;}

	BEGIN_MSG_MAP(CMainDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MSG_WM_CLOSE(OnClose)
        COMMAND_ID_HANDLER_EX(ID_CONFIG_IVCONFIG, OnConfigIvconfig)
    END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(void);
    LRESULT OnConfigIvconfig(WORD wNotifyCode, WORD wID, HWND hWndCtl);

protected:
    void Init();
    void InitPlayer();
private:
    static BOOL VideoComing(FRAMEBUFSTRUCT *bufs);
    static BOOL AudioComing(FRAMEBUFSTRUCT *bufs);

    enum
    {
        Max_Channel = 4,
    };

private:
    HINSTANCE m_Hinstance;
    CDeviceManager* m_pDeviceManager;
    ISnapShotSender* m_pSnapShotSender;
    DWORD m_dwVideoFormat;
    DWORD m_dwChannelCount;

    CPlayerWnd m_PlayerWnd[Max_Channel];  

    CIVCfgDlg m_CIVCfgDlg;

public:
    
};








#endif  // _MAINDLG_H_2010_


// End of file





