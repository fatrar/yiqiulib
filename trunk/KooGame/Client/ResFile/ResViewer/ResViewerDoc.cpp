// ResViewerDoc.cpp : implementation of the CResViewerDoc class
//

#include "stdafx.h"
#include "ResViewer.h"

#include "ResViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CResViewerDoc

IMPLEMENT_DYNCREATE(CResViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CResViewerDoc, CDocument)
END_MESSAGE_MAP()


// CResViewerDoc construction/destruction

CResViewerDoc::CResViewerDoc()
{
	// TODO: add one-time construction code here

}

CResViewerDoc::~CResViewerDoc()
{
}

BOOL CResViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CResViewerDoc serialization

void CResViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CResViewerDoc diagnostics

#ifdef _DEBUG
void CResViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CResViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CResViewerDoc commands
