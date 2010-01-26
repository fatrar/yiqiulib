#if !defined(AFX_TEXTSTATIC_H__D707A555_111A_42DA_88A7_4FBB756C4B51__INCLUDED_)
#define AFX_TEXTSTATIC_H__D707A555_111A_42DA_88A7_4FBB756C4B51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextStatic window

class CTextStatic : public CStatic
{
// Construction
public:
	void SetFont(int High=14,int Wide=8);
	CTextStatic();
	virtual ~CTextStatic();
	void SetAlarmColor(COLORREF color);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextStatic)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CTextStatic)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	COLORREF m_color;
	COLORREF m_colorTmp;
	CFont	 *m_pCFont;
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTSTATIC_H__D707A555_111A_42DA_88A7_4FBB756C4B51__INCLUDED_)
