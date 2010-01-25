/*H***************************************************************************
 File            : IVDeviceSDK.cpp
 Subsystem       : 
 Function Name(s): CIVDeviceSDK
 Author          : YiQiu
 Date            : 2010-1-25  
 Time            : 11:21
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "stdafx.h"
#include "DeviceSDK.h"
#include "DSP.h"


//
// IIVDeviceBase
//

BOOL CDeviceSDK::IsUse( int nChannelID )
{
    return TRUE;
}

BOOL CDeviceSDK::Use( int nChannelID, bool bState )
{
    return TRUE;
}

BOOL CDeviceSDK::ShowObjTrace( bool bState )
{
    return TRUE;
}

BOOL CDeviceSDK::GetObjTraceState( bool& bState )
{
    return TRUE;
}

BOOL CDeviceSDK::IsHaveFreeDevice( void )
{
    return TRUE;
}

//
// IIVStatistic
//

BOOL CDeviceSDK::IsHaveStatisticRule( int nChannelID )
{
    return TRUE;
}

BOOL CDeviceSDK::ResetStatistic( int nChannelID )
{
    return TRUE;
}

BOOL CDeviceSDK::StartStatistic(
    int nChannelID,
    bool bFlag )
{
    return TRUE;
}

BOOL CDeviceSDK::GetStatisticState(
    int nChannelID,
    bool& bFlag )
{
    return TRUE;
}

//
// IIVDeviceSetter
//
void CDeviceSDK::SetIVAlarmOutCallBack(
    AlarmCallBackFn pAlarmCallBackFn,
    void* pParm )
{
    ERROR_RETURN_VOID(!m_bInitialized);
    m_pDSP->SetIVAlarmOutCallBack(pAlarmCallBackFn, pParm);
}

void CDeviceSDK::SetIVDataCallBack(
    IIVDataSender* pIVDataSender )
{
    ERROR_RETURN_VOID(!m_bInitialized);
    m_pDSP->SetIVDataCallBack(pIVDataSender);
}


extern CDeviceSDK g_Dev_Object;

namespace DeviceFactory
{

    IIVDeviceBase* GetIVDeviceBase( void )
    {
        return &g_Dev_Object;
    }

    IIVDeviceSetter* GeIVDeviceSetter( void )
    {
        return &g_Dev_Object;
    }

    IIVStatistic* GetIVStatistic( void )
    {
        return &g_Dev_Object;
    }

}


// End of file