// ResToolBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ResViewer.h"
#include "ResToolBarDlg.h"

enum
{
    X_OffSet = 70,
    Bt_Width = 60,
    Bt_Height = 60,
};

// CResToolBarDlg dialog

IMPLEMENT_DYNAMIC(CResToolBarDlg, CDialogBar)

CResToolBarDlg::CResToolBarDlg()
{

}

CResToolBarDlg::~CResToolBarDlg()
{
}

void CResToolBarDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogBar::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_Add, m_AddBt);
    DDX_Control(pDX, IDC_Unpack, m_UnpackBt);
    DDX_Control(pDX, IDC_Open, m_OpenBt);
    DDX_Control(pDX, IDC_Delete, m_DeleteBt);
}

BEGIN_MESSAGE_MAP(CResToolBarDlg, CDialogBar)
     ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
     ON_UPDATE_COMMAND_UI_RANGE(IDC_Add, IDC_Unpack, OnUpdateButton)
     ON_BN_CLICKED(IDC_Add, &CResToolBarDlg::OnBnClickedAdd)
     ON_BN_CLICKED(IDC_Unpack, &CResToolBarDlg::OnBnClickedUnpack)
     ON_BN_CLICKED(IDC_Open, &CResToolBarDlg::OnBnClickedOpen)
     ON_BN_CLICKED(IDC_Delete, &CResToolBarDlg::OnBnClickedDelete)
END_MESSAGE_MAP()

LONG CResToolBarDlg::OnInitDialog( 
    UINT wParam, LONG lParam )
{
    BOOL bRet = HandleInitDialog(wParam, lParam);
    if (!UpdateData(FALSE))
    {
        TRACE0("Warning: UpdateData failed during dialog init.\n");
    }

//     HBITMAP hBitmap = LoadBitmap(
//         AfxGetApp()->m_hInstance,
//         MAKEINTRESOURCE(IDB_Add));
//     m_AddBt.SetBitmap(hBitmap);
//     m_AddBt.SetWindowText("aaa");
    int xStart = 5, yStart = 5;
    for (UINT i = 0; i< 5; ++i)
    {
        HBITMAP hBitmap = LoadBitmap(
         AfxGetApp()->m_hInstance,
         MAKEINTRESOURCE(IDB_Add+i));
        HWND hwnd = ::GetDlgItem(
         this->GetSafeHwnd(), IDC_Add+i );

        ::SendMessage(
         hwnd, BM_SETIMAGE,
         IMAGE_BITMAP, (long)hBitmap);
        //::SetWindowText(hwnd, "aaa");
        ::MoveWindow(
            hwnd,
            xStart + i*X_OffSet, 
            yStart, Bt_Width, Bt_Height, TRUE );
    }

    //HICON i = LoadIcon(AfxGetApp()->m_hInstance,
    //    MAKEINTRESOURCE(IDB_Add));
    //HWND hwnd = ::GetDlgItem(
    //    this->GetSafeHwnd(), IDC_Add);
    //::SendMessage(
    //    hwnd, BM_SETIMAGE, IMAGE_ICON,  (long)i);
    

    return bRet;
}

void CResToolBarDlg::OnUpdateButton(CCmdUI * pCmdUI){
    pCmdUI->Enable(TRUE);
    pCmdUI->SetCheck();
}


// CResToolBarDlg message handlers

void CResToolBarDlg::OnBnClickedAdd()
{
    AfxMessageBox("Unsupport Now!");
}

void CResToolBarDlg::OnBnClickedUnpack()
{
    // TODO: Add your control notification handler code here
}

void CResToolBarDlg::OnBnClickedOpen()
{
    AfxMessageBox("Unsupport Now!");
}

void CResToolBarDlg::OnBnClickedDelete()
{
    AfxMessageBox("Unsupport Now!");
}
