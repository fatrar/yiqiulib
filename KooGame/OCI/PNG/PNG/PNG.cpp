// PNG.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

enum RawImgFormat
{
    Raw_RGB_Unknown = -1,
    Raw_RGB_565,
    Raw_RGB_5551,
    Raw_RGB_888,
    Raw_RGB_8888,
};

struct RawImgHead
{
    unsigned int FormatFlag;
    unsigned int nPixelFormat:8;
    unsigned int nSize:8;
    unsigned int nWidth:16;
    unsigned int nHeight:16;
    unsigned char* pData;
};

static int ComputePitch( int nWidth, int nBPP )
{
    return( (((nWidth*nBPP)+31)/32)*4 );
}

class CImageFile2RawFile
{
public:
    void Convert(
        const char* pSrc,
        const char* pDst,
        RawImgFormat eImgFormat = Raw_RGB_Unknown
        )
    {
        CInitGDIPlus initGDIPlus;
        initGDIPlus.Init();

        {

        
        Gdiplus::Bitmap bmSrc( (CT2W)pSrc );
        if( bmSrc.GetLastStatus() != Gdiplus::Ok )
        {
            throw "Load Image File Failed!";
        }

        m_nHeight = bmSrc.GetHeight();
        m_nWidth = bmSrc.GetWidth();

        Gdiplus::PixelFormat eSrcPixelFormat = bmSrc.GetPixelFormat();
        UINT nBPP = 32;
        DWORD dwFlags = 0;
        Gdiplus::PixelFormat eDestPixelFormat = PixelFormat32bppRGB;
        if( eSrcPixelFormat&PixelFormatGDI )
        {
            nBPP = Gdiplus::GetPixelFormatSize( eSrcPixelFormat );
            eDestPixelFormat = eSrcPixelFormat;
        }
        if( Gdiplus::IsAlphaPixelFormat( eSrcPixelFormat ) )
        {
            nBPP = 32;
            dwFlags |= 0x01;//createAlphaChannel;
            eDestPixelFormat = PixelFormat32bppARGB;
        }
        m_pDst = new BYTE[nBPP/8*m_nHeight*m_nWidth];

        //BOOL bSuccess = Create( bmSrc.GetWidth(), bmSrc.GetHeight(), nBPP, dwFlags );
        //if( !bSuccess )
        //{
        //    return( E_FAIL );
        //}
        USES_ATL_SAFE_ALLOCA;
        Gdiplus::ColorPalette* pPalette = NULL;
        if( Gdiplus::IsIndexedPixelFormat( eSrcPixelFormat ) )
        {
            //assert(false);
            throw "Unsuport Indexed Pixel!";
            //UINT nPaletteSize = bmSrc.GetPaletteSize();
            //pPalette = static_cast< Gdiplus::ColorPalette* >( _ATL_SAFE_ALLOCA(nPaletteSize, _ATL_SAFE_ALLOCA_DEF_THRESHOLD) );

            //if( pPalette == NULL )
            //    return E_OUTOFMEMORY;

            //bmSrc.GetPalette( pPalette, nPaletteSize );

            //RGBQUAD argbPalette[256];
            //ATLASSERT( (pPalette->Count > 0) && (pPalette->Count <= 256) );
            //for( UINT iColor = 0; iColor < pPalette->Count; iColor++ )
            //{
            //    Gdiplus::ARGB color = pPalette->Entries[iColor];
            //    argbPalette[iColor].rgbRed = (BYTE)( (color>>RED_SHIFT) & 0xff );
            //    argbPalette[iColor].rgbGreen = (BYTE)( (color>>GREEN_SHIFT) & 0xff );
            //    argbPalette[iColor].rgbBlue = (BYTE)( (color>>BLUE_SHIFT) & 0xff );
            //    argbPalette[iColor].rgbReserved = 0;
            //}

            //SetColorTable( 0, pPalette->Count, argbPalette );
        }

        if( eDestPixelFormat == eSrcPixelFormat )
        {
            // The pixel formats are identical, so just memcpy the rows.
            Gdiplus::BitmapData data;
            Gdiplus::Rect rect( 0, 0, bmSrc.GetWidth(), bmSrc.GetHeight() );
            if(bmSrc.LockBits( &rect, Gdiplus::ImageLockModeRead, eSrcPixelFormat, &data )!=Gdiplus::Ok)
            {
                throw "bmSrc.LockBits E_OUTOFMEMORY";
                //return E_OUTOFMEMORY;
            }

            size_t nBytesPerRow = AtlAlignUp( nBPP*bmSrc.GetWidth(), 8 )/8;
            BYTE* pbDestRow = m_pDst;//static_cast< BYTE* >( GetBits() );
            BYTE* pbSrcRow = static_cast< BYTE* >( data.Scan0 );
            for( int y = 0; y < bmSrc.GetHeight(); y++ )
            {
                Checked::memcpy_s(pbDestRow, nBytesPerRow, pbSrcRow, nBytesPerRow);
                pbDestRow += nBytesPerRow;//GetPitch();
                pbSrcRow += data.Stride;
            }

            bmSrc.UnlockBits( &data );
        }
        else
        {
            size_t nBytesPerRow = AtlAlignUp( nBPP*bmSrc.GetWidth(), 8 )/8;
            // Let GDI+ work its magic
            Gdiplus::Bitmap bmDest( 
                bmSrc.GetWidth(),
                bmSrc.GetHeight(),
                nBytesPerRow, // GetPitch(),
                eDestPixelFormat, m_pDst); //static_cast< BYTE* >( GetBits() ) );
            Gdiplus::Graphics gDest( &bmDest );

            gDest.DrawImage( &bmSrc, 0, 0 );
        }

        }
        initGDIPlus.Unit();
    };
public:
    class CInitGDIPlus
    {
    public:
        inline CInitGDIPlus():m_dwToken(0){};
        inline void Init()
        {
            Gdiplus::GdiplusStartupInput input;
            Gdiplus::GdiplusStartupOutput output;
            Gdiplus::Status status = Gdiplus::GdiplusStartup( &m_dwToken, &input, &output );
            if( status != Gdiplus::Ok )
                throw "Init Gdiplus Failed!";
        }

        inline void Unit()
        {
            Gdiplus::GdiplusShutdown( m_dwToken );
        }
    private:
        DWORD m_dwToken;
    };

    CInitGDIPlus s_InitGDIPlus;
    unsigned char* m_pDst;
    unsigned int m_nHeight, m_nWidth;
};



//const char* pImageFile = "C:\\1.jpg";
//Gdiplus::Bitmap image( (CT2W)pImageFile );

template<typename T>
void Max(T x)
{
   // return T(0);
}

struct AA
{
    int a;
    char c;
    int hh;
};

//typedef int (AA::*a);
//typedef char (AA::*c);

// typename T, T RuleSettings::*P,
template<typename T, T AA::*U>
void Set(AA& t, T v)
{
    t.*U = v;
}

int _tmain(int argc, _TCHAR* argv[])
{
    AA a;

    Set<int, &AA::hh>(a, 1);
        

    int  p;
    Max(p);

    CImageFile2RawFile K;
    K.Convert("C:\\1.jpg", "C:\\2.bmp");
    ATL::CAtlFile Write;
    Write.Create(_T("C:\\3.bmp"), GENERIC_WRITE, FILE_SHARE_READ,
        CREATE_ALWAYS );
    DWORD dwErr= GetLastError();
    HRESULT hr = Write.Write(K.m_pDst, K.m_nHeight*K.m_nWidth*3);
    Write.Close();

// 	CImage image;
// 	image.Load(_T("C:\\1.jpg"));
// 
// 	//const void* p = image.GetBits();
// 	int nHeight = image.GetHeight();
// 	int nWidth = image.GetWidth();
// 	int nBufSize = nHeight*nWidth*4;
// 	char* pBuf = new char[nBufSize];
// 
// 	HBITMAP h = (HBITMAP)image;
//     int nSet = ::GetBitmapBits(h, nBufSize, pBuf);
// 
//     //void* pBit = image.GetBits();
//     //int nRc = memcmp(pBit, pBuf, 10000);
// 
// 	ATL::CAtlFile Write;
// 	Write.Create(_T("C:\\1.bmp"), GENERIC_WRITE, FILE_SHARE_READ,
// 		CREATE_ALWAYS );
// 	DWORD dwErr= GetLastError();
//     HRESULT hr = Write.Write(pBuf, nHeight*nWidth*3);
// 	//HRESULT hr = Write.Write(pBuf, nHeight*nWidth*4);
// 	Write.Close();
//     delete pBuf;

	return 0;
}


