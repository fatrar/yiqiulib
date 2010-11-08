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
//  0, "����", LVCFMT_CENTER, 80 );
//  1, "��ϣֵ", LVCFMT_CENTER, 150 );
//  2, "ԭ���ݴ�С", LVCFMT_CENTER, 110 );
//  3, "ѹ�����С", LVCFMT_CENTER, 110 );
//  4, "ѹ���㷨", LVCFMT_CENTER, 110 );
//  4, "ѹ������", LVCFMT_CENTER, 110 );
//  5, "�����㷨", LVCFMT_CENTER, 110 );


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

    CListCtrl& List = GetListCtrl();//�õ����õ�listctrl����

    LONG lStyle;
    lStyle = GetWindowLong(List.m_hWnd, GWL_STYLE);//��ȡ��ǰ���ڷ��
    lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
    lStyle |= LVS_REPORT; //���ñ�����
    SetWindowLong(List.m_hWnd, GWL_STYLE, lStyle); //���ô��ڷ��

    //ѡ��ĳ��ʹ���и�����ֻ�����ڱ������listctrl��
    DWORD dwStyle = List.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;
    //dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ�����뱨�����listctrl��

    List.SetExtendedStyle(dwStyle); //������չ���
    List.SetBkColor(RGB(200, 200, 200)); //���ñ�����ɫ
    List.SetTextBkColor(RGB(200, 200, 200)); //�����ı�������ɫ
    List.SetTextColor(RGB(10, 10, 80)); //�����ı���ɫ


    //�����еı��⣬Ϊ�˼��������ֻ��������
    List.InsertColumn( Col_Name, "����", LVCFMT_CENTER, 80 );
    List.InsertColumn( Col_HashValue, "��ϣֵ", LVCFMT_CENTER, 150 );
    List.InsertColumn( Col_Raw_Size, "ԭ���ݴ�С", LVCFMT_CENTER, 110 );
    List.InsertColumn( Col_Pack_Size, "ѹ�����С", LVCFMT_CENTER, 110 );
    List.InsertColumn( Col_Compress_Algo, "ѹ���㷨", LVCFMT_CENTER, 110 );
    List.InsertColumn( Col_Compress_Param, "ѹ������", LVCFMT_CENTER, 110 );
    List.InsertColumn( Col_Encrypt_Algo, "�����㷨", LVCFMT_CENTER, 110 );
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
