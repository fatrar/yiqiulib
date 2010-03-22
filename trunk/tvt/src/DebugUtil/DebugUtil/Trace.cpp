/**CPP*************************************************************************
 File            : Trace.cpp
 Subsystem       : 
 Function Name(s): CTrace
 Author          : YiQiu
 Date            : 2010-3-22  
 Time            : 13:48
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
**************************************************************************cpp**/
#include "Trace.h"

#ifdef _DEBUG


#pragma warning(disable: 4996)

#define  Max_Buf_Size  1024
static const char s_strLine[] = "=======================================================\n";



void DebugOut(const char* format, ...)
{
    char szbuf[Max_Buf_Size] = {0};
    va_list args;
    va_start(args, format);
    vsprintf_s(szbuf, format, args);
    va_end(args);

    OutputDebugString(szbuf);
    fprintf(stdout, "%s", szbuf) ;
}

void DebugOutIndex( const char *format, ... )
{
    fprintf(stdout,  s_strLine) ;
    char szbuf[Max_Buf_Size] = {0};
    va_list args;
    va_start(args, format);
    vsprintf_s(szbuf, format, args);
    va_end(args);

    OutputDebugString(s_strLine);
    OutputDebugString(szbuf);
    OutputDebugString(s_strLine);

    fprintf(stdout, "%s", szbuf ) ;
    fprintf(stdout, s_strLine) ;
}


void KAssert(const char *format, ...)
{
    char szbuf[Max_Buf_Size] = {0};
    va_list args;
    va_start(args, format);
    vsprintf_s(szbuf, format, args);
    va_end(args);

    OutputDebugString(szbuf);

    fprintf (stderr, "%s", szbuf) ;
    exit ( EXIT_FAILURE ) ;
}


#endif // _DEBUG









// End of file