/**H**************************************************************************
 File            : DropIndexAlgorithm.h
 Subsystem       : 
 Function Name(s): 
 Author          : YiQiu
 Date            : 2010-3-24  
 Time            : 17:37
 Description     : 

 Revision        : 

 History
 -------


 Copyright (c) xxxx Ltd.
***************************************************************************H*/
#ifndef _DROPINDEXALGORITHM_H_2010_3
#define _DROPINDEXALGORITHM_H_2010_3


///**
//*@note 将一个数四舍五入，在一些特殊大数据类型可能会有Bug，先不用
//*/
//template<typename T1, typename T2>
//inline T2 Round_4_5(T1 a)
//{
//    return T(__int64(a+0.5));
//}
//
//template<typename T>
//inline T RoundDiv_4_5(T a, T b)
//{
//    return T(double(a)/b+0.5);
//}

template<typename T, bool bZeroBase>
void GenerateDropIndex(deque<T>*& Table, T Drop, T Base)
{
    assert(Drop > 0);
    assert(Base > Drop);

    T nIntervalNum = Base/Drop;
    int i =0;
    DWORD j = Base%Drop;
    for ( DWORD k=0; k<Drop-j; ++k, ++i)
    {
        register DWORD tmpIndex = i*nIntervalNum+ DWORD(float(nIntervalNum)/2+0.5);
        tmpIndex -= DWORD(bZeroBase);
        Table->push_back(T(tmpIndex));
    }

    DWORD dwBaseIndex = i*nIntervalNum;
    i = 0;
    for ( DWORD k=0; k<j; ++k, ++i)
    {
        register DWORD tmpIndex = dwBaseIndex + i*(nIntervalNum+1)+DWORD(float(nIntervalNum+1)/2+0.5);
        tmpIndex -= DWORD(bZeroBase);
        Table->push_back(T(tmpIndex));
    }
}

template<typename T, T Base, bool bZeroBase>
void GenerateDropIndex(deque<T>*& Table, T Drop)
{
    return GenerateDropIndex<T,bZeroBase>(Table, Drop, Base);
}

template <typename T, T Base, bool bZeroBase=true>
class CDropTableMgr :
    public Singleton<CDropTableMgr<T,Base,bZeroBase> >
{
public:
    typedef deque<T> DropTable;
    typedef const deque<T> c_DropTable;
public:
    const DropTable* GetDropTable(T Drop)
    {
        map<T, DropTable* >::iterator iter;
        iter = DropTableT.find(Drop);
        if ( iter != DropTableT.end() )
        {
            return iter->second;
        }

        DropTable* p = new DropTable;
        GenerateDropIndex<T, Base,bZeroBase>(p, Drop);
        DropTableT[Drop] = p;
        return p;
    }
protected:
    map<T, deque<T>* > DropTableT;
};

template <typename T, bool bZeroBase=true>
class CDropTableMgr2 :
    public Singleton<CDropTableMgr2<T,bZeroBase> >
{
public:
    typedef deque<T> DropTable;
    typedef const deque<T> c_DropTable;
public:
    const DropTable* GetDropTable(T Drop, T Base)
    {
        map<T, DropTable* >::iterator iter;
        iter = DropTableT.find(Drop);
        if ( iter != DropTableT.end() )
        {
            return iter->second;
        }

        DropTable* p = new DropTable;
        GenerateDropIndex<T,bZeroBase>(p, Drop, Base);
        DropTableT[Drop] = p;
        return p;
    }
protected:
    map<T, DropTable* > DropTableT;
};





#endif  // _DROPINDEXALGORITHM_H_2010_


 // End of file



