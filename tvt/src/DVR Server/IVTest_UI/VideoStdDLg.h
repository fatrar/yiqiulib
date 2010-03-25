/**H**************************************************************************
 File            : VideoStdDLg.h
 Subsystem       : 
 Function Name(s): CVideoStdDLg
 Author          : YiQiu
 Date            : 2010-3-25  
 Time            : 18:41
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _VIDEOSTDDLG_H_2010_3
#define _VIDEOSTDDLG_H_2010_3
#pragma once



class CVideoStdDLg :
    public CDialogImpl<CVideoStdDLg>,
    public CWinDataExchange<CVideoStdDLg>
{
public:
    CVideoStdDLg(void);
public:
    int m_bSignal;
public:
	enum { IDD = IDD_DIALOGSELECTSIGNAL };


	BEGIN_MSG_MAP(CVideoStdDLg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
    END_MSG_MAP()

    BEGIN_DDX_MAP(CVideoStdDLg)
        DDX_RADIO(IDC_RADIO1, m_bSignal)
    END_DDX_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};





#endif  // _VIDEOSTDDLG_H_2010_


// End of file



