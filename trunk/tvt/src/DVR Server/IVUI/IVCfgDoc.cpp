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
        Map.clear(); // 防止调多次
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

void CIVCfgDoc::RegisterRuleTrigger( IRuleTrigger* pRuleTrigger )
{
    m_RuleTrigger.push_back(pRuleTrigger);
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
        // [] 以后改为贴图，来区分哪些通道正在使用
        //
        HTREEITEM hTmp = m_CameraTree.InsertItem(strRuleName, Item);
        void* pItemData = MakeTreeItemData(nChannelID, iter->first);
        m_CameraTree.SetItemData(hTmp, (DWORD_PTR)pItemData);
    }
}

/**
*@note  CIVCfgDoc::GetIVRuleCfgXX 用宏实现的版本，
*      由于宏方式不能调式，所以模板复杂实现
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
// 上面写了一个用define实现的同样功能的 由于宏方式不能调式，所以模板复杂实现
//
template<typename T>
T* CIVCfgDoc::GetIVRuleCfgXX(
    int nChannelID,
    HTREEITEM Item )
{
    /**
    *@note  1. Get ID by Tree Item And Update Item Name
    */
    void* pItemData = (void*)m_CameraTree.GetItemData(Item); 
    const char* pID = (const char*)GetUserDataFromItemData(pItemData);

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
    int nChannelID, 
    HTREEITEM Item )
{
    return GetIVRuleCfgXX<WPG_Rule>(nChannelID,Item);
}

void CIVRuleCfgDoc::AddRule( 
    int nChannelID, 
    const WPG_Rule& Rule, 
    HTREEITEM Item )
{
    /**
    @note  1. Save To XML
    */
    IIVCfgMgr* pIVCfgMgr = IIVCfgMgrFactory::GetIIVCfgMgr();
    IIVCfgMgr::IVVistor Iter = pIVCfgMgr->Add(nChannelID, Rule);
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
    int nChannelID, 
    HTREEITEM Item )
{
    /**
    @note  1. Delete Tree Item
    */
    void* pItemData = (void*)m_CameraTree.GetItemData(Item);
    const char* pID = (const char*)GetUserDataFromItemData(pItemData);
    UnMakeTreeItemData(pItemData);
    m_CameraTree.DeleteItem(Item);

    /**
    @note  2. Remove To memory
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
        delete MapIter->second;
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
            bFound = TRUE;
            break;
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

template<typename T>
inline const char* CIVCfgDoc::GetRuleID(
    const T& t,
    HTREEITEM Item)
{
    void* pItemData = (void*)m_CameraTree.GetItemData(Item);
    return (const char*)GetUserDataFromItemData(pItemData);
}

template<>
inline const char* CIVCfgDoc::GetRuleID<WPG_Rule>(
    const WPG_Rule& Rule,
    HTREEITEM Item)
{
    m_CameraTree.SetItemText(Item, CString(Rule.ruleName));
    void* pItemData = (void*)m_CameraTree.GetItemData(Item);
    return (const char*)GetUserDataFromItemData(pItemData);
}

typedef bool (IIVCfgMgr::IVVistor::*ModifyRuleFn)(const WPG_Rule&);
typedef bool (IIVCfgMgr::IVVistor::*ModifyScheduleFn)(const ScheduleSettings&);
typedef bool (IIVCfgMgr::IVVistor::*ModifyAlarmOutFn)(const AlarmOutSettings&);

template<typename T, typename Tfn, Tfn fn>
void CIVCfgDoc::UpdateRuleCfgXX(
    int nChannelID, 
    const T& V,
    HTREEITEM Item,
    BOOL IsRef /*= TRUE*/ )
{
    /**
    @note  1. Get ID by Tree Item And Update Item Name
    */
    const char* pID = GetRuleID<T>(V,Item);

    struct CXXX
    {
        CXXX(RuleSettings* p):m_p(p){}

        // 再次判断用户是不是用我这边的引用,
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
            bFound = TRUE;
            break;
        }
    }

    if ( !bFound )
    {
        // log
        TRACE(_T("CIVRuleCfgDoc::UpdateRule XML No Found Iter!\n"));
    }
}


void CIVRuleCfgDoc::UpdateRule(
    int nChannelID, 
    const WPG_Rule& Rule,
    HTREEITEM Item,
    BOOL IsRef )
{
    UpdateRuleCfgXX<
        WPG_Rule, ModifyRuleFn,
        &IIVCfgMgr::IVVistor::ModifyRule>(
        nChannelID, Rule, Item, IsRef);
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

//
// CIVRuleCfgDoc
// }



// {
// ****************** CIVAlarmOutCfgDoc **********************
//
//void CIVAlarmOutCfgDoc::OnInitCameraTree(
//    int nChannelID, 
//    CTreeCtrl& CameraTree,
//    HTREEITEM Item )
//{
//
//}
//


AlarmOutSettings* CIVAlarmOutCfgDoc::GetAlarmOut(
    int nChannelID,
    HTREEITEM Item )
{
    return GetIVRuleCfgXX<AlarmOutSettings>(nChannelID,Item);
}

void CIVAlarmOutCfgDoc::UpdateAlarmOut( 
    int nChannelID, 
    const AlarmOutSettings& Alarm,
    HTREEITEM Item,
    BOOL IsRef /*= TRUE*/ )
{
    UpdateRuleCfgXX<
        AlarmOutSettings, ModifyAlarmOutFn,
        &IIVCfgMgr::IVVistor::ModifyAlarmOut>(
        nChannelID, Alarm, Item, IsRef);
}

// CIVAlarmOutCfgDoc
// }



// {
// ****************** CIVScheduleCfgDoc **********************
//

//void CIVScheduleCfgDoc::OnInitCameraTree(
//    int nChannelID,
//    CTreeCtrl& CameraTree,
//    HTREEITEM Item )
//{
//
//}

ScheduleSettings* CIVScheduleCfgDoc::GetSchedule( 
    int nChannelID, 
    HTREEITEM Item ) 
{
    return GetIVRuleCfgXX<ScheduleSettings>(nChannelID,Item); 
}

void CIVScheduleCfgDoc::UpdateSchedule(
    int nChannelID, 
    const ScheduleSettings& Sch,
    HTREEITEM Item,
    BOOL IsRef /*= TRUE*/ )
{
    UpdateRuleCfgXX<
        ScheduleSettings, ModifyScheduleFn,
        &IIVCfgMgr::IVVistor::ModifySchedule>(
        nChannelID, Sch, Item, IsRef);
}

//
// CIVScheduleCfgDoc
// }

// End of file




