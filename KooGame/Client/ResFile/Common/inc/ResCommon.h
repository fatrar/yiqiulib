/**H**************************************************************************
 File            : ResCommon.h
 Subsystem       : 
 Function Name(s): 
 Author          : YiQiu
 Date            : 2010-10-8  
 Time            : 16:21
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _RECCOMMON_H_2010_10
#define _RECCOMMON_H_2010_10





namespace ResFile
{

typedef unsigned int DWORD;
typedef unsigned int size_t;
typedef unsigned short WORD; 
typedef unsigned char BYTE;

#if defined(_MSC_VER) && (_MSC_VER <= 1200)	
	typedef unsigned __int64 QWORD;
#else
	typedef unsigned long long QWORD;
#endif

#define NULL 0

namespace Util
{

};


};







#endif  // _RECCOMMON_H_2010_


 // End of file



