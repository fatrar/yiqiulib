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
#include "ResFileUtil.h"
#include "IResReader.h"
#include <set>
using namespace std;

namespace ResFile
{

class CResUpdater :
    public IResUpdater,
    public Util::CUnpackVolumeUtil
{
    // IResUpdater
public:
    virtual bool Update(const char* pFilepath);

    // CUnpackVolumeUtil
protected:
    virtual void DataReadCallBack(
        DataHead1* pHead, BYTE* pData);

protected:
    inline void GetOldFileHead(const char* pFilepath);

    void GetReserveDataIndexFromOldFile(
        FileHead0* OldFileHead,
        const UHashValue* pRemoveList,
        DWORD dwRemoveFileCount );

    bool WriteOldData(
        DWORD dwFileCount,
        const UHashValue* pRemoveList,
        DWORD dwRemoveFileCount );

     void WriteFileHead();

protected:
    inline CResUpdater()
        : m_pPatchFileHead(NULL)
        , m_pOldFileHead(NULL){}
    inline ~CResUpdater();

    // Must Impl 
protected:
    virtual TPatchFileHeadBase* GetPatchFileHead() = 0;

    virtual void DestroyPatchFileHead(
        TPatchFileHeadBase*& Head){};

//     virtual bool WriteNewData(
//         DataIndex1* pVolumeIndex,
//         DWORD dwVolumeCount ) = 0;

protected:
    FileSystem::CFile m_OldResFile;
    FileSystem::CFile m_NewResFile;
    DWORD m_dwPosNow;

    set<DataIndex0> m_NewFileDataIndex;

    // new Add
    TPatchFileHeadBase* m_pPatchFileHead;
    FileHead0* m_pOldFileHead;
};


class CResUpdaterByPatchFile :
    public CResUpdater
{
public:
    CResUpdaterByPatchFile(
        const char* pPatchFilePath );

    // CUnpackVolumeUtil
protected:
    virtual DWORD Read(
        DWORD dwOffset, BYTE* pBuf, DWORD dwLen)
    {
        m_PatchFile.Seek(dwOffset);
        return m_PatchFile.Read(pBuf, dwLen);
    }

    // CResUpdater
protected:
    virtual TPatchFileHeadBase* GetPatchFileHead();

    virtual void DestroyPatchFileHead(
        TPatchFileHeadBase*& Head);

//     virtual bool WriteNewData(
//         DataIndex1* pVolumeIndex,
//         DWORD dwVolumeCount ); 

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

    // CUnpackVolumeUtil
protected:
    virtual DWORD Read(
        DWORD dwOffset, BYTE* pBuf, DWORD dwLen);

    // CResUpdater
protected:
    virtual TPatchFileHeadBase* GetPatchFileHead()
    {
        return (TPatchFileHeadBase*)m_pPatchData;
    }

//     virtual bool WriteNewData(
//         DataIndex1* pVolumeIndex,
//         DWORD dwVolumeCount );

private:
    bool m_bAutoDel;
    BYTE* m_pPatchData;
    size_t m_nSize;
};


}


#endif  // _RESUPDATER_H_2010_


// End of file



