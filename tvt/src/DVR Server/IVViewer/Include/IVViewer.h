/*H***************************************************************************
 File            : IVViewer.h
 Subsystem       : 
 Function Name(s): 
 Author          : YiQiu
 Date            : 2010-1-13  
 Time            : 10:35
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IIVVIEWER_H_2010_
#define _IIVVIEWER_H_2010_

#include <Windows.h>
#include <Common.h>
#include "..\DEVICEControl\IIVDataSender.h"

#ifdef IVVIEWER_EXPORTS
    #define IVVIEWER_CLASS CLASS_EXPORT
    #define IVVIEWER_API API_EXPORT
#else    
    #define IVVIEWER_CLASS CLASS_IMPORT
    #define IVVIEWER_API API_IMPORT
    #ifndef IVVIEWER_LINK
        #define IVVIEWER_LINK
        #pragma comment(lib, "IVViewer.lib")
        #pragma message("Automatically linking with IVViewer.dll") 
    #endif
#endif

/**
*@note IV���ݱ���ӿ�
*/
struct IIVDataSaver
{
    /**
    *@note ��Ƶ�ļ���һ��дʱ�ص���������������ļ������·���Ϳ�ʼ��ʱ��
    */
    virtual BOOL Open(int nChannelID,const char* pPath, const FILETIME& time)=0;

    /**
    *@note ��Ƶ�ļ��ر�ʱ������
    */
    virtual BOOL Close(int nChannelID, const FILETIME& time)=0;

    /**
    *@note ɾ����Ƶ�ļ��ص����Ӷ�֪ͨ�������ɾ�������ļ���
    */
    virtual BOOL DeleteIVFile(const char* pPath)=0;

    /**
    *@note ����Ԥ¼��ʱ��
    */
    virtual BOOL TellPreAlarmTime(int time)=0;
};

/**
*@note IV������ʾ�ӿ�
*/
struct IIVViewer
{
    enum
    {
        Show_Object = 0x1,
        Show_Trace  = 0x2,
        Show_Object_Trace = Show_Object|Show_Trace,
    };

    /**
    *@note ��ʾĿ����ο��·��ʱ�Ļص���
    *      ����DC�����ڲ��ŵ�����֡��ʱ��
    */
    virtual BOOL Paint(
        int nChannelID, 
        const HDC dc,
        const RECT& rect,
        const FILETIME& time)=0;

    /**
    *@note bState=true��ʾĿ���·������������
    */
    virtual void SetObjTraceShowState(
        int nChannelID, bool bShow)=0;
    
    /**
    *@note �õ�Ŀ���·���Ƿ�������ʾ
    */
    virtual void GetObjTraceShowState(
        int nChannelID, bool& bShow)=0;
    
    /**
    *@note ������ʾ��Щ����Ŀ�껹�ǹ켣
    */
    virtual void SetDataShowState(
        int nChannelID, int nState)=0;
   
    /**
    *@note �õ���ʾ����Ŀ�껹�ǹ켣
    */
    virtual void GetDataShowState(
        int nChannelID, int& nState)=0;
};

/**
*@note IV������ʾ�ӿ�
*/
struct IIVLiveViewer :
    public IIVViewer
{
    /**
    *@note ����ͳ��
    */
    virtual BOOL ResetStatistic(int nChannelID)=0;

    /**
    *@note bFlag=true��ʼͳ�ƣ�flaseֹͣ
    */
    virtual BOOL StartStatistic(int nChannelID, bool bFlag)=0;

    /**
    *@note �õ�ͳ��״̬
    */ 
    virtual BOOL GetStatisticState(
        int nChannelID, bool& bFlag)=0;

    /**
    *@note ��ʾ����ͼ��
    */
    virtual void PaintRule(
        int nChannelID,
        const HDC dc,
        const RECT& rect)=0;

    /**
    *@note ��ʾ����ͳ��
    */
    virtual void PaintStatistic(
        int nChannelID,
        const HDC dc,
        const RECT& rect)=0;

    /**
    *@note �����Ƿ���ʾ����ͼ�κ�ͳ��
    */
    virtual void ShowRuleAndStatistic(
        int nChannelID,
        bool bShow)=0;
};

/**
*@note IV������ʾ�ӿ�,
* �������һ�����������������ڸ������úͳ�ʼ��ʱʹ��
* IIVLiveViewerֻ�Ǹ��ϲ��ṩ�ӿ�������ʾ
*/
struct IIVLiveViewerEx:
    public IIVLiveViewer
{
    /**
    *@note ��ӹ����live��ʾ��
    */
    virtual void AddRule(
        int nChannelID,
        const WPG_Rule& Rule)=0;

    /**
    *@note ɾ������
    */
    virtual void RemoveRule(
        int nChannelID,
        BYTE (&ID)[16])=0;

    /**
    *@note ���¹���
    */
    virtual void ModifyRule(
        int nChannelID,
        const WPG_Rule& Rule)=0;

    /**
    *@note ��ո�ͨ�������й���
    */
    virtual void ClearAllRule(int nChannelID)=0;
};

/**
*@note �õ�Live���ӿڶ�Ӧ�Ķ���ָ��
*/
namespace IVLiveFactory
{
    IVVIEWER_API IIVLiveViewer* GetLiveViewer();
    IVVIEWER_API IIVLiveViewerEx* GetLiveViewerEx();
    IVVIEWER_API IIVDataSaver* GetDataSaver();
    IVVIEWER_API IIVDataSender* GetDataSender();
};

/**
*@note IV���ݻطŽӿ�
*/
struct IVDataFound
{
    /**
    *@note ��Ƶ�ļ���ʱ�ص�������ͨ������Ƶ�ļ�·������ʱ���
    */
    virtual int Open(int nChannelID, const char* pPath, const FILETIME& time)=0;

    /**
    *@note ��Ƶ�ļ��ر�ʱ�ص�
    */
    virtual int Close(int nChannelID, const FILETIME& time)=0;
};

/**
*@note �õ�PlayBack���ӿڶ�Ӧ�Ķ���ָ��
*/ 
namespace IVPlaybackFactory
{
    static IIVViewer* GetPlaybackViewer(void);
    static IVDataFound* GetDataFound(void);
};




#endif  // End of file



