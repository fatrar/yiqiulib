/*H***************************************************************************
 File            : HardWareSupport.cpp
 Subsystem       : 
 Function Name(s): CHardWareSupport
 Author          : YiQiu
 Date            : 2010-2-27  
 Time            : 16:51
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "stdafx.h"
#include "HardWareSupport.h"

CPUSupport CPUSupport::s_CPUSupport;

CPUSupport::CPUSupport()
{
    DWORD dwEDX=0;
    __asm 
    {
        mov eax,1
        cpuid
        mov dwEDX,edx
    }

    m_bSupportMMX = (dwEDX&0x800000) != 0;
}







// End of file