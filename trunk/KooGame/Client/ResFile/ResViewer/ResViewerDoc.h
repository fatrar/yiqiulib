// ResViewerDoc.h : interface of the CResViewerDoc class
//


#pragma once


class CResViewerDoc : public CDocument
{
protected: // create from serialization only
	CResViewerDoc();
	DECLARE_DYNCREATE(CResViewerDoc)

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
	virtual ~CResViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


