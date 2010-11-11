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
    ~CResPatchGenerator();

    void Generate(
        const char* pOld,
        const char* pNew, 
        const char* pPatch );

protected:
    void Check(
        const char* pOld,
        const char* pNew, 
        const char* pPatch );

    void UnpackFile();

    void Parse();

    void WritePatchFile(const char* pPatch);

    void MakePatchData();

protected:
    static void OnDataReadCallBack(
        void* pParam,
        DataHead1* pHead,
        BYTE* pData );

    typedef std::map<TDataIndex0, char*> UnapckDataMap;
private:
    FileSystem::CFile m_OldFile, m_NewFile, m_PatchFile;
    TFileHead1* m_pOldFileHead, *m_pNewFileHead;

    /**
    *@note �������ȫ�ǽ�ѹ�ҽ��ܺ��ԭʼ����
    *      TDataIndex0�����ݴ�С���Ժ�������char*��Raw Data
    */
    UnapckDataMap m_OldData, m_NewData, m_Remove;
};

}



#endif