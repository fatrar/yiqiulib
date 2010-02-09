// BaseTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#define USEBASEENINE_NAMESPACE
#include "..\Base\Include\singleton.h"
using namespace OCI;
#include <iostream>
#include <Windows.h>
#include <map>
#include <deque>
#include <string>
#include <list>
#include <memory>
using namespace std;


#include "F:\tvt\src\DVR Server\DEVICECONTROL\\WPG_EventOccurrence.h"

class CVoidType
{

};

struct X
{
    static bool f(int *p)
    {
        return p && 0[p] && !p[1] > p[2];
    }
};



struct TargetQueue
{
    WPG_Target Tar[50];
    WORD nCount;
    short nRef;
};

inline long operator - (const FILETIME&t1, const FILETIME&t2)
{
    return (*((__int64*)&t1)-*((__int64*)&t2))/10000;
}

list<int> TargetList, TargetSaveList;
void Kiil(int test)
{
    int pGroupTarget = NULL;
    list<int>::iterator iter = TargetList.begin();
    for ( iter = TargetList.begin(); ; ++iter)
    {
        if ( *iter == test )
        {
            pGroupTarget = *iter;
            TargetSaveList.splice(
                TargetSaveList.end(), TargetList, TargetList.begin(), ++iter);
            break;
        }
        else if ( *iter > test )
        {
            TargetSaveList.splice(
                TargetSaveList.end(), TargetList, TargetList.begin(), ++iter);
            break;
        } 


        if ( iter == TargetList.end() )
        {
            TargetSaveList.splice(TargetSaveList.end(), TargetList);
            break;
        }
    }
   
    //TargetList.erase(TargetList.begin(), iter);
}

int _tmain(int argc, _TCHAR* argv[])
{
    //for (int i = 1; i<20; ++i)
    //    TargetList.push_back(i);

    //Kiil(10);

    //SYSTEMTIME syt, syt2;
    //GetLocalTime(&syt);
    //syt2 = syt;
    //if ( syt2.wSecond > 2 )
    //{
    //    syt2.wSecond -= 2;
    //}

    //FILETIME f1, f2;
    //SystemTimeToFileTime(&syt, &f1);
    //SystemTimeToFileTime(&syt2, &f2);
    //long nTest = f1 - f2;

    //int nSize = sizeof(TargetQueue);
    //
    
    int a = 10 ;
    for ( int i = 0; i<10; ++i)
        // dfd ffsdf ?sdfds???????????/
        ++a;
    cout << "aa" << endl;
    
    {
        cout << "aa" << endl;
    }


    int szbuf[4] = {-1};
    memset(szbuf, -1, sizeof(szbuf));
    int i = sizeof(WPG_Rule);  // 3748
    const int b = 0;
    const int& aa = b;
    list<int> lk;
  
	return 0;
}

