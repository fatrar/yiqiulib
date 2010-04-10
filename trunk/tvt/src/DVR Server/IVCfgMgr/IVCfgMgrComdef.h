/**H**************************************************************************
 File            : IVCfgMgrComdef.h
 Subsystem       : 
 Function Name(s): 
 Author          : YiQiu
 Date            : 2010-3-10  
 Time            : 17:42
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IVCFGMGRCOMDEF_H_2010_3
#define _IVCFGMGRCOMDEF_H_2010_3






#define _RootName       "IV"
#define _AutoRunChannel "AutoRun"

#define _ChannelName    "Ch"
#define _DataShowState  "ShowState"

#define _IdentityID     "ID"
#define _RuleName       "Name"
#define _RuleEnable     "Enable"
#define _RuleType       "Type"

#define _Rule           "Rule"
#define _Schudule       "Schudule"
#define _AlarmOut       "AlarmOut"

#define _Sun  "Sun."
#define _Mon  "Mon."
#define _Tue  "Tue."
#define _Wed  "Wed."
#define _Thu  "Thu."
#define _Fri  "Fri."
#define _Sat  "Sat."

enum
{
    Week_Count = 7,
};

static const char* g_szWeek[Week_Count] = 
{
   _Sun,_Mon,_Tue,_Wed,_Thu,_Fri, _Sat  
};

#define _IV_Config_File_Path  "C:\\IV_Settings.xml"

#define GetChannelName(i, szBuf) \
    char szBuf[32] = {0}; \
    sprintf(szBuf, "%s%d", _ChannelName, i);


struct MyAOIEventDes
{
    WPG_VIEW_TYPE  viewType:1;	
    WPG_PLANE_TYPE planeType:1;
    bool enters:1;			
    bool exits:1;				
    bool inside:1;			
    bool appears:1;			
    bool disappears:1;		
    bool takenAway:1;	

    bool loitersEnable:1;
    bool leftBehindEnable:1;
    short loitersTime;
    short leftBehindTime;

    WPG_PolygonF polygon;
};

//union MyEventDesUnion
//{
//    WPG_TripwireEventDescription tripwireEventDescription;
//    MyAOIEventDes	aoiEventDescription;
//    WPG_SceneChangeEventDescription	sceneChangeEventDescription;
//};

template<typename T>
struct MyRule
{
    //BYTE ruleId[16];	
    //char ruleName[WPG_MAX_NAME_STRING_LENGTH];  
    //bool isEnabled;
    //WPG_EVENT_TYPE   type;
    //WPG_MinMaxFilter            minimumFilter;
    //WPG_MinMaxFilter            maximumFilter;
    //WPG_MaximumSizeChangeFilter maxSizeChangeFilter;
    //template<typename T>
    void Init(const WPG_Rule& Rule);
    void ToWPGRule(WPG_Rule& Rule);
    MyRule(const WPG_Rule& Rule);
    MyRule(){}

    float maxSizeRatio;
    WPG_RectangleF farRectangle[2];	 // 0 minimumFilter, 1 maximumFilter
    WPG_RectangleF nearRectangle[2];
    bool useShapeDirectionFilter:1;
    bool useMinFilter:1;
    bool useMaxFilter:1;
    bool useSizeChangeFilter:1;
    
    unsigned int targetClassification:4;
    
    T description;
};

/**
*@note [] 搞不清楚为什么这个构造一定要放在头文件里面，cpp就出现链接错误
*/
template<typename T>
MyRule<T>::MyRule(const WPG_Rule& Rule)
{
    maxSizeRatio = Rule.maxSizeChangeFilter.maxSizeRatio;
    useSizeChangeFilter = Rule.maxSizeChangeFilter.useFilter != 0;
    useShapeDirectionFilter = Rule.useShapeDirectionFilter != 0;

    useMinFilter = Rule.minimumFilter.useFilter != 0;
    nearRectangle[0] = Rule.minimumFilter.nearRectangle;
    farRectangle[0] = Rule.minimumFilter.farRectangle;

    useMaxFilter = Rule.maximumFilter.useFilter != 0;
    nearRectangle[1] = Rule.maximumFilter.nearRectangle;
    farRectangle[1] = Rule.maximumFilter.farRectangle;

    targetClassification = Rule.ruleDescription.targetClassification;
    //type = Rule.ruleDescription.type;
    Init(Rule);
}

template<typename T>
WPG_Rule& operator<<(WPG_Rule& WPGRule, MyRule<T>& MyRuleT);



bool _AddRule(
    TiXmlElement* pRuleEle,
    const WPG_Rule& Rule);

bool _AddSchedule(
    TiXmlElement* pRuleEle,
    const ScheduleSettings& Sch );

#define _ModifyRule _AddRule 
#define _ModifySchedule _AddSchedule

bool _GetRule(
    TiXmlElement* pRuleEle,
    WPG_Rule& Rule);

bool _GetSchedule(
    TiXmlElement* pRuleEle,
    ScheduleSettings& Sch );

#endif  // _IVCFGMGRCOMDEF_H_2010_


 // End of file



