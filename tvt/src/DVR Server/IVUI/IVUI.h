// IVUI.h : main header file for the IVUI DLL
//

#if !defined(AFX_IVUI_H__3A73596B_B454_461E_A1D0_64A68B8A2DB0__INCLUDED_)
#define AFX_IVUI_H__3A73596B_B454_461E_A1D0_64A68B8A2DB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIVUIApp
// See IVUI.cpp for the implementation of this class
//

class CIVUIApp : public CWinApp
{
public:
	CIVUIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIVUIApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIVUIApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IVUI_H__3A73596B_B454_461E_A1D0_64A68B8A2DB0__INCLUDED_)
