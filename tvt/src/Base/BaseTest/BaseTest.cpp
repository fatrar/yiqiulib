// BaseTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <iostream>
#include <Windows.h>
#include <map>
#include <deque>
#include <string>
#include <list>
#include <memory>
#include <fstream>
#include <iomanip>
using namespace std;


#include "F:\tvt\src\DVR Server\DEVICECONTROL\\WPG_EventOccurrence.h"

#include "..\Base\Include\StringHelp.h"

#include "..\Base\Include\TransformUtil.h"
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


struct IVFileDataHead
{
    DWORD dwPrePos;
    DWORD dwNextPos;
    WORD dwTargetNum;
    WORD dwReserve;
    FILETIME t;
};


int _tmain(int argc, _TCHAR* argv[])
{
    const char pPath[] = "C:\\1234.txt";
    ofstream Writer(pPath, ios::binary);
    DWORD a = 1;
    Writer.write((char*)&a, sizeof(DWORD));
    a =2;
    Writer.write((char*)&a, sizeof(DWORD));
    WORD b = 3;
    Writer.write((char*)&b, sizeof(WORD));
    b = 4;
    Writer.write((char*)&b, sizeof(WORD));

    FILETIME t;
    t.dwHighDateTime = 0;
    t.dwLowDateTime = 15;
    Writer.write((char*)&t, sizeof(FILETIME));
    Writer.close();

    ifstream Reader(pPath, ios::binary);
    IVFileDataHead Head;
    Reader.read((char*)&Head, sizeof(IVFileDataHead));


    list<int> aa, bb;
    bb.insert(bb.begin(), 10, 100);
    aa.splice(aa.end(), bb);
   // aa.insert(aa.end(), 10, NULL);

    OCI::StringHelp::CParseString<> m_buf; 
   // m_buf  << 1234 << "aaaaa"; 
    //string& strTmp = m_buf.str();
    //cout << strTmp.c_str() << endl;
    //cout << m_buf.str() << endl;

    char szTestBuf[100] = "1234567890";
    BYTE* pTestBuf2 = new BYTE[200];
    ZeroMemory(pTestBuf2, 200);
    size_t nSize = 200;
    OCI::TransformUtil::Base64Encode(
        (BYTE*)szTestBuf, 
        100, 
        pTestBuf2,
        nSize);

    size_t m = sizeof(WPG_AOIEventDescription);
    m = sizeof(WPG_TripwireEventDescription		);				
    m = sizeof(WPG_AOIEventDescription		);						
    m = sizeof(WPG_SceneChangeEventDescription);
    m = sizeof(WPG_MultiLineTripWireEventDescription);				
    m = sizeof(WPG_MultiSegTripwireEventDescription	);			
    m = sizeof(WPG_MultiSegmentMultiLineTripWireEventDescription);	
    m = sizeof(WPG_OccupancyEventDescription); 						
    m = sizeof(WPG_DwellTimeEventDescription);
    size_t n = sizeof(WPG_EventDescriptionUnion);
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


///////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
//// demo.cpp
////
//// (C) Copyright 2002-4 Robert Ramey - http://www.rrsd.com .
//// Use, modification and distribution is subject to the Boost Software
//// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//// http://www.boost.org/LICENSE_1_0.txt)
//
//
//#include <cstddef> // NULL
//#include <iomanip>
//#include <iostream>
//#include <fstream>
//#include <string>
//
//#include <boost/archive/tmpdir.hpp>
//
//#include <boost/archive/text_iarchive.hpp>
//#include <boost/archive/text_oarchive.hpp>
//
//#include <boost/serialization/base_object.hpp>
//#include <boost/serialization/utility.hpp>
//#include <boost/serialization/list.hpp>
//#include <boost/serialization/assume_abstract.hpp>
//
///////////////////////////////////////////////////////////////
//// The intent of this program is to serve as a tutorial for
//// users of the serialization package.  An attempt has been made
//// to illustrate most of the facilities of the package.  
////
//// The intent is to create an example suffciently complete to
//// illustrate the usage and utility of the package while
//// including a minimum of other code. 
////
//// This illustration models the bus system of a small city.
//// This includes, multiple bus stops,  bus routes and schedules.
//// There are different kinds of stops.  Bus stops in general will
//// will appear on multiple routes.  A schedule will include
//// muliple trips on the same route.
//
///////////////////////////////////////////////////////////////
//// gps coordinate
////
//// llustrates serialization for a simple type
////
//class gps_position
//{
//    friend std::ostream & operator<<(std::ostream &os, const gps_position &gp);
//    friend class boost::serialization::access;
//    int degrees;
//    int minutes;
//    float seconds;
//    template<class Archive>
//    void serialize(Archive & ar, const unsigned int /* file_version */){
//        ar & degrees & minutes & seconds;
//    }
//public:
//    // every serializable class needs a constructor
//    gps_position(){};
//    gps_position(int _d, int _m, float _s) : 
//    degrees(_d), minutes(_m), seconds(_s)
//    {}
//};
//std::ostream & operator<<(std::ostream &os, const gps_position &gp)
//{
//    return os << ' ' << gp.degrees << (unsigned char)186 << gp.minutes << '\'' << gp.seconds << '"';
//}
//
///////////////////////////////////////////////////////////////
//// One bus stop
////
//// illustrates serialization of serializable members
////
//
//class bus_stop
//{
//    friend class boost::serialization::access;
//    friend std::ostream & operator<<(std::ostream &os, const bus_stop &gp);
//    virtual std::string description() const = 0;
//    gps_position latitude;
//    gps_position longitude;
//    template<class Archive>
//    void serialize(Archive &ar, const unsigned int version)
//    {
//        ar & latitude;
//        ar & longitude;
//    }
//protected:
//    bus_stop(const gps_position & _lat, const gps_position & _long) :
//         latitude(_lat), longitude(_long)
//         {}
//public:
//    bus_stop(){}
//    virtual ~bus_stop(){}
//};
//
//BOOST_SERIALIZATION_ASSUME_ABSTRACT(bus_stop)
//
//std::ostream & operator<<(std::ostream &os, const bus_stop &bs)
//{
//    return os << bs.latitude << bs.longitude << ' ' << bs.description();
//}
//
///////////////////////////////////////////////////////////////
//// Several kinds of bus stops
////
//// illustrates serialization of derived types
////
//class bus_stop_corner : public bus_stop
//{
//    friend class boost::serialization::access;
//    std::string street1;
//    std::string street2;
//    virtual std::string description() const
//    {
//        return street1 + " and " + street2;
//    }
//    template<class Archive>
//    void serialize(Archive &ar, const unsigned int version)
//    {
//        // save/load base class information
//        ar & boost::serialization::base_object<bus_stop>(*this);
//        ar & street1 & street2;
//    }
//
//public:
//    bus_stop_corner(){}
//    bus_stop_corner(const gps_position & _lat, const gps_position & _long,
//        const std::string & _s1, const std::string & _s2
//        ) :
//    bus_stop(_lat, _long), street1(_s1), street2(_s2)
//    {
//    }
//};
//
//class bus_stop_destination : public bus_stop
//{
//    friend class boost::serialization::access;
//    std::string name;
//    virtual std::string description() const
//    {
//        return name;
//    }
//    template<class Archive>
//    void serialize(Archive &ar, const unsigned int version)
//    {
//        ar & boost::serialization::base_object<bus_stop>(*this) & name;
//    }
//public:
//
//    bus_stop_destination(){}
//    bus_stop_destination(
//        const gps_position & _lat, const gps_position & _long, const std::string & _name
//        ) :
//    bus_stop(_lat, _long), name(_name)
//    {
//    }
//};
//
///////////////////////////////////////////////////////////////
//// a bus route is a collection of bus stops
////
//// illustrates serialization of STL collection templates.
////
//// illustrates serialzation of polymorphic pointer (bus stop *);
////
//// illustrates storage and recovery of shared pointers is correct
//// and efficient.  That is objects pointed to by more than one
//// pointer are stored only once.  In such cases only one such
//// object is restored and pointers are restored to point to it
////
//class bus_route
//{
//    friend class boost::serialization::access;
//    friend std::ostream & operator<<(std::ostream &os, const bus_route &br);
//    typedef bus_stop * bus_stop_pointer;
//    std::list<bus_stop_pointer> stops;
//    template<class Archive>
//    void serialize(Archive &ar, const unsigned int version)
//    {
//        // in this program, these classes are never serialized directly but rather
//        // through a pointer to the base class bus_stop. So we need a way to be
//        // sure that the archive contains information about these derived classes.
//        //ar.template register_type<bus_stop_corner>();
//        ar.register_type(static_cast<bus_stop_corner *>(NULL));
//        //ar.template register_type<bus_stop_destination>();
//        ar.register_type(static_cast<bus_stop_destination *>(NULL));
//        // serialization of stl collections is already defined
//        // in the header
//        ar & stops;
//    }
//public:
//    bus_route(){}
//    void append(bus_stop *_bs)
//    {
//        stops.insert(stops.end(), _bs);
//    }
//};
//std::ostream & operator<<(std::ostream &os, const bus_route &br)
//{
//    std::list<bus_stop *>::const_iterator it;
//    // note: we're displaying the pointer to permit verification
//    // that duplicated pointers are properly restored.
//    for(it = br.stops.begin(); it != br.stops.end(); it++){
//        os << '\n' << std::hex << "0x" << *it << std::dec << ' ' << **it;
//    }
//    return os;
//}
//
///////////////////////////////////////////////////////////////
//// a bus schedule is a collection of routes each with a starting time
////
//// Illustrates serialization of STL objects(pair) in a non-intrusive way.
//// See definition of operator<< <pair<F, S> >(ar, pair) and others in
//// serialization.hpp
//// 
//// illustrates nesting of serializable classes
////
//// illustrates use of version number to automatically grandfather older
//// versions of the same class.
//
//class bus_schedule
//{
//public:
//    // note: this structure was made public. because the friend declarations
//    // didn't seem to work as expected.
//    struct trip_info
//    {
//        template<class Archive>
//        void serialize(Archive &ar, const unsigned int file_version)
//        {
//            // in versions 2 or later
//            if(file_version >= 2)
//                // read the drivers name
//                ar & driver;
//            // all versions have the follwing info
//            ar & hour & minute;
//        }
//
//        // starting time
//        int hour;
//        int minute;
//        // only after system shipped was the driver's name added to the class
//        std::string driver;
//
//        trip_info(){}
//        trip_info(int _h, int _m, const std::string &_d) :
//        hour(_h), minute(_m), driver(_d)
//        {}
//    };
//private:
//    friend class boost::serialization::access;
//    friend std::ostream & operator<<(std::ostream &os, const bus_schedule &bs);
//    friend std::ostream & operator<<(std::ostream &os, const bus_schedule::trip_info &ti);
//    std::list<std::pair<trip_info, bus_route *> > schedule;
//    template<class Archive>
//    void serialize(Archive &ar, const unsigned int version)
//    {
//        ar & schedule;
//    }
//public:
//    void append(const std::string &_d, int _h, int _m, bus_route *_br)
//    {
//        schedule.insert(schedule.end(), std::make_pair(trip_info(_h, _m, _d), _br));
//    }
//    bus_schedule(){}
//};
//BOOST_CLASS_VERSION(bus_schedule, 2)
//
//std::ostream & operator<<(std::ostream &os, const bus_schedule::trip_info &ti)
//{
//    return os << '\n' << ti.hour << ':' << ti.minute << ' ' << ti.driver << ' ';
//}
//std::ostream & operator<<(std::ostream &os, const bus_schedule &bs)
//{
//    std::list<std::pair<bus_schedule::trip_info, bus_route *> >::const_iterator it;
//    for(it = bs.schedule.begin(); it != bs.schedule.end(); it++){
//        os << it->first << *(it->second);
//    }
//    return os;
//}
//
//void save_schedule(const bus_schedule &s, const char * filename){
//    // make an archive
//    std::ofstream ofs(filename);
//    boost::archive::text_oarchive oa(ofs);
//    oa << s;
//}
//
//void
//restore_schedule(bus_schedule &s, const char * filename)
//{
//    // open the archive
//    std::ifstream ifs(filename);
//    boost::archive::text_iarchive ia(ifs);
//
//    // restore the schedule from the archive
//    ia >> s;
//}
//
//int main(int argc, char *argv[])
//{   
//    // make the schedule
//    bus_schedule original_schedule;
//
//    // fill in the data
//    // make a few stops
//    bus_stop *bs0 = new bus_stop_corner(
//        gps_position(34, 135, 52.560f),
//        gps_position(134, 22, 78.30f),
//        "24th Street", "10th Avenue"
//        );
//    bus_stop *bs1 = new bus_stop_corner(
//        gps_position(35, 137, 23.456f),
//        gps_position(133, 35, 54.12f),
//        "State street", "Cathedral Vista Lane"
//        );
//    bus_stop *bs2 = new bus_stop_destination(
//        gps_position(35, 136, 15.456f),
//        gps_position(133, 32, 15.300f),
//        "White House"
//        );
//    bus_stop *bs3 = new bus_stop_destination(
//        gps_position(35, 134, 48.789f),
//        gps_position(133, 32, 16.230f),
//        "Lincoln Memorial"
//        );
//
//    // make a  routes
//    bus_route route0;
//    route0.append(bs0);
//    route0.append(bs1);
//    route0.append(bs2);
//
//    // add trips to schedule
//    original_schedule.append("bob", 6, 24, &route0);
//    original_schedule.append("bob", 9, 57, &route0);
//    original_schedule.append("alice", 11, 02, &route0);
//
//    // make aother routes
//    bus_route route1;
//    route1.append(bs3);
//    route1.append(bs2);
//    route1.append(bs1);
//
//    // add trips to schedule
//    original_schedule.append("ted", 7, 17, &route1);
//    original_schedule.append("ted", 9, 38, &route1);
//    original_schedule.append("alice", 11, 47, &route1);
//
//    // display the complete schedule
//    std::cout << "original schedule";
//    std::cout << original_schedule;
//
//    std::string filename(boost::archive::tmpdir());
//    filename += "/demofile.txt";
//
//    // save the schedule
//    save_schedule(original_schedule, filename.c_str());
//
//    // ... some time later
//    // make  a new schedule
//    bus_schedule new_schedule;
//
//    restore_schedule(new_schedule, filename.c_str());
//
//    // and display
//    std::cout << "\nrestored schedule";
//    std::cout << new_schedule;
//    // should be the same as the old one. (except for the pointer values)
//
//    delete bs0;
//    delete bs1;
//    delete bs2;
//    delete bs3;
//    return 0;
//}
