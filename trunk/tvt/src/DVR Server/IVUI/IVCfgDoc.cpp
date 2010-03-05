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
    CTreeCtrl& CameraTree,
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
        HTREEITEM hTmp = CameraTree.InsertItem(strRuleName, Item);
        void* pItemData = MakeTreeItemData(nChannelID, iter->first);
        CameraTree.SetItemData(hTmp, (DWORD_PTR)pItemData);
    }
}

#define GetIVRuleCfgXX(nChannelID, CameraTree,Item, xxx)\
    void* pItemData = (void*)CameraTree.GetItemData(Item); \
    const char* pID = (const char*)GetUserDataToItemData(pItemData);\
    RuleSettingMap& Map = m_Doc[nChannelID];\
    RuleSettingMap::iterator MapIter = Map.find(pID);\
    if ( MapIter == Map.end() )\
    {\
        TRACE(_T("GetIVRuleCfgXX No Found Iter!\n"));\
        return NULL;\
    }\
    return &MapIter->second->xxx


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
    CTreeCtrl& CameraTree,
    HTREEITEM Item )
{
    GetIVRuleCfgXX(nChannelID,CameraTree,Item, Rule);
    ///**
    //@note  1. Get ID by Tree Item And Update Item Name
    //*/
    //void* pItemData = (void*)CameraTree.GetItemData(Item);
    //const char* pID = (const char*)GetUserDataToItemData(pItemData);

    ///**
    //@note  3. Get Rule To memory
    //*/
    //RuleSettingMap& Map = m_Doc[nChannelID];
    //RuleSettingMap::iterator MapIter = Map.find(pID);
    //if ( MapIter == Map.end() )
    //{
    //    // log
    //    TRACE(_T("CIVRuleCfgDoc::RemoveRule No Found Iter!\n"));
    //    return NULL;
    //}
    //
    //return &MapIter->second->Rule;
}

void CIVRuleCfgDoc::AddRule( 
    int nChannelID, 
    const WPG_Rule& Rule, 
    CTreeCtrl& CameraTree,
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
    HTREEITEM hTmp = CameraTree.InsertItem(strRuleName, Item);
    void* pItemData = MakeTreeItemData(nChannelID, pID);
    CameraTree.SetItemData(hTmp, (DWORD_PTR)pItemData);

    /**
    @note  4. Do Trigger
    */
    DoTriggerTFun<&IRuleTrigger::OnRuleAdd>(nChannelID, pID);
}

void CIVRuleCfgDoc::RemoveRule(
    int nChannelID, 
    CTreeCtrl& CameraTree,
    HTREEITEM Item )
{
    /**
    @note  1. Delete Tree Item
    */
    void* pItemData = (void*)CameraTree.GetItemData(Item);
    const char* pID = (const char*)GetUserDataToItemData(pItemData);
    UnMakeTreeItemData(pItemData);
    CameraTree.DeleteItem(Item);

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

void CIVRuleCfgDoc::UpdateRule(
    int nChannelID, 
    const WPG_Rule& Rule,
    CTreeCtrl& CameraTree,
    HTREEITEM Item,
    BOOL IsRef )
{
    /**
    @note  1. Get ID by Tree Item And Update Item Name
    */
    void* pItemData = (void*)CameraTree.GetItemData(Item);
    const char* pID = (const char*)GetUserDataToItemData(pItemData);
    CameraTree.SetItemText(Item, CString(Rule.ruleName));

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
            // 再次判断用户是不是用我这边的引用,
            if ( &Rule != &MapIter->second->Rule)
            {
                MapIter->second->Rule = Rule;
            }
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
            Iter.ModifyRule(Rule);
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
    CTreeCtrl& CameraTree,
    HTREEITEM Item )
{
    GetIVRuleCfgXX(nChannelID,CameraTree,Item, Alarm);
}

void CIVAlarmOutCfgDoc::UpdateAlarmOut( 
    int nChannelID, 
    const AlarmOutSettings& Alarm,
    CTreeCtrl& CameraTree,
    HTREEITEM Item,
    BOOL IsRef /*= TRUE*/ )
{

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
    CTreeCtrl& CameraTree,
    HTREEITEM Item ) 
{
    GetIVRuleCfgXX(nChannelID,CameraTree,Item, Sch); 
}
//
// CIVScheduleCfgDoc
// }

// End of file




