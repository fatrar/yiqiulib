/**H**************************************************************************
 File            : IVFileCommon.h
 Subsystem       : 
 Function Name(s): CIVFileCommon
 Author          : YiQiu(heliang)
 Date            : 2010-3-23  
 Time            : 14:14
 Description     : 

 Revision        : 1.0

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _IVFILECOMMON_H_2010_3
#define _IVFILECOMMON_H_2010_3


#define _MakeDWORD(a,b,c,d) ((a<<24)|(b<<16)|(c<<8)|(d))

static const DWORD g_dwIVFileFail = _MakeDWORD('7','+','F','@');
static const DWORD g_dwIVFileOK = _MakeDWORD('1','-','o','$');

union IVFileFlag
{
    IVFileFlag():dwFileFlag(g_dwIVFileFail){}
    DWORD dwFileFlag;
    struct {
        BYTE f[4];
    } TFlag;

    bool operator == (const IVFileFlag& a){return dwFileFlag == a.dwFileFlag;}
    void operator = (const DWORD a){ dwFileFlag = a; }
};


#define _MakeIVFileVersion(year,month,day) ((year)|(month<<16)|(day<<24))

enum IVFileVersionDefine
{
    IVFile_Version_1_0 = _MakeIVFileVersion(2010,3,23),
};

union IVFileVersion
{
    IVFileVersion():dwFileVersion(IVFile_Version_1_0){}
    DWORD dwFileVersion;
    struct {
        WORD nYear;
        BYTE nMonth;
        BYTE nDay;
    } TVersion;

    bool operator == (const IVFileVersion& a){return dwFileVersion == a.dwFileVersion;}
    void operator = (const DWORD a){ dwFileVersion = a; }
};

struct IVFileDataIndex
{
    DWORD TimeOffset;
    DWORD DataOffset;
};

struct IVFileHead
{
    IVFileHead():dwIndexNum(0){}
    IVFileFlag FileFlag;
    IVFileVersion Version;
    DWORD dwReserve[2];
    FILETIME BeginTime;
    FILETIME EndTime;
    DWORD dwIndexNum;
    IVFileDataIndex dwIndex[200];
};

struct IVFileDataHead
{
    DWORD dwPrePos;
    DWORD dwNextPos;
    WORD dwTargetNum;
    WORD dwReserve;
    FILETIME t;
};












#endif  // _IVFILECOMMON_H_2010_


 // End of file



