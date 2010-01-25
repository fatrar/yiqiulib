/******************************************************************************
 * Copyright WPG System Pte. Ltd., all rights reserved.
 * Software and information contained herein is
 * confidential and proprietary to WPG System, and any unauthorized copying,
 * dissemination, or use is strictly prohibited.
 *
 * File Name: WPG_EventOccurence.h
 * 
 * Description:	This file defines the API used for the OnBoard alert API.
 * This allows the application utilizing the OnBoard libraries to receive
 * an alert directly on the device.
 *				
 *				  	
 ******************************************************************************/

#ifndef __WPG_EVENTOCCURRENCE_H__
#define __WPG_EVENTOCCURRENCE_H__

//#include "WPG_System.h"
//#include "WPG_Status.h"

/**
 * @file
 * Header file for the OnBoard EventOccurrence API
 */


/**
 * @defgroup WPG_EventOccurrence WPG OnBoard EventOccurrence API
 *  @ingroup WPGOnBoardAPI
 *
 * @brief	Interfaces for operating on events generated from the OnBoard library
 * @{
 */

#define WPG_MAX_NAME_STRING_LENGTH			128

/**
 * @brief	Length of a generic text string
 */
#define WPG_MAX_TEXT_STRING_LENGTH			1024

/**
 * @brief	Length of unique library ID returned by the library
 */
#define WPG_MAX_LIBRARY_ID_LENGTH				20

/**
 * @brief	Maximum number of polygon/multi-segment tripwire points.
 */
#define WPG_MAX_NUM_OF_POINTS				28

/**
 * @brief	Maximum number of slices returned with an event
 */
#define WPG_MAX_NUM_OF_SLICES_PER_EVENT		4

/**
 * @brief	Maximum number of custom response fields in an event
 */
#define WPG_MAX_CUSTOM_RESPONSE_FIELDS		8

/**
 * @brief	Length of a Custom Response Field key
 */
#define WPG_CUSTOM_RESPONSE_KEY_LENGTH		16*2	/* wchar_t */

/**
 * @brief	Length of a Custom Response Field value
 */
#define WPG_CUSTOM_RESPONSE_VALUE_LENGTH		64*2	/* wchar_t */

/**
 * @brief	Maximum number of target IDs stored in the ID list
 */
#define	WPG_MAXIMUM_TARGET_IDS_TO_KEEP		12		/* Maximum target ID's to retain for parent list. */
typedef void* WPG_Handle;

/**
 * @brief	Boolean value in the OnBoard API
 */
typedef unsigned int WPG_Bool;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	Line crossing directions for Tripwires
 */
typedef enum WPG_LINE_CROSS_DIRECTION
{
	INVALID_DIRECTION = -1,
	ANY_DIRECTION,
	LEFT_TO_RIGHT,
	RIGHT_TO_LEFT
} WPG_LINE_CROSS_DIRECTION;

/**
 * @brief	Plane type for AOI rules
 */
typedef enum WPG_PLANE_TYPE
{
	GROUND_PLANE,
	IMAGE_PLANE
} WPG_PLANE_TYPE;

/**
 * @brief	View type for AOI rules
 */
typedef enum WPG_VIEW_TYPE
{
	FULL_VIEW,
	PARTIAL_VIEW
} WPG_VIEW_TYPE;

/**
 * @brief	Supported target classifications
 *
 * @remark	Enum values are bit positions to be used as flags.
 */
typedef enum WPG_TARGET_CLASSIFICATION
{ 
	TARGET_CLASSIFICATION_HUMAN     =  1, 
	TARGET_CLASSIFICATION_VEHICLE   =  2,
	TARGET_CLASSIFICATION_UNKNOWN   =  4,
	TARGET_CLASSIFICATION_ANYTHING  =  7
} WPG_TARGET_CLASSIFICATION;

/**
 * @brief	Supported scene change types
 *
 * @remark	Enum values are bit positions to be used as flags.
 */
typedef enum WPG_SCENE_CHANGE_TYPE
{
	SCENE_CHANGE_LIGHT_ON         =   1,
	SCENE_CHANGE_LIGHT_OFF        =   2,
	SCENE_CHANGE_LIGHT_ON_OR_OFF  =   3,
	SCENE_CHANGE_UNKNOWN          =   4,
	SCENE_CHANGE_CAMERA_MOTION    =   8,
	SCENE_CHANGE_ANYTHING         =   15
} WPG_SCENE_CHANGE_TYPE;

/**
 * @brief	Supported event types
 */
typedef enum WPG_EVENT_TYPE
{
	INVALID_EVENT_TYPE = -1,
	TRIPWIRE_EVENT,
	AOI_EVENT,
	SCENE_CHANGE_EVENT,
	MULTILINE_TRIPWIRE_EVENT,
	MULTISEGMENT_TRIPWIRE_EVENT,
	MULTISEGMENT_MULTILINE_TRIPWIRE_EVENT,
	OCCUPANCY_EVENT,
	DWELLTIME_EVENT
} WPG_EVENT_TYPE;

/**
 * @brief	Counting relationship for occupancy threshold events
 */
typedef enum WPG_OCCUPANCY_COUNTING_MODE
{	
	WPG_OCCUPANCY_COUNTING_MODE_EQUAL,					/**< Alert when count is equal to specified number. */
	WPG_OCCUPANCY_COUNTING_MODE_GREATER_OR_EQUAL = 0x04,	/**< Alert when count is greater than or equal to specified number. */
	WPG_COUNTER_MODE_LESS_OR_EQUAL = 0x06				/**< Alert when count is less than or equal to specified number. */
	
}WPG_OCCUPANCY_COUNTING_MODE;

/**
 * @brief	Crossing order for multi-line tripwire rules
 */
typedef enum WPG_DOUBLE_TRIPWIRE_ORDER
{
	DOUBLE_TRIPWIRE_BEFORE          =  1,
	DOUBLE_TRIPWIRE_AFTER			=  1,
	DOUBLE_TRIPWIRE_BEFORE_OR_AFTER =  2
} WPG_DOUBLE_TRIPWIRE_ORDER;
/**
 * @brief	Structure definition for a Cartesian point
 */
typedef struct WPG_PointF
{
	float x;
	float y;
}WPG_PointF;

/**
 * @brief	Structure definition for a bounding box
 */
typedef struct WPG_RectangleF
{
	float x;
	float y;
	float width;
	float height;
}WPG_RectangleF;

/**
 * @brief	Structure definition for Library ID
 */
/*
typedef struct WPG_LibraryID
{
	char		libraryId[WPG_MAX_LIBRARY_ID_LENGTH];
}WPG_LibraryID;

*/
/**
 * @brief	Polygon structure to hold Cartesian points
 */
typedef struct WPG_PolygonF
{
	unsigned int	numPoints;						/**< Number of points in the polygon. */
	WPG_PointF		points[WPG_MAX_NUM_OF_POINTS];	/**< #WPG_PointF array of points. */
} WPG_PolygonF;


/**
 * @brief	Attributes of the LeftBehind AOI action type.
 */
typedef struct WPG_LeftBehind
{
	WPG_Bool      leftBehindEnable;	/**< Flag to indicate whether Left Behind is enabled. */
	unsigned int duration;			/**< Left Behind duration in seconds. */
} WPG_LeftBehind;

/**
 * @brief	Attributes of the Loiters AOI action type
 */
typedef struct WPG_Loiters
{
	WPG_Bool loitersEnable;	/**< Flag to indicate whether Loiters is enabled. */
	unsigned int duration;	/**< Loiters duration in seconds. */
} WPG_Loiters;

/**
 * @brief	Action types supported by AOI events
 */
typedef struct WPG_AoiActionType
{
	WPG_Bool enters;				/**< Flag to indicate whether Enters is enabled. */
	WPG_Bool exits;				/**< Flag to indicate whether Exits is enabled. */
	WPG_Bool inside;				/**< Flag to indicate whether Inside is enabled. */
	WPG_Bool appears;			/**< Flag to indicate whether Appears is enabled. */
	WPG_Bool disappears;			/**< Flag to indicate whether Disappears is enabled. */
	WPG_Bool takenAway;			/**< Flag to indicate whether TakenAway is enabled. */
	WPG_LeftBehind leftBehind;	/**< @copydoc WPG_LeftBehind */
	WPG_Loiters loiters;			/**< @copydoc WPG_Loiters */
} WPG_AOI_ACTION_TYPE;

/** 
 * @brief	Tripwire event description
 */
typedef struct WPG_TripwireEventDescription
{
	WPG_PointF startPoint;				/**< Starting point of the tripwire. */
	WPG_PointF endPoint;					/**< End point of the tripwire. */
	WPG_LINE_CROSS_DIRECTION direction;	/**< @copydoc WPG_LINE_CROSS_DIRECTION */
} WPG_TripwireEventDescription;

/** 
 * @brief	Area Of Interest (AOI) event description
 */
typedef struct WPG_AOIEventDescription
{
	WPG_AOI_ACTION_TYPE  actionType;	/**< @copydoc WPG_AOI_ACTION_TYPE */
	WPG_VIEW_TYPE        viewType;	/**< @copydoc WPG_VIEW_TYPE */
	WPG_PolygonF         polygon;	/**< @copydoc WPG_PolygonF */
	WPG_PLANE_TYPE       planeType;  /**< @copydoc WPG_PLANE_TYPE */
} WPG_AOIEventDescription;

/** 
 * @brief	Scene change event description
 */
typedef struct WPG_SceneChangeEventDescription
{
    unsigned int sceneChangeType;	/**< Takes values from #WPG_SCENE_CHANGE_TYPE as single values
									or an ORed combination. */
}WPG_SceneChangeEventDescription;


/**
 * @brief	Multi-line Tripwire event description
 */
typedef struct WPG_MultiLineTripWireEventDescription 
{
    WPG_DOUBLE_TRIPWIRE_ORDER     crossingDirection;				/**< @copydoc WPG_DOUBLE_TRIPWIRE_ORDER */     
	WPG_TripwireEventDescription  tripwireEventDescription[ 2 ];	/**< Two tripwires. */
	unsigned int                 duration;						/**< Time limit between crossings of the two tripwires. */
} WPG_MultiLineTripWireEventDescription;

/** 
 * @brief	Segments of a multi-segment tripwire
 */
typedef WPG_PolygonF WPG_TripwireSegments;

/** 
 * @brief	Multi-segment Tripwire event description
 */
typedef struct WPG_MultiSegTripwireEventDescription
{
	WPG_LINE_CROSS_DIRECTION		direction;		/**< @copydoc WPG_LINE_CROSS_DIRECTION */
	WPG_TripwireSegments			tripwirePoints;	/**< Cartesian points specifying start and end points of tripwire segments. */
} WPG_MultiSegTripwireEventDescription;


/** 
 * @brief	Multi-segment Multi-line Tripwire event description
 */
typedef struct WPG_MultiSegmentMultiLineTripWireEventDescription
{
	WPG_DOUBLE_TRIPWIRE_ORDER				crossingDirection;				/**< @copydoc WPG_DOUBLE_TRIPWIRE_ORDER */
	WPG_MultiSegTripwireEventDescription		tripwireEventDescription[ 2 ];	/**< Two multi-segment tripwires. */
	unsigned int							duration;						/**< Time limit between crossings of the two tripwires. */

}WPG_MultiSegmentMultiLineTripWireEventDescription;

/** 
 * @brief	Occupancy and Dwell Time event count data
 */
typedef struct WPG_OccupancyDwellTimePeopleCountData
{
	unsigned int	peopleCount;	/**< Set the person count threshold / returns the person count. */
	float			duration;		/**< Set the threshold duration / returns the dwelltime duration in seconds. */

}WPG_OccupancyDwellTimePersonCountData;

/** 
 * @brief	Occupancy and Dwell Time event data
 */
typedef struct WPG_OccupancyDwellTimeEventData
{
	WPG_Bool									isDataEvent;		/**< 0: Threshold event 1: Data event. */
	WPG_PolygonF								areaOfInterest;		/**< The area of interest. */
	WPG_PLANE_TYPE							planeType;			/**< @copydoc WPG_PLANE_TYPE */
	WPG_OccupancyDwellTimePersonCountData	peopleCountData;	/**< For threshold rules, defines the count and duration thresholds. In the event
																   returns the person count and dwell time. */
} WPG_OccupancyDwellTimeEventData;

/** 
 * @brief	Occupancy event description
 */
typedef struct WPG_OccupancyEventDescription
{
	WPG_OccupancyDwellTimeEventData		eventData;		/**< @copydoc WPG_OccupancyDwellTimeEventData */
	WPG_OCCUPANCY_COUNTING_MODE			countingMode;	/**< @copydoc WPG_OCCUPANCY_COUNTING_MODE */

} WPG_OccupancyEventDescription;

/** 
 * @brief	Dwell Time event description
 */
typedef struct WPG_DwellTimeEventDescription
{
	WPG_OccupancyDwellTimeEventData		eventData;	/**< @copydoc WPG_OccupancyDwellTimeEventData */

}WPG_DwellTimeEventDescription;

/**
 * @brief	Union aggregating all supported event types in the OnBoard library
 */
typedef union WPG_EventDescriptionUnion
{
	WPG_TripwireEventDescription							tripwireEventDescription;					/**< @copydoc WPG_TripwireEventDescription */
	WPG_AOIEventDescription								aoiEventDescription;						/**< @copydoc WPG_AOIEventDescription */
	WPG_SceneChangeEventDescription						sceneChangeEventDescription;				/**< @copydoc WPG_SceneChangeEventDescription */
	WPG_MultiLineTripWireEventDescription				multiLineTripWireEventDescription;			/**< @copydoc WPG_MultiLineTripWireEventDescription */
	WPG_MultiSegTripwireEventDescription					multiSegTripwireEventDescription;			/**< @copydoc WPG_MultiSegTripwireEventDescription */
	WPG_MultiSegmentMultiLineTripWireEventDescription	multiSegMultiLineTripwireEventDescription;	/**< @copydoc WPG_MultiSegmentMultiLineTripWireEventDescription */
	WPG_OccupancyEventDescription 						occupancyEventDescription;					/**< @copydoc WPG_OccupancyEventDescription */
	WPG_DwellTimeEventDescription						dwellTimeEventDescription;					/**< @copydoc WPG_DwellTimeEventDescription */
} WPG_EventDescriptionUnion;

/**
 * @brief	Custom Response Fields
 *
 * Key/Value pairs that specify user data to be returned with an event.
 * The OnBoard library does not process this data.
 */
typedef struct
{
	char key[WPG_CUSTOM_RESPONSE_KEY_LENGTH];		/**< Key string */
	char value[WPG_CUSTOM_RESPONSE_VALUE_LENGTH];	/**< Value string */
} WPG_CustomResponseField;

/**
 * @brief	Description of an event in the OnBoard library
 */
typedef struct WPG_EventDescription
{
	WPG_EVENT_TYPE               type;												/**< Event Type from #WPG_EVENT_TYPE. */
	WPG_EventDescriptionUnion    description;										/**< @copydoc WPG_EventDescriptionUnion */
	unsigned int                targetClassification;								/**< Takes values from #WPG_TARGET_CLASSIFICATION. */
	unsigned int				numResponseFields;									/**< Number of custom response fields in this event. */	
	WPG_CustomResponseField		responseFieldArray[WPG_MAX_CUSTOM_RESPONSE_FIELDS];	/**< Key/value pairs for the Custom Response Fields. */
} WPG_EventDescription;

/**
 * @brief	Parent target ID list for an WPG_Target
 */
typedef struct
{
	unsigned int count;										/**< Number of parent targets. */
	unsigned int targetIds[WPG_MAXIMUM_TARGET_IDS_TO_KEEP];	/**< Array of parent target IDs. */
} WPG_ParentTargetIDContainer;

/**
 * @brief	Description of a target in the OnBoard library
 */
typedef struct WPG_Target
{
	WPG_RectangleF boundingBox;						/**< @copydoc WPG_RectangleF */
	WPG_TARGET_CLASSIFICATION classificationType;	/**< Takes values from #WPG_TARGET_CLASSIFICATION. */
	unsigned int targetId;							/**< Target ID. */
	WPG_ParentTargetIDContainer parentTargetIds;		/**< @copydoc WPG_ParentTargetIDContainer */
	unsigned int equalId;							/**< Equal ID - whether the target is the same as a previously existent target. */
	WPG_PointF centroid;								/**< Centroid of the target. */
	WPG_PointF footPrint;							/**< Footprint of the target. */

} WPG_Target;

/**
 * @brief	Denotes one time instant in an event
 *
 * Most events in the OnBoard library will generate two slices, one corresponding
 * to the time of occurrence, and one from a prior time showing the target before
 * the time of occurrence. The Multi-line Tripwire event will generate four slices,
 * two for each tripwire, and a AOI Loiters event will generate a single slice.
 *
 * @sa		WPG_GetSnapshotByID() for usage of the #snapshotID member
 */
typedef struct WPG_EventSlice
{
	WPG_Target target;				/**< @copydoc WPG_Target */
	int millisOffset;				/**< Time offset in milliseconds from event occurrence time. Zero for first slice. */
	unsigned int snapshotLength;	/**< Length of image snapshot associated with this slice. */
	unsigned char* pSnapshot;		/**< Address of buffer from where application must retrieve this snapshot. */
	unsigned int	direction;		/**< The direction of target motion as an angle - positive X direction being 0 degrees. */
	unsigned char snapshotID[8];	/**< Snapshot ID that can be used to query for the snapshot post event retrieval. */

} WPG_EventSlice;

/**
 * @brief	Description of a single event occurrence from the OnBoard library
 */
typedef struct WPG_EventOccurrence
{
	unsigned char ruleId[16];								/**< Rule ID. @sa WPG_CreateGUID() for note on GUID generation. */
	char ruleName[WPG_MAX_NAME_STRING_LENGTH];				/**< Rule Name. */
	unsigned char viewId[16];								/**< View ID. */
	char viewName[WPG_MAX_NAME_STRING_LENGTH];				/**< View Name. */
	char eventText[WPG_MAX_TEXT_STRING_LENGTH];				/**< Text string returned with this event. This will be set when 
															the rule associated with this event is added. */
	unsigned char timestamp[8];								/**< Time of event occurrence. */
	WPG_EventDescription eventDescription;					/**< @copydoc WPG_EventDescription */
	unsigned int numOfSlices;								/**< Number of valid #WPG_EventSlice structures in this event. */
	WPG_EventSlice slices[WPG_MAX_NUM_OF_SLICES_PER_EVENT];	/**< Event Slices. */
	/* DND status */
	WPG_Bool		isNight;									/**< Flag indicating whether the event fired during night time.
															Only valid if the Day Night Detector is running. */
} WPG_EventOccurrence;





////////////////////////////////////////////////////Rule
#define MAX_NUM_OF_WEEKLY_TIME_SCHEDULES       24

typedef enum WPG_DAY_OF_WEEK
{
	WPG_SUNDAY,
	WPG_MONDAY,
	WPG_TUESDAY,
	WPG_WEDNESDAY,
	WPG_THURSDAY,
	WPG_FRIDAY,
	WPG_SATURDAY
}WPG_DAY_OF_WEEK;

/**
 * @brief	This structure contains the current date and time.
 * 
 * The informtion about the scheduling of alerts
 * (start and end time) has to be entered here. 
 */
typedef struct WPG_Time
{
    WPG_DAY_OF_WEEK dayOfWeek;	/**< The day of the week. */
    unsigned int hour;			/**< Hour in the day. */
    unsigned int min;			/**< Minutes in the hour. */
} WPG_Time;


/**
 * @brief	Specifies a time schedule with a start and end time
 */
typedef struct WPG_ScheduleWindow
{
    WPG_Time startTime;	/**< Start time for this schedule. */
    WPG_Time endTime;	/**< End time for this schedule. */
} WPG_ScheduleWindow;


/**
 * @brief	The time schedules that can be applied to a rule
 */

typedef struct WPG_Schedules
{
    unsigned int         numberOfScheduleTimeRanges;	/**< Number of schedules that are valid. */
    WPG_ScheduleWindow    scheduleTimeRanges [MAX_NUM_OF_WEEKLY_TIME_SCHEDULES]; /**< The actual schedules. */
} WPG_Schedules;


/**
 * @brief	Structure used to define the Minimum Size and Maximum Size filter
 */
typedef struct WPG_MIN_MAX_FILTER
{   
    WPG_Bool       useFilter;		/**< Whether the filter is active. */
    WPG_RectangleF farRectangle;		/**< Size of target in the far field. */
    WPG_RectangleF nearRectangle;	/**< Size of target in the near field. */
} WPG_MinMaxFilter;


/**
 * @brief	Structure used to define the Size Change filter
 */
typedef struct WPG_MAXIMUM_SIZE_CHANGE_FILTER
{
    WPG_Bool  useFilter;		/**< Whether the filter is active. */
    float maxSizeRatio;		/**< Maximum tolerable size change ratio between consecutive frames. */
} WPG_MaximumSizeChangeFilter;

/**
 * @brief	The rule description - is the same as the event description
 */
typedef struct WPG_EventDescription WPG_RuleDescription;

/**
 * @brief	A rule that can be applied to the OnBoard library
 */
typedef struct WPG_Rule
{
    unsigned char       ruleId [16];	/**< A unique ID for the rule. If not provided by the application, this is generated by a call to WPG_CreateGUID(). */
    char                ruleName [WPG_MAX_NAME_STRING_LENGTH]; /**< Rule name. */
    unsigned char       viewId [16];	/**< ID of the view to which this rule is to be applied. An all zero view ID denotes a sensor level rule. */
    char                viewName [WPG_MAX_NAME_STRING_LENGTH ];  /**< Name of the view to which the rule is to be applied. */
    char                eventText [WPG_MAX_TEXT_STRING_LENGTH];  /**< Text string to return with the event generated for this rule. */ 
    WPG_RuleDescription  ruleDescription;	/**< The rule description that includes the event type and associated attributes. */
    WPG_Bool             isEnabled;		/**< Whether the rule is enabled. */
    WPG_Schedules        schedule;		/**< Schedules associated with the rule. */
    WPG_Bool             useShapeDirectionFilter;	/**< Whether the shape direction filter should be used with this rule. */
    WPG_MinMaxFilter               minimumFilter;	/**< Minimum size filter to be used with this rule. */
    WPG_MinMaxFilter               maximumFilter;	/**< Maximum size filter to be used with this rule. */
    WPG_MaximumSizeChangeFilter    maxSizeChangeFilter;	/**< Maximum size change filter to be used with this rule. */
	WPG_Bool                       isCountEvent;	/**< Whether this rule should generate a count only event. */
} WPG_Rule;


/**
 * @brief	Lists the AOI action types that are allowed by this instance of the library
 */
typedef struct WPG_AOI_ACTIONS_ALLOWED
{
    WPG_Bool enters;
    WPG_Bool exits;
    WPG_Bool insides;
    WPG_Bool appears;
    WPG_Bool disappears;
    WPG_Bool taken_Away;
    WPG_Bool left_Behind;
    WPG_Bool loiters;
	WPG_Bool occupancy_Data;			/**< @deprecated Will not be set by the library. Refer #WPG_RULES_ALLOWED for alternative. */		
	WPG_Bool occupancy_Threshold;	/**< @deprecated Will not be set by the library. Refer #WPG_RULES_ALLOWED for alternative.*/		
	WPG_Bool dwelltime_Data;			/**< @deprecated Will not be set by the library. Refer #WPG_RULES_ALLOWED for alternative.*/
	WPG_Bool dwelltime_Threshold;	/**< @deprecated Will not be set by the library. Refer #WPG_RULES_ALLOWED for alternative.*/
} WPG_AOI_ACTIONS_ALLOWED;


/**
 * @brief	Lists the scene change types that are allowed by this instance of the library
 */
typedef struct WPG_SCENE_CHANGE_ALLOWED
{
    WPG_Bool lightsON;
    WPG_Bool lightsOFF;
    WPG_Bool anySceneChange;
} WPG_SCENE_CHANGE_ALLOWED;


/**
 * @brief	Lists the filter types that are allowed by this instance of the library
 */
typedef struct WPG_FILTERS_ALLOWED
{
    WPG_Bool maxFilter;
    WPG_Bool minFilter;
    WPG_Bool sizeChangeFilter;
    WPG_Bool shapeDirectionFilter;    /**< Salience filter. */
} WPG_FILTERS_ALLOWED;


/**
 * @brief	Lists the rule types that are allowed by this instance of the library
 */
typedef struct WPG_RULES_ALLOWED
{
    WPG_SCENE_CHANGE_ALLOWED sceneChangeAllowed;
    WPG_Bool tripwireAllowed;
    WPG_AOI_ACTIONS_ALLOWED aoiActionsAllowed;
    WPG_Bool multiLineTripwireAllowed;
	WPG_Bool multiSegTripwireAllowed;
	WPG_Bool	occupancyDataAllowed;
	WPG_Bool	occupancyThresholdAllowed;
	WPG_Bool dwelltimeDataAllowed;
	WPG_Bool dwelltimeThresholdAllowed;
	WPG_Bool fullViewAllowed;
	WPG_Bool partialViewAllowed;
}WPG_RULES_ALLOWED;


typedef enum WPG_VIEW_STATE
{
	WPG_BAD_SIGNAL,
	WPG_UNKNOWN_VIEW,
	WPG_GOOD_VIEW,
	WPG_INVALID_VIEW,
	WPG_SEARCHING_FOR_VIEW
} WPG_VIEW_STATE;




#ifdef __cplusplus
}
#endif

#endif // __WPG_EVENTOCCURRENCE_H__

