/*H***************************************************************************
 File            : IIVDataSender.h
 Subsystem       : 
 Function Name(s): CIIVDataSender
 Author          : YiQiu
 Date            : 2010-1-13  
 Time            : 10:41
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IIVDATASENDER_H_2010_
#define _IIVDATASENDER_H_2010_


#include "WPG_EventOccurrence.h"

#ifndef TARGET_MAX_NUM
    #define TARGET_MAX_NUM	50
#endif


struct IIVDataSender
{
    // �������ݲɼ�����Ļص�
    virtual BOOL OnIVDataSend(
        int nChannelID,
        const FILETIME& time,
        const WPG_Target* pData,
        size_t nLen ) = 0;

    virtual BOOL Init(
        int nDeviceCount,
        int nEveryDeviceChannelNum )=0;

    virtual BOOL Unit()=0;
};






#endif  // End of file



