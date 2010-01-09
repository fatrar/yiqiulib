// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__941AC137_8B58_4BE4_A1A1_6650586F3514__INCLUDED_)
#define AFX_STDAFX_H__941AC137_8B58_4BE4_A1A1_6650586F3514__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxmt.h>

#include <vfw.h>
#include "..\DEVICEControl\DeviceControl.h"

#include "winioctl.h"
//#include "ioctl.h"

#include <deque>
using namespace std;

#include "common.h"
#include "Define.h"

#include "math.h"

//zhangzhen 2007/02/09
#define	OUT_FILE_VERSION	"081023"					//OUT文件版本号YYMMDD
#define	OUT_FILE_NAME_NTSC	(".\\"OUT_FILE_VERSION"N")	//OUT文件(NTSC制式)相对路径，当前目录下
#define OUT_FILE_NAME_PAL	(".\\"OUT_FILE_VERSION"P")	//OUT文件(PAL制式)相对路径，当前目录下

static char* s_pOutFileName[] = {OUT_FILE_NAME_PAL, OUT_FILE_NAME_NTSC};
static char* s_pStandardName[] = {"PAL", "NTSC"};

//#define TEST_VERSION
#ifdef TEST_VERSION
    extern void WriteTestLog(LPCSTR pstrFormat...);
    extern void PrintFrameRate(int chl, int streamtype);
#else
    #define WriteTestLog __noop  // (exp)	{;}
    #define PrintFrameRate __noop // (exp) 	{;}
#endif

class CAutoCriticalSection
{
public:
    CAutoCriticalSection(CCriticalSection& sec):m_cs(sec)
    {
        //m_cs = sec;
        m_cs.Lock();
    }
    ~CAutoCriticalSection(){m_cs.Unlock();}
private:
    CCriticalSection& m_cs;
};

#define Conn(x,y) Conn1(x,y)
#define Conn1(x,y) x##y

#define AUTOLOCKNAME  Conn(cs,__LINE__)
#define AutoLockAndUnlock(sec)  CAutoCriticalSection AUTOLOCKNAME(sec)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__941AC137_8B58_4BE4_A1A1_6650586F3514__INCLUDED_)
