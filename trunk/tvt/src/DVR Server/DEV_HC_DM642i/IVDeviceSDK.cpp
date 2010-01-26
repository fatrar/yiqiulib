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
#include "DSP.h"

//
//         Do IV Data Function
//
void CDSP::DoIVData(int nDevice, PBYTE pData)
{
    BYTE* pIVData = pData + EACH_TRAN_BUFF_SIZE;
    PTVT_AI_VBI pIVVBI = (PTVT_AI_VBI)pIVData;
    unsigned int& dwNumberOfTargets = pIVVBI->dwNumberOfTargets;
    unsigned int& dwNumberOfEvents = pIVVBI->dwNumberOfEvents;
    
    LONGLONG nTime = ChangeTime(pIVVBI->frameRealTime);
    FILETIME* pTime = (FILETIME*)&nTime;
    
    // 1. Do Obj&Trace
    if ( m_pIVDataSender )
    {
        m_pIVDataSender->OnIVDataSend(
            m_szCurrentIVChannel[nDevice],
            *pTime,
            (WPG_Target*)(pIVData+sizeof(TVT_AI_VBI)),
            dwNumberOfTargets );
    }

    // 2. Do IV Alarm & SnapShot
    WPG_EventOccurrence* pFirstEvent = 
        (WPG_EventOccurrence*)(pIVData+sizeof(TVT_AI_VBI)+dwNumberOfTargets*sizeof(WPG_Target));
    BYTE* pFirstPic = (BYTE*)pFirstEvent + dwNumberOfEvents*sizeof(WPG_EventOccurrence);
    for ( int i=0 ;i<dwNumberOfEvents;
          ++i, ++pFirstEvent )
    {
        int& nChannelID = m_szCurrentIVChannel[nDevice];
        DoIVAlarm(
            nChannelID,
            pFirstEvent,
            pTime );
        DoSnapShot(
            nChannelID,
            pFirstEvent,
            pFirstPic );
    }
}

void CDSP::DoIVAlarm(
    int nChannelID,
    const WPG_EventOccurrence* pEvent, 
    FILETIME* pTime )
{
    IV_RuleID* RuleID = (IV_RuleID*)(pEvent->ruleId);
    if ( m_AlarmCallBackFn )
    {
        // AlarmOutTable []
       /* m_AlarmCallBackFn(
            ,
            (IVRuleType)RuleID->RuleID.nType,
            nChannelID,
            pTime,
            m_pAlarmCallBackParm );*/
    }
    //WPG_EventDescriptionUnion& des = eventDescription.description;
    //switch (eventDescription.type)
    //{
    //case TRIPWIRE_EVENT:     // 絆线
    //    WPG_TripwireEventDescription& tripwireDes = des.tripwireEventDescription;
    //    break;
    //case AOI_EVENT:          // AOI
    //    WPG_AOIEventDescription& AOIDes = des.aoiEventDescription;
    //    break;
    //case SCENE_CHANGE_EVENT: // 场景变化
    //    WPG_SceneChangeEventDescription& SceneChangeDes = des.sceneChangeEventDescription;
    //    break;
    //
    //// 下面两个还不知道具体干啥的，先写出来
    //case OCCUPANCY_EVENT:
    //    WPG_OccupancyEventDescription& OccDes = des.occupancyEventDescription;
    //    break;
    //case DWELLTIME_EVENT: 
    //    WPG_DwellTimeEventDescription& DwellDes = des.dwellTimeEventDescription;
    //    break;
    //default:
    //}
}

void CDSP::DoSnapShot(
    int nChannelID,
    const WPG_EventOccurrence* pEvent,
    BYTE*& pFirstPic)
{
    if ( !m_ShowSnapShot || !m_pSnapShotSender )
    {
        return;
    }

    for (int i = 0; i<pEvent->numOfSlices; ++i)
    {
        const WPG_EventSlice& slices = pEvent->slices[i];
        m_pSnapShotSender->OnSnapShotSend(
            nChannelID,
            (DWORD)pEvent->ruleId,
            pFirstPic,
            slices.snapshotLength );
        pFirstPic += slices.snapshotLength;
    }
}

//
// IIVDeviceBase
//

BOOL CDSP::IsUse( int nChannelID )
{
    int nDeviceID;
    return GetDeviceIDByChannel(nChannelID, nDeviceID);
}

BOOL CDSP::Use( int nChannelID, bool bState )
{
    return TRUE;
}

BOOL CDSP::ShowObjTrace( bool bState )
{
    return TRUE;
}

BOOL CDSP::GetObjTraceState( bool& bState )
{
    return TRUE;
}

BOOL CDSP::IsHaveFreeDevice( void )
{
    return TRUE;
}

//
// IIVStatistic
//

BOOL CDSP::IsHaveStatisticRule( int nChannelID )
{
    return TRUE;
}

BOOL CDSP::ResetStatistic( int nChannelID )
{
    return TRUE;
}

BOOL CDSP::StartStatistic(
    int nChannelID,
    bool bFlag )
{
    return TRUE;
}

BOOL CDSP::GetStatisticState(
    int nChannelID,
    bool& bFlag )
{
    return TRUE;
}

//
// IIVDeviceSetter
//
void CDSP::SetIVAlarmOutCallBack(
    AlarmCallBackFn pAlarmCallBackFn,
    void* pParm )
{
    m_AlarmCallBackFn = pAlarmCallBackFn;
    m_pAlarmCallBackParm = pParm;
}

void CDSP::SetIVDataCallBack(
    IIVDataSender* pIVDataSender )
{
     m_pIVDataSender = pIVDataSender;
}



bool CDSP::GetDeviceIDByChannel(int nChannelID, int& nDeviceID)
{
    nDeviceID = (nChannelID-1)/CHANNEL_PER_DEVICE;
    return m_szCurrentIVChannel[nDeviceID]==nChannelID?true:false;
}

// End of file