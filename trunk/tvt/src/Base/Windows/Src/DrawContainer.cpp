/*H***************************************************************************
 File            : DrawContainer.cpp
 Subsystem       : 
 Function Name(s): CDrawContainer
 Author          : YiQiu
 Date            : 2010-3-2  
 Time            : 15:58
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "stdafx.h"
#include "DrawContainer.h"


IMPLEMENT_DYNAMIC(CDrawContainer, CWnd)

CDrawContainer::CDrawContainer()
    : m_pSelect(NULL)
    , m_pDrawModeNotify(NULL){}

CDrawContainer::~CDrawContainer()
{
    DrawerList::iterator iter;
    for ( iter = m_GraphContainer.begin();
          iter!= m_GraphContainer.end();
          ++iter)
    {
        delete iter->pDrawer;
    }
}

BEGIN_MESSAGE_MAP(CDrawContainer, CWnd)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_PAINT()
END_MESSAGE_MAP()

IDrawer* CDrawContainer::Add( IDrawerGraphType t )
{
    // 如果C++有反射，代码会简单很多
    IDrawerEx* pDrawerEx = NULL;
    switch (t)
    {
    case IDrawer_Rectangle: 
        pDrawerEx = new CRectangleDrawer(this);
        break;
    case IDrawer_Polygon:  
        pDrawerEx = new CPolygonDrawer(this);
        break;
    case IDrawer_Line:  
        pDrawerEx = new CLineDrawer(this);
        break;
    case IDrawer_ArrowLine: 
        pDrawerEx = new CArrowLineDrawer(this);
        break;
    case IDrawer_ArrowLineEx:
        pDrawerEx = new CArrowLineDrawerEx(this);
        break;
    default:
        return NULL;
    }
    assert(pDrawerEx);
    m_GraphContainer.insert( DrawerIter(pDrawerEx));
    return pDrawerEx;
}

BOOL CDrawContainer::Remove( IDrawer* pDrawer )
{
    DrawerIter i((IDrawerEx*)pDrawer);
    DrawerList::iterator iter = m_GraphContainer.find(i);
    if ( iter != m_GraphContainer.end() )
    {
        m_GraphContainer.erase(iter);
        if ( m_pSelect == pDrawer )
        {
            m_pSelect = NULL;
        }
        delete pDrawer;
        return TRUE;
    }
    assert(false);
    return FALSE;
}

void CDrawContainer::SwtichTool( IDrawer* pDrawer )
{
    DrawerIter i((IDrawerEx*)pDrawer);
    DrawerList::iterator iter = m_GraphContainer.find(i);
    if ( iter == m_GraphContainer.end() )
    {
       assert(false);
       return;
    }

    m_pSelect = static_cast<IDrawerEx*>(pDrawer);
}

void CDrawContainer::EnableToolExcept( IDrawer* pDrawer, BOOL bEnbale )
{
    DrawerList::iterator iter;
    for ( iter = m_GraphContainer.begin();
          iter!= m_GraphContainer.end();
          ++iter)
    {
        if (iter->pDrawer != pDrawer)
        {
            iter->pDrawer->Enable(bEnbale);
        }
    }
}

void CDrawContainer::EnableToolAndNotEnableOther(
    IDrawer* pDrawer, BOOL bEnbale )
{
    DrawerList::iterator iter;
    for ( iter = m_GraphContainer.begin();
          iter!= m_GraphContainer.end();
          ++iter)
    {
        iter->pDrawer->Enable(iter->pDrawer == pDrawer ? bEnbale : !bEnbale);
    }
}

// 虽然用define实现代码要简单，但是为了方便调式，才用模板变态实现
template<DoMessgaeFn T>
void CDrawContainer::OnMouseMessage(UINT nFlags, CPoint& point)
{
    if ( m_pSelect && (*m_pSelect.*T)(nFlags,point) )
    {
        return;
    }

    DrawerList::iterator iter;
    for ( iter = m_GraphContainer.begin();
        iter!= m_GraphContainer.end();
        ++iter)
    {
        DrawerIter& Tmp = *iter;
        if ( m_pSelect == Tmp.pDrawer )
        {
            continue;
        }

        IDrawerEx* pDrawer = Tmp.pDrawer;
        if ( pDrawer->IsEnable() && (*pDrawer.*T)(nFlags,point) )
        {
            m_pSelect = Tmp.pDrawer;
            TRACE("CDrawContainer Slect New %x\n", m_pSelect);
            if ( m_pDrawModeNotify )
            {
                m_pDrawModeNotify->OnChange(m_pSelect);
            }
            RedrawWindow();
            //Invalidate();
            return;
        }
    }

    if ( m_pSelect == NULL )
    {
        return;
    }

    TRACE("CDrawContainer Slect Set void!\n");
    m_pSelect = NULL;
    if ( m_pDrawModeNotify )
    {
        m_pDrawModeNotify->OnChange(NULL);
    }
}

void CDrawContainer::OnMouseMove( UINT nFlags, CPoint point )
{
    if ( !(nFlags&MK_LBUTTON) )
    {
        return;
    }
    OnMouseMessage<&IDrawerEx::OnMouseMove>(nFlags,point);
}

void CDrawContainer::OnLButtonUp( UINT nFlags, CPoint point )
{
    OnMouseMessage<&IDrawerEx::OnLButtonUp>(nFlags,point);
}

void CDrawContainer::OnLButtonDown( UINT nFlags, CPoint point )
{
    OnMouseMessage<&IDrawerEx::OnLButtonDown>(nFlags,point);
}

void CDrawContainer::OnPaint()
{
    CPaintDC dc(this);
    DrawerList::iterator iter;
    for ( iter = m_GraphContainer.begin();
          iter!= m_GraphContainer.end();
          ++iter)
    {
        IDrawerEx* p = iter->pDrawer;
        if ( p->IsEnable() )
        {    
            p->OnPaint(dc, p==m_pSelect);
        }  
    }
}


CDrawContainer::DrawerIter::DrawerIter(IDrawerEx* p)
    : pDrawer(p)
    //, bEnbale(TRUE)
{}

CDrawContainer::DrawerIter::~DrawerIter()
{
    //safeDelete(pDrawer);
}

bool CDrawContainer::DrawerIter::operator > (const DrawerIter& a) const
{
    return pDrawer > a.pDrawer;
}

bool CDrawContainer::DrawerIter::operator < (const DrawerIter& a) const
{
    return pDrawer < a.pDrawer;
}

bool CDrawContainer::DrawerIter::operator == (const DrawerIter& a) const
{
    return pDrawer == a.pDrawer;
}

namespace OCI
{


namespace Windows
{

IDrawContainer* CreateDrawContainer()
{
    return new CDrawContainer();
}

void DestoryDrawContainer(IDrawContainer* p)
{
    safeDelete(p);
}

}
}
// End of file