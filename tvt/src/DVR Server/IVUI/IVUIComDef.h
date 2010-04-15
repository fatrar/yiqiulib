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

    ApplyBT_Width = 60,
    ApplyBT_Height = 35,
    ApplyBT_X_ROffset = ApplyBT_Width + 20,
    ApplyBT_Y_ROffset = ApplyBT_Height + 20,


    Default_Max_Channel    = 4,
    Default_Device_Num     = 1,
    Default_IVChannelNum_By_Device   = 1,
    Default_Max_RuleNum_By_IVChannel = 5,
};

// ����ȫ��ģ�������ľ����App���죬��ʼ����ÿ��ȥ��Դʹ����
// ����1�������Ч�ʣ�2���ٴ����л���Դ�����⣩��
extern HMODULE g_hmodule;







#endif  // End of file



