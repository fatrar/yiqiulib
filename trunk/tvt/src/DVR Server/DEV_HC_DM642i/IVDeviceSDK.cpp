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
    
#ifdef _UseLiveTime
    ULONGLONG nTime = ChangeTime(m_prevVideoTime);
#else
    //ULONGLONG nLiveTime = ChangeTime(m_prevVideoTime);
    ULONGLONG nTime = ChangeTime(pIVVBI->frameRealTime);
  
    {
    //CStopWatchCallTest aa;
    }
    //static ULONGLONG s_MaxBt = 0;
    //static ULONGLONG s_PreTime = 0;
    //ULONGLONG nTestTmp = nTime-s_PreTime;
    //
    //if ( (nTestTmp > s_MaxBt || nTestTmp >125*10000 ) && s_PreTime!=0 )
    //{
    //    if ( nTestTmp > s_MaxBt )
    //    {
    //         s_MaxBt = nTestTmp;
    //    }
    //   
    //    long nValue = nTestTmp/10000;
    //    //SYSTEMTIME syt;
    //    //FileTimeToSystemTime((FILETIME*)&nTestTmp, &syt);
    //    static char szbuf[128] = {0};
    //    sprintf_s(
    //        szbuf, "Max Data Time=%dms\n", 
    //        nValue);
    //    //OutputDebugString(szbuf);
    //    TRACE(szbuf);
    //}
    //s_PreTime = nTime;

    static int s_nNum = 0;
    if ( s_nNum )
    {
        SYSTEMTIME syt;
        //FileTimeToSystemTime((FILETIME*)&nLiveTime, &syt1);
        FileTimeToSystemTime((FILETIME*)&nTime, &syt);
        char szbuf[128] = {0};
        sprintf_s(
            szbuf, "First IV coming, Data Time=%d:%d:%d.%d\n", 
            syt.wHour, syt.wMinute,
            syt.wSecond, syt.wMilliseconds);
        //OutputDebugString(szbuf);
        TRACE(szbuf);
        --s_nNum;
    }
    //ASSERT( CompareFileTime((FILETIME*)&nLiveTime, (FILETIME*)&nTime) > 0 );
#endif

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

    //if ( !m_bFisrtIVDataFlag[nDevice] )
    {
        PostThreadMessage(
            m_dwSmoothTheadID[nDevice],
            IV_Data_Coming,
            pTime->dwLowDateTime, pTime->dwHighDateTime);
        //m_bFisrtIVDataFlag[nDevice] = TRUE;
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
   
    if ( bState )
    {
        ResumeThread(m_hSmooth[nDeviceID]);
    }
    else
    {
        SuspendThread(m_hSmooth[nDeviceID]);
    }
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

    AutoLockAndUnlock(m_SimulationCS[nChannelID]);
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

    AutoLockAndUnlock(m_SimulationCS[nChannelID]);
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
    AutoLockAndUnlock(m_VideoSendCS[nChannelID]);
    m_szVideoSend[nChannelID] = pVideoSend;
}

void CDSP::UnRegisterLiveDataCallBack(
    int nChannelID,
    IVideoSend* pVideoSend )
{
    AutoLockAndUnlock(m_VideoSendCS[nChannelID]);
    ASSERT(pVideoSend == m_szVideoSend[nChannelID]);
    m_szVideoSend[nChannelID] = NULL;
}

void CDSP::ReleaseLiveBuf( FRAMEBUFSTRUCT* p )
{
    BufPara Para(p->BufferPara);
    int nIndex = Para.Para.nBufIndex;
    int nChannelID = Para.Para.nChannelID;
    int nDeviceID = Para.Para.nDeviceID;
    ReleasePrvBuf(nDeviceID, nChannelID, nIndex);
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


// {
// *************** Smooth IV Live  ***************
// 

DWORD WINAPI CDSP::OnThreadSmooth( PVOID pParam )
{
    SmoonthThreadParm* pThreadParm = (SmoonthThreadParm*)pParam;
    SmoonthThreadParm p = *pThreadParm;
    delete pThreadParm;
    p.pthis->LoopLiveSmooth(p.dwDeviceID, p.handle);
    return 0;
}

void CDSP::VideoSend(int nChannel, FRAMEBUFSTRUCT* p)
{
    bool bflag = true;
    m_VideoSendCS[nChannel].Lock();
    if ( m_szVideoSend[nChannel] )
    {

        if ( !m_szVideoSend[nChannel]->OnVideoSend(p) )
        {
            ReleaseLiveBuf(p);
        }
        bflag = false;
    }
    m_VideoSendCS[nChannel].Unlock();

    if ( bflag )
    {
        //if ( nChannel == 0 )
        //{
        //    SYSTEMTIME syt;
        //    GetLocalTime(&syt);
        //    xTrace("%d-%d:%d\n", syt.wMinute, syt.wSecond, syt.wMilliseconds);
        //}
        if ( !m_pVideoCallBack(p) )
        {
            ReleaseLiveBuf(p);
        }
    }
}

void CDSP::FreeLiveList(list<FRAMEBUFSTRUCT*>& LiveList)
{
    list<FRAMEBUFSTRUCT*>::iterator iter;
    for ( iter = LiveList.begin();
          iter!= LiveList.end();
          ++iter)
    {
        ReleaseLiveBuf(*iter);
    }
    LiveList.clear();
}

void CDSP::TryPush(deque<FRAMEBUFSTRUCT*>& LiveList, FRAMEBUFSTRUCT* p)
{

}


template<int nFrame>
void AdjustPostMessage(
    list<FRAMEBUFSTRUCT*>& LiveList,
    volatile int& nPostMessage)
{
    size_t nQueueSize = LiveList.size();
    if (nQueueSize < 2)
    {
        return;
    }

    FILETIME& Begin = (FILETIME&)(LiveList.front()->FrameTime);
    FILETIME& End = (FILETIME&)(LiveList.back()->FrameTime);
    __int64 nCompare = End - Begin;
    if ( nCompare <= 0 )
    {
        ASSERT(FALSE);
        return;
    }

    __int64 nTmp =25*nCompare;
    int nTheoryValue = nTmp/1000;
    nPostMessage = int(nTheoryValue) - int(nQueueSize);
}

void AdjustLiveList(
    list<FRAMEBUFSTRUCT*>& LiveList,
    list<FRAMEBUFSTRUCT*>& BufList,
    FILETIME& ftime)
{   
    size_t nQueueSize = BufList.size();
    if ( nQueueSize == 0 )
    {
        return;
    }

    list<FRAMEBUFSTRUCT*>::iterator iter = BufList.begin();
    FILETIME& Test = (FILETIME&)(*iter)->FrameTime;
    if ( Test > ftime )
    {
        return;
    }
    else if (Test == ftime)
    {
        LiveList.splice(LiveList.end(), BufList, iter, iter);
        return;
    }

    for ( ; ; ++iter )
    {
        if ( iter == BufList.end() )
        {
            LiveList.splice(LiveList.end(), BufList);
            break;
        }
        if ( Test > ftime )
        {
            LiveList.splice(LiveList.end(), BufList, BufList.begin(), --iter);
            break;
        }
        if ( Test == ftime )
        {
            LiveList.splice(LiveList.end(), BufList, BufList.begin(), iter);
            break;
        }
    }
    //AdjustPostMessage<nFrame>(LiveList,nPostMessage);
}


template<int nDevice>
void CALLBACK CDSP::SmoothTimer( 
    UINT uID,UINT uMsg,
    DWORD dwUser,
    DWORD dw1,DWORD dw2 )
{
    {
       // CStopWatchCallTest aa;
    }
    CDSP* pThis = (CDSP*)dwUser;
    PostThreadMessage(pThis->m_dwSmoothTheadID[nDevice], Play_Again,0, 0);
}

LPTIMECALLBACK CDSP::GetSmoothTimer(int nDevice)
{
    switch (nDevice)
    {
    case 0:
    	return SmoothTimer<0>;
    case 1:
    	return SmoothTimer<1>;
    case 2:
    	return SmoothTimer<2>;
    case 3:
        return SmoothTimer<3>;
    default:
    	return NULL;
    }
}

typedef void (*AdjustLiveListFn)(list<FRAMEBUFSTRUCT*>&,list<FRAMEBUFSTRUCT*>&,FILETIME&);
typedef void (*AdjustPostMessageFn)(list<FRAMEBUFSTRUCT*>&,volatile int&);

// 先实现一版不带时间修正的（WM_TIMER）
void CDSP::LoopLiveSmooth(int nDevice, HANDLE h)
{
    TRACE("LoopLiveSmooth: Device=%d, Start Create Message Quque!\n", nDevice);
    MSG msg;
    PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    //BOOL bRc = PeekMessage(&msg, NULL, 0, WM_USER, PM_NOREMOVE);
    if(!SetEvent(h))
    {    
        TRACE("LoopLiveSmooth: Device=%d, Set event error,%d\n",
            GetLastError(), nDevice); 
        ASSERT(FALSE);
        return ;
    }

    TRACE("LoopLiveSmooth: Device=%d, Create Message Quque Finish!\n", nDevice);

    // 这个几个变量的申明而不放在线程实际处理函数里面
    // 是为了提高运行效率
    int nChannel = -1;
    FRAMEBUFSTRUCT* p = NULL;
    volatile bool bCallflag = true;
    volatile bool bCanSendLive = false;
    FILETIME fTime;

    static const int s_FrameRate[2] = {25, 30};
    AdjustPostMessageFn AdjustFn =
        m_dwVideoFormat==0 ? AdjustPostMessage<25>:AdjustPostMessage<33>;

    list<FRAMEBUFSTRUCT*> BufList;
    list<FRAMEBUFSTRUCT*> LiveList;

    volatile int nPostMessage = 0;

    int nFrame = s_FrameRate[m_dwVideoFormat];

    UINT uResolution = 1000/nFrame;
    //int nEventID = SetTimer(NULL, 0, 1000/nFrame, NULL);
    LPTIMECALLBACK pSmoothTimer = GetSmoothTimer(nDevice);
    UINT nEventID= ::timeSetEvent(1000/nFrame, 1, pSmoothTimer,(DWORD)this,TIME_PERIODIC); 
    
    while(1)
    {     
        BOOL bRc = GetMessage(&msg, HWND(-1),Suspend_Thread,End_Thead);
        if ( !bRc )
        {
            DWORD dwError = GetLastError();
            if ( dwError != 0 )
            {
                TRACE(
                    _T("LoopLiveSmooth GetMessage Failed at %x, msg.lParam=%x, msg.wParam=%x!\n"),
                    dwError, msg.lParam, msg.wParam);
                //ASSERT(false);
                continue;
            } 
            else
            {
                TRACE(
                    _T("LoopLiveSmooth GetMessage exit! msg.lParam=%x, msg.wParam=%x!\n"),
                    msg.lParam, msg.wParam);
                //if ( msg.message == WM_QUIT )
                {
                    goto end;
                }
            }
        }
   
        switch(msg.message)
        { 
        case Suspend_Thread:
            FreeLiveList(LiveList);
            bCanSendLive = false;
            m_bFisrtIVDataFlag[nDevice] = FALSE;
            SuspendThread(m_hSmooth[nDevice]);
            break;
        case Push_Live_Data:
            if ( msg.wParam != nDevice)
            {
                TRACE(_T("Push_Live_Data Filter Some!\n"));
            }
            else
            {
                BufList.push_back((FRAMEBUFSTRUCT*)msg.lParam);
            }
            break;
        case IV_Data_Coming:
            fTime.dwLowDateTime = msg.wParam;
            fTime.dwHighDateTime = msg.lParam;
            AdjustLiveList(LiveList, BufList, fTime);
            //AdjustPostMessage<25>(LiveList);
            //AdjustFn(LiveList,nPostMessage);
            if ( !bCanSendLive )
            {
                static int nTest = 3;
                --nTest;
                if ( nTest == 0 )
                {
                    //FreeLiveList(LiveList);
                    bCanSendLive = true;
                }
            }
            //AdjustFn(LiveList, BufList, fTime, nPostMessage);
            //bCanSendLive =true;
            break;
        case Play_Again:
           /* {
                CStopWatchCallTest aa;
            }*/
            if (bCanSendLive && LiveList.size() != 0)
            {
                p = LiveList.front();
                if ( p )
                {
                    nChannel = p->ChannelIndex;

                    /* SYSTEMTIME syt1;
                    FileTimeToSystemTime(
                    (FILETIME*)&p->FrameTime, &syt1);*/

                    VideoSend(nChannel, p);
                }

                LiveList.pop_front();
            }
            break;
        case End_Thead:
            goto end;
        }
    }

end:
    timeKillEvent(nEventID);
    //KillTimer(NULL, nEventID);
}


// Smooth IV Live
// }


// End of file