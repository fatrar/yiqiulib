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
        void* pData ) = 0;
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
    Tree_Start_X = 30,
    Tree_Start_Y = 80,
    Tree_Width   = 200,
};

HTREEITEM InitCameraTree(
    CTreeCtrl& CameraTree, 
    IInitCameraTree* pCameraTreeInitor);

void UnitCameraTree(CTreeCtrl& CameraTree);

void PopUpCameraMemu(
    CTreeCtrl& CameraTree,
    int nIndex,
    CWnd* pWnd,
    IUpdateMemu* pUpdateMemu);





#endif  // _IVUICOMMON_H_2010_


 // End of file



