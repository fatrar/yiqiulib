// GameToolDoc.h : interface of the CGameToolDoc class
//


#pragma once


class CGameToolDoc : public CDocument
{
protected: // create from serialization only
	CGameToolDoc();
	DECLARE_DYNCREATE(CGameToolDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CGameToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()


};


