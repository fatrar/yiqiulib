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
        else if (nTestMin < DaySet.starttime[i] )
        {
            break;
        }
    }

    TRACE("Schedule Pass Alarm!\n");
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

    if ( m_bIsOperatorDevice[nDevice] )
    {
        return FALSE;
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
        static char* g_RuleOperatorInfo[3] = 
        {
            "Rule Add",
            "Rule Del",
            "Rule Update"
        };

        AutoLockAndUnlock(m_pIVPack[nDevice].CS);
        
        PPARAMPACK pPack = (PPARAMPACK)(pBuf + sizeof(DWORD));    
        IVParmData& IVData = m_pIVPack[nDevice].param.front();

        size_t nCommandOffset = IVData.Commadparm.paramType-PT_PCI_SET_ADD_RULE;
        ASSERT(nCommandOffset < 3);
        TRACE(
            "IV Command --> ChID=%d, %s\n", 
            IVData.Commadparm.chanNum,
            g_RuleOperatorInfo[nCommandOffset]);
        m_bIsOperatorDevice[nDevice] = TRUE;

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
    int nChannelID = m_szCurrentIVChannel[nDevice];
    if ( nChannelID == Device_Free_Flag )
    {
        TRACE("That Device Is Not Run IV!\n");
        return;
    }

    BYTE* pIVData = pData + EACH_TRAN_BUFF_SIZE;
    PTVT_AI_VBI pIVVBI = (PTVT_AI_VBI)pIVData;
    unsigned int& dwNumberOfTargets = pIVVBI->dwNumberOfTargets;
    unsigned int& dwNumberOfEvents = pIVVBI->dwNumberOfEvents;
    
    if ( pIVVBI->byViewStates == WPG_GOOD_VIEW )
    {
        SetIVParamToDSP(nDevice);
    }

    if ( dwNumberOfEvents != 0 )
    {
        TRACE("NO Zero.........\n");
    }
#ifdef _UseLiveTime
    ULONGLONG nTime = ChangeTime(m_prevVideoTime);
#else
    //ULONGLONG nTime = ChangeTime(m_prevVideoTime);
    ULONGLONG nTime = ChangeTime(pIVVBI->frameRealTime);
    static ULONGLONG s_nPreviousTime = 0;
    if ( s_nPreviousTime >= nTime )
    {
#define Same_Time_Error "IV Same Time error!\n"
        ASSERT(Same_Time_Error);
        TRACE(Same_Time_Error);
    }
    s_nPreviousTime = nTime;
#endif  
    {
    //CStopWatchCallTest aa;
    }

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


    FILETIME* pTime = (FILETIME*)&nTime;

    PostThreadMessage(
        m_dwSmoothTheadID[nDevice],
        IV_Data_Coming,
        pTime->dwLowDateTime, pTime->dwHighDateTime);

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
    {
        AutoLockAndUnlock(m_SimulationCS);
        if (m_pIVAlarmCallBack && nChannelID==m_SimulationChanID)
        {
            for ( unsigned int i=0 ;i<dwNumberOfEvents;
                  ++i, ++pFirstEvent )
            {
                const AlarmOutTable* pTable = NULL;
                IV_RuleID& RuleID = (IV_RuleID&)pFirstEvent->ruleId;
                if ( RuleID.RuleID.nType == IV_Statistic )
                {
                    IIVStatisticFresher::StatisticDir Dir = 
                        m_SimulationRuleID == RuleID ? IIVStatisticFresher::Statistic_Left:IIVStatisticFresher::Statistic_Right;
                    m_pIVAlarmCallBack->OnStatisticFresh(
                        nChannelID,
                        Dir );
                }
                else
                {
                    m_pIVAlarmCallBack->OnAlarmCallBack(
                        RuleID.RuleID.nType,
                        nChannelID, pTime );
                }
            }

            return;
        }
    }
   
    // 如果没有不是模拟通道，就按正常逻辑
    for ( unsigned int i=0 ;i<dwNumberOfEvents;
          ++i, ++pFirstEvent )
    {
        const AlarmOutTable* pTable = NULL;
        IV_RuleID& RuleID = (IV_RuleID&)pFirstEvent->ruleId;
        if ( RuleID.RuleID.nType == IV_Statistic &&
             m_pIVStatisticFresher )
        {
            WPG_Rule& StatisticRule = m_pStatisticRule[nDevice]->Rule;
            IV_RuleID& StatisticRuleID = (IV_RuleID&)StatisticRule.ruleId;
            IIVStatisticFresher::StatisticDir Dir = 
                StatisticRuleID == RuleID ? IIVStatisticFresher::Statistic_Left:IIVStatisticFresher::Statistic_Right;
            m_pIVStatisticFresher->OnStatisticFresh(nChannelID, Dir);
           
            /**
            *@note 过滤统计的截图，并且偏移截图位置
            */
            PassSnapShot(pFirstEvent, pFirstPic);
            continue;
        }

        if ( !IsNeedAlarmOut(nDevice, RuleID, *pTime, pTable) )
        {
            /**
            *@note 偏移截图位置
            */
            PassSnapShot(pFirstEvent, pFirstPic);
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
        TRACE("Alarm Hold Pass Alarm!\n");
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
    if ( NULL == pTable )
    {
        return;
    }

    IV_RuleID& RuleID = (IV_RuleID&)(pEvent->ruleId);
    IVRuleType t = RuleID.RuleID.nType;
    {
        AutoLockAndUnlock(m_AlarmCallBackCS);
        m_IVAlarmCallBackParmQueue.push_back(
            IVAlarmCallBackParm(t, nChannelID, *pTable, *pTime) );
        PostThreadMessage(
            m_dwIVAlarmCallBackTheadID,
            Alarm_Coming, 
            0, 0);
    }
    
    //if ( m_AlarmCallBackFn )
    //{
    //    m_AlarmCallBackFn(
    //        *pTable,
    //        t,
    //        nChannelID,
    //        pTime,
    //        m_pAlarmCallBackParm );
    //}
}

static bool s_bSavePic = false;
#include <fstream>

void CDSP::DoSnapShot(
    int nChannelID,
    const WPG_EventOccurrence* pEvent,
    BYTE*& pFirstPic)
{
    if ( /*!m_ShowSnapShot ||*/ !m_pSnapShotSender )
    {
        return;
    }

    for (unsigned int i = 0; i<pEvent->numOfSlices; ++i)
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


void CDSP::PassSnapShot(
    const WPG_EventOccurrence* pEvent,
    BYTE*& pFirstPic )
{
    /**
    *@note 偏移截图位置
    */
    for (unsigned int i = 0; i<pEvent->numOfSlices; ++i)
    {   
        const WPG_EventSlice& slices = pEvent->slices[i];
        pFirstPic += slices.snapshotLength;
    }
} 


DWORD WINAPI CDSP::OnThreadAlarmCallBack( PVOID pParam )
{
    CDSP* pThis = (CDSP*)pParam;
    return pThis->LoopAlarmCallBack();
}

DWORD CDSP::LoopAlarmCallBack()
{
    if ( m_AlarmCallBackFn == NULL )
    {
        return -1;
    }

   

#define _Max_Alarm_CallBack_Loop 10

    IVAlarmCallBackParm Parm;
    MSG msg;

    //for ( int i = 0; i<10; ++i)
    //{
    //    m_AlarmCallBackFn(
    //        Parm.Table,
    //        Parm.t,
    //        0,
    //        &Parm.Time,
    //        m_pAlarmCallBackParm );
    //    Sleep(1000);
    //}
    //return 0;

    while(1)
    {     
        BOOL bRc = GetMessage(
            &msg, HWND(-1), 0, 0);
        if ( !bRc )
        {
            DWORD dwError = GetLastError();
            TRACE(
                _T("LoopAlarmCallBack GetMessage End at %x, msg.lParam=%x, msg.wParam=%x!\n"),
                dwError, msg.lParam, msg.wParam);
            break; // 是发WM_QUIT
        }

        switch(msg.message)
        { 
        case Alarm_Coming:
        {   
            int nLoopCount = 0;
            while ( m_IVAlarmCallBackParmQueue.size() != 0 )
            {
                {
                    AutoLockAndUnlock(m_AlarmCallBackCS);
                    Parm = m_IVAlarmCallBackParmQueue.front();
                    m_IVAlarmCallBackParmQueue.pop_front();
                }

                m_AlarmCallBackFn(
                    Parm.Table,
                    Parm.t,
                    Parm.nChannel,
                    &Parm.Time,
                    m_pAlarmCallBackParm );
                ++nLoopCount;
                if ( nLoopCount > _Max_Alarm_CallBack_Loop )
                {
                    break;
                }
            }
            break;
        }
        default:
            break;
        }
    }

    return 0;
}

// {
// ********************* IIVSimulation *******************
// 

BOOL CDSP::StartSimulation(
    int nChannelID,
    IIVSimulationAlarmCallBack* p,
    const WPG_Rule& Rule )
{
    int nDeviceID = nChannelID/CHANNEL_PER_DEVICE;
    if ( nDeviceID > m_nDeviceNum )
    {
        return FALSE;
    }

    /**
    *@remarks 模拟的整个过程逻辑很复杂，
    *  牵涉平滑，原本设置的还原，报警的输出
    *这里一定将每种情况调式一下
    */

    /**
    *@note 1. 设置模拟回调，通道，当前正在执行智能的通道
    */
    AutoLockAndUnlock(m_SimulationCS);
    m_pIVAlarmCallBack = p;
    m_SimulationChanID = nChannelID;
    m_LastRunChanID = m_szCurrentIVChannel[nDeviceID];
    m_SimulationRuleID = Rule.ruleId;

    /**
    *@note 2. 将当前使用智能的通道设为模拟通道    
    */
    m_szCurrentIVChannel[nDeviceID] = nChannelID;

    /**
    *@note 3. 根据不同情况处理
    */
    const IV_RuleID& RuleID = Rule.ruleId; 
    if ( m_LastRunChanID == Device_Free_Flag ) 
    {    
        /**
        *@note 3.1. 如果还没使用智能，就直接启用这个通道的智能，
        *           然后添加规则
        */
        if ( !PostThreadMessage( m_dwSmoothTheadID[nDeviceID], Switch_Channel, 0, 0) )
        {
            TRACE("PostThreadMessage Failed at Switch_Channel!\n"); 
            ASSERT(FALSE);
            return FALSE;
        }
        
        SetParam(PT_PCI_SET_AI_ENABLE, nChannelID, 1);
        Add(nChannelID, Rule);
    }
    else if ( m_LastRunChanID == nChannelID )
    {
        /**
        *@note 3.2. 如果刚好正在使用智能，就直接添加规则
        */
        SimulationAddRule(nChannelID, Rule);
    }
    else
    {
        /**
        *@note 3.3. 如果被其他的占用，首先停用占用的，然后开启模拟的，
        *           最后添加规则
        */
        if ( !PostThreadMessage( m_dwSmoothTheadID[nDeviceID], Switch_Channel, 0, 0) )
        {
            TRACE("PostThreadMessage Failed at Switch_Channel!\n"); 
            ASSERT(FALSE);
            return FALSE;
        }
        SetParam(PT_PCI_SET_AI_ENABLE, m_LastRunChanID, 0);
        SetParam(PT_PCI_SET_AI_ENABLE, nChannelID, 1);
 
        SimulationAddRule(nChannelID, Rule);
    }

    return TRUE;
}

BOOL CDSP::StopSimulation( int nChannelID )
{
    ASSERT(nChannelID==m_SimulationChanID);
    int nDeviceID = nChannelID/CHANNEL_PER_DEVICE;
    if ( nDeviceID > m_nDeviceNum )
    {
        return FALSE;
    }

    /**
    *@note 1. 清空模拟回调，通道，当前正在执行智能的通道
    */
    AutoLockAndUnlock(m_SimulationCS);
    m_pIVAlarmCallBack = NULL;
    m_SimulationChanID = Invaild_ChannelID;
    
    /**
    *@note 2. 将模拟前的正在跑到智能通道重新设回    
    */
    m_szCurrentIVChannel[nDeviceID] = m_LastRunChanID;
    
    /**
    *@note 3. 根据不同情况处理
    */
    if ( m_LastRunChanID == Device_Free_Flag ) //
    {
        /**
        *@note 3.1. 如果开启模拟前，还没使用智能，就直接停止模拟的智能，
        */
        if ( !PostThreadMessage( m_dwSmoothTheadID[nDeviceID], Switch_Channel, 0, 0) )
        {
            TRACE("PostThreadMessage Failed at Switch_Channel!\n"); 
            ASSERT(FALSE);
            return FALSE;
        }
        
        //SuspendThread(m_hSmooth[nDeviceID]);
        SetParam(PT_PCI_SET_AI_ENABLE, nChannelID, 0);
    }
    else if ( m_LastRunChanID == nChannelID ) // 
    {
        /**
        *@note 3.1. 如果开启模拟的通道是模拟前正在运行的智能的通道
        *           就直接删除模拟的规则
        */
        //Remove(nChannelID, m_SimulationRuleID);
        SimulationRemoveRule(nChannelID);
    }
    else //被其他的占用
    {
        /**
        *@note 3.1. 如果开启模拟的通道是占用模拟前正在运行的智能的通道
        *           就停止模拟通道，开启模拟前的通道
        *           最后添加所有模拟前的规则
        */
        if ( !PostThreadMessage( m_dwSmoothTheadID[nDeviceID], Switch_Channel, 0, 0) )
        {
            TRACE("PostThreadMessage Failed at Switch_Channel!\n"); 
            ASSERT(FALSE);
            return FALSE;
        }

        SetParam(PT_PCI_SET_AI_ENABLE, nChannelID, 0);
        SetParam(PT_PCI_SET_AI_ENABLE, m_LastRunChanID, 0);
        RuleSettingMap& RuleSetting = m_RuleCfgMap[nDeviceID];
        RuleSettingMap::iterator iter;
        for ( iter = RuleSetting.begin();
              iter!= RuleSetting.end();
              ++iter )
        {
            CurrentRuleSetting* pTmp = iter->second;
            Add(
                m_LastRunChanID, 
                pTmp->Rule,
                pTmp->Sch,
                pTmp->Alarm);
        }
    }

    return TRUE;
}
void CDSP::SimulationAddRule(
    int nChannelID,
    const WPG_Rule& Rule)
{
    /**
    *@note 如果不是统计就直接发送添加命令
    */
    m_SimulationRuleID = Rule.ruleId;
    if ( IV_Statistic != m_SimulationRuleID.RuleID.nType )
    {
        SetIVSpecialParam(
            PT_PCI_SET_ADD_RULE, nChannelID, 0, Rule);
        return;
    }

    /**
    *@note 如果是统计就需要特殊处理，添加两个规则
    */
    m_StatisticDir = 
        Rule.ruleDescription.description.tripwireEventDescription.direction;
     ASSERT(m_StatisticDir!=INVALID_DIRECTION);
    _AddStatistic(nChannelID, Rule);
}

void CDSP::SimulationRemoveRule(
    int nChannelID)
{
    /**
    *@note 如果不是统计就直接发送删除命令
    */
    static WPG_Rule Rule;
    memcpy(Rule.ruleId, &m_SimulationRuleID, 16);
    if ( IV_Statistic != m_SimulationRuleID.RuleID.nType )
    {
        SetIVSpecialParam(
            PT_PCI_SET_DEL_RULE, nChannelID, 0, Rule);
        return;
    }

    /**
    *@note 如果是统计就需要特殊处理，删除两个规则
    */
    ASSERT(m_StatisticDir!=INVALID_DIRECTION);   
    _RemoveStatistic(nChannelID, Rule);
}

// IIVSimulation
// }


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
    IIVDataSender* pIVDataSender,
    IIVStatisticFresher* pIVStatisticFresher )
{
    m_pIVDataSender = pIVDataSender;
    m_pIVStatisticFresher = pIVStatisticFresher;
    //m_pIVDataSender->Init(m_nDeviceNum, CHANNEL_PER_DEVICE);
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
    m_VideoSendCS[nChannel].Lock();
    if ( m_szVideoSend[nChannel] )
    {
        if ( !m_szVideoSend[nChannel]->OnVideoSend(p) )
        {
            ReleaseLiveBuf(p);
        }
    }
    else
    {
        if ( !m_pVideoCallBack(p) )
        {
            ReleaseLiveBuf(p);
        }
    }
    m_VideoSendCS[nChannel].Unlock();
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

//template<int nFrame>
//void AdjustPostMessage(
//    list<FRAMEBUFSTRUCT*>& LiveList,
//    volatile int& nPostMessage)
//{
//    size_t nQueueSize = LiveList.size();
//    if (nQueueSize < 2)
//    {
//        return;
//    }
//
//    FILETIME& Begin = (FILETIME&)(LiveList.front()->FrameTime);
//    FILETIME& End = (FILETIME&)(LiveList.back()->FrameTime);
//    __int64 nCompare = End - Begin;
//    if ( nCompare <= 0 )
//    {
//        ASSERT(FALSE);
//        return;
//    }
//
//    __int64 nTmp =25*nCompare;
//    int nTheoryValue = nTmp/1000;
//    nPostMessage = int(nTheoryValue) - int(nQueueSize);
//}

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
        LiveList.splice(LiveList.end(), BufList, iter, ++iter);
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
            LiveList.splice(LiveList.end(), BufList, BufList.begin(), iter);
            break;
        }
        if ( Test == ftime )
        {
            LiveList.splice(LiveList.end(), BufList, BufList.begin(), ++iter);
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
    MSG msg;
    PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    if(!SetEvent(h))
    {    
        TRACE("LoopLiveSmooth: Device=%d, Set event error,%d\n",
            GetLastError(), nDevice); 
        ASSERT(FALSE);
        return ;
    }

    // 这个几个变量的申明而不放在线程实际处理函数里面
    // 是为了提高运行效率
    int nChannel = -1;
    FRAMEBUFSTRUCT* p = NULL;
    volatile bool bCallflag = true;
    volatile bool bCanSendLive = false;
    FILETIME fTime;

    static const int s_FrameRate[2] = {25, 30};
    //AdjustPostMessageFn AdjustFn =
    //    m_dwVideoFormat==0 ? AdjustPostMessage<25>:AdjustPostMessage<33>;

    list<FRAMEBUFSTRUCT*> BufList;
    list<FRAMEBUFSTRUCT*> LiveList;

    int nFrame = s_FrameRate[m_dwVideoFormat];

    //UINT uResolution = 1000/nFrame;
    LPTIMECALLBACK pSmoothTimer = GetSmoothTimer(nDevice);
    UINT nEventID= ::timeSetEvent(1000/nFrame, 1, pSmoothTimer,(DWORD)this,TIME_PERIODIC); 
    while(1)
    {     
        BOOL bRc = GetMessage(
            &msg, HWND(-1), 
            Switch_Channel, End_Thead);
        if ( !bRc )
        {
            DWORD dwError = GetLastError();
            TRACE(
                _T("LoopLiveSmooth GetMessage End at %x, msg.lParam=%x, msg.wParam=%x!\n"),
                dwError, msg.lParam, msg.wParam);
            goto end; // 是发WM_QUIT
        }
   
        switch(msg.message)
        { 
        case Switch_Channel:
            FreeLiveList(BufList);
            //FreeLiveList(LiveList);
            //bCanSendLive = false;
            //m_bFisrtIVDataFlag[nDevice] = FALSE;
            //SuspendThread(m_hSmooth[nDevice]);
            break;
        case Push_Live_Data:
            if ( msg.wParam != m_szCurrentIVChannel[nDevice] )
            {
                TRACE(_T("Push_Live_Data Filter Some!\n"));
                ReleaseLiveBuf((FRAMEBUFSTRUCT*)msg.lParam);
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
                    VideoSend(nChannel, p);
                }

                LiveList.pop_front();
            }
            else
            {
                TRACE("Play_Again Pass!\n");
            }
            break;
        case Alarm_Occur:
            break;
        case End_Thead:
            goto end;
        }
    }

end:
    timeKillEvent(nEventID);
}


// Smooth IV Live
// }


// End of file