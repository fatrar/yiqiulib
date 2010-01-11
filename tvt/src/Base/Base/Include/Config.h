//==========================================================================
/**
* @file	  : Common.h
* @author : heliang
* created : 2008-1-2   16:08
* purpose : 公共定义文件
*/
//==========================================================================

#ifndef RKT_CONFIG_H
#define RKT_CONFIG_H

/*
	该文件用于对整个引擎进行配置编译，下面是所有可以用于条件编译的宏定义
*/

/////////////////////////////////////////////////////////////////////////////////////
// 手动调整下面的宏来确定需求
/// 是否支持Singleton
#define SUPPORT_SINGLETON

/// 是否支持RPC的相关结构
#define SUPPORT_RPC

/// 是否支持RLE加解压
#define SUPPORT_RLE

/// 是否支持安全List
#define SUPPORT_SAFELIST

/// 是否支持字符串扩展(autostring,StringHash,StringHelper)
#define SUPPORT_AUTOSTRING
#define SUPPORT_STRINGHASH
#define SUPPORT_STRINGHELPER

/// 是否支持LoadLib
#define SUPPORT_LOADLIB

/// 是否支持Handle
#define SUPPORT_HANDLE

/// 是否支持路径搜索算法
#define SUPPORT_PATHFINDER

/// 是否支持IME的处理
#define SUPPORT_IME

/// 是否支持随机数创建器
#define SUPPORT_RANDGENERATOR

/// 是否支持TRACE(依赖THREAD)
#define SUPPORT_TRACE

/// 是否支持APP对象
#define SUPPORT_APP

/// 是否支持tinyxml
#define SUPPORT_TINYXML

/// 是否支持动态对象(依赖TRACE)
//#define SUPPORT_OBJECT

/// 是否支持线程(依赖TRACE)
#define SUPPORT_THREAD

/// 是否支持API(依赖TRACE,FILESYSTEM,APP)
#define SUPPORT_API

/// 是否支持时间对象和时间轴(依赖API,TRACE)
#define SUPPORT_TIMER


/// 是否支持事件引擎(依赖 TRACE)
#define SUPPORT_EVENTENGINE

/// 是否支持配置引擎(依赖 XML,FILESYSTEM,TRACE)
#define SUPPORT_SCHEMEENGINE

/// 是否支持性能测试
#define SUPPORT_PROFILE

/// 是否支持StackWalk
#define SUPPORT_STACKWALK

/// 是否支持LuaEngine(依赖内存定义，FILESYSTEM,TRACE)
#define SUPPORT_LUAENGINE

/// 是否支持网络(依赖TRACE,THREAD)
#define SUPPORT_NET

/// 是否支持虚拟文件系统(依赖THREAD)
#define SUPPORT_STREAM
#define SUPPORT_ASYNC_STREAM //(依赖网络的异步IO)
#define SUPPORT_OBJECTPOOL

/// 是否支持内存
#define SUPPORT_MEMORY

/// 是否支持命令工厂
#define SUPPORT_COMMANDFACTORY




/////////////////////////////////////////////////////////////////////////////////////
// 开关宏
/// 调试宏，定义此行则会加入调试信息（注：不同于编译器的_DEBUG宏）
#define RKT_DEBUG

/// 是否支持UI测试
#define RKT_UI_TEST

/// 是否打开某些局部测试宏
#define RKT_TEST

/// 是否使用系统内存
//#define USE_SYSTEM_MEMORY

/// 是否开启统计信息
#define USE_STAT_INFO

/// 是否使用汇编实现的快速互斥体（否则使用临界区）
//#define USE_ASSEMBLY_MUTEX


/////////////////////////////////////////////////////////////////////////////////////
// 检查(不要修改)

#if !defined(SUPPORT_TINYXML) && defined(SUPPORT_SCHEMEENGINE)
#	undef SUPPORT_SCHEMEENGINE
#endif


#endif // RKT_CONFIG_H