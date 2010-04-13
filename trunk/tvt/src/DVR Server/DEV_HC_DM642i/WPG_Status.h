/******************************************************************************
 * Copyright WPG System Pte. Ltd., all rights reserved.
 * Software and information contained herein is
 * confidential and proprietary to WPG System, and any unauthorized copying,
 * dissemination, or use is strictly prohibited.
 *
 * File Name:	WPG_Status.h
 * 
 * Description:	This defines all the enum states that the library
 *              will return as status.
 *				  	
 ******************************************************************************/
#ifndef __WPG_STATUS_H__
#define __WPG_STATUS_H__

/**
 * @file
 * Header file for the OnBoard WPG_STATUS codes
 */


/**
 * @defgroup WPG_Status WPG OnBoard API Status codes
 *  @ingroup WPGOnBoardAPI
 * 
 * @brief	API Status codes
 * @{
 */

/********************************************
* Macros to identify error and warning codes
*********************************************/

/** 
 * @brief	Use this macro if you want to find out if the return code is success 
 */
#define WPG_SUCCESS_CODE(errorCode)            (errorCode == WPG_STATUS_SUCCESS)

/** 
 * @brief	Use this macro if you want to find out if the return code is a warning
 */
#define WPG_WARNING_CODE(errorCode)        (errorCode < WPG_STATUS_WARNING_CODES_START)

/** 
 * @brief Use this macro if you want to find out if the return code is an error
 */
#define WPG_ERROR_CODE(errorCode)            (!WPG_SUCCESS_CODE(errorCode) && !WPG_WARNING_CODE(errorCode))

/** 
 * @brief	Use this macro if you want to find out if the return code is a warning or error
 */
#define WPG_ERROR_OR_WARNING_CODE(errorCode)       (WPG_WARNING_CODE(errorCode) || WPG_ERROR_CODE(errorCode)) 

/** 
 * @brief	Use this macro if you want to find out if the return code is success or warning
 */
#define WPG_SUCCESS_OR_WARNING_CODE(errorCode) (WPG_SUCCESS_CODE(errorCode) || WPG_WARNING_CODE(errorCode)) 

#ifdef __cplusplus
extern "C" {
#endif

/*****************
* enum definations
******************/
typedef enum 
{
    RULE_INIT_CODE              =  0x02,

	WPG_STATUS_SUCCESS			=  0,
	WPG_STATUS_FAIL				= -1,
	WPG_STATUS_UNKNOWN			= -2,
	WPG_STATUS_TIMEOUT			= -3, 
	WPG_NULL_ARGUMENTS			= -4, 
	WPG_BUFFER_FULL				= -5, 
	WPG_STATUS_BUSY				= -6,
	WPG_RESTART_REQUIRED			= -7,
	WPG_STATUS_PENDING			= -8,
	WPG_STATUS_KNOWN_VIEW		= -9,
	WPG_STATUS_INVALID_VIEW_ID	= -10,  
	WPG_STATUS_INVALID_RULE_ID	= -11,	
	WPG_STATUS_NOT_ACTIVATED		= -12,
	WPG_STATUS_RULE_NOT_ALLOWED  = -13,
    WPG_NO_VIDEO_FRAME			= -14,
    WPG_STATUS_NO_SPACE			= -15,
    WPG_STATUS_WARMUP			= -16,
    WPG_STATUS_DUPLICATE_VIEWNAME = -17,
    WPG_STATUS_DUPLICATE_VIEWID	= -18,
    WPG_STATUS_BAD_SIGNAL		= -19,
    WPG_STATUS_SINGLE_VIEW		= -20,
	WPG_STATUS_NO_MORE_VIEWS		= -21,
	WPG_STATUS_INVALID_CONTEXT	= -22,
	WPG_STATUS_NON_EXIST_VIEW	= -23,
	WPG_STATUS_BAD_INPUT_SIGNAL	= -24,
	WPG_STATUS_VIEW_STORE_FAILED	= -25,
	WPG_STATUS_UNKNOWN_VIEW		= -26,
	WPG_STATUS_TIME_EXPIRED		= -27,
	WPG_STATUS_PAR_MODEL_STORAGE_FAILED = -28,
	WPG_STATUS_MULTIVIEW_NOT_ALLOWED = -29,
	WPG_STATUS_BAD_PARAMETER = -30,
	WPG_STATUS_LICENSE_NOT_VALID=-31,
	WPG_STATUS_BAD_CALIBRATION_DATA = -32,
	WPG_STATUS_HEAP_NOT_AVAILABLE = -33,
	WPG_STATUS_HEAP_SIZE_NOT_MATCHING = -34,
	
	/* Storage related */
	WPG_STATUS_BAD_STORAGE_TYPE = -100,
	WPG_STATUS_STORAGE_WRITE_FAILED = -101,
	WPG_STATUS_NO_ROOM_IN_STORAGE = -102,

	/* Rule related */
	WPG_STATUS_RULE_VIEW_ID_MISMATCH = -200,
	WPG_STATUS_RULE_VIEW_NAME_MISMATCH = -201,
	WPG_STATUS_RULE_TOO_BIG = -202,
	WPG_STATUS_RULE_NOT_FOUND_IN_STORAGE = -203,
	WPG_STATUS_INVALID_MAX_RULES = -204,
	WPG_STATUS_INVALID_RULE_TYPE	= -205,
	WPG_STATUS_INVALID_RULE_LOCATION = -206,
	WPG_STATUS_RULE_CUSTOM_RESPONSE_MAX_EXCEEDED = -207,
	WPG_STATUS_RULE_PRESENT = -208,
	WPG_STATUS_INVALID_FILTER_DEFINITION = -209,

	/* Video related */
	WPG_STATUS_JPEG_ENC_FAIL = -300,
	WPG_STATUS_VIDEO_FRAME_RETRIEVE_FAIL = -301,
	WPG_STATUS_VIDEO_BUFFER_POOL_INSUFFICIENT = -302,

	/* Feature related */
	WPG_STATUS_FEATURE_NOT_ALLOWED = -400,

	/* Sliders */
	WPG_STATUS_SLIDER_DISABLED = -500,
	WPG_STATUS_SLIDER_UPDATE_FAILED = -501,
	
	/* Targets */
	WPG_STATUS_TARGET_NOT_FOUND = -600,

	/* Warnings */
	WPG_STATUS_WARNING_CODES_START=-10000,
	WPG_STATUS_WARNING_RESOURCE_LIMIT_MET = -10001,
	WPG_STATUS_WARNING_RULE_LOCATION_IN_JITTER_AREA = -10002,
	WPG_STATUS_WARNING_RULE_TYPE_NOT_RECOMMENDED = -10003,
	WPG_STATUS_WARNING_FRAME_PROCESSING_INCOMPLETE = -10004,
	WPG_STATUS_WARNING_CLASSIFICATION_IGNORED= -10005,
	WPG_STATUS_WARNING_INSUFFICIENT_BUFFER = -10006,
	WPG_STATUS_WARNING_MAX_FRAMERATE_EXCEEDED = -10007,
	WPG_STATUS_WARNING_INSUFFICIENT_DATA_IN_QUEUE = -10008

} WPG_STATUS;

/**
 * @}
 */


#ifdef __cplusplus
}
#endif
#endif // __WPG_STATUS_H__
