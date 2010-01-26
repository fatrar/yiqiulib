#include "stdAfx.h"
#include "CodecMgr.h"

CCodecMgr::CCodecMgr()
{
	m_hIC=NULL;
	ZeroMemory(&m_cv,sizeof(m_cv));
	m_cv.cbSize=sizeof(m_cv);
	m_cv.dwFlags=ICMF_COMPVARS_VALID ;
	m_cv.hic=m_hIC;
	m_cv.fccType=ICTYPE_VIDEO ;
	//3316
	m_cv.fccHandler=mmioFOURCC('T','V','T','A');//842289229;//v2

	m_cv.lpbiOut=0;
	m_cv.lKey=10;
	m_cv.lDataRate=6;
	m_cv.lQ=10000;
	
}
/*------------------------------------------------------------------------------*/
CCodecMgr::~CCodecMgr()
{
	DestroyCodecA();
	DestroyCodecV();
}
/*------------------------------------------------------------------------------*/
//初始化视频编码解码器
BOOL CCodecMgr::InitCodecV(int Res)
{
	DWORD ret;
	DestroyCodecV();
//将视频格式固定（这是非常常用的格式，一般的视频捕捉设备应该会支持）
	ZeroMemory(&m_BmpU,sizeof(m_BmpU));
	m_BmpU.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	m_BmpU.bmiHeader.biWidth=cs.cx;
	m_BmpU.bmiHeader.biHeight=cs.cy;
	m_BmpU.bmiHeader.biPlanes=1;	
	m_BmpU.bmiHeader.biBitCount=24;	
	m_BmpU.bmiHeader.biSizeImage=cs.cx*cs.cy*m_BmpU.bmiHeader.biBitCount/8;


	BOOL bRet=FALSE;
	m_hIC=ICOpen(ICTYPE_VIDEO,m_cv.fccHandler,ICMODE_DECOMPRESS);
	if(!m_hIC)
		goto RET;

	ret=ICDecompressGetFormat(m_hIC,&m_BmpU,&m_BmpC);
	if (ret)
		goto RET;

	m_cv.hic=m_hIC;
	m_cv.dwFlags=ICMF_COMPVARS_VALID;
	//3316
	//if(!ICSeqCompressFrameStart(&m_cv,&m_BmpU))
	//	goto RET;
	if(ICDecompressBegin(m_hIC,&m_BmpC,&m_BmpU)!=ICERR_OK)
		goto RET;

	bRet=TRUE;
	
RET:
	if(!bRet)
		DestroyCodecV();
	return bRet;
}

/*------------------------------------------------------------------------------*/
void CCodecMgr::DestroyCodecV()
{	
	if(m_hIC)
	{
		ICDecompressEnd(m_hIC);
		ICSeqCompressFrameEnd(&m_cv);
		ICCompressorFree(&m_cv);
		ICClose(m_hIC);
		m_hIC=NULL;
	}	
	
}
/*------------------------------------------------------------------------------*/
BOOL CCodecMgr::InitCodecA()
{	
	return TRUE;
}
/*------------------------------------------------------------------------------*/
void CCodecMgr::DestroyCodecA()
{

}
/*------------------------------------------------------------------------------*/
//压缩音频帧
BOOL CCodecMgr::EncodeAudioData(char *pin,int len,char* pout,int* lenr)
{
	BOOL bRet=FALSE;
	return bRet;
}
/*------------------------------------------------------------------------------*/
//解压缩音频帧
BOOL CCodecMgr::DecodeAudioData(char *pin,int len,char* pout,int* lenr)
{
	
	BOOL bRet=FALSE;
	return bRet;	
}
/*------------------------------------------------------------------------------*/
//解压缩视频帧
BOOL CCodecMgr::DecodeVideoData(char *pin,DWORD len,char* pout,DWORD *lenr,DWORD flag)
{
	if(!pin||!pout||!m_hIC)
		return FALSE;
	m_BmpC.bmiHeader.biSizeImage=len;
	DWORD ret=ICDecompress(m_hIC,
		NULL,
		&m_BmpC.bmiHeader,
		pin,
		&m_BmpU.bmiHeader,
		pout);
//	XTRACE("ICDecompress........\n");

	if(ret!=ICERR_OK)
		return FALSE;
	if(lenr)
		*lenr=m_BmpU.bmiHeader.biSizeImage;
	return TRUE;
}
/*------------------------------------------------------------------------------*/
//压缩视频帧
BOOL CCodecMgr::EncodeVideoData(char* pin,int len,char* pout,int* lenr,BOOL* pKey)
{
	
	BOOL bRet=FALSE;
	char*p;
	long s=8000;
	BOOL k;
	if(!pin||!pout||len!=(int)m_BmpU.bmiHeader.biSizeImage||!m_hIC)
		goto RET;
	
	p=(char*)ICSeqCompressFrame(&m_cv,0,pin,&k,&s);
	if(!p)
		goto RET;
	if(lenr)
		*lenr=s;
	if(pKey)
		*pKey=k;

	CopyMemory(pout,p,s);
	bRet=TRUE;
RET:
	return bRet;	
}

void CCodecMgr::SetFormat(CSize csize)
{
	cs=csize;
}

BOOL CCodecMgr::InitCodecH(int Res)
{
	DestroyCodecV();
	//将视频格式固定（这是非常常用的格式，一般的视频捕捉设备应该会支持）
	
	if (Res == 1)
	{
		m_cv.fccHandler=mmioFOURCC('H','2','6','4');
	}
	
	BOOL bRet=FALSE;
	m_hIC=ICOpen(ICTYPE_VIDEO,m_cv.fccHandler,ICMODE_DECOMPRESS);
	if(!m_hIC)
		goto RET;
	
	m_cv.hic=m_hIC;
	m_cv.dwFlags=ICMF_COMPVARS_VALID;
	//4408
	if(ICDecompressBegin(m_hIC,&m_BmpU,&m_BmpC)!=ICERR_OK)
		goto RET;
	
	bRet=TRUE;
	
RET:
	if(!bRet)
		DestroyCodecV();
	return bRet;
}

BOOL CCodecMgr::DecodeVideoDataH(char *pin, DWORD len, char *pout, DWORD *lenr, DWORD flag)
{
	if(!pin||!pout||!m_hIC)
		return FALSE;	
	m_BmpU.bmiHeader.biSizeImage=len;
	DWORD ret=ICDecompress(m_hIC,
		NULL,
		&m_BmpU.bmiHeader,
		pin,
		&m_BmpC.bmiHeader,
		pout);
	
	if(ret!=ICERR_OK)
		return FALSE;
	if(lenr)
		*lenr=m_BmpC.bmiHeader.biSizeImage;
	return TRUE;
}
