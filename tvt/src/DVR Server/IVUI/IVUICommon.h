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



struct IUpdateMemu 
{
    enum WhichMemu
    {
        Root,     // 这个先预留，暂时不需要点击根节点的消息
        Camera,
        Rule,
    };

    virtual void OnUpdateMemu(
        CMenu* pMenu,
        WhichMemu Which,
        int nChannelID,
        void* pData,
        HTREEITEM Item ) = 0;
};

struct IInitCameraTree
{
    virtual void OnInitCameraTree(
        int nChannelID,
        HTREEITEM Item ) = 0;
};

struct ItemAttribute 
{
    struct ItemInfo
    {
        ItemInfo(
            IUpdateMemu::WhichMemu _Which,
            int _nChannelID) 
            : Which(_Which)
            , nChannelID(_nChannelID){}
        IUpdateMemu::WhichMemu Which:8;
        BYTE nReserve1:8;
        WORD nChannelID:16;
    };

    ItemAttribute(
        IUpdateMemu::WhichMemu Which,
        int nChannelID,
        void* pUseData = NULL)
        : Info(Which, nChannelID)
        , pUseData(pUseData){}
    ~ItemAttribute()
    {
        safeDelete(pUseData);
    }

    ItemInfo Info;
    void* pUseData;
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

HTREEITEM InitCameraTree(
    CTreeCtrl& CameraTree, 
    IInitCameraTree* pCameraTreeInitor,
    CStatic& Group,
    UINT GroupStringID,
    int nHeight );

void UnitCameraTree(CTreeCtrl& CameraTree);

HTREEITEM GetTreeClickItem(CTreeCtrl& CameraTree);

void PopUpCameraMemu(
    CTreeCtrl& CameraTree,
    int nIndex,
    CWnd* pWnd,
    IUpdateMemu* pUpdateMemu);



#endif  // _IVUICOMMON_H_2010_


 // End of file



