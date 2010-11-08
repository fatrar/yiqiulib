// ResPacker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define _Command_Help_Note "\nUse command like: \"ResPacker.exe ?\" can Get help!"
#define _Command_Help "\n xx is your set value. '[]' is not must set.\n"\
    "'ResFileList=xx' xx is a Path.\n"\
    "'PackFilePath=xx' xx is a Path.\n"\
    "'[ResFlod=xx]'  xx is a Path.\n"\
    "'[cAlgo=xx]' xx is must Lzma now!\n"\
    "'[eAlgo=xx]' xx is must Raw or Xor Now!\n"\
    "'[eKey=xx]' xx is must <= 8 char.\n"\
    "'[ExistFileName=xx]' xx is 0 or 1.\n"

namespace ICommand
{
using namespace ResFile;

class CResPackerCmdExecor :
    public ICmdExecor
{
public:
    CResPackerCmdExecor()
        : m_cAlgo(Lzma_C_Algo)
        , m_cParam(Compress_Normal)
        , m_eAlgo(Raw_E_Algo)
        , m_bIsExistFileName(false)
        , m_strResPackFilePath("C:\\ResPacker.pak")
        , m_nVolumeSize(512)  // default 512KB
    {
        memcpy(m_szEncryptPsw, "_Fg87+.?", 8);
    }

protected:
    enum CmdList
    {
        Res_Flod_Path,
        Res_File_List_Path,
        e_cAlgo,
        e_eAlgo,
        e_eKey,
        Res_Pack_File_Path,
        Exist_File_Name,
        CmdList_Count,
    };

    static const char* s_CommandName[]; 

public:
    void InitCmdRule(ICmdParser* pCmdParser)
    {
        for ( int i = 0; i< CmdList_Count; ++i )
        {
            pCmdParser->AddParamRule(
                i, s_CommandName[i], T_String);
        }
    }   

public:
    bool OnSetParam(
        UINT dwParam,
        const void* pParam1,
        const void* pParam2 = NULL)
    {
        const char* pParam = (const char*)pParam1;
        if ( !isValidString(pParam) )
        {
            return false;
        }

        switch (dwParam)
        {
        case Res_Flod_Path:
        {   
            m_strResFoldPath = pParam;
    	    return true;
        }
        case Res_File_List_Path:
        {    
            m_strFileListPath = pParam;  
            return true;
        }
        case e_cAlgo:
        {	
            if ( 0 == stricmp(pParam, "Raw") ){m_cAlgo = Raw_C_Algo;}
            else if( 0 == stricmp(pParam, "Zip") ){m_cAlgo = Zip_C_Algo;}
            else if( 0 == stricmp(pParam, "Lzma") ){m_cAlgo = Lzma_C_Algo;}
            else {return false;}
            return true;
        }
        case e_eAlgo:
        {	
            if ( 0 == stricmp(pParam, "Raw") ){m_eAlgo = Raw_E_Algo;}
            else if( 0 == stricmp(pParam, "Xor") ){m_eAlgo = Xor_E_Algo;}
            else if( 0 == stricmp(pParam, "BlowFish") ){m_eAlgo = BlowFish_E_Algo;}
            else {return false;}
            return true;
        }
        case e_eKey:
        {
            if ( strlen(pParam) >= 8 )
            {
                cout << "Encrypt Psw is Too long, max = 8" << endl;
                return false;
            }
            strcpy((char*)m_szEncryptPsw, pParam);
            return true;
        }
        case Res_Pack_File_Path:
        {
            m_strResPackFilePath = pParam;
            return true;
        }
        case Exist_File_Name:
        {
            if ( 0 == stricmp(pParam, "0") ){m_bIsExistFileName = false;}
            else if( 0 == stricmp(pParam, "1") ){m_bIsExistFileName = true;}
            else {return false;}
        }
        default:
        	return false;
        }
    }

    virtual bool Run()
    {
        if ( m_strFileListPath.size() == 0 )
        {
            cout << "Must Set FileList file Path!"
                    _Command_Help_Note << endl;
            return false;
        }

        IResPacker* pResPacker = ResFile::CreateResPacker(
            File_Version_1_0, m_strResFoldPath.c_str(),
            m_eAlgo, m_szEncryptPsw, m_cAlgo );
        if ( !pResPacker )
        {
            cout << "Can`t Create ResPacker, maybe is some Command Param Invaild!"
                    _Command_Help_Note << endl;
            return false;
        }

        ReadResFileListAndSet(pResPacker);

        pResPacker->MakeFile(
            m_strResPackFilePath.c_str(),
            m_nVolumeSize<<10,
            m_bIsExistFileName);
        ResFile::DestroyResPacker(pResPacker);
        return true;
    }

protected:
    bool ReadResFileListAndSet(IResPacker* pResPacker)
    {
        ifstream Reader;
        Reader.open(m_strFileListPath.c_str());
        if ( !Reader.is_open() )
        {
            cout << "Can`t Open FileList File --> " << m_strFileListPath.c_str() << endl;
            return false;
        }

        string strLine;  
        while( getline(Reader, strLine) )
        {    
            if ( strLine.size() == 0 ||
                 strLine[0] == '#' )
            {
                continue;
            }
            ParseAndSet(pResPacker, strLine);
        }

        return true;
    }

    bool ParseAndSet(IResPacker* pResPacker, const std::string& strLine)
    {
        /**
        *@note 暂时不支持FileListPath有命令，
        */
        // [] 这里可能需要处理两边多余空格
        pResPacker->AddFile(strLine.c_str());
        return true;
    }   
    
private:
    string m_strResFoldPath;
    string m_strFileListPath;

    eCompressAlgo m_cAlgo;
    eCompressParam m_cParam;
    eEncryptAlgo m_eAlgo;
    BYTE m_szEncryptPsw[8];

    string m_strResPackFilePath;
    bool m_bIsExistFileName;
    ResFile::size_t m_nVolumeSize;
};

const char* CResPackerCmdExecor::s_CommandName[] = 
{
    "ResFlod",
    "ResFileList",
    "cAlgo",
    "eAlgo",
    "eKey",
    "PackFilePath",
    "ExistFileName",
};

}

typedef unsigned int DWORD;
typedef unsigned __int64 QWORD;

union UHashValue
{
    DWORD dwValue[2];
    QWORD qwValue;

    inline bool operator ==(const UHashValue& a) const {return qwValue==a.qwValue;}
    inline bool operator > (const UHashValue& a) const {return qwValue>a.qwValue;}
    inline bool operator < (const UHashValue& a) const {return qwValue<a.qwValue;}
    inline void operator = (const QWORD& V){qwValue = V;}
    UHashValue(QWORD V = 0){qwValue = V;}
};

#pragma pack(push)
#pragma pack(4)
struct TDataHead
{
    UHashValue HashValue;
    DWORD dwRawDataLen;
};
#pragma pack(pop)

int _tmain(int argc, _TCHAR* argv[])
{
    int i = sizeof(TDataHead);
    //__asm {int 3}
    if ( argc == 2 && argv[1][0] == '?' )
    {
        cout << _Command_Help << endl;
        return 0;
    }

    ICommand::CResPackerCmdExecor CmdExecor;
    ICommand::ICmdParser* pCmdParser = 
        ICommand::CreateCmdParser(&CmdExecor);
    for ( int i = 1; i< argc; ++i )
    {
        pCmdParser->AddCmdString(argv[i]);
        cout << "Add Command :" << argv[i] << endl;
    }

    CmdExecor.InitCmdRule(pCmdParser);
    try
    {
        pCmdParser->ParseExec(false);
    }
    catch (const char* pErrInfo)
    {
        cout << pErrInfo << _Command_Help_Note << endl;
    }

    pCmdParser->Release();
    system("pause");
	return 0;
}

