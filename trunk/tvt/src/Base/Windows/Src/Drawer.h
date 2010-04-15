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
using namespace OCI::Windows;




#define Pow2(x) ((x)*(x)) 

enum
{
    Point_Radii = 5,
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
    pdc->Ellipse(
        int(point.x-nr),
        int(point.y-nr),
        int(point.x+nr),
        int(point.y+nr) );
}

inline void DrawSquare(CDC* pdc, const CPoint& point, size_t nr)
{
    pdc->Rectangle(
        int(point.x-nr),
        int(point.y-nr),
        int(point.x+nr),
        int(point.y+nr) );
}



class IDrawerEx :
    public IDrawer
{
public:
    ~IDrawerEx(){}
    virtual BOOL OnMouseMove(UINT nFlags, CPoint& point) = 0;
    virtual BOOL OnLButtonUp(UINT nFlags, CPoint& point) = 0;
    virtual BOOL OnLButtonDown(UINT nFlags, CPoint& point) = 0;
    virtual void OnPaint(CDC& dc, BOOL bSelect=FALSE) = 0;
protected:
    CWnd* m_pWnd;
};

class CDrawer :
    public IDrawerEx
{
public:
    CDrawer();

public:
    virtual size_t GetUserInput(CPoint* pPointBuf, size_t nCount);
    virtual size_t PointCount();

    virtual void SetLineColour(DWORD dwColor);
    virtual void SetLineWidth(int nWidth);

    virtual void Clear();
    virtual void SetDefault(const CPoint* pPoint, size_t nCount);

    virtual void Enable(BOOL bEnbale){m_bEnbale=bEnbale;}
    virtual BOOL IsEnable(){return m_bEnbale;}
protected:
    bool IsDargPoint(CPoint& point);
    CPoint CenterPoint();
    bool IsDargCenterPoint(CPoint& point);
    void DrawCenterPoint(CDC* pdc);
    void CenterPointMoveTo(const CPoint& CenterpointNow);

protected:
    bool m_bIsOK;               // Is Drew?
    bool m_bDrawing;            // Is Drawing?

    int m_nDragIndex;           // Drag Point Index,-1 is invalid
    bool m_bDragging;           // Is Dragging?
    bool m_bDragCenter;   

    deque<CPoint> m_PointQueue; // Point Queue
    size_t m_nMaxPoint;

    DWORD m_dwLineColor;        // line Color, default black
    int m_nLineWidth;

    CPen m_Pen;
    CBrush m_Brush;

    BOOL m_bEnbale;
};


#endif  // _DRAWER_H_2010_


 // End of file



