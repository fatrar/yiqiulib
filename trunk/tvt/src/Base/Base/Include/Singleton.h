/*H***************************************************************************
 File            : Singleton.h
 Subsystem       : 
 Function Name(s): CSingleton
 Author          : YiQiu
 Date            : 2010-1-13  
 Time            : 13:42
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _SINGLETON_H_2010_
#define _SINGLETON_H_2010_
#include "Common.h"
#include <assert.h>

BEGIN_BASE_ENGINE


/** ��ʵ��ģ��.

 ʾ����
 @code
 struct CFooSingleton : public SingletonEx<CFooSingleton>
 {
	Void foo()
	{
		Trace("foo!\n");
	}
 };	

 int main(int,int*)
 {
	new CFooSingleton();
	
	// ���� foo �����ķ�����
	CFooSingleton::getInstance().foo();

	delete CFooSingleton::getInstancePtr();
 }
 @endcode
 */

	template<class T, bool mustDelete = true>
	class Singleton
	{
		static T*	_instance;		/// ʵ����ָ̬��
	public:
		static T& getInstance()
		{
			if (!_instance)
			{
				_instance = new T;
				if (mustDelete) atexit(releaseInstance);
			}

			return *_instance;
		}

        static T* getInstancePtr()
        {
            if (!_instance)
            {
                _instance = new T;
                if (mustDelete) atexit(releaseInstance);
            }

            return _instance;
        }

	protected:
		/// ʹ�ñ���������Ϊ���û�������ջ������һ��ʵ��
		Singleton() { }
		static void __cdecl releaseInstance()
		{
			if (_instance && mustDelete)
			{
				delete _instance;
				_instance = 0;
			}
		}
	};

	/// ��̬ʵ��ָ���ʼ��
	template <class T, bool mustDelete> T* Singleton<T, mustDelete>::_instance = NULL;



	/// ��չ�ĵ�ʵ��ģ�壬�����Ķ���Ĵ���������
	/// �����ⲿnew��delete�����ֵ�ʵ��ĺô����ⲿ�ܿ��ƹ����������˳��
	template <typename T>
	class SingletonEx
	{
	protected:
		static T*	_instance;

	public:
		SingletonEx()
		{
			assert(!_instance);
#if defined(_MSC_VER) && (_MSC_VER < 1200)	 
			int offset = (int)(T*)1 - (int)(SingletonEx<T>*)(T*)1;
			_instance = (T*)((int)this + offset);
#else
			_instance = static_cast<T*>(this);
#endif
		}

		~SingletonEx()
		{
			assert(_instance);
			_instance = 0;
		}

		static T& getInstance()		{ assert(_instance); return (*_instance); }
		static T* getInstancePtr()	{ return _instance; }
	};

	template <typename T> T* SingletonEx<T>::_instance = 0;


END_BASE_ENGINE // namespace


#endif // End of file


