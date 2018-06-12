// ControlView.cpp : implementation file
//

#include "stdafx.h"
#include "MapTool2.h"
#include "ControlView.h"


// CControlView

IMPLEMENT_DYNCREATE(CControlView, CFormView)

CControlView::CControlView()
	: CFormView(IDD_CONTROLVIEW)
{

}

CControlView::~CControlView()
{
}

void CControlView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

void CControlView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_pControlProperty = new CControlProperty(this);

	m_pControlProperty->Create(this, WS_CHILD | WS_VISIBLE);
	m_pControlProperty->MoveWindow(0, 0, 300, 500, FALSE);

	m_pControlProperty->SetActivePage(0);
}

void CControlView::OnDestroy()
{
	CFormView::OnDestroy();

	delete m_pControlProperty;
	m_pControlProperty = NULL;
}

BEGIN_MESSAGE_MAP(CControlView, CFormView)
END_MESSAGE_MAP()


// CControlView diagnostics

#ifdef _DEBUG
void CControlView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CControlView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CControlView message handlers
