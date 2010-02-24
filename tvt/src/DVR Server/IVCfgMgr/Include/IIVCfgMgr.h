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
#ifndef _IIVCFGMGR_H_2010_
#define _IIVCFGMGR_H_2010_


#include "..\DEVICEControl\WPG_EventOccurrence.h"
#include "..\DEVICEControl\IIVDevice.h"

class TiXmlElement;

#ifdef IVCFGMGR_EXPORTS
    #define IIVCFGMGR_CLASS CLASS_EXPORT
    #define IIVCFGMGR_API API_EXPORT
#else    
    #define IIVCFGMGR_CLASS CLASS_IMPORT
    #define IIVCFGMGR_API API_IMPORT
    #ifndef IIVCFGMGR_LINK
        #define IIVCFGMGR_LINK
        #pragma comment(lib, "IVCfgMgr.lib")
        #pragma message("Automatically linking with IVCfgMgr.dll") 
    #endif
#endif


struct IIVCfgMgr
{
    #define _MaxAutoChannel 4

    class IIVCFGMGR_CLASS IVVistor
    {
    public:
        friend IIVCfgMgr;
        explicit IVVistor(TiXmlElement* pEle = NULL);
        bool operator == (const IVVistor& v) const;
        bool operator != (const IVVistor& v) const;
    public:
        IVVistor Next();

    public:
        // Get
        const char* GetIdentityID();
        const char* GetRuleName();
        bool IsAutoEnbale();
        IVRuleType GeRuleType();

        bool GetRule(WPG_Rule& Rule);
        bool GetSchedule(ScheduleSettings& Sch);
        bool GetAlarmOut(AlarmOutSettings& Alarm);

        // Modify
        bool ModifyRule(const WPG_Rule& Rule);   // Can`t modify Rule type(ruleId)
        bool ModifySchedule(const ScheduleSettings& Sch);
        bool ModifyAlarmOut(const AlarmOutSettings& Alarm);

    private:
        TiXmlElement* m_pEle;       // 指向一个规则的节点
    };

    virtual IVVistor Begin(int nChannelID) = 0;
    virtual const IVVistor& End()=0;

    /**
    @purpose          : 添加一条规则
    @param nChannelID : 通道号
    @param Rule       : 规则设定的结构体
    @param Sch        : 规则排程的结构体，默认全部有效
    @param Alarm      : 报警输入设定的结构，默认全有，且hold 5秒
    @return	IVVistor  : 返回添加后的规则遍历者。可以用与End()比较是否成功
    */
    virtual IVVistor Add(
        int nChannelID,
        const WPG_Rule& Rule,
        const ScheduleSettings& Sch = g_DefaultScheduleSettings,
        const AlarmOutSettings& Alarm = g_DefaultAlarmOutSettings)=0;

    /**
    @purpose        : 删除一条规则
    @notice         : 删除后可能会使以前遍历的IVVistor失效，
                      这个跟stl的deque类似，所以注意的地方跟它一样
    @param IVVistor : 实际的操作过程，应该是首先通过Begin到End遍历，
                      得到实际要删除的节点，然后调这个函数删除
    @return	bool    : 是否成功
    */
    virtual bool Remove(const IVVistor& Vistor)=0;

    /**
    @purpose : 保存配置数据到XML，注意所有的设置修改后，
               只有调用这个才会保存生效，不然只是修改内存
    @return	 : 是否成功
    */
    virtual bool Apply() = 0;


    /**
    @purpose         : 设置一开机就要运行智能的通道
    @param szChannel : 通道数据，最大为_MaxAutoChannel
    @param nCount    : 实际通道个数
    @return	         : 是否成功
    */
    virtual bool SetAutoRunChannel(
        const int szChannel[_MaxAutoChannel],
        size_t nCount )=0;
    
    /**
    @purpose         : 取得一开机就要运行智能的通道
    @param szChannel : 通道数据，最大为_MaxAutoChannel，这里为buf
    @param nCount    : 返回实际通道个数
    @return	         : bool 是否成功, 
    */
    virtual bool GetAutoRunChannel(
        int szChannel[_MaxAutoChannel],
        size_t& nCount )=0;


    virtual void SetDataShowState(int nChannelID,int nState)=0;

    virtual void GetDataShowState(int nChannelID,int& nState)=0;
    
protected:
    /**
    @purpose : 因为C++友元不可以继承，所以为了实现IIVCfgMgr的派生类能直接调用IVVistor的成员
               但又不想暴露实际的对象给外部。做了一个特化的函数           
    @return	 : TiXmlElement*
    */
    TiXmlElement* Ele(const IVVistor& Vistor)
    {
        return Vistor.m_pEle;
    }
};

namespace IIVCfgMgrFactory
{
    /**
    @purpose : 得到IV配置管理者的单实例对象指针
    @return	 : IIVCfgMgr单实例对象指针
    */
    IIVCFGMGR_API IIVCfgMgr* GetIIVCfgMgr();
}

#endif  // End of file



