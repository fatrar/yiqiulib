#ifndef CARD_ALARM_H
#define CARD_ALARM_H


#ifdef CARDALARM_EXPORT
#define CARDALARM_API __declspec(dllexport)
#else
#define CARDALARM_API __declspec(dllimport)
#endif

CARDALARM_API BOOL ALM_Initial();
CARDALARM_API void ALM_Quit();
CARDALARM_API BOOL ALM_AlarmIn(DWORD * pAlarmIn);
CARDALARM_API BOOL ALM_AlarmOut(DWORD dwAlarmOut);

CARDALARM_API BOOL ALM_CommOut(char *pOut, int OutLen); //djx
CARDALARM_API BOOL ALM_GetCardVer();

#endif //CARDALARM_H