/*H***************************************************************************
 File            : HardWareSupport.h
 Subsystem       : 
 Function Name(s): CHardWareSupport
 Author          : YiQiu
 Date            : 2010-2-27  
 Time            : 16:41
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _HARDWARESUPPORT_H_2010_2
#define _HARDWARESUPPORT_H_2010_2




class CPUSupport
{
protected:
    CPUSupport();
public:
    static CPUSupport s_CPUSupport;
public:
    bool m_bSupportMMX;
};






#endif  // _HARDWARESUPPORT_H_2010_


// End of file



