#if !defined(AFX_IVSWTICHTAB_H__C26A0DBB_D2F8_43E0_AEAA_85D657FB9044__INCLUDED_)
#define AFX_IVSWTICHTAB_H__C26A0DBB_D2F8_43E0_AEAA_85D657FB9044__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IVSwtichTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIVSwtichTab window

class CIVSwtichTab : public CWnd
{
// Construction
public:
	CIVSwtichTab();

// Attributes
public:
	BOOL Init(CWnd* pWnd, const CRect& rect);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIVSwtichTab)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIVSwtichTab();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIVSwtichTab)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void ClickBt(UINT ID, WPARAM w, LPARAM l);
	afx_msg void OnNcHitTest2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	enum
	{
		TAB_BT_NUM = 3,
	};
private:
	CButton m_TabBt[TAB_BT_NUM];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IVSWTICHTAB_H__C26A0DBB_D2F8_43E0_AEAA_85D657FB9044__INCLUDED_)
