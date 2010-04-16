#pragma once
#include "afxcmn.h"
#include "IVUICommon.h"
#include "afxwin.h"
#include "..\devicecontrol\IIVDevice.h"
// CIVRuleDlg dialog

class CIVRuleDlg :
    public CDialog,
    public IUpdateMemu,
    public IInitCameraTree,
    public IClickCameraTree,
    public IVideoSend,
    public CIVRuleCfgDoc
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
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnDestroy();
    afx_msg void OnPaint();
    afx_msg void OnNMRclickRuleCameraTree(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMClickRuleCameraTree(NMHDR *pNMHDR, LRESULT *pResult);
    
    afx_msg void OnRuleEnableallrule();
    afx_msg void OnRuleDisableallrule();
    afx_msg void OnRuleUse();
    afx_msg void OnRuleShowobject();
    afx_msg void OnRuleShowtrace();
    afx_msg void OnRuleShowobjtrace();
    afx_msg void OnRuleNewrule();

    afx_msg void OnRuleDeleterule();
    afx_msg void OnRuleEditrule();
    afx_msg void OnRuleRenamerule();
    afx_msg void OnRuleEnablerule();
    afx_msg void OnRuleDisablerule();
	DECLARE_MESSAGE_MAP()

    // IUpdateMemu
protected:
    virtual void OnUpdateMemu(
        CMenu* pMenu,
        WhichMemu Which,
        int nChannelID,
        const void* pData,
        HTREEITEM Item );

    // IUpdateMemu
protected:
    virtual void OnInitCameraTree(
        int nChannelID,
        HTREEITEM Item );

    // IClickCameraTree
protected:
    virtual void OnClickCameraTree(
        WhichMemu Which,
        int nChannelID,
        const void* pData,
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
    };

    void UpdateLiveChannel(int nChannelID);

    void UpdateCameraMenu(CMenu* pMenu);

    void UpdateRuleMenu(CMenu* pMenu);
private:
    //CTreeCtrl m_CameraTree;
    //CStatic m_RuleGroup;

    CWnd m_PlayerWnd;
    CYUVSingleVideoPlayerEx m_Player;
    int m_nCurrentChan;
    HTREEITEM m_ClickItem;  

    // 这个是为了防止上层重复调用ShowWindow，我用这个做记录
    BOOL m_IsShow;
};
