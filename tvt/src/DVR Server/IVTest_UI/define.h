/**H**************************************************************************
 File            : define.h
 Subsystem       : 
 Function Name(s): 
 Author          : YiQiu
 Date            : 2010-3-26  
 Time            : 10:32
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _DEFINE_H_2010_3
#define _DEFINE_H_2010_3
#include "StdAfx.h"



enum 
{
    Window_X_Offset = 10,
    Window_Y_Offset = 10,

    PAL_SIF_Width = 352,
    PAL_SIF_Height = 288,

    NTSC_SIF_Width = 320,
    NTSC_SIF_Height = 240,
};

static const DWORD g_PALFormat[2] = {PAL_SIF_Width, PAL_SIF_Height};
static const DWORD g_NTSCFormat[2] = {NTSC_SIF_Width, NTSC_SIF_Height};

static const DWORD* g_pFormat[2] = {g_PALFormat, g_NTSCFormat};

static CRect g_PALVideoRect[4] = 
{
    CRect(
        Window_X_Offset, Window_Y_Offset,
        Window_X_Offset+g_PALFormat[0], Window_Y_Offset+g_PALFormat[1]),

    CRect(
        Window_X_Offset+g_PALFormat[0], Window_Y_Offset,
        Window_X_Offset+g_PALFormat[0]*2, Window_Y_Offset+g_PALFormat[1]),

    CRect(
        Window_X_Offset, Window_Y_Offset+PAL_SIF_Height,
        Window_X_Offset+g_PALFormat[0], Window_Y_Offset+g_PALFormat[1]*2),

    CRect(
        Window_X_Offset+g_PALFormat[0], Window_Y_Offset+g_PALFormat[1],
        Window_X_Offset+g_PALFormat[0]*2, Window_Y_Offset+g_PALFormat[1]*2),
};


static CRect g_NTSCVideoRect[4] = 
{
    CRect(
        Window_X_Offset, Window_Y_Offset,
        Window_X_Offset+g_NTSCFormat[0], Window_Y_Offset+g_NTSCFormat[1]),

    CRect(
        Window_X_Offset+g_NTSCFormat[0], Window_Y_Offset,
        Window_X_Offset+g_NTSCFormat[0]*2, Window_Y_Offset+g_NTSCFormat[1]),

    CRect(
        Window_X_Offset, Window_Y_Offset+PAL_SIF_Height,
        Window_X_Offset+g_NTSCFormat[0], Window_Y_Offset+g_NTSCFormat[1]*2),

    CRect(
        Window_X_Offset+g_NTSCFormat[0], Window_Y_Offset+g_NTSCFormat[1],
        Window_X_Offset+g_NTSCFormat[0]*2, Window_Y_Offset+g_NTSCFormat[1]*2),
};

static CRect* g_pVideoRect[2] = {g_PALVideoRect, g_NTSCVideoRect};






#endif  // _DEFINE_H_2010_


 // End of file



