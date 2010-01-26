// MyWaveOut.cpp: implementation of the CMyWaveOut class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyWaveOut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyWaveOut::CMyWaveOut()
{
	TRACE("WaveOut\n");
	m_iBufIndex=0;
	m_hWaveOut=NULL;
	WAVEFORMATEX wavf;
	wavf.wFormatTag=WAVE_FORMAT_PCM;
	wavf.nChannels=1;
	wavf.nSamplesPerSec=8000;
	wavf.wBitsPerSample=8;
	wavf.cbSize=0;
	wavf.nBlockAlign=wavf.nChannels*wavf.wBitsPerSample/8;
	wavf.nAvgBytesPerSec=wavf.nSamplesPerSec*wavf.nBlockAlign;
	if(MMSYSERR_NOERROR != waveOutOpen(&m_hWaveOut,WAVE_MAPPER,&wavf,(DWORD)waveOutProc,(DWORD)this,CALLBACK_FUNCTION))
	{
		m_hWaveOut=NULL;
	}
	for(int i=0;i<MAX_BUF;i++)
	{
		m_WaveHeader[i].lpData=new char[AUDIO_BUF_LEN];
		m_WaveHeader[i].dwBufferLength=AUDIO_BUF_LEN;
		m_WaveHeader[i].dwBytesRecorded=0;
		m_WaveHeader[i].dwLoops=1;
		m_WaveHeader[i].dwUser=0;
		m_WaveHeader[i].dwFlags =WHDR_DONE ;
		waveOutPrepareHeader(m_hWaveOut,&m_WaveHeader[i],sizeof(WAVEHDR));
	}
}

CMyWaveOut::~CMyWaveOut()
{
	waveOutReset(m_hWaveOut);
	for(int i=0;i<MAX_BUF;i++)
	{
		waveOutUnprepareHeader(m_hWaveOut,&m_WaveHeader[i],sizeof(WAVEHDR));
		delete m_WaveHeader[i].lpData;
	}
	if(m_hWaveOut)
		waveOutClose(m_hWaveOut);
	m_hWaveOut=NULL;
}

void CALLBACK CMyWaveOut::waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
//	TRACE("CALLBACK MyWaveOut\n");
	switch(uMsg)
	{
	case WOM_DONE:
		{
			WAVEHDR *pHeader=(WAVEHDR*)dwParam1;
			pHeader->dwUser = 0;
		}
		break;
	}
}

void CMyWaveOut::WriteBuf(BYTE *pByte, DWORD len)
{
	if(m_hWaveOut==NULL) return;
	if(m_WaveHeader[m_iBufIndex].dwUser==0)
	{
		memcpy(m_WaveHeader[m_iBufIndex].lpData,pByte,len);
		m_WaveHeader[m_iBufIndex].dwBufferLength =len;
		m_WaveHeader[m_iBufIndex].dwUser=1;
		waveOutWrite (m_hWaveOut,&(m_WaveHeader[m_iBufIndex]),sizeof( WAVEHDR));
		m_iBufIndex++;
		if(m_iBufIndex>=MAX_BUF)
			m_iBufIndex=0;
	}
}

/**********************************************************************
* �������ƣ�  SetVolume
* ����������  ������Ƶ������
* ���������  dwVolume:����ֵ
* ���������  ��
* �� �� ֵ��  �� 
* �޸�����        �汾��     �޸���	      �޸�����
* -----------------------------------------------
* 2005/09/13	   V1.0	     Ԭʯά	        ����
***********************************************************************/
void CMyWaveOut::SetVolume(DWORD dwVolume)
{
	if (0x00000000 <= dwVolume && dwVolume <= 0xFFFFFFFF)
	{
		waveOutSetVolume(m_hWaveOut, dwVolume);
	}
}//SetVolume end.


/**********************************************************************
* �������ƣ�  SetPlaybackRate
* ����������  ������Ƶ�����ٶ�
* ���������  dwRate:����
* ���������  ��
* �� �� ֵ��  �� 
* �޸�����        �汾��     �޸���	      �޸�����
* -----------------------------------------------
* 2005/09/13	   V1.0	     Ԭʯά	        ����
***********************************************************************/
void CMyWaveOut::SetPlaybackRate(DWORD dwRate)
{
}//SetPlaybackRate end.


/**********************************************************************
* �������ƣ�  Stop
* ����������  ֹͣ������Ƶ
* ���������  ��
* ���������  ��
* �� �� ֵ��  �� 
* �޸�����        �汾��     �޸���	      �޸�����
* -----------------------------------------------
* 2005/09/13	   V1.0	     Ԭʯά	        ����
***********************************************************************/
void CMyWaveOut::Stop()
{
	waveOutReset(m_hWaveOut);
	for(int i=0;i<MAX_BUF;i++)
	{
		waveOutUnprepareHeader(m_hWaveOut,&m_WaveHeader[i],sizeof(WAVEHDR));
		delete m_WaveHeader[i].lpData;
	}
}//Stop end.
