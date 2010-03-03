/*H***************************************************************************
 File            : IVUIComDef.h
 Subsystem       : 
 Function Name(s): 
 Author          : YiQiu
 Date            : 2010-1-27  
 Time            : 16:13
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IVUICOMDEF_H_2010_
#define _IVUICOMDEF_H_2010_
#include "resource.h"
#include "..\DEVICEControl\IIVDevice.h"


enum
{
    // IVFunctionSelDlg Define
    IV_Name_StringID_Start = IDS_Invade,
    IV_Name_DIR_StringID_Start = IDS_IV_Name_Des,
    IV_Name_Number = 8,

    // ZoneAdvDlg Define
    Max_Rule_Name = 20,
    Loiters_Max_Time = 600,     // 10min
    LeftBehind_Max_Time = 600,  // 10min

    Loiters_Default_Time = 30,     // 10min
    LeftBehind_Default_Time = 30,  // 10min

    // Filter
    Filter_Name_Start = IDS_Filter_MinObj,
    Filter_DIR_Start = IDS_Filter_MinObj_Info,
    Filter_Number = 4,


};

// 采用全局模块句柄，改句柄在App构造，初始化，每个去资源使用它
// 这样1可以提高效率，2减少错误（却换资源的问题）。
extern HMODULE g_hmodule;

//// 现有智能的八种类型
//enum IVRuleType
//{
//    IV_Invade,              // 入侵
//    IV_Leave_Disappear,     // 消失/离开
//    IV_LeftBehind,          // 遗留
//    IV_Loiters,             // 徘徊
//    IV_Statistic,           // 流量统计
//    IV_Vehicle_Retrograde,  // 车辆逆行
//    IV_Illegal_Parking,     // 非法停车
//    IV_Stage_Change,        // 场景变换
//};

// Person 1, vehicle=2, unknown=4,anything=7  WPG_TARGET_CLASSIFICATION
// GROUND_PLANE=Parallel=地面场景, IMAGE_PLANE=垂直场景=Vertical WPG_PLANE_TYPE


struct ILineAdvSet
{
    virtual void SetRuleName(const CString& strRuleName)=0;
    virtual void SetTargetObj(DWORD nTargetObj)=0;
};

struct IZoneAdvSet : public ILineAdvSet
{
    virtual void SetViewType(bool bView)=0; // true is Parallel, otherwise,Vertical
    virtual void SetTime(DWORD nTime){};
};






#endif  // End of file



