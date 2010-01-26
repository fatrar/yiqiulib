// DeviceOperation.h: interface for the CDeviceOperation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVICEOPERATION_H__042ADFED_6C65_4049_9BF7_1A0290584A34__INCLUDED_)
#define AFX_DEVICEOPERATION_H__042ADFED_6C65_4049_9BF7_1A0290584A34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "./head/DefineBase.h"
#include "VideoDriver.h"
class CDeviceOperation : public CVideoDriver
{
public:
	CDeviceOperation();
	virtual ~CDeviceOperation();
public:	
	BOOL MakeDeviceCreate(DWORD *hVideoEvent, DWORD *pdwVideoAddress, DWORD *hAudioEvent, DWORD *pdwAudioAddress);
	void SetCaptureRestart(BYTE byDMAStartIdx);
	BOOL I2cRead1Byte(BYTE byAddress, BYTE bysubAddr, BYTE *pbyI2cData);
	BOOL I2cWrite3Byte(BYTE bySlaveAddress, BYTE bySubAdd, BYTE byI2cData);
	WORD GetQuadRegister(WORD wIndex, WORD wAddress);
	BOOL SetQuadRegister(WORD wIndex, WORD wAddress, WORD wData);
	DWORD GetLocalRegister(WORD wAddress);
	BOOL SetLocalRegister(WORD wAddress, DWORD dwData);
	BOOL SetHue(BYTE byChannel, BYTE byHue);
	BOOL SetColorSaturation(BYTE byChannel, BYTE byColorSaturation);
	BOOL SetContrast(BYTE byChannel, BYTE byContrast);
	BOOL SetBrightness(BYTE byChannel, BYTE byBrightness);
	BOOL SetColorControl(BYTE byChannel, BYTE byBrightness, BYTE byContrast, BYTE byColor, BYTE byHue);
	BOOL SetTVOutOSDEnable(BYTE byEnable);
	BOOL SetTVOutOSDString(BYTE byChannel, char *pszOSD, BYTE byLen);
	BOOL GetAllVideoSync(BYTE	*pwData);
	BOOL SetChannelTable(BYTE byRecordPath, BYTE *pbyChannel, BYTE byMaxChannel);
	BOOL GetMotionData(WORD *pwData);
	BOOL SetMotionChannel(BYTE byChannel, BYTE byEnable, BYTE byPass, BYTE byThreshold, BYTE byDetBlockCnt, BYTE *pdwSelBlock);
	BOOL SetDisplayChannel(BYTE *pbyDisplayChannel, BYTE byMaxChannel);
	BOOL SetDisplayMode(BYTE byDisplayMode, BYTE *pbyDisplayChannel, BYTE byMaxChannel);
	VOID SetCaptureReStart(BYTE byDMAStartIdx);
	VOID SetCaptureStop(BYTE byDMAStartIdx);
	BOOL SetCaptureStart(BYTE byDMAStartIdx);
	VOID GetEncoderBufferMapping(DWORD *pdwAddress);
	VOID GetCaptureBufferMapping();
	BOOL SetDMAVideoAddress(DWORD dwOverlayAddress, DWORD dwOverlaySize);
	BOOL AudioInitParam(BYTE byChannel, BYTE byFrq, BYTE byVolume, BOOL bAudioEnable);
	BOOL VideoInitParam(BYTE byVideoFormat, BYTE byImageWidth, BYTE byDataFormat, BYTE *pbyImageSize, BYTE m_byDisplayMode, BYTE byDisplayFormat, DWORD dwlPitch);
	BOOL DeviceAllReset(BYTE byVideoFormat, BYTE byImageWidth, BYTE byDataFormat, BYTE *pbyImageSize, BYTE m_byDisplayMode);
	BOOL GetRing3Event();
	VOID DeviceClose();
	BOOL DeviceCreate(LPCTSTR pDeviceName, BYTE byDeviceID);
	BOOL GetDeviceInfo(BYTE *pbyMasterChip, BYTE *pbyMaxDevice, BYTE *pbyMaxChannel);
	BOOL IsInitDevice(BYTE *pbyMaxChannel);//BYTE *pbyMasterChip, BYTE *pbyMaxDevice, BYTE *pbyMaxChannel);
	BYTE		m_byDeviceID;
	HANDLE	m_hDevice;
	HANDLE	m_hAudioEvent[MAX_AUDIO_CHANNEL];
	HANDLE	m_hVideoEvent[MAX_RECORD_PATH];
	BYTE	*m_pbyAudioBuf[MAX_AUDIO_CHANNEL];
	BYTE	*m_pbyVideoBuf[MAX_RECORD_PATH];
	BYTE	*m_pbyEncoderBuf[MAX_RECORD_PATH];
	BYTE	*m_pbyDecoderBuf[MAX_RECORD_PATH];
	BOOL	m_bStartCapture;
	BYTE	m_byMasterChip;
	BYTE	m_byMaxDevice;
	BYTE	m_byMaxChannel;
};

#endif // !defined(AFX_DEVICEOPERATION_H__042ADFED_6C65_4049_9BF7_1A0290584A34__INCLUDED_)
