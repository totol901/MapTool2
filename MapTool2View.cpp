
// MapTool2View.cpp : CMapTool2View Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MapTool2.h"
#endif

#include "MapTool2Doc.h"
#include "MapTool2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMapTool2View

IMPLEMENT_DYNCREATE(CMapTool2View, CView)

BEGIN_MESSAGE_MAP(CMapTool2View, CView)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CMapTool2View ����/�Ҹ�

CMapTool2View::CMapTool2View()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CMapTool2View::~CMapTool2View()
{
}

BOOL CMapTool2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

BOOL CMapTool2View::OnEraseBkgnd(CDC * pDC)
{
	return FALSE;
}

// CMapTool2View �׸���

void CMapTool2View::OnDraw(CDC* /*pDC*/)
{
	CMapTool2Doc* pDoc = GetDocument();
	this->m_hWnd;
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CMapTool2View ����

#ifdef _DEBUG
void CMapTool2View::AssertValid() const
{
	CView::AssertValid();
}

void CMapTool2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapTool2Doc* CMapTool2View::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapTool2Doc)));
	return (CMapTool2Doc*)m_pDocument;
}
#endif //_DEBUG


// CMapTool2View �޽��� ó����


void CMapTool2View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}


