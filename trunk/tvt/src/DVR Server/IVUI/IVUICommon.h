/*H***************************************************************************
 File            : IVUICommon.h
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
#ifndef _IVUICOMMON_H_2010_2
#define _IVUICOMMON_H_2010_2


enum WhichMemu
{
    IV_Tree_Root,     // 这个先预留，暂时不需要点击根节点的消息
    IV_Tree_Camera,
    IV_Tree_Rule,
};

struct IUpdateMemu 
{
    virtual void OnUpdateMemu(
        CMenu* pMenu,
        WhichMemu Which,
        int nChannelID,
        const void* pData,
        HTREEITEM Item ) = 0;
};

struct IInitCameraTree
{
    virtual void OnInitCameraTree(
        int nChannelID,
        HTREEITEM Item ) = 0;
};

struct IClickCameraTree
{
    virtual void OnClickCameraTree(
        WhichMemu Which,
        int nChannelID,
        const void* pData,
        HTREEITEM Item ) = 0;
};

enum CameraTreeAttibute
{
    TreeGroup_Start_X = 7,
    TreeGroup_Start_Y = 7,
    TreeGroup_Width   = 150,

    Tree_Between_BT_Y = 5,
    Tree_BT_Height = 25,

    Tree_Between_Group_X = 7,
    Tree_Between_Group_Y = 7,

    Tree_Start_X = TreeGroup_Start_X + Tree_Between_Group_X,
    Tree_Start_Y = TreeGroup_Start_Y + 2*Tree_Between_BT_Y+Tree_BT_Height,
    Tree_Width   = TreeGroup_Width - 2*Tree_Between_Group_X,

    CameraCtrl_Width = TreeGroup_Width + 2*TreeGroup_Start_X,
};

enum TreeMenuAttibute
{
    Menu_Copy_Index = 4,
    Menu_Paste_Index = 5,
    Menu_Paste_All_Index = 7,  

    Menu_Disbale = MF_BYCOMMAND | MF_DISABLED | MF_GRAYED,
    Menu_Enbale = MF_BYCOMMAND | MF_ENABLED,
};

namespace CameraTreeUtil
{


HTREEITEM InitCameraTree(
    CTreeCtrl& CameraTree, 
    IInitCameraTree* pCameraTreeInitor,
    CStatic& Group,
    UINT GroupStringID,
    int nHeight );

void UnitCameraTree(CTreeCtrl& CameraTree);

void SendClickCameraTreeMes(
    CTreeCtrl& CameraTree,
    IClickCameraTree* pClickCameraTree);

void PopUpCameraMemu(
    CTreeCtrl& CameraTree,
    int nIndex,
    CWnd* pWnd,
    IUpdateMemu* pUpdateMemu);

void* MakeTreeItemData(
    int nChannelID,
    const void* pUseData,
    WhichMemu Which = IV_Tree_Rule );

void UnMakeTreeItemData(void* p);

const void* GetUserDataFromItemData(void* p);

int GetChannelFromItemData( void* p );

HTREEITEM OnDeleteCameraTreeItem(
    CTreeCtrl& CameraTree,
    int nChannelID, 
    const void* pUseData );

HTREEITEM OnAddCameraTreeItem(
    CTreeCtrl& CameraTree,
    int nChannelID, 
    const void* pUseData );


}


#endif  // _IVUICOMMON_H_2010_


 // End of file



