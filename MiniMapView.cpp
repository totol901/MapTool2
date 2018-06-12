// MiniMapView.cpp : implementation file
//

#include "stdafx.h"
#include "MapTool2.h"
#include "MiniMapView.h"


// CMiniMapView

IMPLEMENT_DYNCREATE(CMiniMapView, CView)

CMiniMapView::CMiniMapView()
{

}

CMiniMapView::~CMiniMapView()
{
}

BEGIN_MESSAGE_MAP(CMiniMapView, CView)
END_MESSAGE_MAP()


// CMiniMapView drawing

void CMiniMapView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	// TODO: add draw code here
}


// CMiniMapView diagnostics

#ifdef _DEBUG
void CMiniMapView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniMapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniMapView message handlers
