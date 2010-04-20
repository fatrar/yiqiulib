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
    DWORD TimeOffset; // ��IVFileHead.BeginTime��ʱ���
    DWORD DataOffset; // �������ļ���λ��
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
    IVFileFlag FileFlag;     // �ļ���־�������ж��ļ��Ƿ������ر�
    IVFileVersion Version;   // �ļ��汾
    DWORD dwReserve;         // ����Ԥ��
    DWORD dwLastFramePos;    // ���һ֡���ݵ�λ��
    FILETIME BeginTime;      // ��һ֡���ݵ�ʱ��
    FILETIME EndTime;        // ���һ֡���ݵ�ʱ��
    DWORD dwIndexNum;        // �����ĸ���
    IVFileDataIndex DataIndex[Max_IVData_Index]; // ��������
};

/**
*@note IV File Data Head
*/
struct IVFileDataHead
{
    DWORD dwPrePos;  // ��һ֡��λ�ã������ǰ�ǵ�һ֡����ֵΪ0
    DWORD dwNextPos; // ��һ֡��λ�ã������ǰ�����һ֡����ֵΪ�ļ��ĳ���
    WORD wTargetNum; // Ŀ��ĸ���
    WORD wReserve;   // ����Ԥ��
    FILETIME t;      // ��ǰ֡��ʱ��
};



static const char c_szIVFileExt[] = ".iv";


enum
{
    /**
    *@note 5�뱣��һ����������������(�ļ�����0.5Hour)����360��������
    *  ��ô��200������£���160������
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



