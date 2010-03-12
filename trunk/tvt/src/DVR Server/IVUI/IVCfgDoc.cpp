/**H**************************************************************************
 File            : IVCfgDoc.cpp
 Subsystem       : 
 Function Name(s): CIVCfgDoc
 Author          : YiQiu
 Date            : 2010-3-4  
 Time            : 17:53
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "StdAfx.h"
#include "IVCfgDoc.h"

using namespace CameraTreeUtil;

typedef bool (IIVCfgMgr::IVVistor::*ModifyRuleFn)(const WPG_Rule&);
typedef bool (IIVCfgMgr::IVVistor::*ModifyScheduleFn)(const ScheduleSettings&);
typedef bool (IIVCfgMgr::IVVistor::*ModifyAlarmOutFn)(const AlarmOutSettings&);

typedef BOOL (IIVDeviceBase2::*DeviceModifyScheduleFn)(int,const IV_RuleID&,const ScheduleSettings&);
typedef BOOL (IIVDeviceBase2::*DeviceModifyAlarmOutFn)(int,const IV_RuleID&,const AlarmOutSettings&);

#ifdef IVCfgDoc_Use_Map
    CIVCfgDoc::AllRuleSettingMap CIVCfgDoc::m_Doc;
#else
    CIVCfgDoc::RuleSettingMap CIVCfgDoc::m_Doc[Max_Channel]; 
#endif

set<int> CIVCfgDoc::m_UseChannel;
CIVCfgDoc::RuleTriggerList CIVCfgDoc::m_RuleTrigger;

void CIVCfgDoc::Init()
{
    IIVCfgMgr* pIVCfgMgr = IIVCfgMgrFactory::GetIIVCfgMgr();
    for (int i=0; i<Max_Channel; ++i)
    {
        RuleSettingMap& Map = m_Doc[i];
        STLDeleteAssociate(Map); // ��ֹ�����
        for ( IIVCfgMgr::IVVistor Iter = pIVCfgMgr->Begin(i);
              Iter != pIVCfgMgr->End();
              Iter = Iter.Next() )
        {
            const char* pID = Iter.GetIdentityID();
            if ( pID == NULL )
            {
                // log ..
                TRACE("Iter.GetIdentityID() == NULL\n");
                continue;
            }

            RuleSettings* pRuleSettings = new RuleSettings();
            if ( Iter.GetRule(pRuleSettings->Rule) )
            {
                delete pRuleSettings;
                continue;
            }

            if ( Iter.GetAlarmOut(pRuleSettings->Alarm) )
            {
                pRuleSettings->Alarm = g_DefaultAlarmOutSettings;
            }

            if ( Iter.GetSchedule(pRuleSettings->Sch) )
            {
                pRuleSettings->Sch = g_DefaultScheduleSettings;
            }

            Map[pID] = pRuleSettings;
        }
    }
}

void CIVCfgDoc::Unit()
{
    for (int i=0; i<Max_Channel; ++i)
    {
        RuleSettingMap& Map = m_Doc[i];
        STLDeleteAssociate(Map);
    }
}

void CIVCfgDoc::RegisterRuleTrigger( IRuleTrigger* pRuleTrigger )
{
    m_RuleTrigger.push_back(pRuleTrigger);
}

BOOL CIVCfgDoc::IsIVChannel( int nChannelID )
{
    return  m_UseChannel.find(nChannelID) != m_UseChannel.end();
}


const IV_RuleID* CIVCfgDoc::GetRuleID(
    HTREEITEM Item )
{
    void* pItemData = (void*)m_CameraTree.GetItemData(Item); 
    const char* pID = (const char*)GetUserDataFromItemData(pItemData);
    int nChannelID = GetChannelFromItemData(pItemData);

    RuleSettingMap& Map = m_Doc[nChannelID];
    RuleSettings* pRuleSettings = Map[pID];
    ASSERT(pRuleSettings);
    return (IV_RuleID*)pRuleSettings->Rule.ruleId;
}

void CIVCfgDoc::OnInitCameraTree( 
    int nChannelID, 
    HTREEITEM Item )
{
    RuleSettingMap& Map = m_Doc[nChannelID];
    RuleSettingMap::iterator iter;
    for ( iter = Map.begin(); 
          iter!= Map.end();
          ++iter )
    {
        RuleSettings* pRuleSettings = iter->second;
        ASSERT(pRuleSettings);
        CString strRuleName(pRuleSettings->Rule.ruleName);

        //
        // [] �Ժ��Ϊ��ͼ����������Щͨ������ʹ��
        //
        HTREEITEM hTmp = m_CameraTree.InsertItem(strRuleName, Item);
        void* pItemData = MakeTreeItemData(nChannelID, iter->first);
        m_CameraTree.SetItemData(hTmp, (DWORD_PTR)pItemData);
    }
}

/**
*@note  CIVCfgDoc::GetIVRuleCfgXX �ú�ʵ�ֵİ汾��
*      ���ں귽ʽ���ܵ�ʽ������ģ�帴��ʵ��
*/
//#define GetIVRuleCfgXX(nChannelID, CameraTree,Item, xxx)\
//    void* pItemData = (void*)CameraTree.GetItemData(Item); \
//    const char* pID = (const char*)GetUserDataFromItemData(pItemData);\
//    RuleSettingMap& Map = m_Doc[nChannelID];\
//    RuleSettingMap::iterator MapIter = Map.find(pID);\
//    if ( MapIter == Map.end() )\
//    {\
//        TRACE(_T("GetIVRuleCfgXX No Found Iter!\n"));\
//        return NULL;\
//    }\
//    return &MapIter->second->xxx

//
// ����д��һ����defineʵ�ֵ�ͬ�����ܵ� ���ں귽ʽ���ܵ�ʽ������ģ�帴��ʵ��
//
template<typename T>
T* CIVCfgDoc::GetIVRuleCfgXX(
    HTREEITEM Item )
{
    /**
    *@note  1. Get ID And channelID by Tree Item
    */
    void* pItemData = (void*)m_CameraTree.GetItemData(Item); 
    const char* pID = (const char*)GetUserDataFromItemData(pItemData);
    int nChannelID = GetChannelFromItemData(pItemData);

    /**
    *@note  2. Get XX To memory
    */
    RuleSettingMap& Map = m_Doc[nChannelID];
    RuleSettingMap::iterator MapIter = Map.find(pID);
    if ( MapIter == Map.end() )
    {
        TRACE(_T("GetIVRuleCfgXX No Found Iter!\n"));
        return NULL;
    }
  
    struct CXXX
    {
        CXXX(RuleSettings* p):m_p(p){}
        T* OutPut(){T* p(NULL); return OutPut(p);};
        WPG_Rule* OutPut(WPG_Rule* p){return &m_p->Rule;}
        ScheduleSettings* OutPut(ScheduleSettings*p){return &m_p->Sch;}
        AlarmOutSettings* OutPut(AlarmOutSettings*p){return &m_p->Alarm;}
        RuleSettings* m_p;
    };

    CXXX XX(MapIter->second);
    return XX.OutPut();
}

template<typename T>
inline const char* CIVCfgDoc::GetRuleIDXX(
    const T& t,
    HTREEITEM Item,
    int& nChannelID)
{
    void* pItemData = (void*)m_CameraTree.GetItemData(Item);
    nChannelID = GetChannelFromItemData(pItemData);
    return (const char*)GetUserDataFromItemData(pItemData);
}

template<>
inline const char* CIVCfgDoc::GetRuleIDXX<WPG_Rule>(
    const WPG_Rule& Rule,
    HTREEITEM Item,
    int& nChannelID )
{
    m_CameraTree.SetItemText(Item, CString(Rule.ruleName));
    void* pItemData = (void*)m_CameraTree.GetItemData(Item);
    nChannelID = GetChannelFromItemData(pItemData);
    return (const char*)GetUserDataFromItemData(pItemData);
}

template<typename T, typename Tfn1, Tfn1 fn1,
    typename Tfn2, Tfn2 fn2>
void CIVCfgDoc::SetCfgToAllXX(const T& V)
{
    struct CXXX
    {
        CXXX(RuleSettings* p):m_p(p){}
        void Set(const ScheduleSettings&p){m_p->Sch=p;}
        void Set(const AlarmOutSettings&p){m_p->Alarm=p;}
        RuleSettings* m_p;
    };

    /**
    *@note  1. Set Cfg To XML and Memory
    */
    IIVCfgMgr* pIVCfgMgr = IIVCfgMgrFactory::GetIIVCfgMgr();
    for (int i=0; i<Max_Channel; ++i)
    {
        IIVCfgMgr::IVVistor IVIter = pIVCfgMgr->Begin(i);
        for ( ; IVIter != pIVCfgMgr->End();
            IVIter=IVIter.Next() )
        {
            (IVIter.*fn1)(V);
        }

        RuleSettingMap& Map = m_Doc[i];
        RuleSettingMap::iterator iter;
        for ( iter = Map.begin();
            iter!= Map.end();
            ++iter )
        {
            RuleSettings* pRuleSettings = iter->second;
            CXXX(pRuleSettings).Set(V);
        }
    }
    pIVCfgMgr->Apply();

    /**
    *@note 2. Update Device Cfg is Channel If Run IV
    */
    set<int>::iterator ChIter;
    for ( ChIter = m_UseChannel.begin();
          ChIter!= m_UseChannel.end();
          ++ChIter )
    {
        int nChannelID = *ChIter;
        RuleSettingMap& Map = m_Doc[nChannelID];
        RuleSettingMap::iterator iter;
        for ( iter = Map.begin();
              iter!= Map.end();
              ++iter  )
        {
            RuleSettings* pRuleSettings = iter->second;
            (g_IIVDeviceBase2->*fn2)(
                nChannelID,
                (IV_RuleID&)pRuleSettings->Rule.ruleId,
                V);
        }
    }
}

// {
// ****************** CIVRuleCfgDoc **********************
//

//void CIVRuleCfgDoc::OnInitCameraTree( 
//    int nChannelID, 
//    CTreeCtrl& CameraTree,
//    HTREEITEM Item )
//{
//
//}
WPG_Rule* CIVRuleCfgDoc::GetRule( 
    HTREEITEM Item )
{
    return GetIVRuleCfgXX<WPG_Rule>(Item);
}

void CIVRuleCfgDoc::AddRule( 
    const WPG_Rule& Rule, 
    HTREEITEM Item )
{
    /**
    @note  1. Save To XML
    */
    IIVCfgMgr* pIVCfgMgr = IIVCfgMgrFactory::GetIIVCfgMgr();
    IIVCfgMgr::IVVistor Iter = pIVCfgMgr->Add(nChannelID, Rule);
    int nChannelID = GetChannelFromItemData(pItemData);
    const char* pID = Iter.GetIdentityID();
    if ( pID == NULL )
    {
        // log
        TRACE(_T("***** CIVRuleCfgDoc::AddRule Failed!\n"));
        pIVCfgMgr->Remove(nChannelID, Iter);
        return;
    }
    pIVCfgMgr->Apply();

    /**
    @note  2. Save To memory
    */
    RuleSettingMap& Map = m_Doc[nChannelID];
    Map[pID] = new RuleSettings(Rule);

    /**
    @note  3. Insert Tree Item
    */
    CString strRuleName(Rule.ruleName);
    HTREEITEM hTmp = m_CameraTree.InsertItem(strRuleName, Item);
    void* pItemData = MakeTreeItemData(nChannelID, pID);
    m_CameraTree.SetItemData(hTmp, (DWORD_PTR)pItemData);

    /**
    @note  4. Do Trigger
    */
    DoTriggerTFun<&IRuleTrigger::OnRuleAdd>(nChannelID, pID);
}

void CIVRuleCfgDoc::RemoveRule(
    HTREEITEM Item )
{
    /**
    @note  1. Delete Tree Item
    */
    void* pItemData = (void*)m_CameraTree.GetItemData(Item);
    const char* pID = (const char*)GetUserDataFromItemData(pItemData);
    int nChannelID = GetChannelFromItemData(pItemData);
    UnMakeTreeItemData(pItemData);
    m_CameraTree.DeleteItem(Item);

    /**
    @note  2. Remove To memory And If Channel Is Use IV, notify Device
    */
    RuleSettingMap& Map = m_Doc[nChannelID];
    RuleSettingMap::iterator MapIter = Map.find(pID);
    if ( MapIter == Map.end() )
    {
        // log
        TRACE(_T("CIVRuleCfgDoc::RemoveRule No Found Iter!\n"));
    }
    else
    {
        RuleSettings* pRuleSettings = MapIter->second;
        if ( IsIVChannel(nChannelID) )
        {
            g_IIVDeviceBase2->Remove(
                nChannelID,
                (IV_RuleID&)(pRuleSettings->Rule.ruleId));
        }
        delete pRuleSettings;
        Map.erase(MapIter);
    }
    
    /**
    @note  3. Remove To XML
    */
    BOOL bFound = FALSE;
    IIVCfgMgr* pIVCfgMgr = IIVCfgMgrFactory::GetIIVCfgMgr();
    for ( IIVCfgMgr::IVVistor Iter = pIVCfgMgr->Begin(nChannelID);
          Iter != pIVCfgMgr->End();
          Iter = Iter.Next() )
    {
        const char* pTmpID = Iter.GetIdentityID();
        if ( pTmpID == pID )
        {
            pIVCfgMgr->Remove(nChannelID, Iter);
            pIVCfgMgr->Apply();
            return;
        }
    }

    if ( !bFound )
    {
        // log
        TRACE(_T("CIVRuleCfgDoc::RemoveRule XML No Found Iter!\n"));
    }

    /**
    @note  4. Do Trigger
    */
    DoTriggerTFun<&IRuleTrigger::OnRuleRemove>(nChannelID, pID);
}

void CIVRuleCfgDoc::EnableRule(
    HTREEITEM Item,
    BOOL bEnable )
{
    /**
    @note  1. Get Info From Tree Item
    */
    void* pItemData = (void*)m_CameraTree.GetItemData(Item);
    const char* pID = (const char*)GetUserDataFromItemData(pItemData);
    int nChannelID = GetChannelFromItemData(pItemData);
    
    /**
    @note  2. Update To memory And If Channel Is Use IV, notify Device
    */
    RuleSettingMap& Map = m_Doc[nChannelID];
    RuleSettingMap::iterator MapIter = Map.find(pID);
    RuleSettings* pRuleSettings = NULL;
    if ( MapIter == Map.end() )
    {
        // log
        TRACE(_T("CIVRuleCfgDoc::RemoveRule No Found Iter!\n"));
    }
    else
    {
        pRuleSettings = MapIter->second;
        if ( bEnable == pRuleSettings->Rule.isEnabled )
        {
            ASSERT(FALSE);
            return;
        }

        if ( IsIVChannel(nChannelID) )
        {
            g_IIVDeviceBase2->EnableRule(
                nChannelID,
                (IV_RuleID&)(pRuleSettings->Rule.ruleId),
                bEnable );
        }
        pRuleSettings->Rule.isEnabled = bEnable;
    }

    /**
    @note  3. Update XML
    */
    BOOL bFound = FALSE;
    IIVCfgMgr* pIVCfgMgr = IIVCfgMgrFactory::GetIIVCfgMgr();
    for ( IIVCfgMgr::IVVistor Iter = pIVCfgMgr->Begin(nChannelID);
          Iter != pIVCfgMgr->End();
          Iter = Iter.Next() )
    {
        const char* pTmpID = Iter.GetIdentityID();
        if ( pTmpID == pID )
        {
            Iter.ModifyRule(pRuleSettings->Rule);
            pIVCfgMgr->Apply();
            return;
        }
    }

    if ( !bFound )
    {
        // log
        TRACE(_T("CIVRuleCfgDoc::RemoveRule XML No Found Iter!\n"));
    }
}

template<typename T, typename Tfn, Tfn fn>
void CIVCfgDoc::UpdateRuleCfgXX(
    const T& V,
    HTREEITEM Item,
    BOOL IsRef /*= TRUE*/ )
{
    /**
    @note  1. Get ID by Tree Item And Update Item Name
    */
    int nChannelID;
    const char* pID = GetRuleIDXX<T>(V,Item,nChannelID);

    struct CXXX
    {
        CXXX(RuleSettings* p):m_p(p){}

        // �ٴ��ж��û��ǲ���������ߵ�����,
        void Set(const WPG_Rule& p){if(&p!=&m_p->Rule) m_p->Rule=p;}
        void Set(const ScheduleSettings&p){if(&p!=&m_p->Sch) m_p->Sch=p;}
        void Set(const AlarmOutSettings&p){if(&p!=&m_p->Alarm) m_p->Alarm=p;}
        RuleSettings* m_p;
    };

    /**
    @note  2. Update To memory
    */
    if ( !IsRef )
    {
        RuleSettingMap& Map = m_Doc[nChannelID];
        RuleSettingMap::iterator MapIter = Map.find(pID);
        if ( MapIter == Map.end() )
        {
            // log
            TRACE(_T("CIVRuleCfgDoc::RemoveRule No Found Iter!\n"));
        }
        else
        {
            CXXX(MapIter->second).Set(V);
        }
    }

    /**
    @note  3. Update To XML
    */
    BOOL bFound = FALSE;
    IIVCfgMgr* pIVCfgMgr = IIVCfgMgrFactory::GetIIVCfgMgr();
    pIVCfgMgr->Begin(nChannelID);
    for ( IIVCfgMgr::IVVistor Iter = pIVCfgMgr->Begin(nChannelID);
        Iter != pIVCfgMgr->End();
        Iter = Iter.Next() )
    {
        const char* pTmpID = Iter.GetIdentityID();
        if ( pTmpID == pID )
        {
            //fn f = &IIVCfgMgr;
            (Iter.*fn)(V);
            pIVCfgMgr->Apply();
            return;
        }
    }

    if ( !bFound )
    {
        // log
        TRACE(_T("CIVRuleCfgDoc::UpdateRuleCfgXX XML No Found Iter!\n"));
    }
}

void CIVRuleCfgDoc::UpdateRule(
    const WPG_Rule& Rule,
    HTREEITEM Item,
    BOOL IsRef )
{
    UpdateRuleCfgXX<
        WPG_Rule, ModifyRuleFn,
        &IIVCfgMgr::IVVistor::ModifyRule>(
        Rule, Item, IsRef);
}

template<OnRuleXXFn T>
void CIVRuleCfgDoc::DoTriggerTFun(
    int nChannelID,
    const char* pIdentityID)
{
    CIVCfgDoc::RuleTriggerList::iterator iter;
    for ( iter = m_RuleTrigger.begin();
         iter!= m_RuleTrigger.end();
         ++iter )
    {
        (*iter->*T)(nChannelID, pIdentityID);
    }
}

void CIVRuleCfgDoc::Use( 
    int nChannelID, 
    BOOL bEnable )
{
    if ( bEnable )
    {
        m_UseChannel.insert(nChannelID);
    }
    else
    {
        m_UseChannel.erase(nChannelID);
    }

    int szChannel[_MaxAutoChannel] = {0};
    set<int>::iterator iter = m_UseChannel.begin();
    for ( size_t i = 0;
          iter != m_UseChannel.end();
          ++iter, ++i )
    {
        szChannel[i] = *iter;
    }

    IIVCfgMgr* pIVCfgMgr = IIVCfgMgrFactory::GetIIVCfgMgr();
    pIVCfgMgr->SetAutoRunChannel(szChannel, i);
    pIVCfgMgr->Apply();
}

//
// CIVRuleCfgDoc
// }



// {
// ****************** CIVAlarmOutCfgDoc **********************
//

AlarmOutSettings* CIVAlarmOutCfgDoc::GetAlarmOut(
    HTREEITEM Item )
{
    return GetIVRuleCfgXX<AlarmOutSettings>(Item);
}

void CIVAlarmOutCfgDoc::UpdateAlarmOut( 
    const AlarmOutSettings& Alarm,
    HTREEITEM Item,
    BOOL IsRef /*= TRUE*/ )
{
    CIVCfgDoc::UpdateRuleCfgXX<
        AlarmOutSettings, ModifyAlarmOutFn,
        &IIVCfgMgr::IVVistor::ModifyAlarmOut>(
        Alarm, Item, IsRef);
}

void CIVAlarmOutCfgDoc::SetCfgToAll(
    const AlarmOutSettings& Alarm )
{
    CIVCfgDoc::SetCfgToAllXX<
        AlarmOutSettings, 
        ModifyAlarmOutFn,
        &IIVCfgMgr::IVVistor::ModifyAlarmOut,
        DeviceModifyAlarmOutFn,
        &IIVDeviceBase2::ModifyAlarmOut>(Alarm);
}

// CIVAlarmOutCfgDoc
// }



// {
// ****************** CIVScheduleCfgDoc **********************
//

ScheduleSettings* CIVScheduleCfgDoc::GetSchedule( 
    HTREEITEM Item ) 
{
    return GetIVRuleCfgXX<ScheduleSettings>(Item); 
}

void CIVScheduleCfgDoc::UpdateSchedule(
    const ScheduleSettings& Sch,
    HTREEITEM Item,
    BOOL IsRef /*= TRUE*/ )
{
    UpdateRuleCfgXX<
        ScheduleSettings, ModifyScheduleFn,
        &IIVCfgMgr::IVVistor::ModifySchedule>(
        Sch, Item, IsRef);
}

void CIVScheduleCfgDoc::SetCfgToAll(
    const ScheduleSettings& Sch )
{
    CIVCfgDoc::SetCfgToAllXX<
        ScheduleSettings,
        ModifyScheduleFn,
        &IIVCfgMgr::IVVistor::ModifySchedule,
        DeviceModifyScheduleFn,
        &IIVDeviceBase2::ModifySchedule>(Sch);
}

//
// CIVScheduleCfgDoc
// }

// End of file



