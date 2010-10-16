/**CPP*************************************************************************
 File            : HashHelper.cpp
 Subsystem       : 
 Function Name(s): CHashHelper
 Author          : YiQiu
 Date            : 2010-10-14  
 Time            : 15:01
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
**************************************************************************cpp**/
#include "HashHelper.h"



namespace OCI
{



CHashHelper::CHashHelper()
{
    m_pdwTable = new DWORD[0x500];

    DWORD seed = 0x00100001, index1 = 0, index2 = 0, i;
    DWORD temp1, temp2;
    for(index1 = 0; index1 < 0x100; index1++)
    {
        for(index2 = index1, i = 0; i < 5; i++, index2 += 0x100)
        {
            seed = (seed * 125 + 3) % 0x2AAAAB;
            temp1 = (seed & 0xFFFF) << 0x10;

            seed = (seed * 125 + 3) % 0x2AAAAB;
            temp2 = (seed & 0xFFFF);

            m_pdwTable[index2] = (temp1 | temp2);
        }
    }
}

CHashHelper::~CHashHelper()
{
    delete[] m_pdwTable;
}

inline int CharUpper(char lc)
{
    if (lc>='a' && lc<='z') return lc+'A'-'a';
    return lc;
}

DWORD CHashHelper::HashString( const char* pStr, DWORD dwHashType )
{
    DWORD seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
    while (*pStr)
    {
        int uc = CharUpper(*pStr++);
        int index = (dwHashType << 8) + uc;
        seed1 = m_pdwTable[index] ^ (seed1 + seed2);
        seed2 = uc + seed1 + seed2 + (seed2 << 5) + 3;
    }
    return seed1;
}

uint64 CHashHelper::HashStringEx( const char* pStr )
{
    DWORD seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
    DWORD seed3 = 0x7FED7FED, seed4 = 0xEEEEEEEE;
    while (*pStr)
    {
        int uc = CharUpper(*pStr++);
        int index1 = 256 + uc;
        
        seed1 = m_pdwTable[index1] ^ (seed1 + seed2);
        seed2 = uc + seed1 + seed2 + (seed2 << 5) + 3;

        int index2 = 512 + uc;
        seed3 = m_pdwTable[index2] ^ (seed3 + seed4);
        seed4 = uc + seed3 + seed4 + (seed4 << 5) + 3;
    }
    
    uint64 nRc = seed1;
    return (nRc<<32) + seed3;
}

DWORD HashString( const char* pStr, DWORD dwHashType )
{
    return CHashHelper::getInstance().HashString(pStr, dwHashType);
}

uint64 HashStringEx( const char* pStr )
{
    return CHashHelper::getInstance().HashStringEx(pStr);
}


}



// End of file


