/**H**************************************************************************
 File            : Common.h
 Subsystem       : 
 Function Name(s): CCommon
 Author          : YiQiu
 Date            : 2008-1-2   16:08 
 Time            : 10:17
 Description     : 公共头文件
 
 编码风格：
 1. 命名跟匈牙利命名基本一致，不同的是标准枚举定义为全大写加下划线，这种存在着中国人看大写不习惯的问题，
 所以我改良为每个词首字大写，下划线连接各词，在windows平台不存在与其他定义相冲突的问题
  有具体意思的宏定义也是采用这种方式，比如宏代替字符串，一个数字，一个宏函数等
 2. 代码在不同情况采用不同的编写标准，在保证功能的前提下，
 在效率要求很高的情况下，以执行效率为先，编码优美简洁优雅易懂为其次。
 在
 3.注释是为了方便doxygen好生成文档，采用其适应的风格

 Revision        : 3.0


 History
 -------
 2008-01-02   1.0  建立基本框架
 2009-11-18   2.0  在1.0的基础上将原有基础库在代码放在OCI在。命名为OCI基础库
 2010-10-08   3.0  为了适应游戏游戏平台需求。增加跨平台需求

 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef OCI_COMMON_H
#define OCI_COMMON_H


//#include "Config.h" // 编译用户的配置


#define BEGIN_BASE_ENGINE namespace OCI {
#define END_BASE_ENGINE }

#define USING_BASE_ENGINE using namespace OCI;

// 导入导出标志
#if defined( _WIN32 ) || defined( WIN32 ) 
#	ifdef _LIB
#		define CLASS_EXPORT
#		define CLASS_IMPORT
#		define API_EXPORT		extern "C"
#		define API_IMPORT		extern "C"
#	else
#		define CLASS_EXPORT		__declspec(dllexport)
#		define CLASS_IMPORT		__declspec(dllimport)
#		define API_EXPORT		extern "C" __declspec(dllexport)
#		define API_IMPORT		extern "C" __declspec(dllimport)
#	endif
#else
#	define CLASS_EXPORT
#	define CLASS_IMPORT
#	define API_EXPORT
#	define API_IMPORT
#endif

namespace OCI {

// 只定义无符号的简单类型，便于书写
typedef unsigned char			uchar, BYTE, u_char;
typedef unsigned short			ushort, u_short, WORD;
typedef unsigned int			uint, u_int, size_t;
typedef unsigned long			ulong, u_long, DWORD;
typedef __int64					int64;
typedef unsigned __int64		uint64;

};

// 安全删除和释放
#ifndef safeDelete
#	define safeDelete(ptr)		if ((ptr)) {delete (ptr); (ptr) = 0;}
#endif
#ifndef safeDeleteArray
#	define safeDeleteArray(ptr)	if ((ptr)) {delete[] (ptr); (ptr) = 0;}
#endif
#ifndef safeRelease
#	define safeRelease(ptr)		if ((ptr)) {(ptr)->Release(); (ptr) = 0;}
#endif
#ifndef saferelease
#	define saferelease(ptr)		if ((ptr)) {(ptr)->release(); (ptr) = 0;}
#endif


/** 编译期Assert,参考boost库,如：
STATIC_ASSERT(sizeof(long) > 8);
*/
namespace COMPILE_ASSERT
{
    template <bool x> struct COMPILE_ASSERT_FAILURE;
    template <> struct COMPILE_ASSERT_FAILURE<true> { enum { value = 1 }; };
    template<int x> struct compile_assert_test{};
};

#define STATIC_ASSERT(exp) \
    typedef COMPILE_ASSERT::compile_assert_test< \
        sizeof(COMPILE_ASSERT::COMPILE_ASSERT_FAILURE< (bool)(exp) >) \
    > compile_assert_typedef_


// hash_map的定义，为了不关心命名空间的差异，可直接用 HashMap 宏
#if _MSC_VER  > 1300
#   define HashMap stdext::hash_map
#else
#   define HashMap std::hash_map
#endif

#define isValidString(s)	(s && *s != 0)



#endif