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

#include <Common.h>
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



/**
*@note IV配置窗口命名空间
*/
namespace IVUIFactory
{
    /**
    *@note  得到IV配置窗口
    *@param	 hWnd 父窗口 
    *@param  IV配置窗口显示的区域
    *@return IV配置窗口句柄
    */
    IVUI_API HWND CreateIVConfigDlg(HWND hWnd, const RECT& rect);
    
    /**
    *@note 销毁配置窗口
    */
    IVUI_API void ReleaseIVConfigDlg();

    /**
    *@note 设置IV SDK设置的指针
    */
    IVUI_API void SetIVOpeator(IIVDeviceBase2* p);
};




#endif  // _IVUIINTERFACE_H_2010_


 // End of file


