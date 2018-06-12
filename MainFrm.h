
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once
#include "MiniMapView.h"
#include "MapTool2View.h"
#include "ControlView.h"
#include "FixedSplitterWnd.h"
#include "ProperyView.h"
#include "MapScene.h"
#include "MiniMapScene.h"

class CMainFrame : public CFrameWnd
{
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư���Դϴ�.
protected:
	
protected:
	CFixedSplitterWnd m_MainSplitter;
	CSplitterWnd m_SubSplitter;

	CMapTool2View* m_pMainView;
	CMiniMapView* m_pMiniMapView;
	CControlView* m_pControlView;
	CProperyView* m_pProperyView;

	
// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	//afx_msg void OnSize(UINT ntype, int cs, int cy);

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	MapScene* m_pMapScene;
	MiniMapScene* m_pMiniMapScene;
	int mClientWidth;
	int mClientHeight;
	bool      mAppPaused;
	bool      mMinimized;
	bool      mMaximized;
	bool      mResizing;
	
};


