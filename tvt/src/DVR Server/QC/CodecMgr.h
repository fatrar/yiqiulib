#ifndef _CODECMGR_H_
#define _CODECMGR_H_

#include <vfw.h>
class CCodecMgr
{
public:
	BOOL DecodeVideoDataH(char *pin,DWORD len,char* pout,DWORD *lenr,DWORD flag);//zld 2009/4/27 4408
	BOOL InitCodecH(int Res);//zld 2009/4/27 4408
	CSize cs;
	void SetFormat(CSize csize);
	void DestroyCodecA();
	BOOL InitCodecA();
	
	void DestroyCodecV();
	BOOL InitCodecV(int Res);
	virtual  ~CCodecMgr();
	CCodecMgr();
	BITMAPINFO m_BmpC;	
	BITMAPINFO m_BmpU;
	BOOL DecodeVideoData(char *pin,DWORD len,char* pout,DWORD *lenr,DWORD flag);
protected:
	COMPVARS m_cv;
	BOOL EncodeVideoData(char* pin,int len,char* pout,int* lenr,BOOL* pKey);
	BOOL EncodeAudioData(char *pin,int len,char* pout,int* lenr);
	BOOL DecodeAudioData(char *pin,int len,char* pout,int* lenr);
	HIC m_hIC;
};



#endif