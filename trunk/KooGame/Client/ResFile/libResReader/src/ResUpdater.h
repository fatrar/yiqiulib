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
    // IResUpdater
public:
    virtual bool Update(const char* pFilepath);

protected:
    typedef TFileHead<File_Version_1_0> FileHead;
    typedef FileHead::TDataIndex DataIndex;
    typedef TDataInfo<File_Version_1_0> DataInfo;

protected:
    void GetReserveDataIndexFromOldFile(
        FileHead* OldFileHead,
        const UHashValue* pRemoveList,
        DWORD dwRemoveFileCount );

    bool WriteOldData(
        FileHead* OldFileHead,
        DWORD dwFileCount,
        const UHashValue* pRemoveList,
        DWORD dwRemoveFileCount );

     void WriteFileHead();

    // Must Impl 
protected:
    virtual TResPatchFileHead* GetPatchFileHead() = 0;

    virtual void DestroyPatchFileHead(
        TResPatchFileHead*& Head){};

    virtual bool WriteNewData(
        DataIndex* pAddDataIndex,
        DWORD dwAddFileCount ) = 0;

protected:
    FileSystem::CFile m_OldResFile;
    FileSystem::CFile m_NewResFile;
    DWORD m_dwPosNow;

    set<DataIndex> m_NewFileDataIndex;
};


class CResUpdaterByPatchFile :
    public CResUpdater
{
public:
    CResUpdaterByPatchFile(
        const char* pPatchFilePath );

protected:
    virtual TResPatchFileHead* GetPatchFileHead();

    virtual void DestroyPatchFileHead(
        TResPatchFileHead*& Head);

    virtual bool WriteNewData(
        DataIndex* pAddDataIndex,
        DWORD dwAddFileCount ); 

private:
    FileSystem::CFile m_PatchFile;
};

class CResUpdaterByPatchData:
    public CResUpdater
{
public:
    inline CResUpdaterByPatchData(
        BYTE* pPatchData, size_t nSize, bool bAutoDel = true );
    ~CResUpdaterByPatchData();

protected:
    virtual TResPatchFileHead* GetPatchFileHead()
    {
        return (TResPatchFileHead*)m_pPatchData;
    }

    virtual bool WriteNewData(
        DataIndex* pAddDataIndex,
        DWORD dwAddFileCount );

private:
    bool m_bAutoDel;
    BYTE* m_pPatchData;
    size_t m_nSize;
};


}


#endif  // _RESUPDATER_H_2010_


// End of file



