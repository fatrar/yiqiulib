/*H***************************************************************************
 File            : Singleton.h
 Subsystem       : 
 Function Name(s): 
 Author          : YiQiu
 Date            : 2008-5-13  
 Time            : 13:42
 Description     : ��ʵ��ģ��

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _SINGLETON_H_2010_
#define _SINGLETON_H_2010_


#include <assert.h>


/** ��ʵ��ģ��.

 ʾ����
 @code
 struct CXXSingleton : public SingletonEx<CFooSingleton>
 {
	Void foo()
	{
		Trace("foo!\n");
	}
 };	

 int main(int,int*)
 {
	new CXXSingleton();
	
	// ���� foo �����ķ�����
	CXXSingleton::getInstance().foo();

	delete CXXSingleton::getInstancePtr();
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
	static /*__cdecl*/ void releaseInstance()
	{
		if (_instance && mustDelete)
		{
			delete _instance;
			_instance = 0;
		}
	}
};

/// ��̬ʵ��ָ���ʼ��
template <class T, bool mustDelete> T* Singleton<T, mustDelete>::_instance = 0;



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



#endif // End of file


