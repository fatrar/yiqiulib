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
*@note IV��ͼ�Կ������ռ�
*/
namespace SnapShotWnd
{
/**
*@note ���ڵ�λ��
*/
enum SnapShotPos
{
    SnapShot_Top,
    SnapShot_Bottom,
    SnapShot_Use_Spectify,
};

/**
*@note �õ�IV��ͼ����
*/
SNAPSHOTUI_API ISnapShotSender* CreateSnapShotWnd(
    HWND hWnd,
    int nWidth = 0,
    SnapShotPos Pos = SnapShot_Bottom,
    int nTop = 0 );

/**
*@note ����IV��ͼ����
*/
SNAPSHOTUI_API BOOL DestroySnapShotWnd();

/**
*@note ��ʾ�������ش���
*@nCmdShow ��ͬ��ShowWinodw�Ĳ�����ex SW_SHOW
*/
SNAPSHOTUI_API BOOL ShowSnapShotWnd(int nCmdShow);

};









#endif  // _ISNAPSHOT_H_2010_


 // End of file



