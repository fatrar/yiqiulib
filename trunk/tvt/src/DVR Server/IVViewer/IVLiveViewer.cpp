/*H***************************************************************************
 File            : IVLiveViewer.cpp
 Subsystem       : 
 Function Name(s): CIVLiveViewer
 Author          : YiQiu
 Date            : 2010-1-14  
 Time            : 17:51
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "stdafx.h"
#include "BaseIVViewer.h"
#include "IVLiveViewer.h"

IIVDataBuf* CIVLiveViewer::s_pIVDataBuf = CIVLiveDataBuf::getInstancePtr();

//CFont
//BOOL CreateFont(int nHeight, int nWidth, int nEscapement,
//		int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline,
//		BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
//		BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
//		LPCTSTR lpszFacename);

CIVLiveViewer::CIVLiveViewer(void)
{
    memset(m_IsShowRule, 1, sizeof(m_IsShowRule));
    //m_hFont = ::CreateFont(
    //    -13,0,0,0,
    //    500,0,0,0,
    //    ANSI_CHARSET,OUT_DEFAULT_PRECIS,
    //    CLIP_DEFAULT_PRECIS,
    //    PROOF_QUALITY,
    //    DEFAULT_PITCH|FF_SWISS,
    //    NULL);
}

CIVLiveViewer::~CIVLiveViewer(void)
{
    //::DeleteObject(m_hFont);
}

BaseTargetQueue* CIVLiveViewer::GetIVData( 
    int nChannelID,
    const FILETIME& time )
{
    return s_pIVDataBuf->GetData(nChannelID, time);
}

//
// ******************* IIVLiveViewer **********************
// {
BOOL CIVLiveViewer::ResetStatistic(
    int nChannelID )
{
    m_StatisticData[nChannelID].dwCount[0] = 
        m_StatisticData[nChannelID].dwCount[1] = 0;
    return TRUE;
}

BOOL CIVLiveViewer::StartStatistic( 
    int nChannelID, 
    bool bFlag )
{
    m_StatisticData[nChannelID].IsOk = bFlag;
    return TRUE;
}

BOOL CIVLiveViewer::GetStatisticState( 
    int nChannelID,
    bool& bFlag )
{
    bFlag = m_StatisticData[nChannelID].IsOk == TRUE;
    return TRUE;
}

void CIVLiveViewer::ShowRuleAndStatistic(
    int nChannelID, bool bShow )
{
    m_IsShowRule[nChannelID] = bShow;
}

void CIVLiveViewer::PaintRule(
    int nChannelID,
    const HDC dc,
    const RECT& rect )
{
    if ( !m_IsShowRule[nChannelID] )
    {
        return;
    }

    RuleGraphMap& GraphMap = m_AllRuleGraph[nChannelID];
    if ( GraphMap.size() == 0 )
    {
        return;
    }

    HGDIOBJ hOldPen = ::SelectObject(dc, m_hPen);
    HGDIOBJ hOldBrush = ::SelectObject(dc, m_hBrush);
    {
        AutoLockAndUnlock(m_cs[nChannelID]);
        RuleGraphMap::iterator iter = GraphMap.begin();
        for ( ; iter!=GraphMap.end(); ++iter )
        {
            GraphInfo* pGraphInfo = iter->second;
            switch ( pGraphInfo->Type )
            {
            case IDrawer_Polygon:
                PaintPolygon(dc, rect, pGraphInfo->PointInfo.PolygonInfo);
            	break;
            case IDrawer_ArrowLine:
            {
                DWORD dwShowCount1(MAXDWORD), dwShowCount2(MAXDWORD);
                if ( pGraphInfo->PointInfo.LineInfo.IsStatistic )
                {
                    dwShowCount1 = m_StatisticData[nChannelID].dwCount[0];
                    dwShowCount2 = m_StatisticData[nChannelID].dwCount[1];
                }
                PaintArrowLine(
                    dc, rect, 
                    pGraphInfo->PointInfo.LineInfo.Info,
                    dwShowCount1,
                    dwShowCount2 );
                break;
            }    
            default:
            	break;
            }
        }
    }
    ::SelectObject(dc, hOldPen);
    ::SelectObject(dc, hOldBrush);
}

// }
// IIVLiveViewer

//
// ******************* IIVLiveViewerEx **********************
// {
void CIVLiveViewer::AddRule( 
    int nChannelID,
    const WPG_Rule& Rule )
{
    RuleGraphMap& GraphMap = m_AllRuleGraph[nChannelID];
    const IV_RuleID& RuleID = (const IV_RuleID&)Rule.ruleId;
    {
        AutoLockAndUnlock(m_cs[nChannelID]);
        RuleGraphMap::iterator iter = GraphMap.find(RuleID);
        if ( iter != GraphMap.end() )
        {
            assert(false);
            delete iter->second;
            GraphMap.erase(iter);
        }
   
        GraphInfo* pTmpGraphInfo = NULL;
        bool bRc = TranslateRuleToGraphInfo(Rule, pTmpGraphInfo);
        if ( !bRc )
        {
            return;
        }

        GraphMap[RuleID] = pTmpGraphInfo;
    }
}

void CIVLiveViewer::RemoveRule(
    int nChannelID,
    BYTE (&ID)[16] )
{
    RuleGraphMap& GraphMap = m_AllRuleGraph[nChannelID];
    IV_RuleID& RuleID = (IV_RuleID&)ID;
    if ( RuleID.RuleID.nType == IV_Stage_Change )
    {
        return;
    }
    {
        AutoLockAndUnlock(m_cs[nChannelID]);
        RuleGraphMap::iterator iter = GraphMap.find(RuleID);
        if ( iter == GraphMap.end() )
        {
            assert(false);
            return;
        }

        delete iter->second;
        GraphMap.erase(iter);
    }
}

void CIVLiveViewer::ModifyRule(
    int nChannelID,
    const WPG_Rule& Rule )
{
    RuleGraphMap& GraphMap = m_AllRuleGraph[nChannelID];
    const IV_RuleID& RuleID = (const IV_RuleID&)Rule.ruleId;
    {
        AutoLockAndUnlock(m_cs[nChannelID]);
        RuleGraphMap::iterator iter = GraphMap.find(RuleID);
        if ( iter == GraphMap.end() )
        {
            assert(false);
            return;
        }

        GraphInfo* pTmpGraphInfo = iter->second;
        bool bRc = TranslateRuleToGraphInfo(Rule, pTmpGraphInfo);
        if ( !bRc )
        {
            return;
        }
    }
}

void CIVLiveViewer::ClearAllRule( 
    int nChannelID )
{
    RuleGraphMap& GraphMap = m_AllRuleGraph[nChannelID];
    AutoLockAndUnlock(m_cs[nChannelID]);
    StlHelper::STLDeleteAssociate(GraphMap);
}

//
// ******************* IIVStatisticFresher ***************
// {
void CIVLiveViewer::OnStatisticFresh(
    int nChannelID,
    StatisticDir Dir )
{
    if ( Dir == Statistic_Left )
    {
        ++m_StatisticData[nChannelID].dwCount[0];
    }
    else
    {
        ++m_StatisticData[nChannelID].dwCount[1];
    }
}
// }
// IIVStatisticFresher

//
// ******************* Other **********************
// {

bool CIVLiveViewer::TranslateRuleToGraphInfo(
    const WPG_Rule& Rule,
    GraphInfo*& Info )
{
    const WPG_RuleDescription& Dir = Rule.ruleDescription;
    const WPG_EventDescriptionUnion& DirUnion = Dir.description;
    IVRuleType t = ((IV_RuleID&)(Rule.ruleId)).RuleID.nType;
    switch (Dir.type)
    {
    case TRIPWIRE_EVENT:
        if ( Info == NULL )
        {
            Info = new GraphInfo;
        }
        Info->Type = IDrawer_ArrowLine;
        Info->PointInfo.LineInfo.Info = DirUnion.tripwireEventDescription;
        Info->PointInfo.LineInfo.IsStatistic = t==IV_Statistic ? true:false;
        
    	break;
    case AOI_EVENT: 
        if ( Info == NULL )
        {
            Info = new GraphInfo;
        }
        Info->Type = IDrawer_Polygon;
        Info->PointInfo.PolygonInfo = DirUnion.aoiEventDescription.polygon;
        break;
    case SCENE_CHANGE_EVENT:
    default:
    	return false;
    }

    return true;
}

void CIVLiveViewer::PaintPolygon(
    const HDC dc,
    const RECT& rect,
    WPG_PolygonF& Polygon )
{
    if ( Polygon.numPoints <= 2 )
    {
        return;
    }

    int x, y, xStart, yStart;
    ViewHelper::TranslateWPGPoint(rect, Polygon.points[0], xStart, yStart);
    ::MoveToEx(dc, xStart, yStart, NULL);
    for (size_t i = 1; i<Polygon.numPoints; ++i)
    {
        ViewHelper::TranslateWPGPoint(rect, Polygon.points[i], x, y);
        ::LineTo(dc, x, y);
    }
    ::LineTo(dc, xStart, yStart);
}

#define Pow2(x) ((x)*(x))

enum 
{
    ArrowLineLen = 50,
    ArrowHeadLen = 10,
};

void DrawArrow(
    HDC dc, 
    const POINT& p,
    size_t d,
    double o,
    bool bUp = true,
    DWORD dwShowCount = MAXDWORD )
{
    POINT p0, p1, p2;
    if ( bUp )
    {
        p0.x = p.x + long(d*cos(o));
        p0.y = p.y + long(d*sin(o));
    }
    else
    {
        p0.x = p.x - long(d*cos(o));
        p0.y = p.y - long(d*sin(o));
    }

    p1.x = p.x - long(d*sin(o));
    p1.y = p.y + long(d*cos(o));
    p2.x = p.x + long(d*sin(o));
    p2.y = p.y - long(d*cos(o));
    ::MoveToEx(dc, p.x, p.y, NULL);
    ::LineTo(dc, p0.x, p0.y);
    ::LineTo(dc, p1.x, p1.y);
    ::MoveToEx(dc, p0.x, p0.y, NULL);
    ::LineTo(dc, p2.x, p2.y);
    
    if ( dwShowCount != MAXDWORD )
    {
        char szBuf[16] = {0};
        sprintf_s(szBuf, "%u", dwShowCount);
        ::TextOutA(
            dc, 
            p.x,
            p.y+CIVLiveViewer::Point_Radii, 
            szBuf, strlen(szBuf));
    }
}

void CIVLiveViewer::PaintArrowLine( 
    const HDC dc, 
    const RECT& rect, 
    WPG_TripwireEventDescription& Line,
    DWORD dwShowCount1,
    DWORD dwShowCount2 )
{
    int x[2], y[2];
    ViewHelper::TranslateWPGPoint(rect, Line.startPoint, x[0], y[0]);
    ViewHelper::TranslateWPGPoint(rect, Line.endPoint, x[1], y[1]);

    ::MoveToEx(dc, x[0], y[0], NULL);  
    ::LineTo(dc, x[1], y[1]);

    // 利用垂直和两点的距离算出两个点的坐标
    POINT MedPoint;
    MedPoint.x = (x[0] + x[1])/2;
    MedPoint.y = (y[0] + y[1])/2;
    double m = sqrt( 
        Pow2(double(MedPoint.x-x[0])) + Pow2(double(MedPoint.y-y[0])) );
    
    POINT A[2];
    A[0].x = MedPoint.x + long(ArrowLineLen/m*(y[0]-MedPoint.y));
    A[0].y = MedPoint.y - long(ArrowLineLen/m*(x[0]-MedPoint.x));
    A[1].x = MedPoint.x - long(ArrowLineLen/m*(y[0]-MedPoint.y));
    A[1].y = MedPoint.y + long(ArrowLineLen/m*(x[0]-MedPoint.x));

    // dc.p

    double o;
    long nXoffset = A[0].x-A[1].x;
    long nYoffset = A[0].y-A[1].y;
    if (nXoffset == 0)
    {
        o = nYoffset > 0 ? M_PI_2 : (-M_PI_2);
    }
    else
    {
        double tanValue = 1.0*(nYoffset)/nXoffset;
        o = atan(tanValue);
    }

    // 修正两种特殊情况，因为我把它的角度转换为[0, pi]区间的，
    // atan取值范围为(-pi/2, pi/2)
    if ( o < 0 )
    {
        o += M_PI;
    }
    else if ( o == 0 && nXoffset > 0 )
    {
        o = M_PI;
    }

    int nOldMode = ::SetBkMode(dc, TRANSPARENT);
    
    COLORREF nOldCol = ::SetTextColor(dc, Font_Color);
    //HGDIOBJ hOldFont = ::SelectObject(dc, m_hFont);
    bool bUp = (A[0].y > A[1].y);
    //bool bUp = (A[0].x > A[1].x) ^ (A[0].y < A[1].y);
    //bool bUp = GetPointRLineState(BeginPoint, EndPoint, A[1]) > 0; 
    //TRACE("%d\n", bUp);
    if ( Line.direction == ANY_DIRECTION || 
         Line.direction == RIGHT_TO_LEFT )
    {
        ::MoveToEx(dc, MedPoint.x, MedPoint.y, NULL);  
        ::LineTo(dc, A[0].x, A[0].y);
        DrawArrow(dc, A[0], ArrowHeadLen, o, bUp, dwShowCount1);
    }
    if (  Line.direction == ANY_DIRECTION || 
          Line.direction == LEFT_TO_RIGHT ) // Line_Show_Left
    {
        ::MoveToEx(dc, MedPoint.x, MedPoint.y, NULL);  
        ::LineTo(dc, A[1].x, A[1].y);
        DrawArrow(dc, A[1], ArrowHeadLen, o, !bUp, dwShowCount2);
    }
    //SelectObject(dc, hOldFont);
    ::SetTextColor(dc, nOldCol);
    ::SetBkMode(dc, nOldMode);
}

// }

// End of file