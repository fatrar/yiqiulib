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

}

CFilterDlg::~CFilterDlg()
{
}

void CFilterDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_list);
    DDX_Text(pDX, IDC_EDIT1, m_strFilterInfo);
    DDX_Control(pDX, IDC_FILTER_ON, m_FiterOnBt);
    DDX_Control(pDX, IDC_FILTER_OFF, m_FiterOffBt);
    DDX_Text(pDX, IDC_Multiplier, m_strMultiplier);
    DDX_Text(pDX, IDC_EDIT2, m_nMultiplierEdit);
	DDV_MinMaxFloat(pDX, m_nMultiplierEdit, 0.01f, 100.0f);
}


BEGIN_MESSAGE_MAP(CFilterDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CFilterDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CFilterDlg::OnBnClickedCancel)
    ON_NOTIFY(NM_CLICK, IDC_LIST1, &CFilterDlg::OnNMClickList1)
    ON_BN_CLICKED(IDC_FILTER_ON, &CFilterDlg::OnBnClickedFilterOn)
    ON_BN_CLICKED(IDC_FILTER_OFF, &CFilterDlg::OnBnClickedFilterOff)
END_MESSAGE_MAP()


// CFilterDlg message handlers

void CFilterDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    OnOK();
}

void CFilterDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    OnCancel();
}

BOOL CFilterDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    CString strTmp; 
    strTmp.LoadString(IDS_Filter_Head_Type);
    m_list.InsertColumn(0, strTmp, LVCFMT_LEFT, 200);
    strTmp.LoadString(IDS_Filter_Head_State);
    m_list.InsertColumn(1, strTmp, LVCFMT_LEFT, 100);

    m_strStateOn.LoadString(IDC_FILTER_ON);
    m_strStateOff.LoadString(IDS_Filter_Off);
    for (int i = 0; i< Filter_Number; ++i)
    {
        strTmp.LoadString(Filter_Name_Start+i);
        m_list.InsertItem(i, strTmp);
        m_list.SetItemText(i, 1, m_strStateOff);
    }

    m_list.SetSelectedColumn(0);
    m_strFilterInfo.LoadString(IDS_Filter_MaxObj_Info);
    m_FiterOnBt.SetCheck(BST_UNCHECKED);
    m_FiterOffBt.SetCheck(BST_CHECKED);

    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
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

#define ShowMultiplier(s) { \
    GetDlgItem(IDC_Multiplier)->ShowWindow(s);\
    GetDlgItem(IDC_EDIT2)->ShowWindow(s);}

    // Change Filter Action
    switch (nItem)
    {
    case 0:
    case 1:
    case 3:
        ShowMultiplier(SW_HIDE);
        break;
    case 2:
        ShowMultiplier(SW_SHOW);
        break;
    default:
        TRACE("CFilterDlg::OnNMClickList1 nitem=%d", nItem);
        return;
    }

    m_strFilterInfo.LoadString(Filter_DIR_Start+nItem);

    LV_ITEM lvitem = {0};
    lvitem.iItem = nItem;
    lvitem.iSubItem = 0;
    lvitem.mask = LVIF_TEXT;
    m_list.GetItem(&lvitem);
    if ( CString(lvitem.pszText) == m_strStateOn )
    {
        m_FiterOnBt.SetCheck(BST_CHECKED);
        m_FiterOffBt.SetCheck(BST_UNCHECKED);
        if ( nItem == 2 )
        {
            GetDlgItem(IDC_EDIT2)->EnableWindow();
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
    }

    UpdateData(FALSE);
}

void CFilterDlg::OnBnClickedFilterOn()
{
    int nSel = m_list.GetSelectedColumn();
    if ( nSel == -1 )
    {
        TRACE("CFilterDlg::OnBnClickedFilterOn() Err...........\n");
        nSel = 0;
    }
   
    m_FiterOffBt.SetCheck(BST_UNCHECKED);
    m_list.SetItemText(nSel, 1, m_strStateOn);
}

void CFilterDlg::OnBnClickedFilterOff()
{
    int nSel = m_list.GetSelectedColumn();
    if ( nSel == -1 )
    {
        TRACE("CFilterDlg::OnBnClickedFilterOff() Err...........\n");
        nSel = 0;
    }

    m_FiterOnBt.SetCheck(BST_UNCHECKED);
    m_list.SetItemText(nSel, 1, m_strStateOff);
}
