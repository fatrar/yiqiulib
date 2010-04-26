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

#include <Common.h>
#include "..\..\DEVICEControl\IIVDevice.h"


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


/**
*@note IV截图对口命名空间
*/
namespace SnapShotWnd
{
/**
*@note 窗口的位置
*/
enum SnapShotPos
{
    SnapShot_Top,
    SnapShot_Bottom,
    SnapShot_Use_Spectify,
};

/**
*@note 得到IV截图窗口
*/
SNAPSHOTUI_API ISnapShotSender* CreateSnapShotWnd(
    HWND hWnd,
    int nWidth = 0,
    SnapShotPos Pos = SnapShot_Bottom,
    int nTop = 0 );

/**
*@note 销毁IV截图窗口
*/
SNAPSHOTUI_API BOOL DestroySnapShotWnd();

/**
*@note 显示还是隐藏窗口
*@nCmdShow 等同于ShowWinodw的参数，ex SW_SHOW
*/
SNAPSHOTUI_API BOOL ShowSnapShotWnd(int nCmdShow);

};









#endif  // _ISNAPSHOT_H_2010_


 // End of file



