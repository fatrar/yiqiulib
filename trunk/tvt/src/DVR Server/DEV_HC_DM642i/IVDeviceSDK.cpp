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

bool IsInScheduleSettings(
    const FILETIME& test,
    const ScheduleSettings& Sch )
{
    SYSTEMTIME syt;
    FileTimeToSystemTime(&test, &syt);
    const Scheduleday& DaySet = Sch.s[syt.wDayOfWeek];
    int nTestMin = syt.wHour*60 + syt.wMinute;
    // 先简单做个，这个可以用二分法去加速
    for (int i=0; i< DaySet.useNo; ++i)
    {
        if ( nTestMin >= DaySet.starttime[i] &&
             nTestMin <= DaySet.endtime[i] )
        {
            return true;
        }
        else if (nTestMin > DaySet.endtime[i] )
        {
            return false;
        }
    }

    return false;
}

BOOL CDSP::SetIVSpecialParam( 
    int nType, int nChannel, int nValue,
    const WPG_Rule& Rule )
{
    int nDevice = nChannel/4;
    if (nDevice >= m_nDeviceNum)
        return FALSE;

    IVParmData IVData;
    IVData.Rule = Rule;
    PARAMPACK& pack = IVData.Commadparm;
    pack.chanNum = nChannel%4;
    pack.paramType = nType;
    pack.value = nValue;

    {
        AutoLockAndUnlock(m_pIVPack[nDevice].CS);
        m_pIVPack[nDevice].param.push_back(IVData);
    }
   
    return TRUE;
}

BOOL CDSP::SetIVParamToDSP( int nDevice )
{
    size_t nQueueSize = m_pIVPack[nDevice].param.size();
    if ( nQueueSize == 0 )
    {
        return TRUE;
    }

    DWORD dwRtn;
    TVT_AP_SET stApSet; //heliang fix

    //判断dsp程序有没有提取上次设定的参数。
    stApSet.dwAddress = AI_STREAM_FLAG;
    ControlDriver(nDevice,
        IOCTL_GET_DSP_PARAM,
        &stApSet, sizeof(stApSet),
        &stApSet, sizeof(stApSet),
        &dwRtn);
    if(stApSet.dwValue != 0xa5a5a5a5)	//如果没有则取消当次设定。
    {
        return TRUE;
    }

    BYTE* pBuf = m_pIVParmBuf[nDevice];
    PDWORD pNum = (PDWORD)pBuf;
    *pNum = 1;
    {
        AutoLockAndUnlock(m_pIVPack[nDevice].CS);
        
        PPARAMPACK pPack = (PPARAMPACK)(pBuf + sizeof(DWORD));    
        IVParmData& IVData = m_pIVPack[nDevice].param.front();

        TRACE(
            "ChannelID=%d, Command=%d\n", 
            IVData.Commadparm.chanNum,
            IVData.Commadparm.paramType);

        *pPack = IVData.Commadparm;
        memcpy(pPack+1, &IVData.Rule, sizeof(WPG_Rule));
        m_pIVPack[nDevice].param.pop_front();
    }

    return ControlDriver(nDevice,
        IOCTL_WRITE_AI_STREAM,
        pBuf, MAX_IV_Parm_Buf_Size,
        NULL, 0,
        &dwRtn );
}

//
// ****************** Do IV Data Function ******************
//
void CDSP::DoIVData(int nDevice, PBYTE pData)
{
    BYTE* pIVData = pData + EACH_TRAN_BUFF_SIZE;
    PTVT_AI_VBI pIVVBI = (PTVT_AI_VBI)pIVData;
    unsigned int& dwNumberOfTargets = pIVVBI->dwNumberOfTargets;
    unsigned int& dwNumberOfEvents = pIVVBI->dwNumberOfEvents;
    
    LONGLONG nTime = ChangeTime(m_prevVideoTime);
    //LONGLONG nTime = ChangeTime(pIVVBI->frameRealTime);
    FILETIME* pTime = (FILETIME*)&nTime;
    
    int& nChannelID = m_szCurrentIVChannel[nDevice];
    // 1. Do Obj&Trace
    if ( m_pIVDataSender )
    {
        m_pIVDataSender->OnIVDataSend(
            nChannelID,
            *pTime,
            (WPG_Target*)(pIVData+sizeof(TVT_AI_VBI)),
            dwNumberOfTargets );
    }

    // 2. Do IV Alarm & SnapShot
    WPG_EventOccurrence* pFirstEvent = 
        (WPG_EventOccurrence*)(pIVData+sizeof(TVT_AI_VBI)+dwNumberOfTargets*sizeof(WPG_Target));
    BYTE* pFirstPic = (BYTE*)pFirstEvent + dwNumberOfEvents*sizeof(WPG_EventOccurrence);
    // 如果是模拟通道
    if (m_pIVAlarmCallBack && nChannelID==m_SimulationChanID)
    {
        for ( int i=0 ;i<dwNumberOfEvents;
              ++i, ++pFirstEvent )
        {
            const AlarmOutTable* pTable = NULL;
            IV_RuleID* RuleID = (IV_RuleID*)pFirstEvent->ruleId;
            m_pIVAlarmCallBack->OnAlarmCallBack(
                (IVRuleType)RuleID->RuleID.nType,
                nChannelID, pTime );
        }

        return;
    }
   
    // 如果没有不是模拟通道，就按正常逻辑
    for ( int i=0 ;i<dwNumberOfEvents;
          ++i, ++pFirstEvent )
    {
        const AlarmOutTable* pTable = NULL;
        IV_RuleID* RuleID = (IV_RuleID*)pFirstEvent->ruleId;
        if ( !IsNeedAlarmOut(nDevice, *RuleID, *pTime, pTable) )
        {
            continue;
        }

        DoIVAlarm(
            nChannelID,
            pFirstEvent,
            pTime,
            pTable );

        if ( pTable && pTable->Table.SnapShot )
        {
            DoSnapShot(
                nChannelID,
                pFirstEvent,
                pFirstPic );
        }
    }
}

bool CDSP::IsNeedAlarmOut(
    int nDevice,
    const IV_RuleID& RuleID,
    const FILETIME& test,
    const AlarmOutTable*& pTable )
{
    pTable = NULL;
    RuleSettingMap& RuleSettings = m_RuleCfgMap[nDevice];

    AutoLockAndUnlock(m_CfgMapCS[nDevice]);
    RuleSettingMap::iterator iter = RuleSettings.find(RuleID);
    if ( iter == RuleSettings.end() )
    {
        // log
        return false;
    }

    __int64* pTestTime = (__int64*)&test;
    CurrentRuleSetting* pCurrentSet = iter->second;
    if ( *pTestTime - pCurrentSet->nLastHoldTime < pCurrentSet->Alarm.nHoldTime*10 )
    {
        return false;
    }

    const ScheduleSettings& Sch = pCurrentSet->Sch;
    if ( IsInScheduleSettings(test, Sch) )
    {
        pTable = &(pCurrentSet->Alarm.table);
        return true;
    }

    return false;
}

void CDSP::DoIVAlarm(
    int nChannelID,
    const WPG_EventOccurrence* pEvent, 
    FILETIME* pTime,
    const AlarmOutTable* pTable )
{
    IV_RuleID* RuleID = (IV_RuleID*)(pEvent->ruleId);
    if ( m_AlarmCallBackFn && pTable )
    {
        m_AlarmCallBackFn(
            *pTable,
            (IVRuleType)RuleID->RuleID.nType,
            nChannelID,
            pTime,
            m_pAlarmCallBackParm );
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

static bool s_bSavePic = true;
#include <fstream>

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

        if( s_bSavePic )
        {
            static int j = 0;
            static char szBuf[MAX_PATH] = {0};
            sprintf_s(szBuf, "C:\\%d.jpg", j++);
            ofstream Write(szBuf, ios::binary);
            Write.write((char*)pFirstPic, slices.snapshotLength-16);
            Write.close();
        }

        BYTE* pPic = pFirstPic;
        SYSTIME* pSysTime = (SYSTIME*)(pFirstPic+slices.snapshotLength-16);
        LONGLONG nTime = ChangeTime(*pSysTime);
        FILETIME* pTime = (FILETIME*)&nTime;
        m_pSnapShotSender->OnSnapShotSend(
            nChannelID,
            (DWORD)pEvent->ruleId,
            pTime,
            pPic,
            slices.snapshotLength-16 );
        pFirstPic = (BYTE*)pSysTime+16;

        
    }
}

// {
// ******************** IIVDeviceBase *********************
//

BOOL CDSP::IsUse( int nChannelID )
{
    int nDeviceID = nChannelID/CHANNEL_PER_DEVICE;
    if ( nDeviceID > m_nDeviceNum )
    {
        // log
        return FALSE;
    }   

    return nChannelID==m_szCurrentIVChannel[nDeviceID];
}

BOOL CDSP::Use( int nChannelID, bool bState )
{
    int nDeviceID = nChannelID/CHANNEL_PER_DEVICE;
    if ( nDeviceID > m_nDeviceNum )
    {
        return FALSE;
    }   
    
    // when is running and Set Run --> pass
    // when is stop and Set Stop --> pass
    do 
    {
        // 如果这个卡，有剩余的智能的通道，只能调用开始智能
        if ( m_szCurrentIVChannel[nDeviceID] == Device_Free_Flag )
        {
            if (bState)
            {
                m_szCurrentIVChannel[nDeviceID] = nChannelID;
                break;
            }
            
            return FALSE;
        }

        if (m_szCurrentIVChannel[nDeviceID]==nChannelID)
        {
            // 如果这个卡使用的通道与传入参数一样
            // 命令为启用，就pass，停止就调用板卡
            if (bState){ return TRUE;}
            
            m_szCurrentIVChannel[nDeviceID] = Device_Free_Flag;
            break;
        }
        else
        {
            // 这里的意思为正在有其他通道使用智能，
            // 如果函数调用非对应通道的停止，那么返回TRUE
            // 如果开始智能那必须先停止正在跑的，所以返回FALSE
            return !bState;
        }
    }
    while (0);
   
    return SetParam(
        PT_PCI_SET_AI_ENABLE, nChannelID, int(bState) );
}

BOOL CDSP::IsHaveFreeDevice( void )
{
    for (int i =0; i< m_nDeviceNum; ++i)
    {
        if ( m_szCurrentIVChannel[i] != Device_Free_Flag )
        {
            return TRUE;
        }
    }
    return FALSE;
}

// IIVDeviceBase
// }

// {
// ********************* IIVSimulation *******************
// 

void CDSP::Start(
    int nChannelID,
    IIVSimulationAlarmCallBack* p,
    const WPG_Rule& Rule )
{
    int nDeviceID = nChannelID/CHANNEL_PER_DEVICE;
    if ( nDeviceID > m_nDeviceNum )
    {
        return;
    }

    AutoLockAndUnlock(m_SimulationCS);
    m_pIVAlarmCallBack = p;
    m_SimulationChanID = nChannelID;
    int nUseChannel = m_szCurrentIVChannel[nDeviceID];
    if ( nUseChannel == Device_Free_Flag ) // 还没使用智能
    {
        SetParam(PT_PCI_SET_AI_ENABLE, nChannelID, 1);
        m_RuleID = Rule.ruleId;
        Add(nChannelID, Rule);
    }
    else if ( nUseChannel == nChannelID ) // 刚好正在使用智能
    {
        m_RuleID = Rule.ruleId;
        Add(nChannelID, Rule);
    }
    else //被其他的占用
    {
        SetParam(PT_PCI_SET_AI_ENABLE, nUseChannel, 0);
        SetParam(PT_PCI_SET_AI_ENABLE, nChannelID, 1);
        m_RuleID = Rule.ruleId;
        Add(nChannelID, Rule);
    }
}

void CDSP::Stop( int nChannelID )
{
    ASSERT(nChannelID==m_SimulationChanID);
    int nDeviceID = nChannelID/CHANNEL_PER_DEVICE;
    if ( nDeviceID > m_nDeviceNum )
    {
        return;
    }

    AutoLockAndUnlock(m_SimulationCS);
    m_pIVAlarmCallBack = NULL;
    m_SimulationChanID = Invaild_ChannelID;
    int nUseChannel = m_szCurrentIVChannel[nDeviceID];
    if ( nUseChannel == Device_Free_Flag ) // 还没使用智能
    {
        SetParam(PT_PCI_SET_AI_ENABLE, nChannelID, 0);
    }
    else if ( nUseChannel == nChannelID ) // 刚好正在使用智能
    {
        Remove(nChannelID, m_RuleID);
    }
    else //被其他的占用
    {
        SetParam(PT_PCI_SET_AI_ENABLE, nChannelID, 0);
        SetParam(PT_PCI_SET_AI_ENABLE, nUseChannel, 0);
        RuleSettingMap& RuleSetting = m_RuleCfgMap[nDeviceID];
        RuleSettingMap::iterator iter;
        for ( iter = RuleSetting.begin();
              iter!= RuleSetting.end();
              ++iter )
        {
            CurrentRuleSetting* pTmp = iter->second;
            Add(nUseChannel, pTmp->Rule, pTmp->Sch, pTmp->Alarm);
        }
    }
}

// IIVSimulation
// }


// {
// ********************* IIVDeviceBase2 *******************
//

BOOL CDSP::Add(
    int nChannelID,
    const WPG_Rule& Rule,
    const ScheduleSettings& Sch /*= g_DefaultScheduleSettings*/, 
    const AlarmOutSettings& Alarm /*= g_DefaultAlarmOutSettings*/ )
{
    int nDeviceID = nChannelID/CHANNEL_PER_DEVICE;
    if ( nDeviceID > m_nDeviceNum )
    {
        return FALSE;
    }

    if ( nChannelID!=m_szCurrentIVChannel[nDeviceID] )
    {
        return FALSE;
    }

    RuleSettingMap& RuleSettings = m_RuleCfgMap[nDeviceID]; 
    {
        AutoLockAndUnlock(m_CfgMapCS[nDeviceID]);
        IV_RuleID* pRuleID = (IV_RuleID*)Rule.ruleId;
        RuleSettingMap::iterator iter = RuleSettings.find(*pRuleID);
        if ( iter != RuleSettings.end() )
        { 
            // log...
            CurrentRuleSetting* pCurrentSet = iter->second;
            pCurrentSet->Rule = Rule;
            pCurrentSet->Sch = Sch;
            pCurrentSet->Alarm = Alarm;
        }
        else
        {
            RuleSettings[*pRuleID] = new CurrentRuleSetting(Rule,Sch,Alarm);
        }
        
    }

    return SetIVSpecialParam(
        PT_PCI_SET_ADD_RULE, nChannelID, 0, Rule);
}

BOOL CDSP::Remove(
    int nChannelID,
    const IV_RuleID& RuleID )
{
    int nDeviceID = nChannelID/CHANNEL_PER_DEVICE;
    if ( nDeviceID > m_nDeviceNum )
    {
        return FALSE;
    }

    if ( nChannelID!=m_szCurrentIVChannel[nDeviceID] )
    {
        return FALSE;
    }

    RuleSettingMap& RuleSettings = m_RuleCfgMap[nDeviceID];
    
    {
        AutoLockAndUnlock(m_CfgMapCS[nDeviceID]);
        RuleSettingMap::iterator iter = RuleSettings.find(RuleID);
        if ( iter == RuleSettings.end() )
        {
            return TRUE;
        }

        delete iter->second;
        RuleSettings.erase(iter);
    }

    WPG_Rule Rule;
    memcpy(Rule.ruleId, RuleID.szRuleId, 16);
    return SetIVSpecialParam(
        PT_PCI_SET_DEL_RULE, nChannelID, 0, Rule);
}

BOOL CDSP::EnableRule( 
    int nChannelID,
    const IV_RuleID& RuleID,
    BOOL bEnable /*=TRUE*/ )
{
    int nDeviceID = nChannelID/CHANNEL_PER_DEVICE;
    if ( nDeviceID > m_nDeviceNum )
    {
        return FALSE;
    }

    if ( nChannelID!=m_szCurrentIVChannel[nDeviceID] )
    {
        return FALSE;
    }

    RuleSettingMap& RuleSettings = m_RuleCfgMap[nDeviceID];
    CurrentRuleSetting* pCurrentSet = NULL;
    {
        AutoLockAndUnlock(m_CfgMapCS[nDeviceID]);
        RuleSettingMap::iterator iter = RuleSettings.find(RuleID);
        if ( iter == RuleSettings.end() )
        {
            return FALSE;
        }

        pCurrentSet = iter->second;
        pCurrentSet->Rule.isEnabled = bEnable;
    }

    return SetIVSpecialParam(
        PT_PCI_SET_UPDATE_RULE, nChannelID, 0, pCurrentSet->Rule);
}

BOOL CDSP::ModifyRule( 
    int nChannelID,
    const WPG_Rule& Rule )
{
    int nDeviceID = nChannelID/CHANNEL_PER_DEVICE;
    if ( nDeviceID > m_nDeviceNum )
    {
        return FALSE;
    }

    if ( nChannelID!=m_szCurrentIVChannel[nDeviceID] )
    {
        return FALSE;
    }

    RuleSettingMap& RuleSettings = m_RuleCfgMap[nDeviceID];
    {
	    AutoLockAndUnlock(m_CfgMapCS[nDeviceID]);
	    IV_RuleID* pRuleID = (IV_RuleID*)Rule.ruleId;
	    RuleSettingMap::iterator iter = RuleSettings.find(*pRuleID);
	    if ( iter == RuleSettings.end() )
	    {
	        return FALSE;
	    }
	
	    CurrentRuleSetting* pCurrentSet = iter->second;
	    pCurrentSet->Rule = Rule;
    }

    return SetIVSpecialParam(
        PT_PCI_SET_UPDATE_RULE, nChannelID, 0, Rule);
}

BOOL CDSP::ModifySchedule( 
    int nChannelID, 
    const IV_RuleID& RuleID,
    const ScheduleSettings& Sch )
{
    int nDeviceID = nChannelID/CHANNEL_PER_DEVICE;
    if ( nDeviceID > m_nDeviceNum )
    {
        return FALSE;
    }

    if ( nChannelID!=m_szCurrentIVChannel[nDeviceID] )
    {
        return FALSE;
    }

    RuleSettingMap& RuleSettings = m_RuleCfgMap[nDeviceID];
    AutoLockAndUnlock(m_CfgMapCS[nDeviceID]);
    RuleSettingMap::iterator iter = RuleSettings.find(RuleID);
    if ( iter == RuleSettings.end() )
    {
        return FALSE;
    }

    CurrentRuleSetting* pCurrentSet = iter->second;
    pCurrentSet->Sch = Sch;
    return TRUE;
}

BOOL CDSP::ModifyAlarmOut( 
    int nChannelID,
    const IV_RuleID& RuleID, 
    const AlarmOutSettings& Alarm )
{
    int nDeviceID = nChannelID/CHANNEL_PER_DEVICE;
    if ( nDeviceID > m_nDeviceNum )
    {
        return FALSE;
    }

    if ( nChannelID!=m_szCurrentIVChannel[nDeviceID] )
    {
        return FALSE;
    }

    RuleSettingMap& RuleSettings = m_RuleCfgMap[nDeviceID];
    AutoLockAndUnlock(m_CfgMapCS[nDeviceID]);
    RuleSettingMap::iterator iter = RuleSettings.find(RuleID);
    if ( iter == RuleSettings.end() )
    {
        return FALSE;
    }

    CurrentRuleSetting* pCurrentSet = iter->second;
    pCurrentSet->Alarm = Alarm;
    return TRUE;
}

void CDSP::RegisterLiveDataCallBack(
    int nChannelID,
    IVideoSend* pVideoSend )
{
    AutoLockAndUnlock(m_VideoSendCS);
    m_szVideoSend[nChannelID] = pVideoSend;
}

void CDSP::UnRegisterLiveDataCallBack(
    int nChannelID,
    IVideoSend* pVideoSend )
{
    AutoLockAndUnlock(m_VideoSendCS);
    ASSERT(pVideoSend == m_szVideoSend[nChannelID]);
    m_szVideoSend[nChannelID] = NULL;
}

void CDSP::ReleaseLiveBuf( FRAMEBUFSTRUCT* p )
{
    DWORD& DelBufPara = p->BufferPara;
    int nDevice = DelBufPara >> 8 & 0xFF;
    int nIndex = DelBufPara & 0xFF;
    ReleasePrvBuf(nDevice, nIndex);
}
//
// ******************* IIVStatistic *********************
//

BOOL CDSP::IsHaveStatisticRule( int nChannelID )
{
    //int nDeviceID;
    //try
    //{
    //    GetDeviceIDByChannel(nChannelID, nDeviceID);
    //}
    //catch (const char* e)
    //{
    //    // log
    //    TRACE(e);
    //    return FALSE;
    //}

    //return m_szHaveStatistic[nDeviceID];
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
     m_pIVDataSender->Init(m_nDeviceNum, CHANNEL_PER_DEVICE);
}

void CDSP::SetSnapShotCallBack( ISnapShotSender* pSnapShotSender )
{
    m_pSnapShotSender = pSnapShotSender;
}


// End of file