/**H**************************************************************************
 File            : ResReader.h
 Subsystem       : 
 Function Name(s): CResFileReader, CMemResReader
 Author          : YiQiu
 Date            : 2010-10-8  
 Time            : 14:30
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _RESFILEREADER_H_2010_10
#define _RESFILEREADER_H_2010_10

#include "ResReaderBase.h"
namespace FileSystem{class CFile;}

namespace ResFile
{

template<DWORD Version> 
class CResFileReader:
    public CResReaderBase<Version>
{
public:
    CResFileReader(
        FileSystem::CFile* pResFile, 
        const TFileHeadBase& HeadBase );
    virtual ~CResFileReader(void);
    
    // CResReaderBase
public:
    virtual void GetDataHead(DWORD dwOffset, DataHead& Head);
    virtual void ReadData(
        DWORD dwOffset, void*& pBuf,
        size_t nRead, bool& bisRef);
    virtual void ReleaseReadBuf(void*& p)
    {
        delete[] p;
        p = NULL;
    }

protected:
    void* GetFileReadBuf(size_t nBufNeed)
    {
        return new char[nBufNeed];
    }
  
protected:
    FileSystem::CFile* m_ResFile;
};

template<DWORD Version> 
class CMemResReader : 
    public CResReaderBase<Version>
{
public:
    CMemResReader(
        BYTE* pData, size_t nSize,
        bool bRef, bool bDelete );
    virtual ~CMemResReader();

    // CResReaderBase
public:
    virtual void GetDataHead(DWORD dwOffset, DataHead& Head);
    virtual void ReadData(
        DWORD dwOffset, void*& pBuf,
        size_t nRead, bool& bisRef);
    virtual void ReleaseReadBuf(void*& p){p=NULL;}

protected:
    BYTE* m_pData;
    size_t m_nSize;
    bool m_bDelete;
};


}


#endif  // _RECFILEREADER_H_2010_


// End of file