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

    CListCtrl& m_list = GetListCtrl();//得到内置的listctrl引用

    LONG lStyle;
    lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);//获取当前窗口风格
    lStyle &= ~LVS_TYPEMASK; //清除显示方式位
    lStyle |= LVS_REPORT; //设置报表风格
    SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle); //设置窗口风格

    //选中某行使整行高亮（只适用于报表风格的listctrl）
    DWORD dwStyle = m_list.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;
    //dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与报表风格的listctrl）

    m_list.SetExtendedStyle(dwStyle); //设置扩展风格
    m_list.SetBkColor(RGB(200, 200, 200)); //设置背景颜色
    m_list.SetTextBkColor(RGB(200, 200, 200)); //设置文本背景颜色
    m_list.SetTextColor(RGB(10, 10, 80)); //设置文本颜色


    //插入列的标题，为了简单起见，我只插入三列
    m_list.InsertColumn( 0, "名称", LVCFMT_CENTER, 80 );
    m_list.InsertColumn( 1, "大小", LVCFMT_CENTER, 110 );
    m_list.InsertColumn( 3, "压缩后大小", LVCFMT_CENTER, 110 );
    m_list.InsertColumn( 4, "压缩算法", LVCFMT_CENTER, 110 );
    m_list.InsertColumn( 5, "加密算法", LVCFMT_CENTER, 110 );


}
