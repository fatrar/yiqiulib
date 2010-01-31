/*H***************************************************************************
 File            : Memory.cpp
 Subsystem       : 
 Function Name(s): CMemory
 Author          : YiQiu
 Date            : 2010-1-30  
 Time            : 15:57
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "StdAfx.h"
#include "..\Include\Memory.h"

using namespace OCI;


void MemoryUtil::Delete( void* p )
{
    safeDelete(p);
}

void MemoryUtil::DeleteArray( void* p )
{
    safeDeleteArray(p);
}








// End of file