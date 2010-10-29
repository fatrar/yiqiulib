/*H***************************************************************************
 File            : StlHelper.h
 Subsystem       : 
 Function Name(s): 
 Author          : YiQiu
 Date            : 2008-5-2  
 Time            : 11:38
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _STLHELPER_H_2010_2
#define _STLHELPER_H_2010_2
#include "Common.h"


namespace StlHelper
{

//#define STL_SAFE_DELETE(x)	{if(x){delete (x);(x)=0;}}
//#define STL_SAFE_RELEASE(x)	{if(x){(x)->release();(x)=0;}}

template<class T>
void STLDeleteAssociate(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		safeDelete((*i).second);
	}
	t.clear();
}

template<class T>
void STLDeleteSequence(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		safeDelete(*i);
	}
	t.clear();
}

template<class T>
void STLReleaseAssociate(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		safeRelease((*i).second);
	}
	t.clear();
}

template<class T>
void STLReleaseSequence(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
        safeRelease(*i);
	}
	t.clear();
}

template<class T,class TS>
void STLDeleteAssociateSequence(T& t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		TS *pTS = (*i).second;
		if(pTS)
		{
			STLDeleteSequence(*pTS);
		}
		safeDelete((*i).second);
	}
	t.clear();
}

#if defined(_MSC_VER) && (_MSC_VER > 1200)	 // vc6 以下模板支持不好，下列函数屏蔽
template<class T, class K, size_t nSize>
void Array2STL(const T (&Array)[nSize], K& Q)
{
    for (size_t i = 0; i<nSize; ++i)
    {
        Q.push_back(Array[i]);
    }
}
template<class T, class K, size_t nSize>
void STL2Array(const T& t, K (&Array)[nSize])
{
    typename T::const_iterator i = t.begin();
    for( size_t j=0; 
	i != t.end() && j<nSize;
	++i, ++j)
    {
        Array[j] = *i;
    }
}
#endif

template<class T, class K>
void Array2STL(const T *Array, size_t nSize, K& Q)
{
    for (size_t i = 0; i<nSize; ++i)
    {
        Q.push_back(Array[i]);
    }
}

#if defined(_SET_) || defined(__ministl_set_h__) 
template<class T, size_t nSize>
void Array2STL(const T (&Array)[nSize], set<T>& Q)
{
    for (size_t i = 0; i<nSize; ++i)
    {
        Q.insert(Array[i]);
    }
}

/**
*@note 为解决，VC6在模板部分特化支持问题，将下面const去掉，以致于
*      编译器认为不是特化，而是重写
*/
template<class T>
void Array2STL(/*const*/ T *Array, size_t nSize, set<T>& Q)
{
    for (size_t i = 0; i<nSize; ++i)
    {
        Q.insert(Array[i]);
    }
}

#endif

template<class T, class K>
void STL2Array(const T& Q, K *Array, size_t nSize)
{
    typename T::const_iterator i = Q.begin();
    for( size_t j=0; 
         i != Q.end() && j<nSize;
         ++i, ++j)
    {
        Array[j] = *i;
    }
}

}

#endif  // _STLHELPER_H_2010_



