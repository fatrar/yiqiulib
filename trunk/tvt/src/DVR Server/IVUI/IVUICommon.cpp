/*H***************************************************************************
 File            : IVUICommon.cpp
 Subsystem       : 
 Function Name(s): CIVUICommon
 Author          : YiQiu
 Date            : 2010-2-5  
 Time            : 16:19
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "StdAfx.h"
#include "IVUICommon.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

struct ItemAttribute 
{
    struct ItemInfo
    {
        ItemInfo(
            WhichMemu _Which,
            int _nChannelID) 
            : Which(_Which)
            , nChannelID(_nChannelID){}
        WhichMemu Which:8;
        BYTE nReserve1:8;
        WORD nChannelID:16;
    };

    ItemAttribute(
        WhichMemu Which,
        int nChannelID,
        const void* pUseData = NULL)
        : Info(Which, nChannelID)
        , pUseData(pUseData){}
    ~ItemAttribute()
    {
        //safeDelete(pUseData);
    }

    ItemInfo Info;
    const void* pUseData;
};

#define IV_Root_Name  _T("IV")
#define Channel_Name  _T("Ch%d")
//#define Max_Channel               4

//HTREEITEM InsertItem(LPCTSTR lpszItem, int nImage, int nSelectedImage,
//    HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST

namespace CameraTreeUtil
{

HTREEITEM InitCameraTree(
    CTreeCtrl& CameraTree,
    IInitCameraTree* pCameraTreeInitor,
    CStatic& Group,
    UINT GroupStringID,
    int nHeight )
{
    // Set Attribute
    Group.MoveWindow(
        TreeGroup_Start_X, TreeGroup_Start_Y, TreeGroup_Width,
        nHeight - 2*TreeGroup_Start_Y );
    CameraTree.MoveWindow(
        Tree_Start_X, Tree_Start_Y, Tree_Width,
        nHeight-Tree_Start_Y-2*Tree_Between_Group_Y);

    CameraTree.DeleteAllItems();
    CameraTree.SetItemHeight(30);
    
    // Init Item
    HTREEITEM Root = CameraTree.InsertItem(IV_Root_Name);
    //CameraTree.InsertItem(IV_Root_Name);
    ItemAttribute* pInfo = new ItemAttribute(IV_Tree_Root, -1, NULL);
    CameraTree.SetItemData(Root, (DWORD_PTR)pInfo);

    CString strCameraName;
    for ( DWORD i = 0; i< CIVCfgDoc::s_nMaxChannel; ++i )
    {
        strCameraName.Format(Channel_Name, i);
        HTREEITEM CurrentItem = CameraTree.InsertItem(strCameraName, Root);
        
        pInfo = new ItemAttribute(IV_Tree_Camera, i, NULL);
        CameraTree.SetItemData(CurrentItem, (DWORD_PTR)pInfo);

        if ( pCameraTreeInitor )
        {
            pCameraTreeInitor->OnInitCameraTree(i, CurrentItem);
        }
    }

    CameraTree.Expand(Root, TVE_EXPAND);
    return Root;
}

//HTREEITEM GetTreeClickItem(CTreeCtrl& CameraTree)
//{
//    CPoint pt; 
//    ::GetCursorPos(&pt); 
//    CameraTree.ScreenToClient(&pt); 
//    return CameraTree.HitTest(pt); 
//}

void PopUpCameraMemu(
    CTreeCtrl& CameraTree,
    int nIndex,
    CWnd* pWnd,
    IUpdateMemu* pUpdateMemu )
{
    CPoint pt, point; 
    ::GetCursorPos(&pt); 
    point = pt;
    CameraTree.ScreenToClient(&pt); 
    HTREEITEM selDevhItem = CameraTree.HitTest(pt); 
    if( selDevhItem == NULL )  
    {
        return;
    }

    CameraTree.SelectItem(selDevhItem); 
    ItemAttribute* pInfo = (ItemAttribute*)CameraTree.GetItemData(selDevhItem);
    CMenu menu; 
    switch ( pInfo->Info.Which )
    {
    case IV_Tree_Camera:
        menu.LoadMenu(IDR_Camera_Menu);
        break;
    case IV_Tree_Rule:
        menu.LoadMenu(IDR_Rule_Menu);
        break;
    case IV_Tree_Root:
    default:
        return;
    }
    
    CMenu* pPopup = menu.GetSubMenu(nIndex);
    if ( pPopup == NULL || 0 == pPopup->GetMenuItemCount() )
    {
        TRACE("Popup Menu Pass!");
        return;
    }

    if ( pUpdateMemu )
    {
        pUpdateMemu->OnUpdateMemu(
            pPopup,
            pInfo->Info.Which,
            pInfo->Info.nChannelID,
            pInfo->pUseData,
            selDevhItem );
    }

    pPopup->TrackPopupMenu(
        TPM_LEFTALIGN | TPM_RIGHTBUTTON,
        point.x,point.y,
        pWnd); 
}

static void TreeVisitDel(CTreeCtrl& tree, HTREEITEM hItem)   
{            
    if ( !tree.ItemHasChildren(hItem) )       
    {
        return;   
    }

    HTREEITEM ChildItem = tree.GetChildItem(hItem);            
    while(ChildItem!=NULL)       
    {   
        ItemAttribute* pInfo = (ItemAttribute*)tree.GetItemData(ChildItem);
        safeDelete(pInfo);

        TreeVisitDel(tree, ChildItem);  //递归遍历孩子节点       
        ChildItem = tree.GetNextItem(ChildItem, TVGN_NEXT);       
    }
}

void UnitCameraTree( CTreeCtrl& CameraTree )
{
    HTREEITEM Root = CameraTree.GetRootItem();
    ItemAttribute* pInfo = (ItemAttribute*)CameraTree.GetItemData(Root);
    safeDelete(pInfo);

    TreeVisitDel(CameraTree, Root);
}

void SendClickCameraTreeMes( 
    CTreeCtrl& CameraTree,
    IClickCameraTree* pClickCameraTree )
{
    CPoint pt;
    ::GetCursorPos(&pt);
    CameraTree.ScreenToClient(&pt);
    HTREEITEM selDevhItem = CameraTree.HitTest(pt);
    if( selDevhItem == NULL )
    {
        return;
    }
    ItemAttribute* pInfo = (ItemAttribute*)CameraTree.GetItemData(selDevhItem);
    if ( pClickCameraTree )
    {
        pClickCameraTree->OnClickCameraTree(
            pInfo->Info.Which,
            pInfo->Info.nChannelID,
            pInfo->pUseData,
            selDevhItem );
    }
}

void* MakeTreeItemData( 
    int nChannelID, const void* pUseData, WhichMemu Which )
{
    return new ItemAttribute(Which, nChannelID, pUseData);
}

void UnMakeTreeItemData( void* p )
{
    delete (ItemAttribute*)p;
}

const void* GetUserDataFromItemData( void* p )
{
    return ((ItemAttribute*)p)->pUseData;
}

int GetChannelFromItemData( void* p )
{
    return ((ItemAttribute*)p)->Info.nChannelID;
}


HTREEITEM OnDeleteCameraTreeItem(
    CTreeCtrl& CameraTree,
    int nChannelID,
    const void* pUseData )
{
    HTREEITEM Root = CameraTree.GetRootItem();
    if ( !CameraTree.ItemHasChildren(Root) )       
    {
        TRACE("OnDeleteCameraTreeItem Not Found");
        return NULL;   
    }

    HTREEITEM ChannelItem = CameraTree.GetChildItem(Root);            
    while (ChannelItem!=NULL)       
    {
        ItemAttribute* pInfo = (ItemAttribute*)CameraTree.GetItemData(ChannelItem);
        if ( pInfo->Info.nChannelID != nChannelID )
        {
            ChannelItem = CameraTree.GetNextItem(ChannelItem, TVGN_NEXT);
            continue;
        }

        HTREEITEM RuleItem = CameraTree.GetChildItem(ChannelItem);
        while (RuleItem!=NULL)       
        {
            ItemAttribute* pInfo = (ItemAttribute*)CameraTree.GetItemData(RuleItem);
            if ( pInfo->pUseData != pUseData )
            {
                RuleItem = CameraTree.GetNextItem(RuleItem, TVGN_NEXT);
                continue;
            }

            CameraTree.DeleteItem(RuleItem);
            TRACE("OnDeleteCameraTreeItem Found");
            return RuleItem;
        }
    }

    TRACE("OnDeleteCameraTreeItem Not Found");
    return NULL;
}

HTREEITEM OnAddCameraTreeItem( 
    CTreeCtrl& CameraTree,
    int nChannelID, 
    const void* pUseData,
    CString& strRuleName )
{
    HTREEITEM Root = CameraTree.GetRootItem();
    if ( !CameraTree.ItemHasChildren(Root) )       
    {
        TRACE("OnAddCameraTreeItem Not Found");
        return NULL;   
    }

    HTREEITEM ChannelItem = CameraTree.GetChildItem(Root);            
    while (ChannelItem!=NULL)       
    {
        ItemAttribute* pInfo = (ItemAttribute*)CameraTree.GetItemData(ChannelItem);
        if ( pInfo->Info.nChannelID != nChannelID )
        {
            ChannelItem = CameraTree.GetNextItem(ChannelItem, TVGN_NEXT);
            continue;
        }

        const char* pID = (const char*)pUseData;
        HTREEITEM RuleItem = CameraTree.InsertItem(strRuleName,ChannelItem);
        ItemAttribute* pTmp = new ItemAttribute(IV_Tree_Rule, nChannelID, pUseData);
        CameraTree.SetItemData(RuleItem, (DWORD_PTR)pTmp);
        TRACE("OnAddCameraTreeItem Found");
        return RuleItem;
    }

    TRACE("OnAddCameraTreeItem Not Found");
    return NULL;
}

}


// End of file
