// PNG.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	//CImage::CInitGDIPlus GDI;
	//GDI.Init();

	CImage image;
	image.Load(_T("C:\\1.png"));

	//const void* p = image.GetBits();
	int nHeight = image.GetHeight();
	int nWidth = image.GetWidth();
	int nBufSize = nHeight*nWidth*4;
	void* pBuf = new char[nBufSize];

	HBITMAP h = (HBITMAP)image;
	GetBitmapBits(h, nBufSize, pBuf);

	ATL::CAtlFile Write;
	Write.Create(_T("C:\\1.bmp"), GENERIC_WRITE, FILE_SHARE_READ,
		CREATE_ALWAYS );
	DWORD dwErr= GetLastError();
	HRESULT hr = Write.Write(pBuf, nHeight*nWidth*4);
	Write.Close();
	//GDI.ReleaseGDIPlus();
	return 0;
}

