
// MapTool2View.h : CMapTool2View Ŭ������ �������̽�
//

#pragma once
class CMapTool2Doc;

class CMapTool2View : public CView
{
protected: // serialization������ ��������ϴ�.
	CMapTool2View();
	DECLARE_DYNCREATE(CMapTool2View)

// Ư���Դϴ�.
public:
	CMapTool2Doc* GetDocument() const;

// �۾��Դϴ�.
public:
	BOOL OnEraseBkgnd(CDC* pDC);
// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// �����Դϴ�.
public:
	virtual ~CMapTool2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	
};

#ifndef _DEBUG  // MapTool2View.cpp�� ����� ����
inline CMapTool2Doc* CMapTool2View::GetDocument() const
   { return reinterpret_cast<CMapTool2Doc*>(m_pDocument); }
#endif

