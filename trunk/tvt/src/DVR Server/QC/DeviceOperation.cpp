// DeviceOperation.cpp: implementation of the CDeviceOperation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdio.h"
#include "malloc.h"
#include "winioctl.h"
#include "DeviceOperation.h"
#include "./head/IOCTL.h"
#include "./head/DefineBase.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDeviceOperation::CDeviceOperation()
{
	m_bStartCapture = FALSE;
	m_hDevice = INVALID_HANDLE_VALUE;
	memset(m_hAudioEvent, 0x00, sizeof(m_hAudioEvent));
	memset(m_hVideoEvent, 0x00, sizeof(m_hVideoEvent));
	memset(m_pbyAudioBuf, 0x00, sizeof(m_pbyAudioBuf));
	memset(m_pbyVideoBuf, 0x00, sizeof(m_pbyVideoBuf));
	
	memset(m_pbyEncoderBuf, 0x00, sizeof(m_pbyVideoBuf));
	memset(m_pbyDecoderBuf, 0x00, sizeof(m_pbyVideoBuf));
	m_byDeviceID = 0;
//	m_byMaxDevice = 0;
	m_byMaxChannel = 0;
}

CDeviceOperation::~CDeviceOperation()
{
	DeviceClose();

}

BOOL CDeviceOperation::IsInitDevice(BYTE *pbyMaxChannel)//BYTE *pbyMasterChip, BYTE *pbyMaxDevice, BYTE *pbyMaxChannel)
{
	CHAR				szDeviceName[32];
	BOOL				bRet = FALSE;
	BYTE				m_byMaxDevice=0;
	BYTE				m_byMasterChip=0;
	_snprintf(szDeviceName, 32, RUBY3_DEVICE_NAME, 0);
	if(DeviceCreate(szDeviceName, 0) == TRUE)
	{
		if(GetDeviceInfo(&m_byMasterChip, &m_byMaxDevice, &m_byMaxChannel) == FALSE)
		{
			;//OutputDebugString(szDebug);
		}
		else
		{
//			*pbyMaxDevice  = m_byMaxDevice;
			*pbyMaxChannel = m_byMaxChannel;
			bRet = TRUE;
		}
	}
	else
	{
		;
//		sprintf(szDebug, "pDeviceControl->DeviceCreate() fail !!\n");
//		OutputDebugString(szDebug);
	}	
	return bRet;
}

BOOL CDeviceOperation::GetDeviceInfo(BYTE *pbyMasterChip, BYTE *pbyMaxDevice, BYTE *pbyMaxChannel)
{
	BYTE		byInBuf;
	BYTE		byDeviceInfo[2];
	DWORD		dwRet;
	BOOL		bRet = FALSE;
	
	if(DeviceIoControl(m_hDevice, IOCTL_GET_DEVICE_INFO, &byInBuf, sizeof(byInBuf), byDeviceInfo, sizeof(BYTE)*2, &dwRet, NULL) == TRUE)
	{
		*pbyMasterChip = byDeviceInfo[0];
		*pbyMaxDevice  = byDeviceInfo[0];
		*pbyMaxChannel = byDeviceInfo[1];
		bRet = TRUE;
	}
	return bRet;
}

BOOL CDeviceOperation::DeviceCreate(LPCTSTR pDeviceName, BYTE byDeviceID)
{
	BOOL		bRet = FALSE;
	BYTE		szDeviceName[MAX_DEVICE_NAME];

	memset(szDeviceName, 0, MAX_DEVICE_NAME);
	if(m_hDevice != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hDevice);
		m_hDevice = INVALID_HANDLE_VALUE;
	}

	strcpy((PCHAR)szDeviceName, pDeviceName);
	m_hDevice = CreateFile((const char *)szDeviceName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if(m_hDevice != INVALID_HANDLE_VALUE)
	{
		m_byDeviceID = (int)byDeviceID;
		GetRing3Event();
		GetCaptureBufferMapping();
		bRet = TRUE;
	}
	return bRet;
}

VOID CDeviceOperation::DeviceClose()
{
	BYTE	i;

	if(m_bStartCapture == TRUE)
	{
		SetCaptureStop(DMA_VIDEO_LIVE|DMA_VIDEO_RECORD|DMA_AUDIO_RECORD);
		m_bStartCapture = FALSE;
	}

	for(i=0; i<MAX_RECORD_PATH; i++)
	{
		if(m_hVideoEvent[i])
		{
			CloseHandle(m_hVideoEvent[i]);
			m_hVideoEvent[i] = NULL;
		}
	}
	for(i=0; i<MAX_AUDIO_CHANNEL; i++)
	{
		if(m_hAudioEvent[i])
		{
			CloseHandle(m_hAudioEvent[i]);
			m_hAudioEvent[i] = NULL;
		}
	}
	if(m_hDevice != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hDevice);
		m_hDevice = INVALID_HANDLE_VALUE;
	}
}

BOOL CDeviceOperation::GetRing3Event()
{
	DWORD	dwInBut[MAX_AUDIO_CHANNEL], dwOutBuf, dwRet;
	int	i;
	BOOL	bRet = FALSE;

	for(i=0; i<MAX_RECORD_PATH; i++)
	{
		if(m_hVideoEvent[i])
		{
			CloseHandle(m_hVideoEvent[i]);
			m_hVideoEvent[i] = NULL;
		}		
	}
	for(i=0; i<MAX_AUDIO_CHANNEL; i++)
	{
		if(m_hAudioEvent[i])
		{
			CloseHandle(m_hAudioEvent[i]);
			m_hAudioEvent[i] = NULL;
		}
	}
	for(i=0; i<MAX_RECORD_PATH; i++)
	{
		m_hVideoEvent[i] = CreateEvent(0, TRUE, FALSE, NULL);
		dwInBut[i] = (DWORD)m_hVideoEvent[i];
	}
	bRet = DeviceIoControl(m_hDevice, IOCTL_SET_VIDEO_EVENT, dwInBut, sizeof(DWORD)*MAX_RECORD_PATH, &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);

	for(i=0; i<MAX_AUDIO_CHANNEL; i++)
	{
		m_hAudioEvent[i] = CreateEvent(0, TRUE, FALSE, NULL);
		dwInBut[i] = (DWORD)m_hAudioEvent[i];
	}
	bRet = DeviceIoControl(m_hDevice, IOCTL_SET_AUDIO_EVENT, dwInBut, sizeof(DWORD)*MAX_AUDIO_CHANNEL, &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);	
	return bRet;
}

BOOL CDeviceOperation::DeviceAllReset(BYTE byVideoFormat, BYTE byImageWidth, BYTE byDataFormat, BYTE *pbyImageSize, BYTE m_byDisplayMode)
{
	BYTE	byInBuf[MAX_VIDEO_CHANNEL+4];
	DWORD	dwOutBuf, dwRet;

	byInBuf[0] = byVideoFormat;
	byInBuf[1] = byImageWidth;	
	byInBuf[2] = byDataFormat;	
	byInBuf[3] = m_byDisplayMode;
	memcpy(&byInBuf[4], pbyImageSize, MAX_VIDEO_CHANNEL);
	return DeviceIoControl(m_hDevice, IOCTL_DEVICE_ALL_RESET, byInBuf, sizeof(byInBuf), &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);

}
BOOL CDeviceOperation::VideoInitParam(BYTE byVideoFormat, BYTE byImageWidth, BYTE byDataFormat, BYTE *pbyImageSize, BYTE m_byDisplayMode, BYTE byDisplayFormat, DWORD dwlPitch)
{
	DWORD	dwInBuf[MAX_VIDEO_CHANNEL+6];
	DWORD	dwOutBuf, dwRet;
	BOOL	bRet;
	memset(dwInBuf, 0x00, sizeof(dwInBuf));
	dwInBuf[0] = byVideoFormat;
	dwInBuf[1] = byImageWidth;	
	dwInBuf[2] = byDataFormat;	
	dwInBuf[3] = m_byDisplayMode;
	dwInBuf[4] = byDisplayFormat;
	dwInBuf[5] = dwlPitch;	
	memcpy((BYTE *)&dwInBuf[6], pbyImageSize, MAX_VIDEO_CHANNEL);

	bRet = DeviceIoControl(m_hDevice, IOCTL_VIDEO_INIT_PARAMETER, dwInBuf, sizeof(dwInBuf), &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);
	return bRet;
}

BOOL CDeviceOperation::AudioInitParam(BYTE byChannel, BYTE byFrq, BYTE byVolume, BOOL bAudioEnable)
{
	BYTE	byInBuf[4];
	DWORD	dwOutBuf, dwRet;

	byInBuf[0] = byChannel;
	byInBuf[1] = byFrq;
	byInBuf[2] = byVolume;
	byInBuf[3] = (BYTE)bAudioEnable;
	return DeviceIoControl(m_hDevice, IOCTL_AUDIO_INIT_PARAMETER, byInBuf, sizeof(byInBuf), &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);	

}

BOOL CDeviceOperation::SetDMAVideoAddress(DWORD dwOverlayAddress, DWORD dwOverlaySize)
{
	DWORD		*pdwLinearAddress;
	DWORD		dwRead;
	
	if(dwOverlaySize != 0)
	{
		pdwLinearAddress = (DWORD *)dwOverlayAddress;
		if(!WriteFile(m_hDevice, pdwLinearAddress, dwOverlaySize, &dwRead, NULL))
		{
			OutputDebugString("InitializeParameter fail \n");
			return FALSE;
		}
	}
	else
		return FALSE;
	return TRUE;
}

VOID CDeviceOperation::GetCaptureBufferMapping()
{
	DWORD dwInBuf, dwOutBuf[MAX_AUDIO_CHANNEL], dwRet;
	BYTE	i;

	memset(dwOutBuf, 0x00, sizeof(dwOutBuf));
	DeviceIoControl(m_hDevice, IOCTL_AUDIO_BUFFER_MAPPING, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, sizeof(DWORD)*MAX_AUDIO_CHANNEL, &dwRet, NULL);
	for(i=0; i<MAX_AUDIO_CHANNEL; i++)
		m_pbyAudioBuf[i] = (BYTE *)dwOutBuf[i];

	memset(dwOutBuf, 0x00, sizeof(dwOutBuf));
	DeviceIoControl(m_hDevice, IOCTL_VIDEO_BUFFER_MAPPING, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, sizeof(DWORD)*MAX_RECORD_PATH, &dwRet, NULL);
	for(i=0; i<MAX_RECORD_PATH; i++)
		m_pbyVideoBuf[i] = (BYTE *)dwOutBuf[i];
}

VOID CDeviceOperation::GetEncoderBufferMapping(DWORD *pdwAddress)
{
	DWORD dwInBuf, dwOutBuf[MAX_RECORD_PATH], dwRet;
	BYTE	i;
	
	DeviceIoControl(m_hDevice, IOCTL_ENCODER_BUFFER_MAPPING, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, sizeof(DWORD)*MAX_AUDIO_CHANNEL, &dwRet, NULL);
	for(i=0; i<MAX_RECORD_PATH; i++)
	{
		pdwAddress[i] = (DWORD)dwOutBuf[i];
		m_pbyEncoderBuf[i] = (BYTE *)dwOutBuf[i];
	}
}

BOOL CDeviceOperation::SetCaptureStart(BYTE byDMAStartIdx)
{
	BYTE	byInBuf;
	DWORD	dwOutBuf, dwRet;

	m_bStartCapture = TRUE;
	byInBuf = byDMAStartIdx;
	return DeviceIoControl(m_hDevice, IOCTL_CAPTURE_START, &byInBuf, sizeof(byInBuf), &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);

}

VOID CDeviceOperation::SetCaptureStop(BYTE byDMAStartIdx)
{
	BYTE	byInBuf;
	DWORD	dwOutBuf, dwRet;

	byInBuf = byDMAStartIdx;
	DeviceIoControl(m_hDevice, IOCTL_CAPTURE_STOP, &byInBuf, sizeof(byInBuf), &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);
	m_bStartCapture = FALSE;

}

VOID CDeviceOperation::SetCaptureReStart(BYTE byDMAStartIdx)
{
	SetCaptureStop(byDMAStartIdx);
	SetCaptureStart(byDMAStartIdx);

}

BOOL CDeviceOperation::SetDisplayMode(BYTE byDisplayMode, BYTE *pbyDisplayChannel, BYTE byMaxChannel)
{
	BYTE	byInBuf[MAX_VIDEO_CHANNEL+1];
	DWORD	dwOutBuf, dwRet;

	memset(byInBuf, 0x00, sizeof(byInBuf));
	byInBuf[0] = byDisplayMode;
	memcpy(&byInBuf[1], pbyDisplayChannel, byMaxChannel);
	return DeviceIoControl(m_hDevice, IOCTL_SET_DISPLAY_MODE, byInBuf, sizeof(byInBuf), &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);

}

BOOL CDeviceOperation::SetDisplayChannel(BYTE *pbyDisplayChannel, BYTE byMaxChannel)
{
	BYTE	byInBuf[MAX_VIDEO_CHANNEL];
	DWORD	dwOutBuf, dwRet;
	
	memset(byInBuf, 0x00, sizeof(byMaxChannel));
	memcpy(byInBuf, pbyDisplayChannel, byMaxChannel);
	return DeviceIoControl(m_hDevice, IOCTL_SET_DISPLAY_CHANNEL, byInBuf, sizeof(byInBuf), &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);

}

BOOL CDeviceOperation::SetMotionChannel(BYTE byChannel, BYTE byEnable, BYTE byPass, BYTE byThreshold, BYTE byDetBlockCnt, BYTE *pdwSelBlock)
{
	BYTE	byInBuf[133]; // 128 + 5
	DWORD	dwOutBuf, dwRet;
	BYTE	i;
		
	byInBuf[0] = byChannel;
	byInBuf[1] = byEnable;
	byInBuf[2] = byPass;	
	byInBuf[3] = byThreshold;
	byInBuf[4] = byDetBlockCnt;
	for(i=0; i<MAX_MOTION_ROW*4; i++)
	{
		byInBuf[5+i] = pdwSelBlock[i];
	}
	return DeviceIoControl(m_hDevice, IOCTL_SET_MOTION_CHANNEL, byInBuf, sizeof(byInBuf), &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);

}

BOOL CDeviceOperation::GetMotionData(WORD *pwData)
{
	BYTE	byInBuf;
	WORD	wOutBuf=0;
	DWORD	dwRet;
	BOOL	bRet = FALSE;

	if(DeviceIoControl(m_hDevice, IOCTL_GET_MOTION_DATA, &byInBuf, sizeof(byInBuf), &wOutBuf, sizeof(wOutBuf), &dwRet, NULL) == TRUE)
	{
		*pwData = wOutBuf;
		bRet = TRUE;
	}
	return bRet;
}

BOOL CDeviceOperation::SetChannelTable(BYTE byRecordPath, BYTE *pbyChannel, BYTE byMaxChannel)
{
	BYTE	byInBuf[MAX_CHANNEL_CHANGEABLE + 2];
	DWORD	dwOutBuf, dwRet;
	
	memset(byInBuf, 0x00, MAX_CHANNEL_CHANGEABLE+2);
	memcpy(byInBuf, pbyChannel, byMaxChannel);
	byInBuf[MAX_CHANNEL_CHANGEABLE] = byRecordPath;
	byInBuf[MAX_CHANNEL_CHANGEABLE+1] = byMaxChannel;
	return DeviceIoControl(m_hDevice, IOCTL_SET_CHANNEL_TABLE, byInBuf, MAX_CHANNEL_CHANGEABLE+2, &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);

}

BOOL CDeviceOperation::GetAllVideoSync(BYTE *pwData)
{
	BYTE	byInBuf;
	WORD	wOutBuf;
	DWORD	dwRet;
	BOOL	bRet = FALSE;
	BYTE i=0;

	if(DeviceIoControl(m_hDevice, IOCTL_GET_ALL_VIDEO_SYNC, &byInBuf, sizeof(byInBuf), &wOutBuf, sizeof(wOutBuf), &dwRet, NULL) == TRUE)
	{
		bRet = TRUE;
		for(i=0; i<16; i++)
		{
			pwData[i] = (BOOL)((wOutBuf>>i)&0x01);
		}

	}
	return bRet;
}

BOOL CDeviceOperation::SetTVOutOSDString(BYTE byChannel, char *pszOSD, BYTE byLen)
{
	BYTE	byInBuf[MAX_OSD_TEXT + 2];
	DWORD	dwOutBuf, dwRet;

	memset(byInBuf, 0x00, MAX_OSD_TEXT+2);
	memcpy(byInBuf, pszOSD, byLen);
	byInBuf[MAX_OSD_TEXT] = byChannel;
	byInBuf[MAX_OSD_TEXT+1] = byLen;
	return DeviceIoControl(m_hDevice, IOCTL_SET_TVOUT_OSD_STRING, byInBuf, sizeof(byInBuf), &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);

}

BOOL CDeviceOperation::SetTVOutOSDEnable(BYTE byEnable)
{
	BYTE	byInBuf; 
	DWORD	dwOutBuf, dwRet;

	byInBuf = byEnable;
	return DeviceIoControl(m_hDevice, IOCTL_SET_TVOUT_OSD_ENABLE, &byInBuf, sizeof(byInBuf), &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);

}

BOOL CDeviceOperation::SetColorControl(BYTE byChannel, BYTE byBrightness, BYTE byContrast, BYTE byColor, BYTE byHue)
{
	BYTE	byInBuf[5];
	DWORD	dwOutBuf, dwRet;
	
	byInBuf[0] = byChannel;
	byInBuf[1] = byBrightness;
	byInBuf[2] = byContrast;
	byInBuf[3] = byColor;
	byInBuf[4] = byHue;
	return DeviceIoControl(m_hDevice, IOCTL_SET_COLOR_PARAMETER, byInBuf, sizeof(BYTE)*5, &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);

}

BOOL CDeviceOperation::SetBrightness(BYTE byChannel, BYTE byBrightness)
{
	BYTE	byInBuf[2];
	DWORD	dwOutBuf, dwRet;

	byInBuf[0] = byChannel;
	byInBuf[1] = byBrightness;
	return DeviceIoControl(m_hDevice, IOCTL_SET_BRIGHTNESS, byInBuf, sizeof(BYTE)*2, &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);

}

BOOL CDeviceOperation::SetContrast(BYTE byChannel, BYTE byContrast)
{
	BYTE	byInBuf[2];
	DWORD	dwOutBuf, dwRet;

	byInBuf[0] = byChannel;
	byInBuf[1] = byContrast;
	return DeviceIoControl(m_hDevice, IOCTL_SET_CONTRAST, byInBuf, sizeof(BYTE)*2, &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);

}

BOOL CDeviceOperation::SetColorSaturation(BYTE byChannel, BYTE byColorSaturation)
{
	BYTE	byInBuf[2];
	DWORD	dwOutBuf, dwRet;

	byInBuf[0] = byChannel;
	byInBuf[1] = byColorSaturation;
	return DeviceIoControl(m_hDevice, IOCTL_SET_COLORSATURATION, byInBuf, sizeof(BYTE)*2, &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);

}

BOOL CDeviceOperation::SetHue(BYTE byChannel, BYTE byHue)
{
	BYTE	byInBuf[2];
	DWORD	dwOutBuf, dwRet;

	byInBuf[0] = byChannel;
	byInBuf[1] = byHue;
	return DeviceIoControl(m_hDevice, IOCTL_SET_HUE, byInBuf, sizeof(BYTE)*2, &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);

}

BOOL CDeviceOperation::SetLocalRegister(WORD wAddress, DWORD dwData)
{
	DWORD	dwInBuf[2];
	DWORD	dwOutBuf, dwRet;

	dwInBuf[0] = (DWORD)wAddress;
	dwInBuf[1] = dwData;
	return DeviceIoControl(m_hDevice, IOCTL_SET_LOCAL_REGISTER, dwInBuf, sizeof(dwInBuf), &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);

}

DWORD CDeviceOperation::GetLocalRegister(WORD wAddress)
{
	WORD	wInBuf;
	DWORD	dwOutBuf;
	DWORD	dwRet;

	wInBuf = wAddress;	
	if(DeviceIoControl(m_hDevice, IOCTL_GET_LOCAL_REGISTER, &wInBuf, sizeof(wInBuf), &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL) == FALSE)
		dwOutBuf = 0;
	return dwOutBuf;
}

BOOL CDeviceOperation::SetQuadRegister(WORD wIndex, WORD wAddress, WORD wData)
{
	WORD	wInBuf[3];
	DWORD	dwOutBuf, dwRet;

	wInBuf[0] = wIndex;
	wInBuf[1] = wAddress;
	wInBuf[2] = wData;
	return DeviceIoControl(m_hDevice, IOCTL_SET_QUAD_REGISTER, wInBuf, sizeof(wInBuf), &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);

}

WORD CDeviceOperation::GetQuadRegister(WORD wIndex, WORD wAddress)
{
	WORD	wInBuf[2];
	WORD	wOutBuf=0;
	DWORD	dwRet;

	wInBuf[0] = wIndex;
	wInBuf[1] = wAddress;	
	if(DeviceIoControl(m_hDevice, IOCTL_GET_QUAD_REGISTER, wInBuf, sizeof(wInBuf), &wOutBuf, sizeof(wOutBuf), &dwRet, NULL) == FALSE)
		wOutBuf = 0;
	return wOutBuf;
}

BOOL CDeviceOperation::I2cWrite3Byte(BYTE bySlaveAddress, BYTE bySubAdd, BYTE byI2cData)
{
	BYTE	byInBuf[3];
	DWORD	dwOutBuf, dwRet;

	byInBuf[0] = bySlaveAddress;
	byInBuf[1] = bySubAdd;
	byInBuf[2] = byI2cData;
	return DeviceIoControl(m_hDevice, IOCTL_I2C_WRITE_3BYTE, byInBuf, sizeof(byInBuf), &dwOutBuf, sizeof(dwOutBuf), &dwRet, NULL);

}

BOOL CDeviceOperation::I2cRead1Byte(BYTE byAddress, BYTE bysubAddr, BYTE *pbyI2cData)
{
	BYTE		byInBuf[2];
	BYTE		byOutBuf;
	DWORD		dwRet;
	BOOL		bRet;

	byInBuf[0] = byAddress;
	byInBuf[1] = bysubAddr;
	
	bRet = DeviceIoControl(m_hDevice, IOCTL_I2C_READ_1BYTE, byInBuf, sizeof(byInBuf), &byOutBuf, sizeof(byOutBuf), &dwRet, NULL);
	pbyI2cData[0] = byOutBuf;
	return bRet;
}

void CDeviceOperation::SetCaptureRestart(BYTE byDMAStartIdx)
{
	SetCaptureStop(byDMAStartIdx);
	SetCaptureStart(byDMAStartIdx);
}

BOOL CDeviceOperation::MakeDeviceCreate(DWORD *hVideoEvent, DWORD *pdwVideoAddress, DWORD *hAudioEvent, DWORD *pdwAudioAddress)
{
	CHAR	szDeviceName[32];	
	BOOL	bRet = FALSE;
	BYTE	i;	
		
//	if(m_byMaxDevice == 0)
//	{
//		return bRet;
//	}
	_snprintf(szDeviceName, 32, RUBY3_DEVICE_NAME, 0);

		if(DeviceCreate(szDeviceName, 0) == FALSE)
		{
			;
		}
		else
		{
			for(i=0; i<MAX_RECORD_PATH; i++)
			{
				hVideoEvent[i]		= (DWORD)m_hVideoEvent[i];
				pdwVideoAddress[i]= (DWORD)m_pbyVideoBuf[i];
			}
			for(i=0; i<MAX_AUDIO_CHANNEL; i++)
			{
				hAudioEvent[i]		= (DWORD)m_hAudioEvent[i];
				pdwAudioAddress[i]= (DWORD)m_pbyAudioBuf[i];
			}
			bRet = TRUE;
		}
	return bRet;
}
