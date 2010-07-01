// GameTool.h : main header file for the GameTool application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGameToolApp:
// See GameTool.cpp for the implementation of this class
//

class CGameToolApp : public CWinApp
{
public:
	CGameToolApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGameToolApp theApp;