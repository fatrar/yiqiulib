// stdafx.cpp : source file that includes just the standard includes
//	DEV_HC_DM642.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


/*
一.
修改内容 : 实现局域网实时传输
修改方案 : 复制录像流数据，生成另外一个新流(实时网络流)，在网络上传输，发送到客户端
修 改 人 : 张振
修改日期 : 2008/01/23
修改标示 : <REC-NET>
*/

//{For Test
#ifdef TEST_VERSION
#include <Afxmt.h>
void WriteTestLog(LPCSTR pstrFormat...)
{
	static CCriticalSection	cs;
	static FILE*	pf = NULL;
	static BOOL		b = FALSE;

	cs.Lock();

	if(!b)
	{
		pf = fopen("D:\\SDK.txt", "w");
		b = TRUE;
	}

	va_list vaList;
	va_start(vaList, pstrFormat);

	vfprintf(pf, pstrFormat, vaList);

	va_end(vaList);

	cs.Unlock();
}


void WriteTestLog(int fileno, LPCSTR pstrFormat...)
{
#define	MAX_FILE_NO	16*3
	static BOOL		bInit	= FALSE;
	static CCriticalSection	csInit;
	static CCriticalSection	cs[MAX_FILE_NO];
	static FILE*	pf[MAX_FILE_NO];
	static BOOL		b[MAX_FILE_NO];
	static LPCSTR	FileName[MAX_FILE_NO] = {
		"D:\\PrvFRM\\1.txt",
		"D:\\PrvFRM\\2.txt",
		"D:\\PrvFRM\\3.txt",
		"D:\\PrvFRM\\4.txt",
		"D:\\PrvFRM\\5.txt",
		"D:\\PrvFRM\\6.txt",
		"D:\\PrvFRM\\7.txt",
		"D:\\PrvFRM\\8.txt",
		"D:\\PrvFRM\\9.txt",
		"D:\\PrvFRM\\10.txt",
		"D:\\PrvFRM\\11.txt",
		"D:\\PrvFRM\\12.txt",
		"D:\\PrvFRM\\13.txt",
		"D:\\PrvFRM\\14.txt",
		"D:\\PrvFRM\\15.txt",
		"D:\\PrvFRM\\16.txt",
		"D:\\CapFRM\\1.txt",
		"D:\\CapFRM\\2.txt",
		"D:\\CapFRM\\3.txt",
		"D:\\CapFRM\\4.txt",
		"D:\\CapFRM\\5.txt",
		"D:\\CapFRM\\6.txt",
		"D:\\CapFRM\\7.txt",
		"D:\\CapFRM\\8.txt",
		"D:\\CapFRM\\9.txt",
		"D:\\CapFRM\\10.txt",
		"D:\\CapFRM\\11.txt",
		"D:\\CapFRM\\12.txt",
		"D:\\CapFRM\\13.txt",
		"D:\\CapFRM\\14.txt",
		"D:\\CapFRM\\15.txt",
		"D:\\CapFRM\\16.txt",
		"D:\\NetFRM\\1.txt",
		"D:\\NetFRM\\2.txt",
		"D:\\NetFRM\\3.txt",
		"D:\\NetFRM\\4.txt",
		"D:\\NetFRM\\5.txt",
		"D:\\NetFRM\\6.txt",
		"D:\\NetFRM\\7.txt",
		"D:\\NetFRM\\8.txt",
		"D:\\NetFRM\\9.txt",
		"D:\\NetFRM\\10.txt",
		"D:\\NetFRM\\11.txt",
		"D:\\NetFRM\\12.txt",
		"D:\\NetFRM\\13.txt",
		"D:\\NetFRM\\14.txt",
		"D:\\NetFRM\\15.txt",
		"D:\\NetFRM\\16.txt"
	};

	csInit.Lock();
	if(!bInit)
	{
		memset(pf, 0, sizeof(pf));
		memset(b,  0, sizeof(b));
		bInit	= TRUE;
	}
	csInit.Unlock();

	cs[fileno].Lock();

	if(!(b[fileno]))
	{
		pf[fileno] = fopen(FileName[fileno], "a");
		b[fileno] = TRUE;
	}

	va_list vaList;
	va_start(vaList, pstrFormat);

	vfprintf(pf[fileno], pstrFormat, vaList);

	va_end(vaList);

	{
		fclose(pf[fileno]);
		b[fileno] = FALSE;
	}
	cs[fileno].Unlock();
}

void PrintFrameRate(int chl, int streamtype)
{
	static float FrameRate[16*3] = {0};
//	static float FrameRate8000[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    static int nFrameNum[16*3] = {0};
//	static int nFrameNum8000[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	static DWORD dwStartTime[16*3] = {0};

	int	no = -1;
	switch(streamtype)
	{
	case VIDEO_STREAM_PREVIEW:
			no = chl;
			break;
	case VIDEO_STREAM_CAPTURE:
			no = 16 + chl;
			break;
	case VIDEO_STREAM_NET:
			no = 32 + chl;
			break;
	}

	DWORD	dwNow = GetTickCount();

	if(dwStartTime[no] == 0)
	{
		dwStartTime[no] = dwNow;
	}

	nFrameNum[no] += 1;
	if(dwNow - dwStartTime[no] >= 10000)
	{
		FrameRate[no] = nFrameNum[no]*1000.0/(dwNow - dwStartTime[no]);
		WriteTestLog(no, "%f\n", FrameRate[no]);
		nFrameNum[no] = 0;
		dwStartTime[no] = dwNow;
	}
}
#endif
//}


//CAutoCriticalSection::CAutoCriticalSection( CCriticalSection& sec ):m_cs(sec)
//{
//    m_cs.Lock();
//}
//
//CAutoCriticalSection::~CAutoCriticalSection()
//{
//    m_cs.Unlock();
//}