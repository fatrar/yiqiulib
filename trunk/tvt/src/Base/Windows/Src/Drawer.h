/*H***************************************************************************
 File            : Drawer.h
 Subsystem       : 
 Function Name(s): CDrawer
 Author          : YiQiu
 Date            : 2010-2-22  
 Time            : 11:10
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _DRAWER_H_2010_2
#define _DRAWER_H_2010_2


#define _USE_MATH_DEFINES
#include <math.h>
#include <deque>
using namespace std;
#include "IDrawer.h"
using namespace OCI::System::Windows;




#define Pow2(x) ((x)*(x)) 

enum
{
    Point_Radii = 9,
    Point_Default_Color = 0x00ff00,
    Line_Default_Width = 3,

    /*MSG_0 = 0x8100,
    MSG_1,*/
    // ..

};

//template<typename T>
inline double Distance(const CPoint& p1, const CPoint& p2)
{
    return sqrt( Pow2(double(p1.x-p2.x)) + Pow2(double(p1.y-p2.y)) );
}

static int IsDargPoint(const CPoint& point, const deque<CPoint>& TestQueue)
{
    size_t nSize = TestQueue.size();
    if ( nSize == 0 )
    {
        return -1;
    }

#define SimpleDistance(x)  Distance((point), (x))

    int nIndex = 0;
    double nMin = SimpleDistance( TestQueue[nIndex] );
    for (size_t i=1; i<nSize; ++i)
    {
        double nTmp = SimpleDistance( TestQueue[i] );
        if ( nTmp < nMin )
        {
            nMin = nTmp;
            nIndex = i;
        }
    }

    if ( nMin > (double)Point_Radii )
    {
        return -1;
    }

    return nIndex;
}

inline void DrawCircle(CDC* pdc, const CPoint& point, size_t nr)
{
    pdc->Ellipse(point.x-nr,point.y-nr,point.x+nr, point.y+nr);
}


class CDrawer :
    public IDrawer
{
public:
    CDrawer()
        : m_bDrawing(false)
        , m_nDragIndex(-1)
        , m_bDragging(false)
        , m_bIsOK(false)
        //, m_dwColor(Point_Default_Color)
        , m_bDragCenter(false)
        , m_Pen(PS_SOLID, Line_Default_Width, Point_Default_Color)
        , m_Brush(Point_Default_Color) {}

public:
    virtual size_t GetUserInput(CPoint* pPointBuf, size_t nCount)
    {
        if ( !m_bIsOK || nCount < m_PointQueue.size() )
        {
            return 0;
        }

        size_t i = 0;
        for ( deque<CPoint>::iterator iter = m_PointQueue.begin();
            iter != m_PointQueue.end();
            ++iter, ++i )
        {
            pPointBuf[i] = *iter;
        }
        return i;
    }

    virtual size_t PointCount() 
    {
        if ( m_bIsOK )
        {
            return m_PointQueue.size();
        }
        return 0;
    }

    virtual void SetColour(DWORD dwColor)
    {
        m_Pen.DeleteObject();
        m_Pen.CreatePen(PS_SOLID, Line_Default_Width, dwColor);
        m_Brush.DeleteObject();
        m_Brush.CreateSolidBrush(dwColor);
        Invalidate();
    }
    virtual void Clear(){ m_PointQueue.clear(); m_bIsOK=false; };
    virtual void SetDefault(const CPoint* pPoint, size_t nCount)
    {
        ASSERT(pPoint);

        m_PointQueue.clear();
        m_bIsOK=true;

        for (size_t i = 0; i<nCount; ++i)
        {
            m_PointQueue.push_back(pPoint[i]);
        }
    }

protected:
    bool IsDargPoint(CPoint& point)
    {
        m_nDragIndex = ::IsDargPoint(point, m_PointQueue);
        return  m_nDragIndex != -1 ;
    }

    CPoint CenterPoint()
    {
        CPoint p(0, 0);
        for ( deque<CPoint>::iterator iter = m_PointQueue.begin();
            iter != m_PointQueue.end();
            ++iter )
        {
            p += *iter;
        }

        size_t nSize = m_PointQueue.size();
        p.x /= nSize;
        p.y /= nSize;
        return p;
    }

    bool IsDargCenterPoint(CPoint& point)
    {
        if ( !m_bIsOK )
        {
            return false;
        }

        CPoint p = CenterPoint();
        return Distance(point, p) <= Point_Radii;
    }

    void DrawCenterPoint(CDC* pdc)
    {
        if ( !m_bIsOK )
        {
            return;
        }

        CPoint& p = CenterPoint();
        DrawCircle(pdc, p, Point_Radii);
    }

    void CenterPointMoveTo(const CPoint& CenterpointNow)
    {
        CPoint CenterpointOld = CenterPoint();
        CPoint Offset = CenterpointNow - CenterpointOld;
        for ( deque<CPoint>::iterator iter = m_PointQueue.begin();
            iter != m_PointQueue.end();
            ++iter )
        {
            *iter += Offset;
        }
    }

protected:
    bool m_bIsOK;               // Is Drew?
    bool m_bDrawing;            // Is Drawing?

    int m_nDragIndex;           // Drag Point Index,-1 is invalid
    bool m_bDragging;           // Is Dragging?
    bool m_bDragCenter;   

    deque<CPoint> m_PointQueue; // Point Queue
    size_t m_nMaxPoint;

    DWORD m_dwColor;            // line Color, default black

    CPen m_Pen;
    CBrush m_Brush;   
};




#endif  // _DRAWER_H_2010_


 // End of file



