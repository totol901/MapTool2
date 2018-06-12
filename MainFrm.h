
// MainFrm.h : CMainFrame 클래스의 인터페이스
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
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
protected:
	
protected:
	CFixedSplitterWnd m_MainSplitter;
	CSplitterWnd m_SubSplitter;

	CMapTool2View* m_pMainView;
	CMiniMapView* m_pMiniMapView;
	CControlView* m_pControlView;
	CProperyView* m_pProperyView;

	
// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	//afx_msg void OnSize(UINT ntype, int cs, int cy);

// 생성된 메시지 맵 함수
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


