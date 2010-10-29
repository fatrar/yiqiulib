// ResListView.cpp : implementation file
//

#include "stdafx.h"
#include "ResViewer.h"
#include "ResListView.h"


// CResListView

IMPLEMENT_DYNCREATE(CResListView, CListView)

CResListView::CResListView()
{

}

CResListView::~CResListView()
{
}

BEGIN_MESSAGE_MAP(CResListView, CListView)
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

    CListCtrl& m_list = GetListCtrl();//�õ����õ�listctrl����

    LONG lStyle;
    lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);//��ȡ��ǰ���ڷ��
    lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
    lStyle |= LVS_REPORT; //���ñ�����
    SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle); //���ô��ڷ��

    //ѡ��ĳ��ʹ���и�����ֻ�����ڱ������listctrl��
    DWORD dwStyle = m_list.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;
    //dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ�����뱨�����listctrl��

    m_list.SetExtendedStyle(dwStyle); //������չ���
    m_list.SetBkColor(RGB(200, 200, 200)); //���ñ�����ɫ
    m_list.SetTextBkColor(RGB(200, 200, 200)); //�����ı�������ɫ
    m_list.SetTextColor(RGB(10, 10, 80)); //�����ı���ɫ


    //�����еı��⣬Ϊ�˼��������ֻ��������
    m_list.InsertColumn( 0, "����", LVCFMT_CENTER, 80 );
    m_list.InsertColumn( 1, "��С", LVCFMT_CENTER, 110 );
    m_list.InsertColumn( 3, "ѹ�����С", LVCFMT_CENTER, 110 );
    m_list.InsertColumn( 4, "ѹ���㷨", LVCFMT_CENTER, 110 );
    m_list.InsertColumn( 5, "�����㷨", LVCFMT_CENTER, 110 );


}
