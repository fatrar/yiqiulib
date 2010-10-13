/**H**************************************************************************
 File            : ResPacker.h
 Subsystem       : 
 Function Name(s): CResPacker
 Author          : YiQiu
 Date            : 2010-10-12  
 Time            : 16:06
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) Shenzhen KooGame Technologies Co.,Ltd.
***************************************************************************H*/
#ifndef _RESPACKER_H_2010_10
#define _RESPACKER_H_2010_10


#include "IResPacker.h"
#include <string>
#include <list>
#include <set>
using namespace std;
#include <Windows.h>


namespace ResFile
{

template<DWORD Version>
class CResPacker :
    public IResPacker
{
public:
    typedef TEncryptParam<Version> EncryptParam;
    typedef TCompressParam<Version> CompressParam;

    CResPacker( 
        const char* pResFlodPath, 
        eCompressAlgo cAlgo,
        const CompressParam& pcParam,
        eEncryptAlgo eAlgo,
        const EncryptParam& peParam );
    ~CResPacker(void);

    // IResPacker
public:
    virtual void AddFile(
        const char* pFileName );

    virtual void AddFile(
        const char* pFileName,
        eCompressAlgo cAlgo,
        void* pcParam,
        eEncryptAlgo eAlgo,    
        void* peParam );

    virtual bool MakeFile(
        const char* pPackFilePath,
        eFileNamePos eFileNamePos);

    // Struct Define
protected:
    struct FileInfo
    {
        inline FileInfo(){}
        string strFileName;
        eEncryptAlgo eAlgo;
        TEncryptParam<Version> eParam;
        eCompressAlgo cAlgo;
        TCompressParam<Version> cParam;

        FileInfo(const FileInfo& a)
        {
            strFileName = a.strFileName;
            eAlgo = a.eAlgo;
            eParam = a.eParam;
            cAlgo = a.cAlgo;
            cParam = a.cParam;
        }
    };

    typedef list<FileInfo> FileInfoList;
    typedef TFileHead<Version> FileHead;
    template<DWORD Version> class DataIndexList;

    // Version 1.0 Define
protected:
    template<> class DataIndexList<File_Version_1_0> :
        public set<TFileHead<File_Version_1_0>::TDataIndex >{};

protected:
    static DWORD WINAPI DataTransform(void* p)
    {
        CResPacker* pThis = (CResPacker*)p;
        return pThis->DataTransform();
    }
    DWORD DataTransform();

    static DWORD WINAPI DataSave(void* p)
    {
        CResPacker* pThis = (CResPacker*)p;
        return pThis->DataSave();
    }
    DWORD DataSave();

private:
    eEncryptAlgo m_DefeAlgo;
    EncryptParam m_DefeParam;

    eCompressAlgo m_DefcAlgo;
    CompressParam m_DefcParam;

    string m_strResFold;

    FileInfoList m_FileInfoList;
    DataIndexList<Version> m_DataIndexList;

    // Thread
    HANDLE m_hDataTransformThread;
    HANDLE m_hDataSaveThread;
    HANDLE m_ReadFinsihEvent;
    BYTE* m_pFileBuf;
    size_t m_nUsePos;
    size_t m_n
};

}




#endif  // _RESPACKER_H_2010_


// End of file

