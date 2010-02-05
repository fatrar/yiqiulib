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
        WhichMemu Which) = 0;
};




HTREEITEM InitCameraTree(CTreeCtrl& CameraTree);

void PopUpCameraMemu(CTreeCtrl& CameraTree, int nIndex, CWnd* pWnd, IUpdateMemu* pUpdateMemu);





#endif  // _IVUICOMMON_H_2010_


 // End of file



