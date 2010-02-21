/********************************************************************
	created:	2009/12/26
	created:	26:12:2009   9:53
	filename: 	f:\code\Darw\Darw\DarwCommon.h
	file path:	f:\code\Darw\Darw
	file base:	DarwCommon
	file ext:	h
	author:		yiqiu
	
	purpose:	
*********************************************************************/
#pragma once


//#include "IDrawer.h"
#include <math.h>
#include <deque>
using namespace std;

//enum MouseMsg
//{
//	MouseMsg_LButtonDown,
//	MouseMsg_LButtonUp,
//	MouseMsg_MouseMove,
//};
//
//struct IMsgTranslator
//{
//	virtual void Translate(MouseMsg msg, CPoint& point, int nID) = 0;
//};





inline BOOL LockCursor(const CRect& LockRect){ return ClipCursor(&LockRect); }

#ifdef Support_Dyn_Resolution
#   define OCI_Static
#else
#   define OCI_Static static
#endif

static BOOL UnLockCursor()
{
	OCI_Static int x = GetSystemMetrics(SM_CXSCREEN);
	OCI_Static int y = GetSystemMetrics(SM_CYSCREEN); 
	return ClipCursor(&CRect(0,0,x,y));
}

#define ParentInvalidate() \
    CRect FreshRect;                \
    GetClientRect(&FreshRect);      \
    ClientToScreen(&FreshRect);     \
    CWnd* pParentWnd = GetParent(); \
    pParentWnd->ScreenToClient(&FreshRect); \
    pParentWnd->InvalidateRect(&FreshRect);

enum
{
    Point_Radii = 9,
	Point_Color = 0x00ff00,
    Max_Point = 36,

	MSG_0 = 0x8100,
	MSG_1,
	// ..

};

#define Pow2(x) ((x)*(x)) 


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

//inline CRect& CalculateRect(int x, int y, CRect& rect)
//{
//	int x1 = x - Point_Button_Widht/2; 
//	int y1 = y - Point_Button_Height/2; 
//	int x2 = x + Point_Button_Widht/2; 
//	int y2 = y + Point_Button_Height/2; 
//	rect.SetRect(x1, y1, x2, y2);
//	return rect;
//}
//template<size_t MaxPoint = 36>
class IDrawer :
    public CWnd
{
public:
    virtual size_t GetUserInput(CPoint (&szPoint)[Max_Point]) = 0;
    virtual void SetColour(DWORD dwColor) = 0;
    virtual void Clear() = 0;
    virtual void SetDefault(CPoint* pPoint, size_t nCount) = 0;
    
    enum DrawCommond
    {
        Line_Show_Left  = 0x1,
        Line_Show_Right = 0x2,
        Line_Show_All   = Line_Show_Left | Line_Show_Right,
    };

    virtual void SendCommond(DrawCommond c, void* p1, void* p2) {};
};

enum 
{
    Nothing,
    Drawing,
    Drag_Base_Point,
    Drag_Two_Point_Middle,
    Drag_All_Point_Middle,

};
//template<size_t MaxPoint = 36>
class CDrawer :
	public IDrawer
{
public:
	CDrawer()
		: m_bDrawing(false)
		, m_nDragIndex(-1)
		, m_bDragging(false)
		, m_bIsOK(false)
        , m_dwColor(0)
        , m_bDragCenter(false) {}

public:
    virtual size_t GetUserInput(CPoint (&szPoint)[Max_Point])
    {
        if ( !m_bIsOK || Max_Point < m_PointQueue.size() )
        {
            return 0;
        }

        size_t i = 0;
        for ( deque<CPoint>::iterator iter = m_PointQueue.begin();
              iter != m_PointQueue.end();
              ++iter, ++i )
        {
            szPoint[i] = *iter;
        }
        return i;
    }

    virtual void SetColour(DWORD dwColor){ m_dwColor = dwColor; }
    virtual void Clear(){ m_PointQueue.clear(); m_bIsOK=false; };
    virtual void SetDefault(CPoint* pPoint, size_t nCount)
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

    DWORD m_dwColor;            // line Color, default black
};






// End of file



