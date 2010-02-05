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
#define Channel_Item_Data  3721

//HTREEITEM InsertItem(LPCTSTR lpszItem, int nImage, int nSelectedImage,
//    HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST

HTREEITEM InitCameraTree(CTreeCtrl& CameraTree)
{
    CameraTree.DeleteAllItems();

    CameraTree.SetItemHeight(30);
    CString strCameraName;
    HTREEITEM Root = CameraTree.InsertItem(IV_Root_Name);
    for ( int i = 0; i< 4; ++i )
    {
        strCameraName.Format(Channel_Name, i);
        HTREEITEM CurrentItem = CameraTree.InsertItem(strCameraName, Root);
        CameraTree.SetItemData(CurrentItem, Channel_Item_Data);
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

    IUpdateMemu::WhichMemu Which;
    CameraTree.SelectItem(selDevhItem); 
    DWORD dwData = (DWORD)CameraTree.GetItemData(selDevhItem);
    CMenu menu; 
    if ( dwData == Channel_Item_Data )
    {
        menu.LoadMenu(IDR_Camera_Menu);
        Which = IUpdateMemu::Camera;
    }
    else if (dwData==0)
    {
        TRACE(_T("Click Root, Pass!"));
        return;
    }
    else
    {
        menu.LoadMenu(IDR_Rule_Menu);
        Which = IUpdateMemu::Rule;
    }
    
    CMenu* pPopup = menu.GetSubMenu(nIndex); 
    if ( pPopup == NULL || 0 == pPopup->GetMenuItemCount() )
    {
        TRACE(_T("Popup Menu Pass!"));
        return;
    }

    if ( pUpdateMemu )
    {
        pUpdateMemu->OnUpdateMemu(pPopup, Which);
    }

    pPopup->TrackPopupMenu(
        TPM_LEFTALIGN | TPM_RIGHTBUTTON,
        point.x,point.y,
        pWnd); 
}











// End of file
