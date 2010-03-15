/*H***************************************************************************
 @File            : ParseString.h
 Subsystem       : 
 Function Name(s): CParseString
 Author          : YiQiu
 Date            : 2010-2-2  
 Time            : 15:41
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _PARSESTRING_H_2010_2
#define _PARSESTRING_H_2010_2

#include <deque>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

BEGIN_BASE_ENGINE

namespace StringHelp
{
/**
* @note Parse String like  12;45;fgf;
* @version 1.0
*/
template<const char cDelimiter = ';', bool bFilterNull = true>
class CParseString
{
public:
    CParseString(const char* pStr = NULL){ Parse(pStr); }

    //typedef const string& const_reference;
public:
    size_t Count(){ return m_DataQueue.size(); }
    void Reset(const char* pStr) {Clear(); Parse(pStr); }
    void Clear(){ return m_DataQueue.clear(); }

public:
    void At(size_t _Pos, int& _Value)
    {
        const string& strTmp = (*this)[_Pos];
        _Value = atoi(strTmp.c_str());
    }
    void At(size_t _Pos, unsigned int& _Value)
    {
        const string& strTmp = (*this)[_Pos];
        _Value = (unsigned int)strtoul(strTmp.c_str());
    }
    void At(size_t _Pos, long& _Value)
    {
        const string& strTmp = (*this)[_Pos];
        _Value = atol(strTmp.c_str());
    }
    void At(size_t _Pos, unsigned long& _Value)
    {
        const string& strTmp = (*this)[_Pos];
        _Value = strtoul(strTmp.c_str(), NULL, 10);
    }
    void At(size_t _Pos, bool& _Value)
    {
        const string& strTmp = (*this)[_Pos];
        _Value = (bool)atoi(strTmp.c_str());
    }
    void At(size_t _Pos, double& _Value)
    {
        const string& strTmp = (*this)[_Pos];
        _Value = atof(strTmp.c_str());
    }
    void At(size_t _Pos, string& _Value)
    {
        _Value = (*this)[_Pos];
    }
    void At(size_t _Pos, __int64& _Value)
    {
        const string& strTmp = (*this)[_Pos];
        _Value = _atoi64(strTmp.c_str());
    }
    void At(size_t _Pos, unsigned __int64& _Value)
    {
        const string& strTmp = (*this)[_Pos];
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
        if ( NULL == pStr ){ return; }

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

/**
 @note Make String like 12;45;fgf;
 @version 1.0
*/
template<const char cDelimiter = ';'>
class CMakeString
{
public:
    CMakeString(){ m_buf<< std::setw(4);}
    typedef CMakeString<cDelimiter> _Myt;

    template<typename T>
    _Myt& operator << (T _Value)
    {
        m_buf << _Value << cDelimiter;
        return *this;
    }

    string str(){ return m_buf.str(); }
protected:
    stringstream m_buf;
};


} // namespace StringHelp


END_BASE_ENGINE



#endif  // _PARSESTRING_H_2010_


 // End of file



