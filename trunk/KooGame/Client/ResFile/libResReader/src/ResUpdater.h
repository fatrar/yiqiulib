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
#include <string.h>
#include "FileSystem.h"
#include "ResFileDef.h"
#include "IResReader.h"
#include <set>
using namespace std;

namespace ResFile
{

class CResUpdater :
    public IResUpdater
{
public:
    CResUpdater(
        BYTE* pPatchData,
        size_t nSize,
        bool bAutoDel = true );
    ~CResUpdater(void);

    // IResUpdater
public:
    bool Update(const char* pFilepath);

protected:
    typedef TFileHead<File_Version_1_0> FileHead;
    typedef FileHead::TDataIndex DataIndex;
    typedef TDataInfo<File_Version_1_0> DataInfo;

protected:
    void GetReserveDataIndexFromOldFile(
        const UHashValue* pRemoveList,
        DWORD dwRemoveFileCount );

    bool WriteOldData(
        DWORD dwFileCount,
        const UHashValue* pRemoveList,
        DWORD dwRemoveFileCount );

    void WriteNewData(
        DataIndex* pAddDataIndex,
        DWORD dwAddFileCount );

    void WriteFileHead();

protected:
    bool m_bAutoDel;
    FileSystem::CFile m_OldResFile;
    FileSystem::CFile m_NewResFile;
    DWORD m_dwPosNow;
    BYTE* m_pPatchData;
    size_t m_nSize;

    FileHead* m_OldFileHead;

    set<DataIndex> m_NewFileDataIndex;
};



}


#endif  // _RESUPDATER_H_2010_


// End of file



