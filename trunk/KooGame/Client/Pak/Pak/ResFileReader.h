/**H**************************************************************************
 File            : ResFileReader.h
 Subsystem       : 
 Function Name(s): CResFileReader
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

#include "ResFileDef.h"
#include "IResFileReader.h"
#include <FileSystem.h>

namespace ResFile
{
template<DWORD Version> 
class CResFileReader:
    public IResReader
{
public:
    CResFileReader(
        FileSystem::CFile* pResFile, 
        const TFileHeadBase& HeadBase );
    virtual ~CResFileReader(void);

    // IResReader
public:
    virtual bool Parse();
    virtual size_t GetDataLen(const char* pFileName);
    virtual bool GetData(
        const char* pFileName,
        CUnPackDataInfo& UnPackDataInfo);

    virtual void Release(CUnPackDataInfo* pUnPackDataInfo);

protected:


protected:
    DWORD m_dwHeadSize, m_dwFileCount;
    FileSystem::CFile* m_ResFile;
};


class CMemResReader : 
    public ResFile::IResReader
{
public:
    CMemResReader();
    virtual ~CMemResReader();
};




}


#endif  // _RECFILEREADER_H_2010_


// End of file