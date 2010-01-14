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


/** 单实例模板.

 示例：
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
	
	// 调用 foo 函数的方法：
	CFooSingleton::getInstance().foo();

	delete CFooSingleton::getInstancePtr();
 }
 @endcode
 */

	template<class T, bool mustDelete = true>
	class Singleton
	{
		static T*	_instance;		/// 实例静态指针
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
		/// 使用保护构造是为了用户不能在栈上声明一个实例
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

	/// 静态实例指针初始化
	template <class T, bool mustDelete> T* Singleton<T, mustDelete>::_instance = NULL;



	/// 扩展的单实体模板，不关心对象的创建和销毁
	/// 采用外部new和delete，这种单实体的好处是外部能控制构造和析构的顺序
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


