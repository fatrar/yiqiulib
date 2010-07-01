// GameToolDoc.cpp : implementation of the CGameToolDoc class
//

#include "stdafx.h"
#include "GameTool.h"

#include "GameToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGameToolDoc

IMPLEMENT_DYNCREATE(CGameToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CGameToolDoc, CDocument)
END_MESSAGE_MAP()


// CGameToolDoc construction/destruction

CGameToolDoc::CGameToolDoc()
{

}

CGameToolDoc::~CGameToolDoc()
{
}

BOOL CGameToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CGameToolDoc serialization

void CGameToolDoc::Serialize(CArchive& ar)
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


// CGameToolDoc diagnostics

#ifdef _DEBUG
void CGameToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGameToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGameToolDoc commands
