// QC.h : main header file for the QC application
//

#if !defined(AFX_QC_H__30485C4B_54A2_4364_9282_0903758C8CA1__INCLUDED_)
#define AFX_QC_H__30485C4B_54A2_4364_9282_0903758C8CA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CQCApp:
// See QC.cpp for the implementation of this class
//

class CQCApp : public CWinApp
{
public:
	CQCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CQCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QC_H__30485C4B_54A2_4364_9282_0903758C8CA1__INCLUDED_)
