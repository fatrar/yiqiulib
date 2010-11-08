// ResListView.cpp : implementation file
//

#include "stdafx.h"
#include "ResViewer.h"
#include "ResListView.h"

ResFile::IResReaderEx* g_pResReaderEx = NULL;

enum ColumnDefine
{
    Col_Name,
    Col_HashValue,
    Col_Raw_Size,
    Col_Pack_Size,
    Col_Compress_Algo,
    Col_Compress_Param,
    Col_Encrypt_Algo,
};
//  0, "名称", LVCFMT_CENTER, 80 );
//  1, "哈希值", LVCFMT_CENTER, 150 );
//  2, "原数据大小", LVCFMT_CENTER, 110 );
//  3, "压缩后大小", LVCFMT_CENTER, 110 );
//  4, "压缩算法", LVCFMT_CENTER, 110 );
//  4, "压缩参数", LVCFMT_CENTER, 110 );
//  5, "加密算法", LVCFMT_CENTER, 110 );


// CResListView

IMPLEMENT_DYNCREATE(CResListView, CListView)

CResListView::CResListView()
 //   : m_IterList(NULL)
{}

CResListView::~CResListView()
{
    //safeDeleteArray(m_IterList);
}

BEGIN_MESSAGE_MAP(CResListView, CListView)
    ON_WM_CREATE()
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CResListView diagnostics

#ifdef _DEBUG
void CResListView::AssertValid() const
{
	CListView::AssertValid();
}

void CResListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG


// CResListView message handlers

void CResListView::OnInitialUpdate()
{
    CListView::OnInitialUpdate();

    // TODO: Add your specialized code here and/or call the base class
    CListCtrl& List = GetListCtrl();
    List.DeleteAllItems();

    RemoveAllData();
    AddAllData();
}

int CResListView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CListView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here

    CListCtrl& List = GetListCtrl();//得到内置的listctrl引用

    LONG lStyle;
    lStyle = GetWindowLong(List.m_hWnd, GWL_STYLE);//获取当前窗口风格
    lStyle &= ~LVS_TYPEMASK; //清除显示方式位
    lStyle |= LVS_REPORT; //设置报表风格
    SetWindowLong(List.m_hWnd, GWL_STYLE, lStyle); //设置窗口风格

    //选中某行使整行高亮（只适用于报表风格的listctrl）
    DWORD dwStyle = List.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;
    //dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与报表风格的listctrl）

    List.SetExtendedStyle(dwStyle); //设置扩展风格
    List.SetBkColor(RGB(200, 200, 200)); //设置背景颜色
    List.SetTextBkColor(RGB(200, 200, 200)); //设置文本背景颜色
    List.SetTextColor(RGB(10, 10, 80)); //设置文本颜色


    //插入列的标题，为了简单起见，我只插入三列
    List.InsertColumn( Col_Name, "名称", LVCFMT_CENTER, 80 );
    List.InsertColumn( Col_HashValue, "哈希值", LVCFMT_CENTER, 150 );
    List.InsertColumn( Col_Raw_Size, "原数据大小", LVCFMT_CENTER, 110 );
    List.InsertColumn( Col_Pack_Size, "压缩后大小", LVCFMT_CENTER, 110 );
    List.InsertColumn( Col_Compress_Algo, "压缩算法", LVCFMT_CENTER, 110 );
    List.InsertColumn( Col_Compress_Param, "压缩参数", LVCFMT_CENTER, 110 );
    List.InsertColumn( Col_Encrypt_Algo, "加密算法", LVCFMT_CENTER, 110 );
    return 0;
}

void CResListView::OnDestroy()
{
    CListView::OnDestroy();

    // TODO: Add your message handler code here
    RemoveAllData();
}

void CResListView::AddAllData()
{
    if ( NULL == g_pResReaderEx )
    {
        return;
    }
    ResFile::CResIterator* Iter = NULL;
    CString strName, strHashValue, strDataPackLen;
    CListCtrl& List = GetListCtrl();
    List.SetRedraw(FALSE);
    for ( ResFile::size_t i = 0; i< g_pResReaderEx->Count(); ++i )
    {
        Iter = new ResFile::CResIterator();
        g_pResReaderEx->GetSomeInfo(i, *Iter);
        if ( Iter->m_pFileName )
        {
            strName = Iter->m_pFileName;
        }
        else
        {
            strName.Format("%d", i);
        }
        List.InsertItem(i, strName);

        strHashValue.Format("%8x-%8x", 
            Iter->m_HashValue.dwValue[0],
            Iter->m_HashValue.dwValue[1]);
        List.SetItemText(i, Col_HashValue, strHashValue);

        strDataPackLen.Format("%d", Iter->m_dwPackDataLen);
        List.SetItemText(i, Col_Pack_Size, strDataPackLen);

        List.SetItemData(i, (DWORD_PTR)Iter);
    }
    List.SetRedraw(TRUE);

    g_pResReaderEx->StartGetAllInfo(this);
}

void CResListView::RemoveAllData()
{
    ResFile::CResIterator* Iter = NULL;
    CListCtrl& List = GetListCtrl();
    for ( int i = 0; i < List.GetItemCount(); ++i )
    {
        Iter = (ResFile::CResIterator*)List.GetItemData(i);
        safeDelete(Iter);  
    }

    List.DeleteAllItems();
}

const char* c_szEncryptAlgoStr[] = 
{
    "Raw_Algo",
    "Xor_Algo",
    "BlowFish_Algo"
};
const char* c_szCompressAlgoStr[] = 
{
    "Raw_Algo",
    "Zip_Algo",   // Zip
    "Lzma_Algo",
};
const char* c_szCompressParamStr[] = 
{
    "Unpack_Fast",
    "Compress_Normal",
    "Compress_High",
    "Compress_Auto",
};

void CResListView::OnInfoRead(
    ResFile::DWORD nIndex, 
    ResFile::eEncryptAlgo eAlgo,
    ResFile::eCompressAlgo cAlgo, 
    ResFile::eCompressParam cParam,
    ResFile::DWORD dwRawDataLen )
{
    CListCtrl& List = GetListCtrl();
    CString strRawLen;
    strRawLen.Format("%d", dwRawDataLen);
    List.SetItemText(nIndex, Col_Raw_Size, strRawLen);

    List.SetItemText(nIndex, Col_Compress_Algo, c_szCompressAlgoStr[cAlgo]);
    List.SetItemText(nIndex, Col_Compress_Param, c_szCompressParamStr[cParam]);
    List.SetItemText(nIndex, Col_Encrypt_Algo, c_szEncryptAlgoStr[eAlgo]);
}
