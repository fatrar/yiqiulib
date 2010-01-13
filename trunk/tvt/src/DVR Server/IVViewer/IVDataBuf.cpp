/*H***************************************************************************
 File            : IVDataBuf.cpp
 Subsystem       : 
 Function Name(s): CIVDataBuf
 Author          : YiQiu
 Date            : 2010-1-13  
 Time            : 14:25
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#include "stdafx.h"
#include "IVDataBuf.h"

enum
{
    MAX_IVS_CHANNEL = 64,
};

static const char c_szIVFileExt[] = ".iv";

CIVDataBuf::CIVDataBuf(void)
{
}

CIVDataBuf::~CIVDataBuf(void)
{
}

//
// IIVDataBuf
//
BOOL CIVDataBuf::GetData(
    int nChannelID,
    const FILETIME& time, 
    TargetQueue*& DataQueue )
{
    AutoLockAndUnlock(m_cs);
    ChannelTarget& ChanTarget = m_TargetMap[nChannelID];
    GroupTarget* pGroupTarget = ChanTarget.Find(time);
    if ( NULL == pGroupTarget )
    {
        return FALSE;
    }

    DataQueue = &pGroupTarget->m_TargetQueue;
    return TRUE;
}

//
// IIVDataSender
//
BOOL CIVDataBuf::OnIVDataSend( 
    int nChannelID,
    const FILETIME& time,
    const WPG_Target* pData,
    size_t nLen )
{
    GroupTarget* pGroupTarget = new GroupTarget(time, pData, nLen);
   
    AutoLockAndUnlock(m_cs);
    ChannelTarget& ChanTarget = m_TargetMap[nChannelID];
    ChanTarget.PoshBack(pGroupTarget);
    return TRUE;
}


//
// IIVDataSaver
//
BOOL CIVDataBuf::Open(
    int nChannelID,
    const char* pPath,
    const FILETIME& time )
{
    return TRUE;
}

BOOL CIVDataBuf::Close(
    int nChannelID, 
    const FILETIME& time )
{
    return TRUE;
}

BOOL CIVDataBuf::DeleteIVFile( 
    const char* pPath )
{
    if ( !isValidString(pPath) )
    {
        return FALSE;
    }

    // 这里要判断文件是否在读。 PlayBack
    string strPath = pPath;
    strPath += pPath;
    DeleteFile(strPath.c_str());
    return TRUE;
}

size_t WINAPI CIVDataBuf::SaveFileThread( void* pParm )
{
    CIVDataBuf* pThis = (CIVDataBuf*)pParm;
    return pThis->SaveFileLoopFun();
}

size_t CIVDataBuf::SaveFileLoopFun()
{
    return 0;
}






GroupTarget* CIVDataBuf::ChannelTarget::Find( const FILETIME& time )
{
    list<GroupTarget*>::iterator iter = iterIndex;
    if ( iter == TargetList.end() )
    {
        iter = TargetList.begin();
    }

    for (; iter != TargetList.end(); ++iter)
    {
        GroupTarget* pGroupTarget = *iter;
        if ( *pGroupTarget == time )
        {
            return pGroupTarget;
        }
    }

    return NULL;
}

void CIVDataBuf::ChannelTarget::PoshBack( GroupTarget* pGroupTarget )
{
    TargetList.push_back(pGroupTarget);
}



// End of file