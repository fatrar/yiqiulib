/**H**************************************************************************
 File            : Common.h
 Subsystem       : 
 Function Name(s): CCommon
 Author          : YiQiu
 Date            : 2008-1-2   16:08 
 Time            : 10:17
 Description     : ����ͷ�ļ�
 
 ������
 1. ��������������������һ�£���ͬ���Ǳ�׼ö�ٶ���Ϊȫ��д���»��ߣ����ִ������й��˿���д��ϰ�ߵ����⣬
 �����Ҹ���Ϊÿ�������ִ�д���»������Ӹ��ʣ���windowsƽ̨�������������������ͻ������
  �о�����˼�ĺ궨��Ҳ�ǲ������ַ�ʽ�����������ַ�����һ�����֣�һ���꺯����
 2. �����ڲ�ͬ������ò�ͬ�ı�д��׼���ڱ�֤���ܵ�ǰ���£�
 ��Ч��Ҫ��ܸߵ�����£���ִ��Ч��Ϊ�ȣ�����������������׶�Ϊ��Ρ�
 ��
 3.ע����Ϊ�˷���doxygen�������ĵ�����������Ӧ�ķ��
 Revision        : 


 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef OCI_COMMON_H
#define OCI_COMMON_H

//#include "Config.h" // �����û�������


#define BEGIN_BASE_ENGINE namespace OCI {
#define END_BASE_ENGINE };

#define USING_BASE_ENGINE using namespace OCI;

/// ģ������ú�׺
#if defined(_DEBUG)
#	define CFG_SUFFIX		"_d"
#else
#	define CFG_SUFFIX		""
#endif

	/// ��̬��ǰ��׺
#define DLL_PREFIX	""
#define DLL_SUFFIX	".dll"


	/// ��Ͽ��ļ���
#define MAKE_DLL_NAME(module)			"./"DLL_PREFIX#module""CFG_SUFFIX""DLL_SUFFIX
#define	MAKE_LIB_NAME(module)			"lib"#module""CFG_SUFFIX
#define MAKE_DLL_LIB_NAME(module)		#module""CFG_SUFFIX


// ���뵼����־
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
// ΢��İ�ȫ����
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



// ֻ�����޷��ŵļ����ͣ�������д
typedef unsigned char			uchar, BYTE, u_char;
typedef unsigned short			ushort, u_short, WORD;
typedef unsigned int			uint, u_int, size_t;
typedef unsigned long			ulong, u_long, DWORD;
typedef __int64					int64;
typedef unsigned __int64		uint64;


// ��ȫɾ�����ͷ�
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


/** ������Assert,�ο�boost��,�磺
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


// hash_map�Ķ��壬Ϊ�˲����������ռ�Ĳ��죬��ֱ���� HashMap ��
#if _MSC_VER  > 1300
    #define HashMap stdext::hash_map
#else
    #define HashMap std::hash_map
#endif



// ������������ڷ������öϵ�
//#define	breakable		switch(1) case 1: default:

#define isValidString(s)	(s && *s != 0)

///// �������ж�
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
