// Img2RGB.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

namespace RGBFile
{

CImg2RGB::CImg2RGB()
    : m_pDst(NULL)
{
    Gdiplus::GdiplusStartupInput input;
    Gdiplus::GdiplusStartupOutput output;
    Gdiplus::Status status = Gdiplus::GdiplusStartup( &m_dwToken, &input, &output );
    if( status != Gdiplus::Ok )
        throw "Init Gdiplus Failed!";
}

CImg2RGB::~CImg2RGB()
{
    if ( m_pDst )
    {
        delete[] m_pDst;
    }
    Gdiplus::GdiplusShutdown( m_dwToken );
}

void CImg2RGB::Convert(
    const char* pSrc,
    const char* pDst,
    RawImgFormat eImgFormat )
{
    Gdiplus::Bitmap bmSrc( (CA2W)pSrc );
    if( bmSrc.GetLastStatus() != Gdiplus::Ok )
    {
        throw "Load Image File Failed!";
    }

    m_eImgFormat = eImgFormat;
    Gdiplus::PixelFormat eSrcPixelFormat = bmSrc.GetPixelFormat();
    bool bNeedConvert = false;
    bool bIsHasAlpha = false;
    switch (eSrcPixelFormat)
    {
    case PixelFormat24bppRGB:
        m_nSrcBPP = 24;
        if ( m_eImgFormat == Raw_RGB_Unknown )
        {
            m_eImgFormat = Raw_RGB_888;
        }
        if ( m_eImgFormat != Raw_RGB_888 )
        {
            bNeedConvert = true;
        }
        break;
    case PixelFormat32bppARGB:
        m_nSrcBPP = 32;
        bIsHasAlpha = true;
        if ( m_eImgFormat == Raw_RGB_Unknown )
        {
            m_eImgFormat = Raw_RGBA_8888;
        }
        if ( m_eImgFormat != Raw_RGBA_8888 )
        {
            bNeedConvert = true;
        }
        break;
    default:
        throw "只支持原始图的RGB数据格式为RGB24或者ARGB!";
    }

    m_nHeight = bmSrc.GetHeight();
    m_nWidth = bmSrc.GetWidth();

    Convert2RGB(pSrc, pDst, bmSrc, bNeedConvert, bIsHasAlpha);
}

bool CImg2RGB::OptimAlpha( unsigned char* pData )
{
    return false;
    // 暂时先写到这。
    struct RGBA
    {
        BYTE R; BYTE G; BYTE B; BYTE A;
    };

    bool bHas0 = 0, bHas255 = 0;
    RGBA* p = (RGBA*)pData;
    for ( unsigned int y = 0; y < m_nHeight; ++y )
    {
        for ( unsigned int x = 0; x<m_nWidth; ++x )
        {
            switch (p->A)
            {
            case 0:
                bHas0 = true;
            	break;
            case 255:
                bHas255 = true;
            	break;
            default:
            	return false;
            }
        }
    }

    if ( bHas0 && bHas255 )
    {
        // 5551, can
    }
    else if ( !bHas0 && bHas255 )
    {
        // 888, or 565
    } 
}

void CImg2RGB::Convert2RGB(
    const char* pSrc,
    const char* pDst,
    Gdiplus::Bitmap& bmSrc,
    bool bNeedConvert, bool bIsHasAlpha )
{
    USES_ATL_SAFE_ALLOCA;

    if( !bNeedConvert )
    {
        // The pixel formats are identical, so just memcpy the rows.
        Gdiplus::BitmapData data;
        Gdiplus::PixelFormat eSrcPixelFormat = bmSrc.GetPixelFormat();
        Gdiplus::Rect rect( 0, 0, m_nWidth, m_nHeight );
        Gdiplus::Status nRc = bmSrc.LockBits(
            &rect, Gdiplus::ImageLockModeRead,
            eSrcPixelFormat, &data );
        if(nRc!=Gdiplus::Ok)
        {
            throw "bmSrc.LockBits E_OUTOFMEMORY";
        }

        BYTE* pbSrcRow = static_cast< BYTE* >( data.Scan0 );
        if ( !bIsHasAlpha || !OptimAlpha(pbSrcRow) )
        {
            if ( m_nSrcBPP == 32 )
            {
                m_pDst = new BYTE[4*m_nHeight*m_nWidth];
                BYTE* pbDstTmp = m_pDst;
                BYTE* pbSrcTmp = pbSrcRow;
                for( unsigned int y = 0; y < m_nHeight; y++ )
                {
                    for ( unsigned int x = 0; x < m_nWidth; x++ )
                    {
                        pbDstTmp[0] = pbSrcTmp[2];
                        pbDstTmp[1] = pbSrcTmp[1];
                        pbDstTmp[2] = pbSrcTmp[0];
                        pbDstTmp[3] = pbSrcTmp[3];
                        pbDstTmp += 4;
                        pbSrcTmp += 4;
                    }
                }
            }
            else if ( m_nSrcBPP == 24 )
            {
                m_pDst = new BYTE[3*m_nHeight*m_nWidth];
                BYTE* pbDstTmp = m_pDst;
                BYTE* pbSrcTmp = pbSrcRow;
                for( unsigned int y = 0; y < m_nHeight; y++ )
                {
                    for ( unsigned int x = 0; x < m_nWidth; x++ )
                    {
                        pbDstTmp[0] = pbSrcTmp[2];
                        pbDstTmp[1] = pbSrcTmp[1];
                        pbDstTmp[2] = pbSrcTmp[0];
                        pbDstTmp += 3;
                        pbSrcTmp += 3;
                    }
                }
            }
            else{}

            //size_t nBytesPerRow = AtlAlignUp( m_nSrcBPP*m_nWidth, 8 )/8;
            //             for( unsigned int y = 0; y < m_nHeight; y++ )
            //             {
            //                 memcpy(pbDestRow, pbSrcRow, nBytesPerRow);
            //                 pbDestRow += nBytesPerRow;//GetPitch();
            //                 pbSrcRow += data.Stride;
            //             }
        }

        bmSrc.UnlockBits( &data );
    }
    else
    {
        throw "暂时不支持转换到565，5551!";
    }

    // Save

    char szPath[260] = {0};
    if (NULL == pDst)
    {
        int nLen = strlen(pSrc);
        int nPos = nLen-1;
        while ( nPos >= 0 && pSrc[nPos] != '.')
        {
            --nPos;
        }
        strncpy(szPath, pSrc, nPos+1);
        strcat(szPath, "xmg");
        pDst = szPath;
    }
    ATL::CAtlFile Write;
    HRESULT hr = Write.Create(
        pDst, GENERIC_WRITE, 
        FILE_SHARE_READ, CREATE_ALWAYS );
    if ( hr != S_OK)
    {
        throw "Can`t Create RGB File!";
    }
    RawImgHead head;
    head.FormatFlag = 0;
    head.nPixelFormat = m_eImgFormat;
    head.nSize = sizeof(head);
    head.nWidth = m_nWidth;
    head.nHeight = m_nHeight;
    hr = Write.Write(&head, sizeof(head));
    if ( hr != S_OK)
    {
        throw "Write RGB File Failed!";
    }
    hr = Write.Write(m_pDst, m_nSrcBPP/8*m_nHeight*m_nWidth);
    if ( hr != S_OK)
    {
        throw "Write RGB File Failed!";
    }
    Write.Flush();
    Write.Close();
}

}