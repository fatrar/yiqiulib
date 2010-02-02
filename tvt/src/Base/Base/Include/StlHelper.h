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


#define STL_SAFE_DELETE(x)	{if(x){delete (x);(x)=0;}}
#define STL_SAFE_RELEASE(x)	{if(x){(x)->release();(x)=0;}}

template<class T>void STLDeleteAssociate(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		STL_SAFE_DELETE((*i).second);
	}
	t.clear();
}

template<class T>void STLDeleteSequence(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		STL_SAFE_DELETE(*i);
	}
	t.clear();
}

template<class T>void STLReleaseAssociate(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		STL_SAFE_RELEASE((*i).second);
	}
	t.clear();
}

template<class T>void STLReleaseSequence(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		(*i)->release();
	}
	t.clear();
}

template<class T,class TS>void STLDeleteAssociateSequence(T& t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		TS *pTS = (*i).second;
		if(pTS)
		{
			STLDeleteSequence(*pTS);
		}
		STL_SAFE_DELETE((*i).second);
	}
	t.clear();
}





#endif  // _STLHELPER_H_2010_



