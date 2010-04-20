/**H**************************************************************************
 File            : IVFileCommon.h
 Subsystem       : 
 Function Name(s): 
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
#pragma pack(push, 1)

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

    bool operator == (const IVFileFlag& a)const{return dwFileFlag == a.dwFileFlag;}
    bool operator == (const DWORD& a)const{return dwFileFlag == a;}
    bool operator != (const IVFileFlag& a)const{return dwFileFlag != a.dwFileFlag;}
    bool operator != (const DWORD& a)const{return dwFileFlag != a;}
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

    bool operator == (const IVFileVersion& a)const{return dwFileVersion == a.dwFileVersion;}
    bool operator == (const IVFileVersionDefine& a)const{return dwFileVersion == a;}
    void operator = (const DWORD a){ dwFileVersion = a; }
    operator DWORD(){return dwFileVersion;}
};

struct IVFileDataIndex
{
    DWORD TimeOffset; // 与IVFileHead.BeginTime的时间差
    DWORD DataOffset; // 数据在文件的位置
};

enum
{
    Max_IVData_Index  = 200,  
};

/**
*@note IV File Head Data
*/
struct IVFileHead
{
    IVFileHead():dwIndexNum(0){}
    IVFileFlag FileFlag;     // 文件标志，用于判断文件是否正常关闭
    IVFileVersion Version;   // 文件版本
    DWORD dwReserve;         // 对齐预留
    DWORD dwLastFramePos;    // 最后一帧数据的位置
    FILETIME BeginTime;      // 第一帧数据的时间
    FILETIME EndTime;        // 最后一帧数据的时间
    DWORD dwIndexNum;        // 索引的个数
    IVFileDataIndex DataIndex[Max_IVData_Index]; // 索引数据
};

/**
*@note IV File Data Head
*/
struct IVFileDataHead
{
    DWORD dwPrePos;  // 上一帧的位置，如果当前是第一帧，该值为0
    DWORD dwNextPos; // 下一帧的位置，如果当前是最后一帧，该值为文件的长度
    WORD wTargetNum; // 目标的个数
    WORD wReserve;   // 对齐预留
    FILETIME t;      // 当前帧的时间
};



static const char c_szIVFileExt[] = ".iv";


enum
{
    /**
    *@note 5秒保存一个索引，在最坏的情况(文件保存0.5Hour)，有360个索引，
    *  那么在200的情况下，有160做丢弃
    */
    Head_Index_Interval = 10*5,
};

/*
+------------------+
| IVFileHead       |
+------------------+
| IVFileDataHead   | 
+------------------+
| IV Data          |
+------------------+
| IVFileDataHead   | 
+------------------+
| IV Data          |
+------------------+
|  ..........      |
+------------------+
*/



#pragma pack(pop)


#endif  // _IVFILECOMMON_H_2010_


 // End of file



