#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CFilterDlg dialog

class CFilterDlg 
    : public CDialog
    , public IVideoSend
    , public IVideoPlayCallBack
{
	DECLARE_DYNAMIC(CFilterDlg)

public:
	CFilterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFilterDlg();

// Dialog Data
	enum { IDD = IDD_FILTER };

    void SetComomParm(int nChannelID, WPG_Rule* pRule);

    // IVideoSend
protected:
    virtual BOOL OnVideoSend(FRAMEBUFSTRUCT *bufStruct);

    // IVideoPlayCallBack
protected:
    virtual BOOL OnVideoPlay(
        HDC dc,
        const tagRECT* rect,    // Õë¶ÔÆÁÄ»×ø±ê
        const FILETIME* pTime,
        HWND hwnd,
        int nFlag );

    void InitByRule();

    void DoClickOne(int nItem);
protected:
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg void OnPaint();
    afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedFilterOn();
    afx_msg void OnBnClickedFilterOff();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
	DECLARE_MESSAGE_MAP()
 
    enum 
    {
        DrawWnd_ID = 0x8101,
    };

private:
    CListCtrl m_list;
    CString m_strFilterInfo;
    CButton m_FiterOnBt;
    CButton m_FiterOffBt;

    CString m_strStateOff;
    CString m_strStateOn;

    CString m_strMultiplier;
    float m_nMultiplierEdit;

    Windows::IDrawContainer* m_pDrawContainer;
    Windows::IDrawer* m_szRectangleDrawer[4];
    CYUVSingleVideoPlayer m_Player;
    CRect m_Rect;

    int m_nCurrentChan;
    WPG_Rule* m_pRule;
    int m_nSelectRow;
};
