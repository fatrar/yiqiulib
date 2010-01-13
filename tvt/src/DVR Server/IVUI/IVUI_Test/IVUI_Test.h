// IVUI_Test.h : main header file for the IVUI_TEST application
//

#if !defined(AFX_IVUI_TEST_H__6B3F439D_704C_46E8_A6D8_662B827020D6__INCLUDED_)
#define AFX_IVUI_TEST_H__6B3F439D_704C_46E8_A6D8_662B827020D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIVUI_TestApp:
// See IVUI_Test.cpp for the implementation of this class
//

class CIVUI_TestApp : public CWinApp
{
public:
	CIVUI_TestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIVUI_TestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIVUI_TestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IVUI_TEST_H__6B3F439D_704C_46E8_A6D8_662B827020D6__INCLUDED_)
