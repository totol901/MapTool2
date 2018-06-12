// ProperyView.cpp : implementation file
//

#include "stdafx.h"
#include "MapTool2.h"
#include "ProperyView.h"


// CProperyView

IMPLEMENT_DYNCREATE(CProperyView, CView)

CProperyView::CProperyView()
{

}

CProperyView::~CProperyView()
{
}

BEGIN_MESSAGE_MAP(CProperyView, CView)
END_MESSAGE_MAP()


// CProperyView drawing

void CProperyView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	// TODO: add draw code here
}


// CProperyView diagnostics

#ifdef _DEBUG
void CProperyView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CProperyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CProperyView message handlers
