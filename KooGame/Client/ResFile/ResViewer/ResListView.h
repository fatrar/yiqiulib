#pragma once
#include "IResReaderEx.h"

// CResListView view

//namespace ResFile{class CResIterator;}

class CResListView 
    : public CListView
    , public ResFile::IInfoReadCallback
{
	DECLARE_DYNCREATE(CResListView)

protected:
	CResListView();           // protected constructor used by dynamic creation
	virtual ~CResListView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual void OnInitialUpdate();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();

protected:
    void AddAllData();
    void RemoveAllData();

protected:
    //ResFile::CResIterator* m_IterList;

    // IInfoReadCallback
protected:
    virtual void OnInfoRead(
        ResFile::DWORD nIndex,
        ResFile::eEncryptAlgo eAlgo,
        ResFile::eCompressAlgo cAlgo,
        ResFile::eCompressParam cParam,
        ResFile::DWORD dwRawDataLen );
};


