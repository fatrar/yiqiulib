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
 Revision        : 


 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef OCI_COMMON_H
#define OCI_COMMON_H

//#include "Config.h" // 包含用户的配置


#define BEGIN_BASE_ENGINE namespace OCI {
#define END_BASE_ENGINE };

#define USING_BASE_ENGINE using namespace OCI;

/// 模块的配置后缀
#if defined(_DEBUG)
#	define CFG_SUFFIX		"_d"
#else
#	define CFG_SUFFIX		""
#endif

	/// 动态库前后缀
#define DLL_PREFIX	""
#define DLL_SUFFIX	".dll"


	/// 组合库文件名
#define MAKE_DLL_NAME(module)			"./"DLL_PREFIX#module""CFG_SUFFIX""DLL_SUFFIX
#define	MAKE_LIB_NAME(module)			"lib"#module""CFG_SUFFIX
#define MAKE_DLL_LIB_NAME(module)		#module""CFG_SUFFIX


// 导入导出标志
#ifdef _LIB
#	define CLASS_EXPORT
#	define CLASS_IMPORT
#	define API_EXPORT		extern "C"
#	define API_IMPORT		extern "C"
#else
#	define CLASS_EXPORT		__declspec(dllexport)
#	define CLASS_IMPORT		__declspec(dllimport)
#	define API_EXPORT		extern "C" __declspec(dllexport)
#	define API_IMPORT		extern "C" __declspec(dllimport)
#endif

//#define Conn(x,y) Conn1(x,y)
//#define Conn1(x,y) x##y
//
//#define OCI_DllExport_Define(Project, Package) \
//#ifdef Conn(Project, _EXPORTS)\
//    #define ##Package_API API_EXPORT\
//    #define ##Package_CLASS CLASS_EXPORT\
//#else    \
//    #define ##Package_API API_IMPORT\
//    #define ##Package_CLASS CLASS_IMPORT\
//    #ifndef BASE_LINK\
//        #define ##Project_LINK\
//        #pragma comment(lib, #Project".lib")\
//        #pragma message("Automatically linking with "#Project"Base.dll") \
//    #endif\
//#endif


//#if defined(_LIB) || defined(_USELIB) || defined(BASE_STATIC_LIB) || defined(BASE_STATIC_LIB_MD)
//#	define OCI_EXPORT
//#if defined(BASE_STATIC_LIB)
//#	pragma comment(lib, MAKE_LIB_NAME(Base))
//#elif defined(BASE_STATIC_LIB_MD)
//#	pragma comment(lib, "libBase_md.lib")
//#endif
//#elif defined(BASE_EXPORTS)
//#	define OCI_EXPORT	CLASS_EXPORT
//#else
//#	define OCI_EXPORT	CLASS_IMPORT
//#	pragma comment(lib, MAKE_DLL_LIB_NAME(Base))
//#endif


//#if defined(_LIB) || defined(_USELIB) || defined(BASE_STATIC_LIB)
//#	define OCI_API	extern "C"
////#		pragma comment(lib, MAKE_LIB_NAME(Base))
//#elif defined(BASE_EXPORTS)
//#	define OCI_API	extern "C" OCI_EXPORT
//#else
//#	define OCI_API	extern "C"
////#		pragma comment(lib, MAKE_DLL_LIB_NAME(Base))
//#endif


/*
// 微软的安全警告
#if defined(RKT_WIN32) && defined(RKT_COMPILER_MSVC) && RKT_COMPILER_VER > 1300
#	ifndef _CRT_NONSTDC_NO_WARNINGS
#		define _CRT_NONSTDC_NO_WARNINGS
#	endif
#	ifndef _CRT_SECURE_NO_WARNINGS
#		define _CRT_SECURE_NO_WARNINGS
#	endif
#	ifndef _CRT_SECURE_NO_DEPRECATE
#		define _CRT_SECURE_NO_DEPRECATE
#	endif
#	ifndef _CRT_NONSTDC_NO_DEPRECATE
#		define _CRT_NONSTDC_NO_DEPRECATE
#	endif
#endif
*/



// 只定义无符号的简单类型，便于书写
typedef unsigned char			uchar, BYTE, u_char;
typedef unsigned short			ushort, u_short, WORD;
typedef unsigned int			uint, u_int, size_t;
typedef unsigned long			ulong, u_long, DWORD;
typedef __int64					int64;
typedef unsigned __int64		uint64;


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
}

#define STATIC_ASSERT(exp) \
	typedef COMPILE_ASSERT::compile_assert_test< \
	sizeof(COMPILE_ASSERT::COMPILE_ASSERT_FAILURE< (bool)(exp) >) \
	> compile_assert_typedef_

//} // namespace


// hash_map的定义，为了不关心命名空间的差异，可直接用 HashMap 宏
#if _MSC_VER  > 1300
    #define HashMap stdext::hash_map
#else
    #define HashMap std::hash_map
#endif



// 下面这个宏用于方便设置断点
//#define	breakable		switch(1) case 1: default:

#define isValidString(s)	(s && *s != 0)

///// 调试器中断
//void debuggerBreak(const char* exp, const char* file, uint line, bool* ignore = 0);
//
//#ifdef _DEBUG
//#	define debugBreak			{static bool __s_ignore__ = false; debuggerBreak(0, __FILE__, __LINE__, &__s_ignore__);}
//#	define debugBreakEx(str)	{static bool __s_ignore__ = false; debuggerBreak(str, __FILE__, __LINE__, &__s_ignore__);}
//#else
//#	define debugBreak			(void)0
//#	define debugBreakEx(str)	(void)0
//#endif


#endif // OCI_COMMON_H
