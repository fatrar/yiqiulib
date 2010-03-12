/*H***************************************************************************
 File            : IVCfgMgr.h
 Subsystem       : 
 Function Name(s): CIVCfgMgr
 Author          : YiQiu
 Date            : 2010-1-22  
 Time            : 17:35
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IVCFGMGR_H_2010_
#define _IVCFGMGR_H_2010_

#include ".\Include\IIVCfgMgr.h"



class CIVCfgMgr :
    public IIVCfgMgr ,
    public Singleton<CIVCfgMgr>
{
public:
    CIVCfgMgr();
    virtual ~CIVCfgMgr();

    typedef IIVCfgMgr::IVVistor IVVistor;
public:
    virtual IVVistor Begin(int nChannelID);
    virtual const IVVistor& End();

    virtual IVVistor Add(
        int nChannelID,
        const WPG_Rule& Rule,
        const ScheduleSettings& Sch = g_DefaultScheduleSettings,
        const AlarmOutSettings& Alarm = g_DefaultAlarmOutSettings);

    virtual bool Remove(
        int nChannelID,
        const IVVistor& Vistor);

    virtual bool Apply();

    virtual bool SetAutoRunChannel(
        const int szChannel[_MaxAutoChannel],
        size_t nCount );

    virtual bool GetAutoRunChannel(
        int szChannel[_MaxAutoChannel],
        size_t& nCount );


    virtual void SetDataShowState(int nChannelID,int nState);


    virtual void GetDataShowState(int nChannelID,int& nState);

protected: 


private:
    TiXmlDocument m_Doc;
    TiXmlElement* m_pRootEle;
};


#endif  // End of file



