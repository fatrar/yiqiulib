
#define DLL_API extern "C" __declspec(dllimport)

/************************************************************************************************************/
DLL_API int AM_YUV411_Decoder(BYTE *pbyInBuffer,BYTE *pbyOutBuffer,DWORD dwPitch,BOOL PPFlag,BOOL RGBFlag);
/***********************************************************************************************************/
