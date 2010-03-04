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
    // define move to  IIVDevice.h
 
    // Filter
    Filter_Name_Start = IDS_Filter_MinObj,
    Filter_DIR_Start = IDS_Filter_MinObj_Info,
    Filter_Number = 4,

    ApplyBT_Width = 40,
    ApplyBT_Height = 25,
    ApplyBT_X_ROffset = ApplyBT_Width + 20,
    ApplyBT_Y_ROffset = ApplyBT_Height + 20,
};

// ����ȫ��ģ�������ľ����App���죬��ʼ����ÿ��ȥ��Դʹ����
// ����1�������Ч�ʣ�2���ٴ����л���Դ�����⣩��
extern HMODULE g_hmodule;

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


//struct ILineAdvSet
//{
//    virtual void SetRuleName(const CString& strRuleName)=0;
//    virtual void SetTargetObj(DWORD nTargetObj)=0;
//};
//
//struct IZoneAdvSet : public ILineAdvSet
//{
//    virtual void SetViewType(bool bView)=0; // true is Parallel, otherwise,Vertical
//    virtual void SetTime(DWORD nTime){};
//};






#endif  // End of file



