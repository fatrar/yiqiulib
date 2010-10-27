/**CPP*************************************************************************
 File            : ResUpdater.cpp
 Subsystem       : 
 Function Name(s): CResUpdater
 Author          : YiQiu
 Date            : 2010-10-27  
 Time            : 17:57
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
**************************************************************************cpp**/
#include "StdAfx.h"
#include "ResUpdater.h"


namespace ResFile
{
using namespace FileSystem;


CResUpdater::CResUpdater(
    const char* pFilepath,
    BYTE* pData,
    size_t nSize,
    bool bAutoDel )
    : m_bAutoDel(bAutoDel)
    , m_pPatchData(pData)
    , m_nSize(nSize)
    , m_ResFile(pFilepath, CFile::modeReadWrite, CFile::modeOpenAlway)
{
    
}

CResUpdater::~CResUpdater(void)
{
}

bool CResUpdater::ParsePatchData()
{

}

bool CResUpdater::Update( )
{
    if ( !m_ResFile.IsOpen() )
    {
        return false;
    }

    TResPatchFileHead* pPatchFileHead = (TResPatchFileHead*)m_pPatchData;
    DWORD dwRemoveFileCount = pPatchFileHead->dwRemoveFileCount;
    UHashValue* pRemoveList = (UHashValue*)(pPatchFileHead + 1);
    DataIndex* pAddDataIndex = (DataIndex*)(pRemoveList + dwRemoveFileCount);



}

bool CResUpdater::RemoveOld(
    const UHashValue* pRemoveList, 
    DWORD dwRemoveFileCount )
{
    
}

CResUpdater::FileHead* CResUpdater::GetResFileHead()
{

}
}





// End of file