#pragma once
#include "afxcmn.h"
#include "IVUICommon.h"
#include "afxwin.h"
#include "SingleVideoPlayer.h"
#include "..\devicecontrol\IIVDevice.h"
// CIVRuleDlg dialog

class CIVRuleDlg :
    public CDialog,
    public IUpdateMemu,
    public IInitCameraTree,
    public IVideoSend
{
	DECLARE_DYNAMIC(CIVRuleDlg)

public:
	CIVRuleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIVRuleDlg();

// Dialog Data
	enum { IDD = IDD_IVRULE };

public:
    BOOL Init(CWnd* pWnd, const CRect& Rect);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
    afx_msg void OnClose();
    afx_msg void OnNMRclickRuleCameraTree(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnRuleEnableallrule();
    afx_msg void OnRuleDisableallrule();
    afx_msg void OnRuleUse();
    afx_msg void OnRuleShowobject();
    afx_msg void OnRuleShowtrace();
    afx_msg void OnRuleShowobjtrace();
    afx_msg void OnRuleNewrule();
	DECLARE_MESSAGE_MAP()

protected:
    virtual void OnUpdateMemu(
        CMenu* pMenu,
        WhichMemu Which,
        int nChannelID,
        void* pData);

    virtual void OnInitCameraTree(
        int nChannelID,
        HTREEITEM Item );

    // IVideoSend
protected:
    virtual BOOL OnVideoSend(FRAMEBUFSTRUCT *bufStruct);

protected:
    enum
    {
        X_Offset = 30,
        Y_Offset = 30,
        PlayerWnd_ID = 0x8100,

        Invaild_ChannelID = -1,
        Max_Channel = 4,
    };

    typedef map<string, WPG_Rule*> ChannelRule;

    void LoadCfgDataToBuf();

private:
    CTreeCtrl m_CameraTree;
    CStatic m_RuleGroup;

    CWnd m_PlayerWnd;
    CYUVSingleVideoPlayer m_Player;
    int m_nCurrentChan;

    ChannelRule m_AllRule[Max_Channel];
};