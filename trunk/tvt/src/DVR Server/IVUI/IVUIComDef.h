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
    IV_Name_Number = 10,

    // ZoneAdvDlg Define
    Max_Rule_Name = 20,
    Loiters_Max_Time = 600,     // 10min
    LeftBehind_Max_Time = 600,  // 10min

    Loiters_Default_Time = 30,     // 10min
    LeftBehind_Default_Time = 30,  // 10min
};

//// �������ܵİ�������
//enum IVRuleType
//{
//    IV_Invade,              // ����
//    IV_Leave_Disappear,     // ��ʧ/�뿪
//    IV_LeftBehind,          // ����
//    IV_Loiters,             // �ǻ�
//    IV_Statistic,           // ����ͳ��
//    IV_Vehicle_Retrograde,  // ��������
//    IV_Illegal_Parking,     // �Ƿ�ͣ��
//    IV_Stage_Change,        // �����任
//};

// Person 1, vehicle=2, unknown=4,anything=7  WPG_TARGET_CLASSIFICATION
// GROUND_PLANE=Parallel=���泡��, IMAGE_PLANE=��ֱ����=Vertical WPG_PLANE_TYPE


struct IZoneInvadeAdvSet
{
    virtual void OnZoneAdvSet(
        const CString& strRuleName,
        bool bView,    // true is Parallel, otherwise�� Vertical
        DWORD nTargetObj) = 0;
};

typedef IZoneInvadeAdvSet IZoneLeaveAdvSet;

struct IZoneLeftBehindAdvSet
{
    virtual void OnZoneAdvSet(
        const CString& strRuleName,
        bool bView,    // true is Parallel, otherwise�� Vertical
        DWORD nTargetObj,
        DWORD nTime ) = 0;
};

typedef IZoneLeftBehindAdvSet IZoneLoitersAdvSet;

struct IZoneStageChangeAdvSet
{
    virtual void OnZoneAdvSet(
        const CString& strRuleName,
        bool bView   // true is Parallel, otherwise�� Vertical
        ) = 0;
};




#endif  // End of file



