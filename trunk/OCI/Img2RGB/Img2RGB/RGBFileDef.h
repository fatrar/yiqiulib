/**H**************************************************************************
 File            : RGBFileDef.h
 Subsystem       : 
 Function Name(s): CRGBFileDef
 Author          : YiQiu
 Date            : 2010-12-8  
 Time            : 14:33
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _RGBFILEDEF_H_2010_12
#define _RGBFILEDEF_H_2010_12


namespace RGBFile
{

enum RawImgFormat
{
    Raw_RGB_Unknown = -1,
    Raw_RGB_565,
    Raw_RGBA_5551,
    Raw_RGB_888,
    Raw_RGBA_8888,
    Raw_RGBA_6666,

    Pack_RGB_PVRTC4,
    Pack_RGB_PVRTC2,
    Pack_RGBA_PVRTC4,
    Pack_RGBA_PVRTC2
};

struct RawImgHead
{
    unsigned int FormatFlag;
    unsigned int nPixelFormat:8;
    unsigned int nSize:8;
    unsigned int nReserve:16;
    unsigned int nWidth:16;
    unsigned int nHeight:16;
};

};






#endif  // _RGBFILEDEF_H_2010_


 // End of file



