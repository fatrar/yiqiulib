// Windows.h : main header file for the Windows DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CWindowsApp
// See Windows.cpp for the implementation of this class
//

class CWindowsApp : public CWinApp
{
public:
	CWindowsApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
