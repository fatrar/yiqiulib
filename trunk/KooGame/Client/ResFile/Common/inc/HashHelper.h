/**H**************************************************************************
 File            : HashHelper.h
 Subsystem       : 
 Function Name(s): CHashHelper
 Author          : YiQiu
 Date            : 2010-10-14  
 Time            : 15:00
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _HASHHELPER_H_2010_10
#define _HASHHELPER_H_2010_10

#include "Common.h"
#include "Singleton.h"

namespace OCI
{

class CHashHelper :
    public Singleton<CHashHelper>
{
public:
    DWORD HashString(const char* pStr, DWORD dwHashType = 0);
    uint64 HashStringEx(const char* pStr);
public:
    CHashHelper();
    ~CHashHelper();
protected:
    DWORD* m_pdwTable;
};

DWORD HashString(const char* pStr, DWORD dwHashType = 0);

uint64 HashStringEx(const char* pStr);
}







#endif  // _HASHHELPER_H_2010_


 // End of file



