/**H**************************************************************************
 File            : ISnapShot.h
 Subsystem       : 
 Function Name(s): CISnapShot
 Author          : YiQiu
 Date            : 2010-3-15  
 Time            : 18:19
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _ISNAPSHOT_H_2010_3
#define _ISNAPSHOT_H_2010_3



#include "..\..\Base\Base\Include\Common.h"


#ifdef SNAPSHOTUI_EXPORTS
    #define SNAPSHOTUI_CLASS CLASS_EXPORT
    #define SNAPSHOTUI_API API_EXPORT
#else    
    #define SNAPSHOTUI_CLASS CLASS_IMPORT
    #define SNAPSHOTUI_API API_IMPORT
    #ifndef SNAPSHOTUI_LINK
        #define SNAPSHOTUI_LINK
        #pragma comment(lib, "SnapShotUI.lib")
        #pragma message("Automatically linking with SnapShotU.dll") 
    #endif
#endif





SNAPSHOTUI_API HWND CreateSnapShotWnd(HWND hWndParent, int nWidth);

SNAPSHOTUI_API BOOL DestroySnapShotWnd();




#endif  // _ISNAPSHOT_H_2010_


 // End of file



