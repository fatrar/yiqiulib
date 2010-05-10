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

#define Channel_Name  _T("Ch%d")
size_t CIVCfgDoc::s_nDeviceNum  = Default_Device_Num;
size_t CIVCfgDoc::s_nMaxChannel = Default_Max_Channel;
size_t CIVCfgDoc::s_nIVChannelNumByDevice  = Default_IVChannelNum_By_Device;
size_t CIVCfgDoc::s_nMaxRuleNumByIVChannel = Default_Max_RuleNum_By_IVChannel;

using namespace CameraTreeUtil;

typedef bool (IIVCfgMgr::IVVistor::*ModifyRuleFn)(const WPG_Rule&);
typedef bool (IIVCfgMgr::IVVistor::*ModifyScheduleFn)(const ScheduleSettings&);
typedef bool (IIVCfgMgr::IVVistor::*ModifyAlarmOutFn)(const AlarmOutSettings&);

typedef BOOL (IIVDeviceBase2::*DeviceModifyScheduleFn)(int,const IV_RuleID&,const ScheduleSettings&);
typedef BOOL (IIVDeviceBase2::*DeviceModifyAlarmOutFn)(int,const IV_RuleID&,const AlarmOutSettings&);


CIVCfgDoc::RuleSettingMap* CIVCfgDoc::s_pDoc = NULL; 
int* CIVCfgDoc::s_pShowState = NULL;
BOOL* CIVCfgDoc::s_pIsHaveStatistic = NULL;

DWORD CIVCfgDoc::s_dwRelayCount = 0;
BOOL CIVCfgDoc::s_bTelphone = FALSE;

set<int> CIVCfgDoc::s_UseChannel;
CIVCfgDoc::RuleTriggerList CIVCfgDoc::s_RuleTrigger;

CImageList CIVCfgDoc::s_CameraImageList;
CString* CIVCfgDoc::s_pChannelName = NULL;


void CIVCfgDoc::Init()
{
    CIVCfgDoc::s_pDoc = new CIVCfgDoc::RuleSettingMap[s_nMaxChannel];
    CIVCfgDoc::s_pShowState = new int[s_nMaxChannel];
    ZeroMemory(s_pShowState, sizeof(int)*s_nMaxChannel);
    CIVCfgDoc::s_pIsHaveStatistic = new BOOL[s_nMaxChannel];
    ZeroMemory(s_pIsHaveStatistic, sizeof(BOOL)*s_nMaxChannel);

    s_pChannelName = new CString[s_nMaxChannel];
    for ( size_t i = 0; i<s_nMaxChannel; ++i )
    {
        s_pChannelName[i].Format(Channel_Name, i+1);
    }

 BOOL bRc;
 DWORD dwErr; //21
 bRc = s_CameraImageList.Create(25, 25, 
     /*ILC_COLORDDB*/ILC_COLOR32/*| ILC_MASK*/,
        3, 1);  
   // s_CameraImageList.Add( hicon );
    dwErr =  GetLastError();
    s_CameraImageList.Add( AfxGetApp()->LoadIcon(IDI_Camera1));
    s_CameraImageList.Add( AfxGetApp()->LoadIcon(IDI_Camera2));
    s_CameraImageList.Add( AfxGetApp()->LoadIcon(IDI_Camera1));
    int nImageCount = s_CameraImageList.GetImageCount();

    // bRc = s_CameraImageList.Create(IDI_Camera1, 32, 1, 0);
    //s_CameraImageList.Create(IDI_Camera2, 32, 1, 0);

    IIVCfgMgr* pIVCfgMgr = IIVCfgMgrFactory::GetIIVCfgMgr();
    IIVLiveViewerEx* pIVLiveViewerEx = IVLiveFactory::GetLiveViewerEx();

    int szChannel[_MaxAutoChannel] = {0};
    size_t nCount;
    pIVCfgMgr->GetAutoRunChannel(szChannel, nCount);
    StlHelper::Array2STL(szChannel, nCount, s_UseChannel);

    for (int i=0; i<s_nMaxChannel; ++i)
    {
        BOOL bUse = s_UseChannel.find(i) != s_UseChannel.end();

        RuleSettingMap& Map = s_pDoc[i];
        StlHelper::STLDeleteAssociate(Map); // ��ֹ�����

        int& nShowState = s_pShowState[i];
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
                s_pIsHaveStatistic[i] = TRUE;
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
    if ( s_pDoc == NULL )
    {
        return;
    }

    for (int i=0; i<s_nMaxChannel; ++i)
    {
        RuleSettingMap& Map = s_pDoc[i];
        StlHelper::STLDeleteAssociate(Map);
    }

    safeDeleteArray(s_pDoc);
    safeDeleteArray(s_pShowState);
    safeDeleteArray(s_pIsHaveStatistic);
    safeDeleteArray(s_pChannelName);
}

void CIVCfgDoc::SetChannelName( 
    int nChannelID,
    const char* pChannelName )
{
    s_pChannelName[nChannelID] = pChannelName;
}

const CString& CIVCfgDoc::GetChannelName( int nChannelID )
{
    return s_pChannelName[nChannelID];
}

void CIVCfgDoc::RegisterRuleTrigger( IRuleTrigger* pRuleTrigger )
{
    s_RuleTrigger.push_back(pRuleTrigger);
}

BOOL CIVCfgDoc::IsIVChannel( int nChannelID )
{
    return  s_UseChannel.find(nChannelID) != s_UseChannel.end();
}


const IV_RuleID* CIVCfgDoc::GetRuleID(
    HTREEITEM Item )
{
    void* pItemData = (void*)m_CameraTree.GetItemData(Item); 
    const char* pID = (const char*)GetUserDataFromItemData(pItemData);
    int nChannelID = GetChannelFromItemData(pItemData);

    RuleSettingMap& Map = s_pDoc[nChannelID];
    RuleSettings* pRuleSettings = Map[pID];
    ASSERT(pRuleSettings);
    return (IV_RuleID*)pRuleSettings->Rule.ruleId;
}

void CIVCfgDoc::OnInitCameraTree( 
    int nChannelID, 
    HTREEITEM Item )
{
    RuleSettingMap& Map = s_pDoc[nChannelID];
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
//    RuleSettingMap& Map = m_pDoc[nChannelID];\
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
    RuleSettingMap& Map = s_pDoc[nChannelID];
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
    for (size_t i=0; i<s_nMaxChannel; ++i)
    {
        IIVCfgMgr::IVVistor IVIter = pIVCfgMgr->Begin(int(i));
        for ( ; IVIter != pIVCfgMgr->End();
            IVIter=IVIter.Next() )
        {
            (IVIter.*fn1)(V);
        }

        RuleSettingMap& Map = s_pDoc[i];
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
    for ( ChIter = s_UseChannel.begin();
          ChIter!= s_UseChannel.end();
          ++ChIter )
    {
        int nChannelID = *ChIter;
        RuleSettingMap& Map = s_pDoc[nChannelID];
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
    RuleSettingMap& Map = s_pDoc[nChannelID];
    Map[pID] = new RuleSettings(Rule);
    if ( IVUtil::IsStatisticRule(Rule) )
    {
        s_pIsHaveStatistic[nChannelID] = TRUE;
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
    RuleSettingMap& Map = s_pDoc[nChannelID];
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
            s_pIsHaveStatistic[nChannelID] = FALSE;
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
    RuleSettingMap& Map = s_pDoc[nChannelID];
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

        // �ٴ��ж��û��ǲ���������ߵ�����,
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
    
    RuleSettingMap& Map = s_pDoc[nChannelID];
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
    for ( iter = s_RuleTrigger.begin();
          iter!= s_RuleTrigger.end();
          ++iter )
    {
        (*iter->*T)(nChannelID, pIdentityID, strRuleName);
    }
}

BOOL CIVRuleCfgDoc::Use( 
    int nChannelID, 
    bool bEnable )
{
    if ( s_pDoc == NULL ||
         g_IIVDeviceBase2 == NULL )
    {
        return FALSE;
    }

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
        s_UseChannel.insert(nChannelID);

        RuleSettingMap& Map = s_pDoc[nChannelID];
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

        s_UseChannel.erase(nChannelID);
    }

    /**
    *@note 3. update To XML
    */
    int szChannel[_MaxAutoChannel] = {0};
    StlHelper::STL2Array(s_UseChannel, szChannel);

    IIVCfgMgr* pIVCfgMgr = IIVCfgMgrFactory::GetIIVCfgMgr();
    pIVCfgMgr->SetAutoRunChannel(szChannel, s_UseChannel.size());
    pIVCfgMgr->Apply();


    CIVCfgDoc::RuleTriggerList::iterator iter;
    for ( iter = s_RuleTrigger.begin();
          iter!= s_RuleTrigger.end();
          ++iter )
    {
        (*iter)->OnUseIV(nChannelID, bEnable);
    }
    return TRUE;
}

bool CIVRuleCfgDoc::IsUse( int nChannelID )
{
    return s_UseChannel.find(nChannelID) != s_UseChannel.end();
}

int CIVRuleCfgDoc::GetShowState(int nChannelID)
{
    return s_pShowState[nChannelID];
}

void CIVRuleCfgDoc::SetShowState(int nChannelID, int nState)
{
    s_pShowState[nChannelID] = nState;
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
    RuleSettingMap& Map = s_pDoc[nChannelID];
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
    RuleSettingMap& Map = s_pDoc[nChannelID];
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
    if ( s_pIsHaveStatistic[nChannelID] )
    {
        if ( RuleType == IV_Statistic )
        {
            return FALSE;
        }
        return s_nMaxRuleNumByIVChannel-1 > s_pDoc[nChannelID].size();
    }
    else
    {
        if ( RuleType == IV_Statistic )
        {
            return s_nMaxRuleNumByIVChannel-1 > s_pDoc[nChannelID].size();
        }

        return s_nMaxRuleNumByIVChannel > s_pDoc[nChannelID].size();
    }
}

BOOL CIVRuleCfgDoc::ModifyRuleName(
    HTREEITEM Item, 
    const _bstr_t& bstrRuleName )
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
    RuleSettingMap& Map = s_pDoc[nChannelID];
    RuleSettingMap::iterator MapIter = Map.find(pID);
    if ( MapIter == Map.end() )
    {
        TRACE("GetIVRuleCfgXX No Found Iter!\n");
        return FALSE;
    }

    strcpy(MapIter->second->Rule.ruleName, (char*)bstrRuleName);

    BOOL bFound = FALSE;
    IIVCfgMgr* pIVCfgMgr = IIVCfgMgrFactory::GetIIVCfgMgr();
    for ( IIVCfgMgr::IVVistor Iter = pIVCfgMgr->Begin(nChannelID);
          Iter != pIVCfgMgr->End();
          Iter = Iter.Next() )
    {
        const char* pTmpID = Iter.GetIdentityID();
        if ( pTmpID == pID )
        {
            Iter.ModifyRuleName((char*)bstrRuleName);
            pIVCfgMgr->Apply();
            bFound = TRUE;
            break;
        }
    }

    return bFound;
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




