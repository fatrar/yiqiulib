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



#define IV_Root_Name  _T("IV")
#define Channel_Name  _T("Ch%d")
#define Max_Channel               4

//HTREEITEM InsertItem(LPCTSTR lpszItem, int nImage, int nSelectedImage,
//    HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST

HTREEITEM InitCameraTree(
    CTreeCtrl& CameraTree,
    IInitCameraTree* pCameraTreeInitor )
{
    // Set Attribute
    CameraTree.DeleteAllItems();
    CameraTree.SetItemHeight(30);
    
    // Init Item
    HTREEITEM Root = CameraTree.InsertItem(IV_Root_Name);
    ItemAttribute* pInfo = new ItemAttribute(IUpdateMemu::Root, -1, NULL);
    CameraTree.SetItemData(Root, (DWORD_PTR)pInfo);

    CString strCameraName;
    for ( DWORD i = 0; i< Max_Channel; ++i )
    {
        strCameraName.Format(Channel_Name, i);
        HTREEITEM CurrentItem = CameraTree.InsertItem(strCameraName, Root);
        
        pInfo = new ItemAttribute(IUpdateMemu::Camera, i, NULL);
        CameraTree.SetItemData(CurrentItem, (DWORD_PTR)pInfo);

        if ( pCameraTreeInitor )
        {
            pCameraTreeInitor->OnInitCameraTree(i, CurrentItem);
        }
    }

    CameraTree.Expand(Root, TVE_EXPAND);
    return Root;
}

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
    case IUpdateMemu::Camera:
        menu.LoadMenu(IDR_Camera_Menu);
    case IUpdateMemu::Rule:
        menu.LoadMenu(IDR_Rule_Menu);
    case IUpdateMemu::Root:
    default:
        return;
    }
    
    CMenu* pPopup = menu.GetSubMenu(nIndex); 
    if ( pPopup == NULL || 0 == pPopup->GetMenuItemCount() )
    {
        TRACE(_T("Popup Menu Pass!"));
        return;
    }

    if ( pUpdateMemu )
    {
        pUpdateMemu->OnUpdateMemu(
            pPopup,
            pInfo->Info.Which,
            pInfo->Info.nChannelID,
            pInfo->pUseData );
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

        TreeVisitDel(tree, ChildItem);  //�ݹ�������ӽڵ�       
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










// End of file
