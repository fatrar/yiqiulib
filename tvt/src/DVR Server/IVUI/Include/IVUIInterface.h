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
    *@note 将IV设置从XML读出，并将默认运行智能的通道设置数据
    *@param	p IV SDK设置的指针  
    *@param bTelphone 是否是打电话功能 
    *@param dwRelayCount Relay的个数 
    */
    IVUI_API void InitIVConfig(
        IIVDeviceBase2* p,
        BOOL bTelphone,
        DWORD dwRelayCount);

    /**
    *@note 回收资源
    */
    IVUI_API void UnitIVConfig();

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
    *@note 对某个通道使用智能
    */
    IVUI_API BOOL UseIV(int nChannelID, bool bEnable);

    /**
    *@note 设置通道的名称
    */
    IVUI_API void SetChannelName(
        int nChannelID, 
        const char* pChannelName);
};




#endif  // _IVUIINTERFACE_H_2010_


 // End of file


