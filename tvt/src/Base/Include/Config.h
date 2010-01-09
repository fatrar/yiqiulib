//==========================================================================
/**
* @file	  : Common.h
* @author : heliang
* created : 2008-1-2   16:08
* purpose : ���������ļ�
*/
//==========================================================================

#ifndef RKT_CONFIG_H
#define RKT_CONFIG_H

/*
	���ļ����ڶ���������������ñ��룬���������п���������������ĺ궨��
*/

/////////////////////////////////////////////////////////////////////////////////////
// �ֶ���������ĺ���ȷ������
/// �Ƿ�֧��Singleton
#define SUPPORT_SINGLETON

/// �Ƿ�֧��RPC����ؽṹ
#define SUPPORT_RPC

/// �Ƿ�֧��RLE�ӽ�ѹ
#define SUPPORT_RLE

/// �Ƿ�֧�ְ�ȫList
#define SUPPORT_SAFELIST

/// �Ƿ�֧���ַ�����չ(autostring,StringHash,StringHelper)
#define SUPPORT_AUTOSTRING
#define SUPPORT_STRINGHASH
#define SUPPORT_STRINGHELPER

/// �Ƿ�֧��LoadLib
#define SUPPORT_LOADLIB

/// �Ƿ�֧��Handle
#define SUPPORT_HANDLE

/// �Ƿ�֧��·�������㷨
#define SUPPORT_PATHFINDER

/// �Ƿ�֧��IME�Ĵ���
#define SUPPORT_IME

/// �Ƿ�֧�������������
#define SUPPORT_RANDGENERATOR

/// �Ƿ�֧��TRACE(����THREAD)
#define SUPPORT_TRACE

/// �Ƿ�֧��APP����
#define SUPPORT_APP

/// �Ƿ�֧��tinyxml
#define SUPPORT_TINYXML

/// �Ƿ�֧�ֶ�̬����(����TRACE)
//#define SUPPORT_OBJECT

/// �Ƿ�֧���߳�(����TRACE)
#define SUPPORT_THREAD

/// �Ƿ�֧��API(����TRACE,FILESYSTEM,APP)
#define SUPPORT_API

/// �Ƿ�֧��ʱ������ʱ����(����API,TRACE)
#define SUPPORT_TIMER


/// �Ƿ�֧���¼�����(���� TRACE)
#define SUPPORT_EVENTENGINE

/// �Ƿ�֧����������(���� XML,FILESYSTEM,TRACE)
#define SUPPORT_SCHEMEENGINE

/// �Ƿ�֧�����ܲ���
#define SUPPORT_PROFILE

/// �Ƿ�֧��StackWalk
#define SUPPORT_STACKWALK

/// �Ƿ�֧��LuaEngine(�����ڴ涨�壬FILESYSTEM,TRACE)
#define SUPPORT_LUAENGINE

/// �Ƿ�֧������(����TRACE,THREAD)
#define SUPPORT_NET

/// �Ƿ�֧�������ļ�ϵͳ(����THREAD)
#define SUPPORT_STREAM
#define SUPPORT_ASYNC_STREAM //(����������첽IO)
#define SUPPORT_OBJECTPOOL

/// �Ƿ�֧���ڴ�
#define SUPPORT_MEMORY

/// �Ƿ�֧�������
#define SUPPORT_COMMANDFACTORY




/////////////////////////////////////////////////////////////////////////////////////
// ���غ�
/// ���Ժ꣬������������������Ϣ��ע����ͬ�ڱ�������_DEBUG�꣩
#define RKT_DEBUG

/// �Ƿ�֧��UI����
#define RKT_UI_TEST

/// �Ƿ��ĳЩ�ֲ����Ժ�
#define RKT_TEST

/// �Ƿ�ʹ��ϵͳ�ڴ�
//#define USE_SYSTEM_MEMORY

/// �Ƿ���ͳ����Ϣ
#define USE_STAT_INFO

/// �Ƿ�ʹ�û��ʵ�ֵĿ��ٻ����壨����ʹ���ٽ�����
//#define USE_ASSEMBLY_MUTEX


/////////////////////////////////////////////////////////////////////////////////////
// ���(��Ҫ�޸�)

#if !defined(SUPPORT_TINYXML) && defined(SUPPORT_SCHEMEENGINE)
#	undef SUPPORT_SCHEMEENGINE
#endif


#endif // RKT_CONFIG_H