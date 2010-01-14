#if !defined(AFX_IVSCHUDULEWND_H__34E75BD8_9663_4F7C_8A8F_A8A3A25D52C8__INCLUDED_)
#define AFX_IVSCHUDULEWND_H__34E75BD8_9663_4F7C_8A8F_A8A3A25D52C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IVSchuduleWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIVSchuduleWnd window

class CIVSchuduleWnd : public CWnd
{
// Construction
public:
	CIVSchuduleWnd();

// Attributes
public:

public:
	BOOL Init(const CRect& rect);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIVSchuduleWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIVSchuduleWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIVSchuduleWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CTreeCtrl m_Tree;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IVSCHUDULEWND_H__34E75BD8_9663_4F7C_8A8F_A8A3A25D52C8__INCLUDED_)
