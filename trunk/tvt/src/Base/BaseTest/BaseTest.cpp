// BaseTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#define USEBASEENINE_NAMESPACE
#include "..\Base\Include\singleton.h"
using namespace OCI;
#include <iostream>
#include <Windows.h>
#include <map>
#include <deque>
#include <string>
#include <list>
#include <memory>
using namespace std;


#include "F:\tvt\src\DVR Server\DEVICECONTROL\\WPG_EventOccurrence.h"

template<const char cDelimiter = ';', bool bFilterNull = true>
class CParseString
{
public:
    CParseString(const char* pStr = NULL){ Parse(pStr); }

    //typedef const string& const_reference;
public:
    size_t Count(){ return m_DataQueue.size() };
    void Reset(const char* pStr) {Clear(); Parse(pStr); }
    void Clear(){ return m_DataQueue.clear(); }

public:
    void At(size_t _Pos, int& _Value)
    {
        const string& strTmp = [_Pos];
        _Value = atoi(strTmp.c_str());
    }
    void At(size_t _Pos, unsigned int& _Value)
    {
        const string& strTmp = [_Pos];
        _Value = (unsigned int)strtoul(strTmp.c_str());
    }
    void At(size_t _Pos, long& _Value)
    {
        const string& strTmp = [_Pos];
        _Value = atol(strTmp.c_str());
    }
    void At(size_t _Pos, unsigned long& _Value)
    {
        const string& strTmp = [_Pos];
        _Value = strtoul(strTmp.c_str(), NULL, 10);
    }
    void At(size_t _Pos, bool& _Value)
    {
        const string& strTmp = [_Pos];
        _Value = (bool)atoi(strTmp.c_str());
    }
    void At(size_t _Pos, double& _Value)
    {
        const string& strTmp = [_Pos];
        _Value = atof(strTmp.c_str());
    }
    void At(size_t _Pos, string& _Value)
    {
        _Value = [_Pos];
    }
    void At(size_t _Pos, __int64& _Value)
    {
        const string& strTmp = [_Pos];
        _Value = _atoi64(strTmp.c_str());
    }
    void At(size_t _Pos, unsigned __int64& _Value)
    {
        const string& strTmp = [_Pos];
        _Value = _strtoui64(strTmp.c_str(), NULL, 10);
    }
  
public:
    const string& operator[](size_t _Pos) const
    {
        return m_DataQueue[_Pos];
    }
protected:
    void Parse(const char* pStr)
    {
        const char *pNew, *pOld = pStr;
        for (pNew=pStr; *pNew ; ++pNew)
        {
            if (*pNew != cDelimiter)
            {
                continue;
            }

            size_t nSize = pNew-pOld;
            if ( nSize==0 && bFilterNull )
            {
                pOld = pNew+1;
                continue;
            }

            m_DataQueue.push_back(string(pOld, nSize));
            pOld = pNew+1;
        }

        if ( pOld != pNew )
        {
            m_DataQueue.push_back(string(pOld, pNew-pOld));
            // = m_DataQueue.push_back(string(pOld));
        }
    }
protected:
    deque<string> m_DataQueue;
};


class CVoidType
{

};

int _tmain(int argc, _TCHAR* argv[])
{
    char szTest[] = "0123456789";
    string abc(szTest, size_t(0));
    int _Value = _strtoui64(abc.c_str(), NULL, 10);

    CParseString<> test(";aa;aaaa;;ggg");
    const string str = test[2];
    //type_info i =  typeid(i);

    int szbuf[4] = {-1};
    memset(szbuf, -1, sizeof(szbuf));
    int i = sizeof(WPG_Rule);  // 3748
    const int b = 0;
    const int& aa = b;
    list<int> lk;
  
	return 0;
}

