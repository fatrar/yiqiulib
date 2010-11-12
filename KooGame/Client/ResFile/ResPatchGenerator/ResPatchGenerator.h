/*H***************************************************************************
File            : ResPatchGenerator.h
Subsystem       : 
Function Name(s): CResPatchGenerator

Description     : 
Author          : Yiqiu
Date            : 2010/10/20
Revision        : 

History
-------


Copyright (c) Shenzhen KooGame Co.,Ltd.
***************************************************************************H*/
#ifndef _RESPATCHGENERATOR_H_2010_10
#define _RESPATCHGENERATOR_H_2010_10

#include "ResFileDef.h"
#include <map>

namespace ResFile
{

typedef TFileHead<File_Version_1_0> TFileHead0;
typedef TFileHead<File_Version_1_1> TFileHead1;
typedef TFileHead0::TDataIndex TDataIndex0;
typedef TFileHead1::TDataIndex TDataIndex1;

class CResPatchGenerator
{
public:
    CResPatchGenerator();
    ~CResPatchGenerator();

    void Generate(
        const char* pOld,
        const char* pNew, 
        const char* pPatch,
        size_t nMaxVolumeSize );

protected:
    void Check(
        const char* pOld,
        const char* pNew, 
        const char* pPatch );

    void UnpackFile();

    void Parse();

    void WritePatchFile(
        const char* pPatch,
        size_t nMaxVolumeSize );

    void MakePatchData(size_t nMaxVolumeSize);

#define DestroyPatchData(p) safeDeleteArray(p)

    void DataTransfrom();

    void FillData(
        BYTE*& pBuf, BYTE* pData,
        const TDataIndex0& Index );

protected:
    static void OnDataReadCallBack(
        void* pParam,
        DataHead1* pHead,
        BYTE* pData );

    typedef std::map<TDataIndex0, BYTE*> UnapckDataMap;

    enum
    {
        Res_File_Buf = 32*1024*1024,  // 32MB
    };

private:
    FileSystem::CFile m_OldFile, m_NewFile, m_PatchFile;
    TFileHead1* m_pOldFileHead, *m_pNewFileHead;

    /**
    *@note 这个数据全是解压且解密后的原始数据
    *      TDataIndex0放数据大小和以后索引，BYTE*放Raw Data
    */
    UnapckDataMap m_OldData, m_NewData, m_Remove;

    BYTE* m_pPatchData;
    size_t m_nPatchNow;

    BYTE* m_pVolume;
    size_t m_nVolumeSize;
    size_t m_nVolumeNow;

    DataIndex1* m_pAddIndex;
    size_t m_IndexNow;
};

}



#endif