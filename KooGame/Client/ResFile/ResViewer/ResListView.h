#pragma once


// CResListView view

class CResListView : public CListView
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
};


