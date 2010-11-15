/**CPP*************************************************************************
 File            : ResFileUtil.cpp
 Subsystem       : 
 Function Name(s): CResFileUtil
 Author          : YiQiu
 Date            : 2010-11-11  
 Time            : 14:01
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
**************************************************************************cpp**/
#include "ResFileUtil.h"
#include "LzmaUtil.h"



namespace ResFile
{

namespace Util
{
static ResCodeDef GetFileBaseHead(
    const char* pResFilePath,
    TFileHeadBase& HeadBase,
    FileSystem::CFile& File )
{
    if ( !isValidString(pResFilePath) )
    {
        return Input_Param_Error;
    }

    FileSystem::BOOL bRc = File.OpenByRead(pResFilePath);
    if ( !bRc )
    {
        return File_Can_Not_Open;
    }

    FileSystem::size_t nRead = File.Read(
        &HeadBase, sizeof(TFileHeadBase));
    if ( nRead != sizeof(TFileHeadBase) )
    {
        return File_Read_Failed;
    }

    if ( HeadBase.FormatFlag != Res_File_Format_Flag )
    {
        return File_Format_Error;
    }
    return No_Error;
}

bool UpackFileData(
    FileSystem::CFile& File,
    const FileHead1* pHead,
    DataReadCallBackFn DataReadCallBack,
    void* pParam )
{
    bool bRc = true;
    DWORD nPackBufSize = pHead->dwMaxVolumeSize; 
    BYTE* pPackBuf = new BYTE[nPackBufSize];
    DWORD nRawBufSize = pHead->dwMaxVolumeSize;
    BYTE* pRawBuf = new BYTE[nRawBufSize];
    for (DWORD i = 0; i< pHead->dwFileCount; ++i)
    {
        const DataIndex1& Index1 = pHead->DataIndex[i];

        /**
        *@note 1. 将该卷数据从文件中读出
        */
        const DWORD& dwPackLen = Index1.dwLen;
        TryResetBuf(pPackBuf, nPackBufSize, dwPackLen);
        File.Seek(Index1.dwOffset);
        if ( dwPackLen != File.Read(pPackBuf, dwPackLen) )
        {
            bRc = false;
            break;
        }

        /**
        *@note 2. 接下来应该 解密[]， 暂时默认xor
        */
        //QWORD* pStart = (QWORD*)pFileBuf;
        //for ( int i = 0 ; i < 4; ++i ) // 4 == 32 / 8
        //{
        //    *pStart ^= pHead1->dwKey;
        //    ++pStart;
        //}

        /**
        *@note 3. 解压该卷
        */
        size_t nRawDataSize = Index1.dwRawVolumeLen;
        TryResetBuf(pRawBuf, nRawBufSize, nRawDataSize);
        int nRc = LzmaUtil::LzmaUncompress(
            pRawBuf, &nRawDataSize,
            pPackBuf, nPackBufSize,
            Compress_High);
            //pHead1->nCompressLevel);
        if ( nRc != 0 )
        {
            bRc = false;
            break;
        }

        /**
        *@note 4. 分析该卷数据，然后分析的数据回调出去
        */
        DataHead1* pDataHead1 = NULL;
        BYTE* pData1 = NULL;
        size_t nDataOffSetNow = 0;
        while (1)
        {
            if ( nDataOffSetNow >= nRawDataSize )
            {
                break;
            }

            pDataHead1 = (DataHead1*)(pRawBuf+nDataOffSetNow);
            pData1 = (BYTE*)(pDataHead1 + 1);
            DataReadCallBack(pParam, pDataHead1, pData1);

            /**
            *@note 更新文件当前位置和数据位置
            */
            size_t nDataSize1 = pDataHead1->dwRawDataLen + sizeof(DataHead1);
            nDataOffSetNow += nDataSize1;
        }

        // 检验前面的循环是否正常退出，非正常则文件格式有问题
        if ( nDataOffSetNow != nRawDataSize )
        {
            bRc = false;
            break;
        }
    }
    delete[] pPackBuf;
    delete[] pRawBuf;
    return bRc;
}

bool CUnpackVolumeUtil::Unpack(const FileHead1* pHead)
{
    bool bRc = true;
    DWORD nPackBufSize = pHead->dwMaxVolumeSize; 
    BYTE* pPackBuf = new BYTE[nPackBufSize];
    DWORD nRawBufSize = pHead->dwMaxVolumeSize;
    BYTE* pRawBuf = new BYTE[nRawBufSize];
    for (DWORD i = 0; i< pHead->dwFileCount; ++i)
    {
        const DataIndex1& Index1 = pHead->DataIndex[i];

        /**
        *@note 1. 将该卷数据从文件中读出
        */
        const DWORD& dwPackLen = Index1.dwLen;
        TryResetBuf(pPackBuf, nPackBufSize, dwPackLen);
        if ( dwPackLen != Read(Index1.dwOffset, pPackBuf, dwPackLen) )
        {
            bRc = false;
            break;
        }
//         File.Seek(Index1.dwOffset);
//         if ( dwPackLen != File.Read(pPackBuf, dwPackLen) )
//         {
//             bRc = false;
//             break;
//         }

        /**
        *@note 2. 接下来应该 解密[]， 暂时默认xor
        */
        //QWORD* pStart = (QWORD*)pFileBuf;
        //for ( int i = 0 ; i < 4; ++i ) // 4 == 32 / 8
        //{
        //    *pStart ^= pHead1->dwKey;
        //    ++pStart;
        //}

        /**
        *@note 3. 解压该卷
        */
        size_t nRawDataSize = Index1.dwRawVolumeLen;
        TryResetBuf(pRawBuf, nRawBufSize, nRawDataSize);
        int nRc = LzmaUtil::LzmaUncompress(
            pRawBuf, &nRawDataSize,
            pPackBuf, nPackBufSize,
            Compress_High);
            //pHead1->nCompressLevel);
        if ( nRc != 0 )
        {
            bRc = false;
            break;
        }

        /**
        *@note 4. 分析该卷数据，然后分析的数据回调出去
        */
        DataHead1* pDataHead1 = NULL;
        BYTE* pData1 = NULL;
        size_t nDataOffSetNow = 0;
        while (1)
        {
            if ( nDataOffSetNow >= nRawDataSize )
            {
                break;
            }

            pDataHead1 = (DataHead1*)(pRawBuf+nDataOffSetNow);
            pData1 = (BYTE*)(pDataHead1 + 1);
            DataReadCallBack(pDataHead1, pData1);

            /**
            *@note 更新文件当前位置和数据位置
            */
            size_t nDataSize1 = pDataHead1->dwRawDataLen + sizeof(DataHead1);
            nDataOffSetNow += nDataSize1;
        }

        // 检验前面的循环是否正常退出，非正常则文件格式有问题
        if ( nDataOffSetNow != nRawDataSize )
        {
            bRc = false;
            break;
        }
    }
    delete[] pPackBuf;
    delete[] pRawBuf;
    return bRc;
}

}  // namespace Util


}  // namespace ResFile


// End of file