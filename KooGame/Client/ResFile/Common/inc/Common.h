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

 Revision        : 3.0


 History
 -------
 2008-01-02   1.0  �����������
 2009-11-18   2.0  ��1.0�Ļ����Ͻ�ԭ�л������ڴ������OCI�ڡ�����ΪOCI������
 2010-10-08   3.0  Ϊ����Ӧ��Ϸ��Ϸƽ̨�������ӿ�ƽ̨����

 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef OCI_COMMON_H
#define OCI_COMMON_H


//#include "Config.h" // �����û�������


#define BEGIN_BASE_ENGINE namespace OCI {
#define END_BASE_ENGINE }

#define USING_BASE_ENGINE using namespace OCI;

// ���뵼����־
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

// ֻ�����޷��ŵļ����ͣ�������д
typedef unsigned char			uchar, BYTE, u_char;
typedef unsigned short			ushort, u_short, WORD;
typedef unsigned int			uint, u_int, size_t;
typedef unsigned long			ulong, u_long, DWORD;
typedef __int64					int64;
typedef unsigned __int64		uint64;

};

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
};

#define STATIC_ASSERT(exp) \
    typedef COMPILE_ASSERT::compile_assert_test< \
        sizeof(COMPILE_ASSERT::COMPILE_ASSERT_FAILURE< (bool)(exp) >) \
    > compile_assert_typedef_


// hash_map�Ķ��壬Ϊ�˲����������ռ�Ĳ��죬��ֱ���� HashMap ��
#if _MSC_VER  > 1300
#   define HashMap stdext::hash_map
#else
#   define HashMap std::hash_map
#endif

#define isValidString(s)	(s && *s != 0)



#endif