// FilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IVUI.h"
#include "FilterDlg.h"


// CFilterDlg dialog

IMPLEMENT_DYNAMIC(CFilterDlg, CDialog)

CFilterDlg::CFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFilterDlg::IDD, pParent)
    , m_strFilterInfo(_T(""))
    , m_strMultiplier(_T(""))
    , m_nMultiplierEdit(0)
{
    m_pDrawContainer = Windows::CreateDrawContainer();
    m_szRectangleDrawer[0] = m_pDrawContainer->Add(Windows::IDrawer_Rectangle);
    m_szRectangleDrawer[1] = m_pDrawContainer->Add(Windows::IDrawer_Rectangle);
    m_szRectangleDrawer[2] = m_pDrawContainer->Add(Windows::IDrawer_Rectangle);
    m_szRectangleDrawer[3] = m_pDrawContainer->Add(Windows::IDrawer_Rectangle);
}

CFilterDlg::~CFilterDlg()
{
    Windows::DestoryDrawContainer(m_pDrawContainer);
}

void CFilterDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_list);
    DDX_Text(pDX, IDC_EDIT1, m_strFilterInfo);
    DDX_Control(pDX, IDC_FILTER_ON, m_FiterOnBt);
    DDX_Control(pDX, IDC_FILTER_OFF, m_FiterOffBt);
    DDX_Text(pDX, IDC_Multiplier, m_strMultiplier);
    DDX_Text(pDX, IDC_Muti_Edit, m_nMultiplierEdit);
	DDV_MinMaxFloat(pDX, m_nMultiplierEdit, 0.01f, 100.0f);
}


BEGIN_MESSAGE_MAP(CFilterDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CFilterDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CFilterDlg::OnBnClickedCancel)
    ON_NOTIFY(NM_CLICK, IDC_LIST1, &CFilterDlg::OnNMClickList1)
    ON_BN_CLICKED(IDC_FILTER_ON, &CFilterDlg::OnBnClickedFilterOn)
    ON_BN_CLICKED(IDC_FILTER_OFF, &CFilterDlg::OnBnClickedFilterOff)
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CFilterDlg message handlers

void CFilterDlg::OnBnClickedOk()
{
    UpdateData();

    CRect Rect;
    m_pDrawContainer->GetClientRect(&Rect);
    CPoint szPointBuf[4];

    //
    // 1. Get miniFilter
    //
    WPG_MinMaxFilter& miniFilter = m_pRule->minimumFilter;
    miniFilter.useFilter = (m_list.GetItemText(0, 1) == m_strStateOn);
    m_szRectangleDrawer[0]->GetUserInput(szPointBuf, 4);
    IVUtil::PointListToWPGRect(szPointBuf, Rect, miniFilter.nearRectangle);
    m_szRectangleDrawer[1]->GetUserInput(szPointBuf, 4);
    IVUtil::PointListToWPGRect(szPointBuf, Rect, miniFilter.farRectangle);

    //
    // 2. Get maxiFilter
    //
    WPG_MinMaxFilter& maxiFilter = m_pRule->maximumFilter;
    maxiFilter.useFilter = (m_list.GetItemText(1, 1) == m_strStateOn);
    m_szRectangleDrawer[2]->GetUserInput(szPointBuf, 4);
    IVUtil::PointListToWPGRect(szPointBuf, Rect, miniFilter.nearRectangle);
    m_szRectangleDrawer[3]->GetUserInput(szPointBuf, 4);
    IVUtil::PointListToWPGRect(szPointBuf, Rect, miniFilter.farRectangle);

    //
    // 3. Get maxSizeChangeFilter
    //
    m_pRule->maxSizeChangeFilter.useFilter = (m_list.GetItemText(2, 1) == m_strStateOn);
    m_pRule->maxSizeChangeFilter.maxSizeRatio = m_nMultiplierEdit;

    //
    // 4. Get ShapeDirectionFilter
    //
    m_pRule->useShapeDirectionFilter = (m_list.GetItemText(3, 1) == m_strStateOn);

    //
    // 5. Free Live call back
    //
    if ( g_IIVDeviceBase2 )
    {
        g_IIVDeviceBase2->UnRegisterLiveDataCallBack(m_nCurrentChan, this);
    }
    OnOK();
}

void CFilterDlg::OnBnClickedCancel()
{
    if ( g_IIVDeviceBase2 )
    {
        g_IIVDeviceBase2->UnRegisterLiveDataCallBack(m_nCurrentChan, this);
    }
    OnCancel();
}

BOOL CFilterDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    //
    // 1. Init Filter List
    //
    CString strTmp; 
    strTmp.LoadString(IDS_Filter_Head_Type);
    m_list.InsertColumn(0, strTmp, LVCFMT_LEFT, 200);
    strTmp.LoadString(IDS_Filter_Head_State);
    m_list.InsertColumn(1, strTmp, LVCFMT_LEFT, 100);
    m_list.SetExtendedStyle(m_list.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
    for (int i = 0; i< Filter_Number; ++i)
    {
        strTmp.LoadString(Filter_Name_Start+i);
        m_list.InsertItem(i, strTmp);
        //m_list.SetItemText(i, 1, m_strStateOff);
    }
    m_list.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

    //
    // 2. Init On And off Button and Default String
    //
    m_strStateOn.LoadString(IDS_Filter_On);
    m_strStateOff.LoadString(IDS_Filter_Off);

    m_strFilterInfo.LoadString(IDS_Filter_MaxObj_Info);
    m_FiterOnBt.SetCheck(BST_UNCHECKED);
    m_FiterOffBt.SetCheck(BST_CHECKED);

    m_strMultiplier.LoadString(IDS_Filter_Multiplier);
    UpdateData(FALSE);

    //
    // 3. Init Media And Drawer
    //
    int x = 12, y = 215;
    m_Rect.SetRect(x,y, x+352,y+288);
    m_Player.InitDirectDraw(
        this->m_hWnd, 352, 288, &m_Rect);
    m_Player.SetVideoPlayCallback(this, m_nCurrentChan);
    if ( g_IIVDeviceBase2 )
    {
        g_IIVDeviceBase2->RegisterLiveDataCallBack(m_nCurrentChan, this);
    }

    m_pDrawContainer->Create(
        NULL,NULL,
        WS_VISIBLE|WS_CHILD,
        m_Rect, this, DrawWnd_ID);

    //
    // 4 .Init List Control Default State
    //
    InitByRule();
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


#define ShowMultiplier(s)  \
    GetDlgItem(IDC_Multiplier)->ShowWindow(s);\
    GetDlgItem(IDC_Muti_Edit)->ShowWindow(s)

#define EnableMultiplier(s) \
    GetDlgItem(IDC_Multiplier)->EnableWindow(s);\
    GetDlgItem(IDC_Muti_Edit)->EnableWindow(s)

#define DisableAllDrawer() \
    m_szRectangleDrawer[0]->Enable(FALSE); \
    m_szRectangleDrawer[1]->Enable(FALSE); \
    m_szRectangleDrawer[2]->Enable(FALSE); \
    m_szRectangleDrawer[3]->Enable(FALSE)

// 默认minimumFilter为红色，maximumFilterfiter为蓝色
void CFilterDlg::InitByRule()
{
    CString& strTmp = m_pRule->minimumFilter.useFilter ? m_strStateOn : m_strStateOff;
    m_list.SetItemText(0, 1, strTmp);
    strTmp = m_pRule->maximumFilter.useFilter ? m_strStateOn : m_strStateOff;
    m_list.SetItemText(1, 1, strTmp);
    strTmp = m_pRule->maxSizeChangeFilter.useFilter ? m_strStateOn : m_strStateOff;
    m_list.SetItemText(2, 1, strTmp);
    strTmp = m_pRule->useShapeDirectionFilter ? m_strStateOn : m_strStateOff;
    m_list.SetItemText(3, 1, strTmp);
 
    m_nMultiplierEdit = m_pRule->maxSizeChangeFilter.maxSizeRatio;

    CRect Rect;
    m_pDrawContainer->GetClientRect(&Rect);

    CPoint szPointBuf[4];
    IVUtil::WPGRectToPointList(
        m_pRule->minimumFilter.farRectangle, Rect, szPointBuf);
    m_szRectangleDrawer[0]->SetDefault(szPointBuf, 4);
    m_szRectangleDrawer[0]->SetLineColour(RGB(255,0,0));

    IVUtil::WPGRectToPointList(
        m_pRule->minimumFilter.nearRectangle, Rect, szPointBuf);
    m_szRectangleDrawer[1]->SetDefault(szPointBuf, 4);
    m_szRectangleDrawer[1]->SetLineColour(RGB(0,255,0));

    IVUtil::WPGRectToPointList(
        m_pRule->maximumFilter.farRectangle, Rect, szPointBuf);
    m_szRectangleDrawer[2]->SetDefault(szPointBuf, 4);
    m_szRectangleDrawer[2]->SetLineColour(RGB(255,0,0));

    IVUtil::WPGRectToPointList(
        m_pRule->maximumFilter.nearRectangle, Rect, szPointBuf);
    m_szRectangleDrawer[3]->SetDefault(szPointBuf, 4);
    m_szRectangleDrawer[3]->SetLineColour(RGB(0,255,0));

    DoClickOne(0);
}

void CFilterDlg::DoClickOne(int nItem)
{
    m_nSelectRow = nItem;
    //
    // 1. Update Filter On or Off
    //
    BOOL bOn = m_list.GetItemText(nItem, 1) == m_strStateOn;
    if ( bOn )
    {
        m_FiterOnBt.SetCheck(BST_CHECKED);
        m_FiterOffBt.SetCheck(BST_UNCHECKED);
        if ( nItem == 2 )
        {
            GetDlgItem(IDC_EDIT2)->EnableWindow();
        }

        //
        // Update Draw Tool and Multiplier
        //
        switch (nItem)
        {
        case 0:
            m_szRectangleDrawer[0]->Enable(TRUE);
            m_szRectangleDrawer[1]->Enable(TRUE);
            m_szRectangleDrawer[2]->Enable(FALSE);
            m_szRectangleDrawer[3]->Enable(FALSE);
            ShowMultiplier(SW_HIDE);
            break;
        case 1:
            m_szRectangleDrawer[0]->Enable(FALSE);
            m_szRectangleDrawer[1]->Enable(FALSE);
            m_szRectangleDrawer[2]->Enable(TRUE);
            m_szRectangleDrawer[3]->Enable(TRUE);
            ShowMultiplier(SW_HIDE);
            break;
        case 2:
            DisableAllDrawer();
            ShowMultiplier(SW_SHOW);
            EnableMultiplier(TRUE);
            break;
        case 3:
            DisableAllDrawer();
            ShowMultiplier(SW_HIDE);
            break;
        default:
            TRACE("CFilterDlg::OnNMClickList1 nitem=%d", nItem);
            return;
        }
    }
    else
    {
        m_FiterOnBt.SetCheck(BST_UNCHECKED);
        m_FiterOffBt.SetCheck(BST_CHECKED);
        if ( nItem == 2 )
        {
            GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
        }

        //
        // Update Draw Tool and Multiplier
        //
        DisableAllDrawer();
        switch (nItem)
        {
        case 0:
        case 1:
        case 3:
            ShowMultiplier(SW_HIDE);
            break;
        case 2:
            ShowMultiplier(SW_SHOW);
            EnableMultiplier(FALSE);
            break;
        }
    }

    //
    // 2. Update Filter Info
    //
    m_strFilterInfo.LoadString(Filter_DIR_Start+nItem);


    UpdateData(FALSE);
    Invalidate();
}

void CFilterDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
    *pResult = 0;

    int nItem = -1;
    LPNMITEMACTIVATE lpNMItemActivate = (LPNMITEMACTIVATE)pNMHDR;
    if(lpNMItemActivate != NULL)
    {
        nItem = lpNMItemActivate->iItem;
    }

    if ( nItem == -1 )
    {
        return;
    }

    // Change Filter Action
    DoClickOne(nItem);
}

void CFilterDlg::OnBnClickedFilterOn()
{
    //if ( nSel == -1 )
    //{
    //    TRACE("CFilterDlg::OnBnClickedFilterOn() Err...........\n");
    //    nSel = 0;
    //}
   
    switch (m_nSelectRow)
    {
    case 0:
        m_szRectangleDrawer[0]->Enable(TRUE);
        m_szRectangleDrawer[1]->Enable(TRUE);
        break;
    case 1:
        m_szRectangleDrawer[2]->Enable(TRUE);
        m_szRectangleDrawer[3]->Enable(TRUE);
        break;
    case 2:
        ShowMultiplier(SW_SHOW);
        GetDlgItem(IDC_Muti_Edit)->EnableWindow(TRUE);
        break;
    case 3:      
        break;
    }

    m_FiterOffBt.SetCheck(BST_UNCHECKED);
    m_list.SetItemText(m_nSelectRow, 1, m_strStateOn);
    Invalidate();
}

void CFilterDlg::OnBnClickedFilterOff()
{
    //int nSel = m_list.GetSelectedColumn();
    //if ( nSel == -1 )
    //{
    //    TRACE("CFilterDlg::OnBnClickedFilterOff() Err...........\n");
    //    nSel = 0;
    //}

    switch (m_nSelectRow)
    {
    case 0:
        m_szRectangleDrawer[0]->Enable(FALSE);
        m_szRectangleDrawer[1]->Enable(FALSE);
        break;
    case 1:
        m_szRectangleDrawer[2]->Enable(FALSE);
        m_szRectangleDrawer[3]->Enable(FALSE);
        break;
    case 2:
        ShowMultiplier(SW_SHOW);
        GetDlgItem(IDC_Muti_Edit)->EnableWindow(FALSE);
        break;
    case 3:      
        break;
    }

    m_FiterOnBt.SetCheck(BST_UNCHECKED);
    m_list.SetItemText(m_nSelectRow, 1, m_strStateOff);
    Invalidate();
}

#define YUV422 1
BOOL CFilterDlg::OnVideoSend( FRAMEBUFSTRUCT *bufStruct )
{
    CRect Rect = m_Rect;
    FILETIME* time = (FILETIME*)&bufStruct->localTime;
    //GetClientRect(&Rect);
    ClientToScreen(&Rect);
    m_Player.Show(&Rect, bufStruct->pBuf, YUV422, 0, time);

    g_IIVDeviceBase2->ReleaseLiveBuf(bufStruct);
    return TRUE;
}

BOOL CFilterDlg::OnVideoPlay( 
    HDC dc, const tagRECT* rect, 
    const FILETIME* pTime,
    HWND hwnd, int nFlag, DWORD dwUserData )
{
    m_pDrawContainer->Invalidate();
    return TRUE;
}

void CFilterDlg::SetCommonParm( int nChannelID, WPG_Rule* pRule )
{
    m_pRule = pRule;
    m_nCurrentChan = nChannelID;
}

void CFilterDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call __super::OnPaint() for painting messages
    m_Player.ShowBack();
    m_pDrawContainer->Invalidate();
}

// End of file





