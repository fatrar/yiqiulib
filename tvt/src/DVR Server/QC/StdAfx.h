// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__FEAC91D6_3417_4B52_BB16_BE700F2CD3E1__INCLUDED_)
#define AFX_STDAFX_H__FEAC91D6_3417_4B52_BB16_BE700F2CD3E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
//#include "define.h"
#include <ddraw.h>
#include <atlbase.h>
#include "define.h"
#endif // _AFX_NO_AFXCMN_SUPPORT
//#include <ole2.h>			//ADO
//*************引入ADO库文件***************//
#pragma once
#pragma warning (disable: 4146)
#import "C:\Program Files\Common Files\System\ado\msado15.dll"\
no_namespace rename("EOF","adoEOF")

extern int g_6800_6805_check;

#include "DeviceDriver.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__FEAC91D6_3417_4B52_BB16_BE700F2CD3E1__INCLUDED_)
