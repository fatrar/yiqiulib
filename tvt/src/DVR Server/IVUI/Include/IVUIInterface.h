/*H***************************************************************************
 File            : IVUIInterface.h
 Subsystem       : 
 Function Name(s): 
 Author          : YiQiu
 Date            : 2010-2-3  
 Time            : 16:59
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IVUIINTERFACE_H_2010_2
#define _IVUIINTERFACE_H_2010_2

#include "..\..\..\Base\Base\Include\Common.h"
#include "..\..\devicecontrol\IIVDevice.h"

#ifdef IVUI_EXPORTS
    #define IVUI_CLASS CLASS_EXPORT
    #define IVUI_API API_EXPORT
#else    
    #define IVUI_CLASS CLASS_IMPORT
    #define IVUI_API API_IMPORT
    #ifndef IVUI_LINK
        #define IVUI_LINK
        #pragma comment(lib, "IVUI.lib")
        #pragma message("Automatically linking with IVUI.dll") 
    #endif
#endif


//#include <afxwin.h>

class CDialog;


IVUI_API CDialog* CreateIVConfigDlg(CWnd* pWnd, const CRect& rect);
IVUI_API void ReleaseIVConfigDlg();
IVUI_API void SetIVOpeator(IIVDeviceBase2* p);




#endif  // _IVUIINTERFACE_H_2010_


 // End of file


