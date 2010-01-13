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




struct WPG_Target;


struct IIVDataSender
{
    // 智能数据采集到后的回调
    virtual BOOL OnIVDataSend(
        int nChannelID,
        const FILETIME& time,
        const WPG_Target* pData,
        size_t nLen ) = 0;
};






#endif  // End of file



