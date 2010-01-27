// QCDlg.h : header file
//

#if !defined(AFX_QCDLG_H__610DAF09_9F68_40E1_BE5D_F0B78D9916CB__INCLUDED_)
#define AFX_QCDLG_H__610DAF09_9F68_40E1_BE5D_F0B78D9916CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "define.h"
/////////////////////////////////////////////////////////////////////////////
// CQCDlg dialog
//#include "testcard.h"
#include "DirectDraw.h"	// Added by ClassView
//#include "alarmtest.h"
#include "Overlay.h"	// Added by ClassView
#include "vfw.h"
#include ".\devicecontrol\devicecontrol.h"
#include "CodecMgr.h"
#include "TextStatic.h"

class CDirectDraw;
class CQCDlg : public CDialog
{
public:
	static PBYTE m_pDecodedData;
	CCodecMgr m_CodecMgr;	//
//	COverlay m_Overlay;
//	CTestCard m_TestCard;	//
	CARD_INFOR m_card_infor;
	CCodecMgr* m_CodecMgrH_Rec;//zld 2009/4/27 4408
	CCodecMgr* m_CodecMgrH_Net;//zld 2009/4/27 4408
	static PBYTE m_pDecodedData_Rec; //zld 2009/4/29 4408
	static PBYTE m_pDecodedData_Net; //zld 2009/4/29 4408
	static int m_Vchannel_Num;//视频通道数 zld 2009/4/29 4408
	int m_Achannel_Num;//音频通道数 zld 2009/4/29 4408
//    int m_PreWidth;
//    int m_Preheight;
    int m_RECWidth;//by chenlong
    int m_RECHeight;//by chenlong
    int m_NetWidth;//by chenlong
    int m_NetHeight;//by chenlong
    BOOL m_bFresh;//by chenlong
// Construction
public:

	BOOL m_bRun;
	CString m_strUser;
	DWORD m_dwVideoFormat;
	CDirectDraw *m_ddraw;	//
	DVRENUM_VIDEO_SIZE_TYPE m_video_size_type;
	BOOL WriterConfigInfor();
	BOOL GetConfigInfor();
	BOOL m_bCanDoSectionTest;
	void RefreshMainControl();

	CString m_ExePath;
	BITMAPINFOHEADER bitmapinfohead;
	static BOOL VideoCALLBACKFUNC(FRAMEBUFSTRUCT *bufs);
	static BOOL AudioCALLBACKFUNC(FRAMEBUFSTRUCT *bufs);
	void InitialVar();
	BOOL m_ChShow[16];
	BOOL m_ChChecked[16];
	void VideoChannelControl(int ch_num);
	BOOL m_haveAlarmCard;
	CQCDlg(CWnd* pParent = NULL);	// standard constructor
    void InitChangeVideoSize(int nID);
    int GetChannelNum(int nId);
    void FormatCodecMgrH(int nIndex, int codecType);
    void VideoFormatChange(DWORD videoformat);
    BOOL LoadMatchLibrary(int nId);
    void LoadVideoRadio(int nBegin, int nEnd, BOOL bShow);

	CDeviceManager *m_MyDSP; //zld 2009/4/23 3304

    int m_VideoSize;   //by chenlong

// Dialog Data
	//{{AFX_DATA(CQCDlg)
	enum { IDD = IDD_QC_DIALOG };

	CButton	m_cCh4;
	CButton	m_cCh3;
	CButton	m_cCh2;
	CButton	m_cCh1;

	BOOL	m_ch1;
	BOOL	m_ch2;
	BOOL	m_ch3;
	BOOL	m_ch4;

	CString	m_view_card_type;
	int		m_nCurrentChnanel;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQCDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	/*CDeviceManager *m_MyDSP;*/	//
	HINSTANCE m_DSPDLL;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CQCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFinish();
	afx_msg void OnCh1();
	afx_msg void OnCh2();
	afx_msg void OnCh3();
	afx_msg void OnCh4();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnViewmode1();
	afx_msg void OnViewmode16();
	afx_msg void OnBtnNextview();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	VOID YUV420_YUV422Pack(void *pDst, void *pSrc, unsigned int nWidth, unsigned int nHeight, unsigned int nPitch,INT src420Subtype);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QCDLG_H__610DAF09_9F68_40E1_BE5D_F0B78D9916CB__INCLUDED_)
