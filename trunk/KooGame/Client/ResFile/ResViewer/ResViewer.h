// ResViewer.h : main header file for the ResViewer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CResViewerApp:
// See ResViewer.cpp for the implementation of this class
//

class CResViewerApp : public CWinApp
{
public:
	CResViewerApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
    virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
};

extern CResViewerApp theApp;