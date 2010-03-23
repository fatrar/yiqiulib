// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__74D34C38_DEE2_49E5_A902_E8723FE38CA8__INCLUDED_)
#define AFX_STDAFX_H__74D34C38_DEE2_49E5_A902_E8723FE38CA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

//#include <afxwin.h>         // MFC core and standard components
//#include <afxext.h>         // MFC extensions
//
//#ifndef _AFX_NO_OLE_SUPPORT
//#include <afxole.h>         // MFC OLE classes
//#include <afxodlgs.h>       // MFC OLE dialog classes
//#include <afxdisp.h>        // MFC Automation classes
//#endif // _AFX_NO_OLE_SUPPORT


//#ifndef _AFX_NO_DB_SUPPORT
//#include <afxdb.h>			// MFC ODBC database classes
//#endif // _AFX_NO_DB_SUPPORT
//
//#ifndef _AFX_NO_DAO_SUPPORT
//#include <afxdao.h>			// MFC DAO database classes
//#endif // _AFX_NO_DAO_SUPPORT

//#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>			// MFC support for Windows Common Controls
//#endif // _AFX_NO_AFXCMN_SUPPORT

#include <windows.h>
#include <process.h>

#include <deque>
#include <map>
//#include <set>
#include <string>
#include <list>
#include <fstream>
using namespace std;


#include "..\..\Base\Base\Include\Singleton.h"
#include "..\..\Base\System\Include\Thread.h"
#include "..\..\Base\Base\Include\StlHelper.h"
#include "..\..\Base\System\Include\TimeUtil.h"
using namespace OCI;

#include "..\DEVICEControl\WPG_EventOccurrence.h"
#include "..\DEVICEControl\IIVDataSender.h"

#include "..\..\DebugUtil\DebugUtil\MemCheck.h"
#include "..\..\DebugUtil\DebugUtil\Trace.h"
#include "IVViewerUtil.h"

#include "Include\IVViewer.h"
#include "IVFileCommon.h"


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__74D34C38_DEE2_49E5_A902_E8723FE38CA8__INCLUDED_)
