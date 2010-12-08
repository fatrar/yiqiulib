/**H**************************************************************************
 File            : Img2RGB.h
 Subsystem       : 
 Function Name(s): CImg2RGB
 Author          : YiQiu
 Date            : 2010-12-8  
 Time            : 14:27
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _IMG2RGB_H_2010_12
#define _IMG2RGB_H_2010_12

#include "RGBFileDef.h"


namespace RGBFile
{

class CImg2RGB
{
public:
    CImg2RGB();
    ~CImg2RGB();

public:
    void Convert(
        const char* pSrc,
        RawImgFormat eImgFormat = Raw_RGB_Unknown);

    void Save(const char* pDst);

protected:
    bool OptimAlpha(unsigned char* pData);

public:
    unsigned char* m_pDst;
    RawImgFormat m_eImgFormat;
    UINT m_nSrcBPP;
    unsigned int m_nHeight, m_nWidth;

    DWORD m_dwToken;
};


}

#endif  // _IMG2RGB_H_2010_


 // End of file



