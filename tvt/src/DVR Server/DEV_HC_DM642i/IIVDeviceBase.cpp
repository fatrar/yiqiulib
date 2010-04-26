/**CPP*************************************************************************
 File            : IIVDeviceBase.cpp
 Subsystem       : 
 Function Name(s): CIIVDeviceBase
 Author          : YiQiu
 Date            : 2010-4-9  
 Time            : 10:09
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "stdafx.h"
#include "DSP.h"

#define _Rule_Conifg_Check(nDeviceID) \
    int nDeviceID = nChannelID/CHANNEL_PER_DEVICE;\
    if ( nDeviceID > m_nDeviceNum )\
    {\
        return FALSE;\
    }\
    if ( nChannelID!=m_szCurrentIVChannel[nDeviceID] )\
    {\
        TRACE(__FUNCTION__##", Channel Is not Run IV!\n");\
        return FALSE;\
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
        // ������������ʣ������ܵ�ͨ����ֻ�ܵ��ÿ�ʼ����
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
            // ��������ʹ�õ�ͨ���봫�����һ��
            // ����Ϊ���ã���pass��ֹͣ�͵��ð忨
            if (bState){ return TRUE;}

            m_szCurrentIVChannel[nDeviceID] = Device_Free_Flag;
            break;
        }
        else
        {
            // �������˼Ϊ����������ͨ��ʹ�����ܣ�
            // ����������÷Ƕ�Ӧͨ����ֹͣ����ô����TRUE
            // �����ʼ�����Ǳ�����ֹͣ�����ܵģ����Է���FALSE
            return !bState;
        }
    }
    while (0);

    if ( bState )
    {
        //ResumeThread(m_hSmooth[nDeviceID]);
    }
    else
    {
        AutoLockAndUnlock(m_CfgMapCS[nDeviceID]);
        m_RuleCfgMap[nDeviceID].clear();
        //SuspendThread(m_hSmooth[nDeviceID]);
    }
    return SetParam(
        PT_PCI_SET_AI_ENABLE, nChannelID, int(bState) );
}

BOOL CDSP::IsHaveFreeDevice( void )
{
    for (int i =0; i< m_nDeviceNum; ++i)
    {
        if ( m_szCurrentIVChannel[i] == Device_Free_Flag )
        {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CDSP::IsHaveStatisticRule( int nChannelID )
{
    _Rule_Conifg_Check(nDeviceID);

    return m_pStatisticRule[nDeviceID]->bIsEnable;
}

// IIVDeviceBase
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
    _Rule_Conifg_Check(nDeviceID);

    /**
    *@note 1. �ж��Ƿ�ͳ�ƣ���ͳ�������ͳ�ƹ�����Ӻ���
    */
    RuleSettingMap& RuleSettings = m_RuleCfgMap[nDeviceID]; 
    IV_RuleID& RuleID = (IV_RuleID&)Rule.ruleId;
    if ( RuleID.RuleID.nType == IV_Statistic )
    {
        /**
        *@note 2. ��ͳ�������ͳ�ƹ�����Ӻ���
        */
        return AddStatistic(
            nChannelID, nDeviceID, Rule);
    }

    /**
    *@note 2. �����򰴳��洦��
    *@note 2.1 �ж�Map���Ƿ���ڣ����������Ӧ��û�У�
    *      Ȼ���µ�Rule�Ž�map
    */
    {
        AutoLockAndUnlock(m_CfgMapCS[nDeviceID]);
        RuleSettingMap::iterator iter = RuleSettings.find(RuleID);
        if ( iter != RuleSettings.end() )
        { 
            TRACE("______CDSP::Add Rule is Exist!\n");
            CurrentRuleSetting* pCurrentSet = iter->second;
            pCurrentSet->Rule = Rule;
            pCurrentSet->Sch = Sch;
            pCurrentSet->Alarm = Alarm;
        }
        else
        {
            RuleSettings[RuleID] = new CurrentRuleSetting(Rule,Sch,Alarm);
        }
    }

    /**
    *@note 2.2 Ȼ�����ݲ������ܵ��忨���������
    */
    return SetIVSpecialParam(
        PT_PCI_SET_ADD_RULE, nChannelID, 0, Rule);
}

BOOL CDSP::Remove(
    int nChannelID,
    const IV_RuleID& RuleID )
{
    _Rule_Conifg_Check(nDeviceID);

    /**
    *@note 1. �ж��Ƿ�ͳ�ƣ���ͳ�������ͳ�ƹ���ɾ������
    */
    RuleSettingMap& RuleSettings = m_RuleCfgMap[nDeviceID];
    if ( RuleID.RuleID.nType == IV_Statistic )
    {
        return RemoveStatistic(
            nChannelID, nDeviceID, RuleID );
    }

    /**
    *@note 2   �����򰴳��洦��
    *      �ж�Map���Ƿ���ڣ����������Ӧ���У�
    *      Ȼ��Rule��map��ɾ��,���������ܵ��忨���������
    */
    {
        AutoLockAndUnlock(m_CfgMapCS[nDeviceID]);
        RuleSettingMap::iterator iter = RuleSettings.find(RuleID);
        if ( iter == RuleSettings.end() )
        {
            TRACE("______CDSP::Remove Rule is not Exist!\n");
            return TRUE;
        }

        WPG_Rule& Rule = iter->second->Rule;
        SetIVSpecialParam(
            PT_PCI_SET_DEL_RULE, nChannelID, 0, Rule);
        delete iter->second;
        RuleSettings.erase(iter);
    } 
    return TRUE;
}

BOOL CDSP::EnableRule( 
    int nChannelID,
    const IV_RuleID& RuleID,
    BOOL bEnable /*=TRUE*/ )
{
    _Rule_Conifg_Check(nDeviceID);

    /**
    *@note 1. �ж��Ƿ�ͳ�ƣ���ͳ�������ͳ�ƹ���Enable����
    */
    RuleSettingMap& RuleSettings = m_RuleCfgMap[nDeviceID];
    CurrentRuleSetting* pCurrentSet = NULL;
    if ( RuleID.RuleID.nType == IV_Statistic )
    {
        return EnableStatistic(
            nChannelID, nDeviceID, RuleID, bEnable);
    }

    /**
    *@note 2. �����򰴳��洦��
    *@note 2.1 �ж�map���Ƿ���ڣ����������Ӧ���У�
    *      ����map�е�����
    */ 
    {
        AutoLockAndUnlock(m_CfgMapCS[nDeviceID]);
        RuleSettingMap::iterator iter = RuleSettings.find(RuleID);
        if ( iter == RuleSettings.end() )
        {
            TRACE("______CDSP::EnableRule Rule is not Exist!\n");
            return FALSE;
        }

        pCurrentSet = iter->second;
        pCurrentSet->Rule.isEnabled = bEnable;

        /**
        *@note 2.2 ���������ݲ������ܵ��忨���������
        */
        return SetIVSpecialParam(
            PT_PCI_SET_UPDATE_RULE, nChannelID, 0, pCurrentSet->Rule);
     }
}

BOOL CDSP::ModifyRule( 
    int nChannelID,
    const WPG_Rule& Rule )
{
    _Rule_Conifg_Check(nDeviceID);

    /**
    *@note 1. �ж��Ƿ�ͳ�ƣ���ͳ�������ͳ�ƹ���Enable����
    */
    RuleSettingMap& RuleSettings = m_RuleCfgMap[nDeviceID];
    IV_RuleID& RuleID = (IV_RuleID&)Rule.ruleId;
    if ( RuleID.RuleID.nType == IV_Statistic )
    {
        return ModifyStatistic(
            nChannelID, nDeviceID, Rule);
    }

    /**
    *@note 2. �����򰴳��洦��
    *@note 2.1 �ж�map���Ƿ���ڣ����������Ӧ���У�
    *          ����map�е�����
    */
    {
	    AutoLockAndUnlock(m_CfgMapCS[nDeviceID]);
	    RuleSettingMap::iterator iter = RuleSettings.find(RuleID);
	    if ( iter == RuleSettings.end() )
	    {
            TRACE("______CDSP::ModifyRule Rule is not Exist!\n");
	        return FALSE;
	    }
	
	    CurrentRuleSetting* pCurrentSet = iter->second;
	    pCurrentSet->Rule = Rule;
    }

    /**
    *@note 2.2 ���������ݲ������ܵ��忨���������
    */
    return SetIVSpecialParam(
        PT_PCI_SET_UPDATE_RULE, nChannelID, 0, Rule);
}

template<typename T, T CDSP::CurrentRuleSetting::*t>
BOOL CDSP::ModifyXX(
    int nChannelID, 
    const IV_RuleID& RuleID,
    const T& V )
{
    _Rule_Conifg_Check(nDeviceID);

    RuleSettingMap& RuleSettings = m_RuleCfgMap[nDeviceID];
    
    {
        AutoLockAndUnlock(m_CfgMapCS[nDeviceID]);
        RuleSettingMap::iterator iter = RuleSettings.find(RuleID);
        if ( iter == RuleSettings.end() )
        {
            return FALSE;
        }

        CurrentRuleSetting* pCurrentSet = iter->second;
        *pCurrentSet.*t = V;
    }
    return TRUE;
}

BOOL CDSP::ModifySchedule( 
    int nChannelID, 
    const IV_RuleID& RuleID,
    const ScheduleSettings& Sch )
{
    return ModifyXX<
        ScheduleSettings, 
        &CurrentRuleSetting::Sch>(nChannelID,RuleID,Sch);
}

BOOL CDSP::ModifyAlarmOut( 
    int nChannelID,
    const IV_RuleID& RuleID, 
    const AlarmOutSettings& Alarm )
{
    return ModifyXX<
        AlarmOutSettings, 
        &CurrentRuleSetting::Alarm>(nChannelID,RuleID,Alarm);
}

void CDSP::RegisterLiveDataCallBack(
    int nChannelID,
    IVideoSend* pVideoSend )
{
    {   
        AutoLockAndUnlock(m_VideoSendCS[nChannelID]);
        m_szVideoSend[nChannelID] = pVideoSend;
    }
}

void CDSP::UnRegisterLiveDataCallBack(
    int nChannelID,
    IVideoSend* pVideoSend )
{
    {
        AutoLockAndUnlock(m_VideoSendCS[nChannelID]);
        ASSERT(pVideoSend == m_szVideoSend[nChannelID]);
        m_szVideoSend[nChannelID] = NULL;
    } 
}

void CDSP::ReleaseLiveBuf( FRAMEBUFSTRUCT* p )
{
    BufPara Para(p->BufferPara);
    int nIndex = Para.Para.nBufIndex;
    int nChannelID = Para.Para.nChannelID;
    int nDeviceID = Para.Para.nDeviceID;
    ReleasePrvBuf(nDeviceID, nChannelID, nIndex);
}

void CDSP::GetDeviceInfo(
    size_t* pnDeviceNum,
    size_t* pnChannelNumByDevice )
{
    if ( pnDeviceNum )
    {
        *pnDeviceNum = m_nDeviceNum;
    }
    if ( pnChannelNumByDevice )
    {
        *pnChannelNumByDevice = CHANNEL_PER_DEVICE;
    }
}

void CDSP::GetIVDeviceInfo(
    size_t* pnIVChannelNumByDevice,
    size_t* pnMaxRuleNumByIVChannel )
{
    if ( pnIVChannelNumByDevice )
    {
        *pnIVChannelNumByDevice = IV_Num_PER_Device;
    }
    if ( pnMaxRuleNumByIVChannel )
    {
        *pnMaxRuleNumByIVChannel = AI_MAX_RULE_COUNT;
    }
}

// IIVDeviceBase2
// }


// End of file