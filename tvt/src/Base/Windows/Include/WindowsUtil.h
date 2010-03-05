/*H***************************************************************************
 File            : WindowsUtil.h
 Subsystem       : 
 Function Name(s): CWindowsUtil
 Author          : YiQiu
 Date            : 2010-2-22  
 Time            : 12:16
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _WINDOWSUTIL_H_2010_2
#define _WINDOWSUTIL_H_2010_2

#include "..\..\Base\Include\Common.h"

BEGIN_BASE_ENGINE

namespace Windows
{


inline BOOL LockCursor(const CRect& LockRect){ return ClipCursor(&LockRect); }

#ifdef Support_Dyn_Resolution
#   define OCI_Static
#else
#   define OCI_Static static
#endif

static BOOL UnLockCursor()
{
    OCI_Static int x = GetSystemMetrics(SM_CXSCREEN);
    OCI_Static int y = GetSystemMetrics(SM_CYSCREEN); 
    return ClipCursor(&CRect(0,0,x,y));
}

#define ParentInvalidate() \
    CRect FreshRect;                \
    GetClientRect(&FreshRect);      \
    ClientToScreen(&FreshRect);     \
    CWnd* pParentWnd = GetParent(); \
    pParentWnd->ScreenToClient(&FreshRect); \
    pParentWnd->InvalidateRect(&FreshRect);

#define ParentInvalidateEx() \
    CRect FreshRect;                \
    m_pWnd->GetClientRect(&FreshRect);      \
    m_pWnd->ClientToScreen(&FreshRect);     \
    CWnd* pParentWnd = m_pWnd->GetParent(); \
    pParentWnd->ScreenToClient(&FreshRect); \
    pParentWnd->InvalidateRect(&FreshRect);


//#define ParentInvalidate()  Invalidate()

};  // Windows


END_BASE_ENGINE

#endif  // _WINDOWSUTIL_H_2010_


 // End of file



