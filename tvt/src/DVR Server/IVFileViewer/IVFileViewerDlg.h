// IVFileViewerDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CIVFileViewerDlg dialog
class CIVFileViewerDlg : public CDialog
{
// Construction
public:
	CIVFileViewerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_IVFILEVIEWER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
    BOOL ParseFile();

    template<IVFileVersionDefine Version>
    BOOL LoadHead();

    template<IVFileVersionDefine Version>
    BOOL PushDataTolist(
        IVFileHead<Version>& Head );

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOpen();


    ifstream m_Reader;
    CListBox m_DataList;
    CString m_strStartTime;
    CString m_strEndTime;
};
