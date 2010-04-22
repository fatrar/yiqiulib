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

int CIVCfgDoc::s_nDeviceNum  = Default_Device_Num;
int CIVCfgDoc::s_nMaxChannel = Default_Max_Channel;
int CIVCfgDoc::s_nIVChannelNumByDevice  = Default_IVChannelNum_By_Device;
int CIVCfgDoc::s_nMaxRuleNumByIVChannel = Default_Max_RuleNum_By_IVChannel;

using namespace CameraTreeUtil;

typedef bool (IIVCfgMgr::IVVistor::*ModifyRuleFn)(const WPG_Rule&);
typedef bool (IIVCfgMgr::IVVistor::*ModifyScheduleFn)(const ScheduleSettings&);
typedef bool (IIVCfgMgr::IVVistor::*ModifyAlarmOutFn)(const AlarmOutSettings&);

typedef BOOL (IIVDeviceBase2::*DeviceModifyScheduleFn)(int,const IV_RuleID&,const ScheduleSettings&);
typedef BOOL (IIVDeviceBase2::*DeviceModifyAlarmOutFn)(int,const IV_RuleID&,const AlarmOutSettings&);


CIVCfgDoc::RuleSettingMap* CIVCfgDoc::m_pDoc = NULL; 
int* CIVCfgDoc::m_pShowState = NULL;
BOOL* CIVCfgDoc::m_pIsHaveStatistic = NULL;


set<int> CIVCfgDoc::m_UseChannel;
CIVCfgDoc::RuleTriggerList CIVCfgDoc::m_RuleTrigger;

void CIVCfgDoc::Init()
{
    CIVCfgDoc::m_pDoc = new CIVCfgDoc::RuleSettingMap[CIVCfgDoc::s_nMaxChannel];
    CIVCfgDoc::m_pShowState = new int[CIVCfgDoc::s_nMaxChannel];
    ZeroMemory(m_pShowState, sizeof(int)*CIVCfgDoc::s_nMaxChannel);
    CIVCfgDoc::m_pIsHaveStatistic = new BOOL[CIVCfgDoc::s_nMaxChannel];
    ZeroMemory(m_pIsHaveStatistic, sizeof(BOOL)*CIVCfgDoc::s_nMaxChannel);

    IIVCfgMgr* pIVCfgMgr = IIVCfgMgrFactory::GetIIVCfgMgr();
    IIVLiveViewerEx* pIVLiveViewerEx = IVLiveFactory::GetLiveViewerEx();

    int szChannel[_MaxAutoChannel] = {0};
    size_t nCount;
    pIVCfgMgr->GetAutoRunChannel(szChannel, nCount);
    StlHelper::Array2STL(szChannel, nCount, m_UseChannel);

    for (int i=0; i<s_nMaxChannel; ++i)
    {
        BOOL bUse = m_UseChannel.find(i) != m_UseChannel.end();

        RuleSettingMap& Map = m_pDoc[i];
        StlHelper::STLDeleteAssociate(Map); // 防止调多次

        int& nShowState = m_pShowState[i];
        pIVCfgMgr->GetDataShowState(i, nShowState);
        pIVLiveViewerEx->SetDataShowState(i, nShowState);
        
        if ( bUse )
        {
            g_IIVDeviceBase2->Use(i, true);
        }
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
            if ( !Iter.GetRule(pRuleSettings->Rule) )
            {
                delete pRuleSettings;
                continue;
            }

            if ( IVUtil::IsStatisticRule(pRuleSettings->Rule) )
            {
                m_pIsHaveStatistic[i] = TRUE;
            }

            if ( !Iter.GetAlarmOut(pRuleSettings->Alarm) )
            {
                pRuleSettings->Alarm = g_DefaultAlarmOutSettings;
            }

            if ( !Iter.GetSchedule(pRuleSettings->Sch) )
            {
                pRuleSettings->Sch = g_DefaultScheduleSettings;
            }

            Map[pID] = pRuleSettings;

            if ( bUse )
            {
                g_IIVDeviceBase2->Add(
                    i, 
                    pRuleSettings->Rule,
                    pRuleSettings->Sch,
                    pRuleSettings->Alarm );
                pIVLiveViewerEx->AddRule(i, pRuleSettings->Rule);
            }
        }
    }
}

void CIVCfgDoc::Unit()
{
    if ( m_pDoc == NULL )
    {
        return;
    }

    for (int i=0; i<s_nMaxChannel; ++i)
    {
        RuleSettingMap& Map = m_pDoc[i];
        StlHelper::STLDeleteAssociate(Map);
    }

    safeDeleteArray(CIVCfgDoc::m_pDoc);
    safeDeleteArray(CIVCfgDoc::m_pShowState);
    safeDeleteArray(CIVCfgDoc::m_pIsHaveStatistic);
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

    RuleSettingMap& Map = m_pDoc[nChannelID];
    RuleSettings* pRuleSettings = Map[pID];
    ASSERT(pRuleSettings);
    return (IV_RuleID*)pRuleSettings->Rule.ruleId;
}

void CIVCfgDoc::OnInitCameraTree( 
    int nChannelID, 
    HTREEITEM Item )
{
    RuleSettingMap& Map = m_pDoc[nChannelID];
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
//    RuleSettingMap& Map = m_pDoc[nChannelID];\
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
template<typename T, T CIVCfgDoc::RuleSettings::*P>
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
    RuleSettingMap& Map = m_pDoc[nChannelID];
    RuleSettingMap::iterator MapIter = Map.find(pID);
    if ( MapIter == Map.end() )
    {
        TRACE("GetIVRuleCfgXX No Found Iter!\n");
        return NULL;
    }
  
    return &(MapIter->second->*P);
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

template<
    typename T, T CIVCfgDoc::RuleSettings::*P,
    typename Tfn1, Tfn1 fn1,
    typename Tfn2, Tfn2 fn2>
void CIVCfgDoc::SetCfgToAllXX(const T& V)
{
    /**
    *@note  1. Set Cfg To XML and Memory
    */
    IIVCfgMgr* pIVCfgMgr = IIVCfgMgrFactory::GetIIVCfgMgr();
    for (int i=0; i<s_nMaxChannel; ++i)
    {
        IIVCfgMgr::IVVistor IVIter = pIVCfgMgr->Begin(i);
        for ( ; IVIter != pIVCfgMgr->End();
            IVIter=IVIter.Next() )
        {
            (IVIter.*fn1)(V);
        }

        RuleSettingMap& Map = m_pDoc[i];
        RuleSettingMap::iterator iter;
        for ( iter = Map.begin();
            iter!= Map.end();
            ++iter )
        {
            RuleSettings* pRuleSettings = iter->second;
            pRuleSettings->*P = V;
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
        RuleSettingMap& Map = m_pDoc[nChannelID];
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
    return GetIVRuleCfgXX<WPG_Rule, &RuleSettings::Rule>(Item);
}

void CIVRuleCfgDoc::AddRule( 
    const WPG_Rule& Rule, 
    HTREEITEM Item )
{
    void* pItemData = (void*)m_CameraTree.GetItemData(Item);
    int nChannelID = GetChannelFromItemData(pItemData);

    /**
    @note  1. Save To XML
    */
    IIVCfgMgr* pIVCfgMgr = IIVCfgMgrFactory::GetIIVCfgMgr();
    IIVCfgMgr::IVVistor Iter = pIVCfgMgr->Add(nChannelID, Rule);    
    const char* pID = Iter.GetIdentityID();
    if ( pID == NULL )
    {
        // log
        TRACE("***** CIVRuleCfgDoc::AddRule Failed!\n");
        pIVCfgMgr->Remove(nChannelID, Iter);
        return;
    }
    pIVCfgMgr->Apply();

    /**
    @note  2. Save To memory
    */
    RuleSettingMap& Map = m_pDoc[nChannelID];
    Map[pID] = new RuleSettings(Rule);
    if ( IVUtil::IsStatisticRule(Rule) )
    {
        m_pIsHaveStatistic[nChannelID] = TRUE;
    }

    /**
    @note  3. Update To Device
    */
    if ( IsIVChannel(nChannelID) )
    {
        g_IIVDeviceBase2->Add(nChannelID, Rule);

        IIVLiveViewerEx* pIVLiveViewerEx = IVLiveFactory::GetLiveViewerEx();
        pIVLiveViewerEx->AddRule(nChannelID, Rule);
    }

    /**
    @note  4. Insert Tree Item
    */
    CString strRuleName(Rule.ruleName);
    HTREEITEM hTmp = m_CameraTree.InsertItem(strRuleName, Item);
    void* pTmpItemData = MakeTreeItemData(nChannelID, pID);
    m_CameraTree.SetItemData(hTmp, (DWORD_PTR)pTmpItemData);

    /**
    @note  4. Do Trigger
    */
    DoTriggerTFun<&IRuleTrigger::OnRuleAdd>(nChannelID, pID, strRuleName);
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
    RuleSettingMap& Map = m_pDoc[nChannelID];
    RuleSettingMap::iterator MapIter = Map.find(pID);
    if ( MapIter == Map.end() )
    {
        // log
        TRACE("CIVRuleCfgDoc::RemoveRule No Found Iter!\n");
    }
    else
    {
        RuleSettings* pRuleSettings = MapIter->second;
        if ( IVUtil::IsStatisticRule(pRuleSettings->Rule) )
        {
            m_pIsHaveStatistic[nChannelID] = FALSE;
        }

        if ( IsIVChannel(nChannelID) )
        {
            g_IIVDeviceBase2->Remove(
                nChannelID,
                (IV_RuleID&)(pRuleSettings->Rule.ruleId));
            IIVLiveViewerEx* pIVLiveViewerEx = IVLiveFactory::GetLiveViewerEx();
            pIVLiveViewerEx->RemoveRule(nChannelID, pRuleSettings->Rule.ruleId);
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
            bFound = TRUE;
            pIVCfgMgr->Remove(nChannelID, Iter);
            pIVCfgMgr->Apply();
            break;
        }
    }

    if ( !bFound )
    {
        // log
        TRACE("CIVRuleCfgDoc::RemoveRule XML No Found Iter!\n");
    }

    /**
    @note  4. Do Trigger
    */
    CString strTmp;
    DoTriggerTFun<&IRuleTrigger::OnRuleRemove>(nChannelID, pID, strTmp);
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
    RuleSettingMap& Map = m_pDoc[nChannelID];
    RuleSettingMap::iterator MapIter = Map.find(pID);
    RuleSettings* pRuleSettings = NULL;
    if ( MapIter == Map.end() )
    {
        // log
        TRACE("CIVRuleCfgDoc::RemoveRule No Found Iter!\n");
        ASSERT(FALSE);
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
            bFound = TRUE;
            break;
        }
    }

    if ( !bFound )
    {
        // log
        TRACE("CIVRuleCfgDoc::RemoveRule XML No Found Iter!\n");
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

        // 再次判断用户是不是用我这边的引用,
        void Set(const WPG_Rule& p){if(&p!=&m_p->Rule) m_p->Rule=p;}
        void Set(const ScheduleSettings&p){if(&p!=&m_p->Sch) m_p->Sch=p;}
        void Set(const AlarmOutSettings&p){if(&p!=&m_p->Alarm) m_p->Alarm=p;}

        void ToDevice(int nChannelID, const WPG_Rule& p)
        {
            g_IIVDeviceBase2->ModifyRule(nChannelID, p);
        }
        void ToDevice(int nChannelID, const ScheduleSettings&p)
        {
            g_IIVDeviceBase2->ModifySchedule(
                nChannelID,
                (const IV_RuleID&)m_p->Rule.ruleId,
                p );
        }
        void ToDevice(int nChannelID, const AlarmOutSettings&p)
        {
            g_IIVDeviceBase2->ModifyAlarmOut(
                nChannelID,
                (const IV_RuleID&)m_p->Rule.ruleId,
                p );
        }
    private:
        RuleSettings* m_p;
    };

    /**
    @note  2. Update To memory
    */ 
    
    RuleSettingMap& Map = m_pDoc[nChannelID];
    RuleSettingMap::iterator MapIter = Map.find(pID);
    CXXX UpdateXXX(MapIter->second);
    if ( MapIter == Map.end() )
    {
        // log
        assert(false);
        TRACE("CIVRuleCfgDoc::RemoveRule No Found Iter!\n");
    }
    else
    {
        if ( !IsRef )
        {
            UpdateXXX.Set(V);
        }
        if ( g_IIVDeviceBase2->IsUse(nChannelID) )
        {
            UpdateXXX.ToDevice(nChannelID, V);
        }
    }

    /**
    @note  3. Update To XML
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
            (Iter.*fn)(V);
            pIVCfgMgr->Apply();
            bFound = TRUE;
            break;
        }
    }

    if ( !bFound )
    {
        // log
        TRACE("CIVRuleCfgDoc::UpdateRuleCfgXX XML No Found Iter!\n");
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
    const char* pIdentityID,
    CString& strRuleName )
{
    CIVCfgDoc::RuleTriggerList::iterator iter;
    for ( iter = m_RuleTrigger.begin();
         iter!= m_RuleTrigger.end();
         ++iter )
    {
        (*iter->*T)(nChannelID, pIdentityID, strRuleName);
    }
}

void CIVRuleCfgDoc::Use( 
    int nChannelID, 
    bool bEnable )
{
    /**
    *@note 1. Set is Enable to Device
    */
    g_IIVDeviceBase2->Use(nChannelID, bEnable);

    /**
    *@note 2. Update Device and Live Viewer, m_UseChannel
    */
    IIVLiveViewerEx* pIVLiveViewerEx = IVLiveFactory::GetLiveViewerEx();
    if ( bEnable )
    {
        /**
        *@note if Enable, Add That channel All Rule To Device and 
        *           set to live Viewer and update m_UseChannel
        */
        m_UseChannel.insert(nChannelID);

        RuleSettingMap& Map = m_pDoc[nChannelID];
        RuleSettingMap::iterator iter = Map.begin();
        for ( ; iter != Map.end(); ++iter)
        {
            RuleSettings* pRuleSettings = iter->second;
            g_IIVDeviceBase2->Add(
                nChannelID,
                pRuleSettings->Rule,
                pRuleSettings->Sch,
                pRuleSettings->Alarm );
            pIVLiveViewerEx->AddRule(
                nChannelID, 
                pRuleSettings->Rule);
        }
    }
    else
    {
        /**
        *@note if Disable, Clear All Rule To Live Viewer 
        *        and update m_UseChannel        
        */
        pIVLiveViewerEx->ClearAllRule(nChannelID);

        m_UseChannel.erase(nChannelID);
    }

    /**
    *@note 3. update To XML
    */
    int szChannel[_MaxAutoChannel] = {0};
    StlHelper::STL2Array(m_UseChannel, szChannel);

    IIVCfgMgr* pIVCfgMgr = IIVCfgMgrFactory::GetIIVCfgMgr();
    pIVCfgMgr->SetAutoRunChannel(szChannel, m_UseChannel.size());
    pIVCfgMgr->Apply();
}

bool CIVRuleCfgDoc::IsUse( int nChannelID )
{
    return m_UseChannel.find(nChannelID) != m_UseChannel.end();
}

int CIVRuleCfgDoc::GetShowState(int nChannelID)
{
    return m_pShowState[nChannelID];
}

void CIVRuleCfgDoc::SetShowState(int nChannelID, int nState)
{
    m_pShowState[nChannelID] = nState;
    IIVCfgMgr* pIVCfgMgr = IIVCfgMgrFactory::GetIIVCfgMgr();
    pIVCfgMgr->SetDataShowState(nChannelID, nState);
    pIVCfgMgr->Apply();
}

BOOL CIVRuleCfgDoc::IsRuleEnbale( HTREEITEM Item )
{
    /**
    @note  1. Get Info From Tree Item
    */
    void* pItemData = (void*)m_CameraTree.GetItemData(Item);
    const char* pID = (const char*)GetUserDataFromItemData(pItemData);
    int nChannelID = GetChannelFromItemData(pItemData);

    /**
    *@note  2. Get XX To memory
    */
    RuleSettingMap& Map = m_pDoc[nChannelID];
    RuleSettingMap::iterator MapIter = Map.find(pID);
    if ( MapIter == Map.end() )
    {
        TRACE("GetIVRuleCfgXX No Found Iter!\n");
        return NULL;
    }

    return MapIter->second->Rule.isEnabled;
}

void CIVRuleCfgDoc::EnableAllRule( int nChannelID, bool bEnable )
{
    /**
    @note  1. Update To Memory
    *         if Use IV, update to Device
    */
    RuleSettingMap& Map = m_pDoc[nChannelID];
    RuleSettingMap::iterator MapIter;
    bool bIsUseIV = IsUse(nChannelID);
    if ( bIsUseIV )
    {
        for ( MapIter = Map.begin();
              MapIter!= Map.end();
              ++MapIter )
        {
            WPG_Rule& Rule = MapIter->second->Rule; 
            if ( Rule.isEnabled == WPG_Bool(bEnable) )
            {
                continue;
            }

            Rule.isEnabled = bEnable;
            IV_RuleID& RuleID = (IV_RuleID&)Rule.ruleId;
            g_IIVDeviceBase2->EnableRule(nChannelID, RuleID, bEnable);
        }
    }
    else
    {
        for ( MapIter = Map.begin();
              MapIter!= Map.end();
              ++MapIter )
        {
            MapIter->second->Rule.isEnabled = bEnable;
        }
    }

    /**
    @note  2. Update To XML
    */
    BOOL bFound = FALSE;
    IIVCfgMgr* pIVCfgMgr = IIVCfgMgrFactory::GetIIVCfgMgr();
    for ( IIVCfgMgr::IVVistor Iter = pIVCfgMgr->Begin(nChannelID);
          Iter != pIVCfgMgr->End();
          Iter = Iter.Next() )
    {
        Iter.EnableRule(bEnable);
    }
    pIVCfgMgr->Apply();
}

BOOL CIVRuleCfgDoc::IsCanAddRule(
    int nChannelID,
    IVRuleType RuleType )
{
    if ( m_pIsHaveStatistic[nChannelID] )
    {
        if ( RuleType == IV_Statistic )
        {
            return FALSE;
        }
        return s_nMaxRuleNumByIVChannel-1 > m_pDoc[nChannelID].size();
    }
    else
    {
        if ( RuleType == IV_Statistic )
        {
            return s_nMaxRuleNumByIVChannel-1 > m_pDoc[nChannelID].size();
        }

        return s_nMaxRuleNumByIVChannel > m_pDoc[nChannelID].size();
    }
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
    return GetIVRuleCfgXX<AlarmOutSettings, &RuleSettings::Alarm>(Item);
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
        &RuleSettings::Alarm,
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
    return GetIVRuleCfgXX<ScheduleSettings, &RuleSettings::Sch>(Item); 
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
        &RuleSettings::Sch,
        ModifyScheduleFn,
        &IIVCfgMgr::IVVistor::ModifySchedule,
        DeviceModifyScheduleFn,
        &IIVDeviceBase2::ModifySchedule>(Sch);
}

//
// CIVScheduleCfgDoc
// }

// End of file




