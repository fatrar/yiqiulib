/*H***************************************************************************
 File            : Thread.h
 Subsystem       : 
 Function Name(s): Thread
 Author          : YiQiu
 Date            : 2010-1-13  
 Time            : 16:32
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _THREAD_H_2010_1_
#define _THREAD_H_2010_1_



#ifdef SYSTEM_EXPORTS
    #define THREAD_API __declspec(dllexport) 
#else    
    #define THREAD_API __declspec(dllimport)
    #ifndef SYSTEM_LINK
        #define SYSTEM_LINK
        #pragma comment(lib, "System.lib")
        #pragma message("Automatically linking with System.dll") 
    #endif
#endif


BEGIN_BASE_ENGINE


class AutoCriticalSection;

class THREAD_API CriticalSection
{
public:
    friend AutoCriticalSection;

    CriticalSection();
    virtual ~CriticalSection();

    void Lock();
    void Unlock();

protected:
    CRITICAL_SECTION critical_section;
};

//
// user get a init CRITICAL_SECTION, auto lock and unlock
//
class THREAD_API AutoCriticalSection
{
public:
    AutoCriticalSection(CRITICAL_SECTION& critical_section);
    AutoCriticalSection(CriticalSection& critical_section);
    ~AutoCriticalSection();
private:
    CRITICAL_SECTION& m_critical;
};

#define Conn(x,y) Conn1(x,y)
#define Conn1(x,y) x##y

#define AUTOLOCKNAME  Conn(cs,__LINE__)
#define AUTOLOCKNAME1  Conn(cs_,__LINE__)

#define AutoLockAndUnlock(sec)  AutoCriticalSection AUTOLOCKNAME(sec)

#define SingleFunctionAutoLock()  static CriticalSection AUTOLOCKNAME(); AutoCriticalSection AUTOLOCKNAME1(AUTOLOCKNAME)
// ex  -- > like
/*
void Add()
{
static CriticalSection cs;
AutoCriticalSection cs2(cs);
// ....
} 
<-  ==  ->
void Add()
{
SingleFunctionAutoLock();
// ....
}
*/




END_BASE_ENGINE // namespace

















#endif  // End of file



