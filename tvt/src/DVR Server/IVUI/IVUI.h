// IVUI.h : main header file for the IVUI DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CIVUIApp
// See IVUI.cpp for the implementation of this class
//

class CIVUIApp : public CWinApp
{
public:
	CIVUIApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
