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

//#ifndef _No_Use_UnpackVolumeUtil
#include "LzmaUtil.h"
//#endif

namespace ResFile
{

namespace Util
{
ResCodeDef GetFileBaseHead(
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

//#ifndef _No_Use_UnpackVolumeUtil
bool CUnpackVolumeUtil::Unpack(
    DWORD dwMaxVolumeSize, 
    DWORD dwVolumeCount,
    DWORD eAlgo,
    const BYTE (&szKey)[8],
    const DataIndex1* pIndex )
{
    bool bRc = true;
    DWORD nPackBufSize = dwMaxVolumeSize; 
    BYTE* pPackBuf = new BYTE[nPackBufSize];
    DWORD nRawBufSize = dwMaxVolumeSize;
    BYTE* pRawBuf = new BYTE[nRawBufSize];
    for (DWORD i = 0; i< dwVolumeCount; ++i)
    {
        const DataIndex1& Index1 = pIndex[i];

        /**
        *@note 1. ���þ����ݴ��ļ��ж���
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
        *@note 2. ������Ӧ�� ����[]�� ��ʱĬ��xor
        */
        //QWORD* pStart = (QWORD*)pFileBuf;
        //for ( int i = 0 ; i < 4; ++i ) // 4 == 32 / 8
        //{
        //    *pStart ^= pHead1->dwKey;
        //    ++pStart;
        //}

        /**
        *@note 3. ��ѹ�þ�
        */
        size_t nRawDataSize = Index1.dwRawVolumeLen;
        TryResetBuf(pRawBuf, nRawBufSize, nRawDataSize);
        int nRc = LzmaUtil::LzmaUncompress(
            pRawBuf, &nRawDataSize,
            pPackBuf, nPackBufSize,
            Compress_Normal);
            //pHead1->nCompressLevel);
        if ( nRc != 0 )
        {
            bRc = false;
            break;
        }

        /**
        *@note 4. �����þ����ݣ�Ȼ����������ݻص���ȥ
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
            *@note �����ļ���ǰλ�ú�����λ��
            */
            size_t nDataSize1 = pDataHead1->dwRawDataLen + sizeof(DataHead1);
            nDataOffSetNow += nDataSize1;
        }

        // ����ǰ���ѭ���Ƿ������˳������������ļ���ʽ������
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
    return Unpack(
        pHead->dwMaxVolumeSize,
        pHead->dwFileCount,
        pHead->eAlgo,
        pHead->szKey,
        pHead->DataIndex );
}
//#endif  // _No_Use_UnpackVolumeUtil

inline void XorSome(
    QWORD* pqwIn, const QWORD& qwKey )
{
    for ( int i = 0 ; i < 4; ++i ) // 4 == 32 / 8
    {
        *pqwIn ^= qwKey;
        ++pqwIn;
    }
}

class CVoidCrypto :
    public IResCrypto
{
public:
    CVoidCrypto(const BYTE (&szKey)[8])
        /*: m_qwKey(*(QWORD&)szKey)*/{}
    virtual bool Encrypt(void* pIn, size_t nIn){return true;}
    virtual bool Decrypt(void* pIn, size_t nIn){return true;}
    virtual void Release(){delete this;}
protected:
    union
    {
        BYTE m_szKey[8];  // �����Կ8���ֽ�
        QWORD m_qwKey;    // �����XOR���������
    };
};

class CXorCrypto :
    public CVoidCrypto
{
public:
    CXorCrypto(const BYTE (&szKey)[8]) 
        : CVoidCrypto(szKey){}
        
protected:
    virtual bool Encrypt(void* pIn, size_t nIn)
    {
        if ( nIn < Default_Encrypt_Len )
        {
            return false;
        }
        XorSome((QWORD*)pIn, m_qwKey);
        return true;
    }
    virtual bool Decrypt(void* pIn, size_t nIn)
    {      
        return Encrypt(pIn, nIn);
    }
};


IResCrypto* IResCrypto::CreateResCrypto(
    eEncryptAlgo eAlgo, const BYTE (&szKey)[8] )
{
    switch (eAlgo)
    {
    case Raw_E_Algo:
    	return new CVoidCrypto(szKey);
    case Xor_E_Algo:
    	return new CXorCrypto(szKey);
    case BlowFish_E_Algo:
    	return new CVoidCrypto(szKey);
    default:
    	return NULL;
    }
}

}  // namespace Util


}  // namespace ResFile


// End of file