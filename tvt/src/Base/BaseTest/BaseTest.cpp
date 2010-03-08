// BaseTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.VC80.CRT' version='8.0.50608.0' processorArchitecture='X86' publicKeyToken='1fc8b3b9a1e18e3b' language='*'\"")

#include <iostream>
#include <Windows.h>
#include <map>
#include <deque>
#include <string>
#include <list>
#include <memory>
using namespace std;


//#include "F:\tvt\src\DVR Server\DEVICECONTROL\\WPG_EventOccurrence.h"


struct X
{
    static bool f(int *p)
    {
        return p && 0[p] && !p[1] > p[2];
    }
};


union AlarmOutTable
{
    AlarmOutTable():nTable(0xffff){}
    struct {
        bool AlarmRecord:1;
        bool FullScreen:1;
        bool Relay:1;
        bool Sensor:1;
        bool EMail:1;
        bool EMap:1;
        bool Buzzer:1; 
        bool SnapShot:1;
        bool PlaySound:1;
        bool TelphoneCall:1;   
        bool resvr1:1;
        bool resvr2:1;
        bool resvr3:1;
        bool resvr4:1;
        bool resvr5:1;
        bool resvr6:1;
    } Table;
    unsigned short nTable;
};


namespace van 
{
     namespace type_traits
     {
        namespace detail
        {
            typedef char Small;
            struct Big {char dummy[2];};

            template<typename Type,Type Ptr>
            struct MemberHelperClass;

            //template<typename T,typename Type>
            //Small MemberHelper_f(MemberHelperClass<Type,&T::f> *);

            template<typename T,typename Type>
            Small MemberHelper_f(MemberHelperClass<T,&T::Type> *);

            template<typename T,typename Type>
            Big MemberHelper_f(...);
        }

        template<typename T,typename Type>
        struct has_member_f
        {
            enum 
            {
                value=sizeof(detail::MemberHelper_f<T,Type>(0))==sizeof(detail::Small)
            };
        };
    }
}

struct A
{
    static void f(){};
};
struct B
{
    static int a;
};
int B::a = 0;

struct My
{
    int a;
    bool b;
};

//typedef My::*a  AAA;

//
// 还是 用老方法 用位移搞定
//  
template<size_t Offset, typename T2>
void TempTest(T2 a)
{
    My kk;
    T2* pTmp = (T2*)((char*)&kk + Offset);
    *pTmp = a;
    //kk.*T = a;
};

const char* s_Test = "aaaaaa";

int _tmain(int argc, _TCHAR* argv[])
{
    map<const char*, int> Map;
    Map[s_Test] = 2;

    cout<<boolalpha;
    cout<<van::type_traits::has_member_f<B,int>::value<<endl;
    cout<<van::type_traits::has_member_f<A,void (A::*)()>::value<<endl;
    cout<<van::type_traits::has_member_f<B,void (*)()>::value<<endl;

    TempTest<0, int>(1);

    char szPath[] = "C:\\1\\2\\3.txt";
    char szdrive[10] = {0};
    char szdir[10] = {0};
    char szfname[10] = {0};
    char szext[10] = {0};
    errno_t err =  _splitpath_s(
        szPath,
        szdrive,
        szdir,
        szfname,
        szext);


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
    
  
	return 0;
}

