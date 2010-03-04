/*H***************************************************************************
 File            : IVUtil.h
 Subsystem       : 
 Function Name(s): CIVUtil
 Author          : YiQiu
 Date            : 2010-2-24  
 Time            : 11:29
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IVUTIL_H_2010_2
#define _IVUTIL_H_2010_2

#include "..\..\devicecontrol\IIVDevice.h"
#include "assert.h"



//struct IVRuleInit:
//    public WPG_Rule
//{
//    IVRuleInit(IVRuleType t)
//    {
//
//
//    }
//};

namespace IVUtil
{


static const char* g_szDefaultRuleName[] =
{
    "Invade",              // 入侵
    "Leave Disappear",     // 消失/离开
    "LeftBehind",          // 遗留
    "Loiters",             // 徘徊
    "Statistic",           // 流量统计
    "Vehicle Retrograde",  // 车辆逆行
    "Illegal Parking",     // 非法停车
    "Stage Change",        // 场景变换
    ""
};

static void InitWPGRuleByType(WPG_Rule* pRule, IVRuleType t)
{
    ZeroMemory(pRule, sizeof(WPG_Rule));
    IV_RuleID* RuleID = (IV_RuleID*)pRule->ruleId;
    RuleID->RuleID.Init(t);
    strcpy(pRule->ruleName, g_szDefaultRuleName[t]);
    pRule->isEnabled = TRUE;
    pRule->useShapeDirectionFilter = TRUE;

    const static WPG_RectangleF s_DefaultfarRectangle = {0, 0, 0.4f,0.4f};
    const static WPG_RectangleF s_DefaultnearRectangle = {0.5f,0.5f,0.4f,0.4f};

    pRule->minimumFilter.farRectangle = s_DefaultfarRectangle;
    pRule->minimumFilter.nearRectangle = s_DefaultnearRectangle;
    pRule->maximumFilter.farRectangle = s_DefaultfarRectangle;
    pRule->maximumFilter.nearRectangle = s_DefaultnearRectangle;
    pRule->maxSizeChangeFilter.maxSizeRatio = 1;
    pRule->isCountEvent = FALSE;
    WPG_EventDescription& des = pRule->ruleDescription;
    des.targetClassification = TARGET_CLASSIFICATION_ANYTHING;
    switch(t)
    {
    case IV_Invade:
        break;
    case IV_Leave_Disappear:
        break;
    case IV_LeftBehind:
    {
        des.type = AOI_EVENT;
        des.description.aoiEventDescription.viewType = PARTIAL_VIEW;
        WPG_LeftBehind& LeftBehind = des.description.aoiEventDescription.actionType.leftBehind;
        LeftBehind.duration = Default_LeftBehind_duration;
        LeftBehind.leftBehindEnable = TRUE;
        break;
     }
    case IV_Loiters:
    {
        des.type = AOI_EVENT;
        des.description.aoiEventDescription.viewType = PARTIAL_VIEW;
        WPG_Loiters& Loiters = des.description.aoiEventDescription.actionType.loiters;
        Loiters.duration = Default_Loiters_duration;
        Loiters.loitersEnable = TRUE; 
        break;
    }
    case IV_Statistic:
        break;
    case IV_Vehicle_Retrograde:
        break;
    case IV_Illegal_Parking:
        break;
    case IV_Stage_Change:
        des.type = SCENE_CHANGE_EVENT;
        break;
    default:
        assert(false);
        break;
    }
}

//static void SetDefaultRuleDes(WPG_EventDescriptionUnion& des)
//{
//    
//}

static void InvadeOrLeaveChangeToLine(WPG_EventDescriptionUnion& des)
{
    ZeroMemory(&des, sizeof(WPG_EventDescriptionUnion));
    WPG_TripwireEventDescription& LineDes = des.tripwireEventDescription;
}

static void InvadeChangeToZone(WPG_EventDescriptionUnion& des)
{
    ZeroMemory(&des, sizeof(WPG_EventDescriptionUnion));
    WPG_AOIEventDescription& aoiDes = des.aoiEventDescription;
    aoiDes.actionType.appears = TRUE;
    aoiDes.actionType.enters = TRUE;
    aoiDes.actionType.inside = TRUE;
    aoiDes.viewType = PARTIAL_VIEW;
}

static void LeaveChangeToZone(WPG_EventDescriptionUnion& des)
{
    ZeroMemory(&des, sizeof(WPG_EventDescriptionUnion));
    WPG_AOIEventDescription& aoiDes = des.aoiEventDescription;
    aoiDes.actionType.disappears = TRUE;
    aoiDes.actionType.exits = TRUE;
    aoiDes.viewType = PARTIAL_VIEW;
}


static void WPGRectToPointList(
    const WPG_RectangleF& Rect,
    const CRect& WindowRect,
    CPoint (&szPointBuf)[4] )
{
    int nWidth = WindowRect.Width();
    int nHeight = WindowRect.Height();
    szPointBuf[0].x = int(Rect.x*nWidth);
    szPointBuf[0].y = int(Rect.y*nHeight);
    szPointBuf[1].x = int((Rect.x+Rect.width)*nWidth);
    szPointBuf[1].y = szPointBuf[0].y;

    szPointBuf[2].x = szPointBuf[1].x;
    szPointBuf[2].y = int((Rect.y+Rect.height)*nHeight);;
    szPointBuf[3].x = szPointBuf[0].x;
    szPointBuf[3].y = szPointBuf[2].y;
}

static void PointListToWPGRect(
    const CPoint (&szPointBuf)[4],
    const CRect& WindowRect,
    WPG_RectangleF& Rect )
{
    int nWidth = WindowRect.Width();
    int nHeight = WindowRect.Height();
    Rect.x = float(szPointBuf[0].x)/nWidth;
    Rect.y = float(szPointBuf[0].y)/nHeight;
    Rect.width = float(szPointBuf[2].x-szPointBuf[0].x)/nWidth;
    Rect.height = float(szPointBuf[2].y-szPointBuf[0].y)/nHeight;
}

}

#endif  // _IVUTIL_H_2010_


 // End of file


//
//typedef struct WPG_TripwireEventDescription
//{
//    WPG_PointF startPoint;				/**< Starting point of the tripwire. */
//    WPG_PointF endPoint;					/**< End point of the tripwire. */
//    WPG_LINE_CROSS_DIRECTION direction;	/**< @copydoc WPG_LINE_CROSS_DIRECTION */
//} WPG_TripwireEventDescription;
//
//typedef enum WPG_VIEW_TYPE
//{
//    FULL_VIEW,
//    PARTIAL_VIEW
//} WPG_VIEW_TYPE;
//
//
//typedef struct WPG_AoiActionType
//{
//    WPG_Bool enters;				/**< Flag to indicate whether Enters is enabled. */
//    WPG_Bool exits;				/**< Flag to indicate whether Exits is enabled. */
//    WPG_Bool inside;				/**< Flag to indicate whether Inside is enabled. */
//    WPG_Bool appears;			/**< Flag to indicate whether Appears is enabled. */
//    WPG_Bool disappears;			/**< Flag to indicate whether Disappears is enabled. */
//    WPG_Bool takenAway;			/**< Flag to indicate whether TakenAway is enabled. */
//    WPG_LeftBehind leftBehind;	/**< @copydoc WPG_LeftBehind */
//    WPG_Loiters loiters;			/**< @copydoc WPG_Loiters */
//} WPG_AOI_ACTION_TYPE;
//
//typedef struct WPG_AOIEventDescription
//{
//    WPG_AOI_ACTION_TYPE  actionType;	/**< @copydoc WPG_AOI_ACTION_TYPE */
//    WPG_VIEW_TYPE        viewType;	/**< @copydoc WPG_VIEW_TYPE */
//    WPG_PolygonF         polygon;	/**< @copydoc WPG_PolygonF */
//    WPG_PLANE_TYPE       planeType;  /**< @copydoc WPG_PLANE_TYPE */
//} WPG_AOIEventDescription;
//
//typedef struct WPG_SceneChangeEventDescription
//{
//    unsigned int sceneChangeType;	/**< Takes values from #WPG_SCENE_CHANGE_TYPE as single values
//                                    or an ORed combination. */
//}WPG_SceneChangeEventDescription;