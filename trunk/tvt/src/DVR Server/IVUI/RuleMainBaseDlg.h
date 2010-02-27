/*H***************************************************************************
 File            : RuleMainBaseDlg.h
 Subsystem       : 
 Function Name(s): CRuleMainBaseDlg
 Author          : YiQiu
 Date            : 2010-2-24  
 Time            : 14:51
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _RULEMAINBASEDLG_H_2010_2
#define _RULEMAINBASEDLG_H_2010_2
#include "afxwin.h"



class CRuleMainBaseDlg : 
    public CDialog,
    public IVideoSend,
    public IVideoPlayCallBack,
    public IIVSimulationAlarmCallBack
{
    DECLARE_DYNAMIC(CRuleMainBaseDlg)
public:
    //enum OperatorMode
    //{
    //    Rule_Add_Mode,
    //    Rule_Edit_Mode,
    //};

    CRuleMainBaseDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CRuleMainBaseDlg();

    // Dialog Data
    enum { IDD = IDD_Add_Edit_Rule_Main };

public:
    void SetComomParm(int nChannelID, WPG_Rule* pRule, IVRuleType type);

    // IVideoSend
protected:
    virtual BOOL OnVideoSend(FRAMEBUFSTRUCT *bufStruct);

    // IIVSimulationAlarmCallBack
protected:
    virtual void OnAlarmCallBack(
        IVRuleType type,
        int nChannelID,
        const FILETIME* pTime);

    // IVideoPlayCallBack
protected:
    virtual BOOL OnVideoPlay(
        HDC dc,
        const tagRECT* rect,    // Õë¶ÔÆÁÄ»×ø±ê
        const FILETIME* pTime,
        HWND hwnd );

protected:
    virtual BOOL GatherUseSet(){return TRUE;};

protected:
    enum ToolMode
    {
        Nothing_Mode,
        Line_Mode,
        Zone_Mode,
        Use_All_Mode,
    };

    void UseToolCtrlMode(ToolMode Mode);

    void DrawToolChange(Windows::IDrawerGraphType type);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnClose();
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()

    virtual afx_msg void OnBnClickedOk();
    virtual afx_msg void OnBnClickedCancel();

    virtual afx_msg void OnBnClickedAdvBt();
    virtual afx_msg void OnBnClickedFilterBt();
    virtual afx_msg void OnBnClickedSimulationBt();

    virtual afx_msg void OnBnClickedLineCheck();
    virtual afx_msg void OnBnClickedZoneCheck();
    virtual afx_msg void OnBnClickedSelectCheck();

    afx_msg void OnBnClickedRightCheck();
    afx_msg void OnBnClickedLeftCheck();
    afx_msg void OnBnClickedBothCheck();

    afx_msg void OnBnClickedColourBt();

    virtual afx_msg void OnBnClickedPolygonCheck();
    virtual afx_msg void OnBnClickedRectangleCheck();

protected:
    enum 
    {
        Choose_Line,
        Choose_Rectangle,
        Choose_Polygon,

        PlayerWnd_ID = 0x8100,
        DrawWnd_ID = 0x8101,
    };

    CButton m_LineBT;
    CButton m_ZoneBT;
    CButton m_SelectBT;

    CButton m_LineRightBT;
    CButton m_LineLeftBT;
    CButton m_LineBothBT;

    CButton m_ColourBT;
    CButton m_AdvBT;
    CButton m_FilterBT;
    CButton m_SimulationBT;
    BOOL m_bUse;

    CButton m_RectangleBT;
    CButton m_PolygonBT;

    int m_nToolsChoose;

    CWnd m_PlayerWnd;
    CYUVSingleVideoPlayer m_Player;
    int m_nCurrentChan;  
    CRect m_Rect;
    Windows::IDrawer* m_Drawer;

    WPG_Rule* m_pRule;
    IVRuleType m_type;
private:
    Windows::CAlarmTextStatic m_AlarmOccurStatic;
    //CStatic m_AlarmOccurStatic;
};






#endif  // _RULEMAINBASEDLG_H_2010_


// End of file


