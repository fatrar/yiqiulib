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
#pragma comment(lib, "winmm.lib") 

#include "winioctl.h"
//#include "ioctl.h"
#pragma warning(disable: 4996)
#include <list>
#include <deque>
#include <map>
using namespace std;


#include "..\..\Base\Base\Include\Common.h"
#include "..\..\Base\System\Include\TimeUtil.h"
using namespace OCI;

#include "..\DEVICEControl\IIVDevice.h"
#include "common.h"
#include "Define.h"

#undef TRACE
#define TRACE XTRACE
#include "XTrace.h"

#include "..\..\DebugUtil\DebugUtil\StopWatch.h"
union BufPara
{
    BufPara(int nBufIndex,int nDeviceID,int nChannelID, TVT_ENUM_VIDEO_STREAM_TYPE nBufType)
    {
        Para.nBufIndex = nBufIndex;
        Para.nDeviceID = nDeviceID;
        Para.nChannelID = nChannelID;
        Para.nBufType = nBufType;
    }
    BufPara(DWORD a){dwPara = a;}
    BufPara& operator = (DWORD a){dwPara=a;return *this;}
    operator DWORD(){return dwPara;}
   
    DWORD dwPara;
    struct PARA
    {
        int nBufIndex:8;
        int nDeviceID:8;
        int nChannelID:8;
        TVT_ENUM_VIDEO_STREAM_TYPE nBufType:8;
    } Para; 
};


#include <math.h>


//zhangzhen 2007/02/09
//#define	OUT_FILE_VERSION	"081023"
#define	OUT_FILE_VERSION	"100113"					//OUT文件版本号YYMMDD
#define	OUT_FILE_NAME_NTSC	(".\\"OUT_FILE_VERSION"N")	//OUT文件(NTSC制式)相对路径，当前目录下
#define OUT_FILE_NAME_PAL	(".\\"OUT_FILE_VERSION"P")	//OUT文件(PAL制式)相对路径，当前目录下

static char* s_pOutFileName[] = {OUT_FILE_NAME_PAL, OUT_FILE_NAME_NTSC};
static char* s_pStandardName[] = {"PAL", "NTSC"};

#define safeCloseHandle(h) if((h)){CloseHandle((h)); (h)=NULL;}


//#define TEST_VERSION
#ifdef TEST_VERSION
    extern void WriteTestLog(LPCSTR pstrFormat...);
    extern void PrintFrameRate(int chl, int streamtype);
#else
	#if defined(_MSC_VER) && (_MSC_VER <= 1200 )  // vc6 and old version use
		#define WriteTestLog   // (exp)	{;}
		#define PrintFrameRate // (exp) 	{;}	
	#else
		#define WriteTestLog __noop  // (exp)	{;}
		#define PrintFrameRate __noop // (exp) 	{;}
	#endif
#endif

class CAutoCriticalSection
{
public:
    inline CAutoCriticalSection(CCriticalSection& sec):m_cs(sec)
    {
        //m_cs = sec;
        m_cs.Lock();
    }
    inline ~CAutoCriticalSection()
    {
        m_cs.Unlock();
    }
private:
    CCriticalSection& m_cs;
};

#define Conn(x,y) Conn1(x,y)
#define Conn1(x,y) x##y

#if defined(_MSC_VER) && (_MSC_VER <= 1200 )  // vc6 and old version use
	#define AUTOLOCKNAME  Conn(cs,10000)
#else
	#define AUTOLOCKNAME  Conn(cs,__LINE__) // vc6 __LINE__是一个变量 
#endif

// 后面加个数字是为了防止重命名
#define AutoLockAndUnlock(sec)  CAutoCriticalSection AUTOLOCKNAME(sec)


#define safeCloseHandle(h) if((h)){CloseHandle((h)); (h)=NULL;}

//#define safeDelete(ptr)		 if((ptr)) {delete (ptr); (ptr) = 0;}
//#define safeDeleteArray(ptr) if((ptr)) {delete[] (ptr); (ptr) = 0;}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

// 定义编译宏，live数据是使用Copy一份还是引用数据
//#define PRECOPY
//#define Use_Single_Buffer
//#define _UseLiveTime


#define ERROR_RETURN_VOID(x) if(x) {SetLastError(DVRERR_DRIVERMUSTINIT); return;}
#define ERROR_RETURN(x,y) if(x) {SetLastError(DVRERR_DRIVERMUSTINIT); return (y);}

#define ERROR_RETURN_FALSE(x) ERROR_RETURN(x, FALSE)

#include "WPG_Status.h"


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




#endif // !defined(AFX_STDAFX_H__941AC137_8B58_4BE4_A1A1_6650586F3514__INCLUDED_)
