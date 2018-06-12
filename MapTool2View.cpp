
// MapTool2View.cpp : CMapTool2View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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

// CMapTool2View 생성/소멸

CMapTool2View::CMapTool2View()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMapTool2View::~CMapTool2View()
{
}

BOOL CMapTool2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

BOOL CMapTool2View::OnEraseBkgnd(CDC * pDC)
{
	return FALSE;
}

// CMapTool2View 그리기

void CMapTool2View::OnDraw(CDC* /*pDC*/)
{
	CMapTool2Doc* pDoc = GetDocument();
	this->m_hWnd;
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMapTool2View 진단

#ifdef _DEBUG
void CMapTool2View::AssertValid() const
{
	CView::AssertValid();
}

void CMapTool2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapTool2Doc* CMapTool2View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapTool2Doc)));
	return (CMapTool2Doc*)m_pDocument;
}
#endif //_DEBUG


// CMapTool2View 메시지 처리기


void CMapTool2View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}


