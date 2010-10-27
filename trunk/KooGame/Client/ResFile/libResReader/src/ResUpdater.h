/**H**************************************************************************
 File            : ResUpdater.h
 Subsystem       : 
 Function Name(s): CResUpdater
 Author          : YiQiu
 Date            : 2010-10-27  
 Time            : 17:22
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _RESUPDATER_H_2010_10
#define _RESUPDATER_H_2010_10

#include "FileSystem.h"
#include "ResFileDef.h"

namespace ResFile
{

class CResUpdater :
    public IResUpdater
{
public:
    CResUpdater(
        const char* pFilepath,
        BYTE* pData,
        size_t nSize,
        bool bAutoDel = true );
    ~CResUpdater(void);

    // IResUpdater
public:
    bool ParsePatchData();

    bool Update();

    typedef TFileHead<File_Version_1_0> FileHead;
    typedef FileHead::TDataIndex DataIndex;
    typedef TDataInfo<File_Version_1_0> DataInfo;

protected:
    bool RemoveOld(
        const UHashValue* pRemoveList,
        DWORD dwRemoveFileCount );

    FileHead* GetResFileHead();

protected:
    bool m_bAutoDel;
    FileSystem::CFile m_ResFile;
    BYTE* m_pPatchData;
    size_t m_nSize;
};



}


#endif  // _RESUPDATER_H_2010_


// End of file



