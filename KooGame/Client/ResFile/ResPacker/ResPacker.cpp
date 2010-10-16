// ResPacker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


namespace ICommand
{

class CResPackerCmdExecor :
    public ICmdExecor
{
public:
    CResPackerCmdExecor()
        : m_cAlgo(Lzma_C_Algo)
        , m_cParam(Compress_Normal)
        , m_eAlgo(Raw_E_Algo)
        , m_eFileNamePos(Not_Exist){}

protected:
    enum CmdList
    {
        Res_Flod_Path,
        Res_File_List_Path,
        Def_cAlgo,
        Def_cParam,
        Def_eAlgo,
        Def_eParam,
        Res_Pack_File_Path,
        File_Name_Pos,
        CmdList_Count,
    };

    static const char* s_CommandName[]; 

public:
    void InitCmdRule(ICmdParser* pCmdParser)
    {
        for ( int i = 0; i< CmdList_Count; ++i )
        {
            pCmdParser->AddParamRule(
                i, s_CommandName[i], ICmdParser::T_String);
        }
    }   

public:
    bool OnSetParam(
        UINT dwParam,
        const void* pParam1,
        const void* pParam2 = NULL)
    {
        const char* pParam = (const char*)pParam1;
        if ( isValidString(pParam) )
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
        case Def_cAlgo:
        {	
            if ( 0 == stricmp(pParam, "Raw") ){m_cAlgo = Raw_C_Algo;}
            else if( 0 == stricmp(pParam, "Zip") ){m_cAlgo = Zip_C_Algo;}
            else if( 0 == stricmp(pParam, "Lzma") ){m_cAlgo = Lzma_C_Algo;}
            else {return false;}
            return true;
        }
        case Def_cParam:
        {	
            if ( 0 == stricmp(pParam, "Fast") ){m_cParam = Unpack_Fast;}
            else if( 0 == stricmp(pParam, "Normal") ){m_cParam = Compress_Normal;}
            else if( 0 == stricmp(pParam, "High") ){m_cParam = Compress_High;}
            else if( 0 == stricmp(pParam, "Auto") ){m_cParam = Compress_Auto;}
            else {return false;}
            return true;
        }
        case Def_eAlgo:
        {	
            if ( 0 == stricmp(pParam, "Raw") ){m_eAlgo = Raw_E_Algo;}
            else if( 0 == stricmp(pParam, "Xor") ){m_eAlgo = Xor_E_Algo;}
            else if( 0 == stricmp(pParam, "BlowFish") ){m_eAlgo = BlowFish_E_Algo;}
            else {return false;}
            return true;
        }
        case Def_eParam:
        {
            if ( strlen(pParam) >= 8 )
            {
                cout << "Encrypt Psw is Too long, max = 8" << endl;
                return false;
            }
            m_strEncryptPsw = pParam;
            return true;
        }
        case Res_Pack_File_Path:
        {
            m_strResPackFilePath = pParam;
            return true;
        }
        case File_Name_Pos:
        {
            if ( 0 == stricmp(pParam, "No") ){m_eFileNamePos = Not_Exist;}
            else if( 0 == stricmp(pParam, "Out") ){m_eFileNamePos = In_Out;}
            else if( 0 == stricmp(pParam, "BebindHead") ){m_eFileNamePos = Bebind_Head;}
            else if( 0 == stricmp(pParam, "FileTail") ){m_eFileNamePos = File_Tail;}
            else {return false;}
        }
        default:
        	return false;
        }
    }

    virtual bool Run()
    {
        TCompressParam<File_Version_1_0> cParam(m_cParam);
        TEncryptParam<File_Version_1_0> eParam;
        memcpy(eParam.cEncryptParam, m_strEncryptPsw.c_str(), m_strEncryptPsw.length());
        IResPacker* pResPacker = ResFile::CreateResPacker(
            m_strResFoldPath.c_str(), 
            m_cAlgo, &cParam, m_eAlgo, &eParam);
        if ( !pResPacker )
        {
            cout << "Can`t Create ResPacker, maybe is some Command Param Invaild!" << endl;
            return false;
        }

        ReadResFileListAndSet(pResPacker);

        pResPacker->MakeFile(m_strResPackFilePath.c_str(), m_eFileNamePos);
        ResFile::DestroyResPacker(pResPacker);
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
            ParseAndSet(pResPacker, strLine);
        }

        return true;
    }

    bool ParseAndSet(IResPacker* pResPacker, const string& strLine)
    {
        /**
        *@note 暂时不支持FileListPath有命令，
        */
        // [] 这里可能需要处理两边多余空格
        pResPacker->AddFile(strLine.c_str());
    }   
    
private:
    string m_strResFoldPath;
    string m_strFileListPath;

    eCompressAlgo m_cAlgo;
    eCompressParam m_cParam;
    eEncryptAlgo m_eAlgo;
    string m_strEncryptPsw;

    string m_strResPackFilePath;
    eFileNamePos m_eFileNamePos;
};

const char* CResPackerCmdExecor::s_CommandName[] = 
{
    "ResFlod",
    "ResFileList",
    "DefcAlgo",
    "DefcParam",
    "DefeAlgo",
    "DefeParam",
    "PackFilePath",
    "FileNamePos",
};

}

int _tmain(int argc, _TCHAR* argv[])
{
    ICommand::CResPackerCmdExecor CmdExecor;
    ICommand::ICmdParser* pCmdParser = 
        ICommand::CreateCmdParser(&CmdExecor);
    for ( int i = 0; i< argc; ++i )
    {
        pCmdParser->AddCmdString(argv[i]);
    }

    CmdExecor.InitCmdRule(pCmdParser);
    try
    {
        pCmdParser->ParseExec(false);
    }
    catch (const char* pErrInfo)
    {
        cout << pErrInfo << endl;	
    }

    ICommand::DestroyCmdParser(pCmdParser);
	return 0;
}

