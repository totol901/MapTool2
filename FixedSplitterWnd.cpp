#include "stdafx.h"
#include "FixedSplitterWnd.h"

BEGIN_MESSAGE_MAP(CFixedSplitterWnd, CSplitterWnd)
	//ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CFixedSplitterWnd::CFixedSplitterWnd()
{
}

CFixedSplitterWnd::~CFixedSplitterWnd()
{
}

//LRESULT CFixedSplitterWnd::OnNcHitTest(CPoint point)
//{
//	return HTNOWHERE;
//}

void CFixedSplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (point.x > 150 && point.x < 450)  
		CSplitterWnd::OnMouseMove(nFlags, point);
}

void CFixedSplitterWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (point.x > 150)
		CSplitterWnd::OnLButtonDown(nFlags, point);
}
