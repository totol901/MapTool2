// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "MapTool2.h"
#include "MainFrm.h"
//#include "TextureManager.h"
#include "InputClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	//ON_WM_SIZE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
	CD3dClass::GetInstance();

	AllocConsole();	//�ܼ� �Ҵ�
	FILE *acStreamOut;
	FILE *acStreamIn;
	freopen_s(&acStreamOut, "CONOUT$", "wt", stdout);
	freopen_s(&acStreamIn, "CONIN$", "r", stdin);
	//TextureManager::GetInstance();
}

CMainFrame::~CMainFrame()
{
	m_pMapScene->Shutdown();
	SAFE_DELETE(m_pMapScene);
	m_pMiniMapScene->Shutdown();
	SAFE_DELETE(m_pMiniMapScene);

	InputClass::GetInstance()->Shutdown();
	//TextureManager::GetInstance()->Destroy();
	CD3dClass::GetInstance()->Shutdown();
}

//void CMainFrame::OnSize(UINT ntype, int cs, int cy)
//{
//	CD3dClass::GetInstance()->OnResize(
//		CD3dClass::GetInstance()->GetMainView(),
//		CD3dClass::GetInstance()->GetMiniView(),
//		CD3dClass::GetInstance()->GetPropertyView());
//}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	// TODO: ���� ������ ��ŷ�� �� ���� �Ϸ��� �� �� ���� �����Ͻʽÿ�.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	mAppPaused = (false);
	mMinimized = (false);
	mMaximized = (false);
	mResizing = (false);

	m_MainSplitter.CreateStatic(this, 1, 2);
	m_SubSplitter.CreateStatic(&m_MainSplitter, 3, 1);

	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CMapTool2View),
		CSize(800, 600), pContext);
	m_SubSplitter.CreateView(0, 0, RUNTIME_CLASS(CMiniMapView),
		CSize(300, 150), pContext);
	m_SubSplitter.CreateView(1, 0, RUNTIME_CLASS(CProperyView),
		CSize(300, 150), pContext);
	m_SubSplitter.CreateView(2, 0, RUNTIME_CLASS(CControlView),
		CSize(300, 150), pContext);

	m_pMainView = (CMapTool2View*)m_MainSplitter.GetPane(0, 1);
	m_pMiniMapView = (CMiniMapView*)m_SubSplitter.GetPane(0, 0);
	m_pProperyView = (CProperyView*)m_SubSplitter.GetPane(1, 0);
	m_pControlView = (CControlView*)m_SubSplitter.GetPane(2, 0);

	m_MainSplitter.SetColumnInfo(0, 300, 10);

	//���̷�Ʈ11 �ʱ�ȭ
	CD3dClass::GetInstance()->Initialize(m_pMainView, m_pMiniMapView, m_pProperyView);
	//�ؽ��� �ҷ�����
	//TextureManager::GetInstance()->AddTexture(TEXT("../MapTool2/texture/dirt01.dds"), TEXT("dirt01"));

	//��ǲ �ʱ�ȭ
	InputClass::GetInstance()->Initialize(AfxGetInstanceHandle(),
		this->m_hWnd);

	//�ʽ� �ʱ�ȭ
	m_pMapScene = new MapScene;
	m_pMapScene->Initialize(AfxGetInstanceHandle(),
		CD3dClass::GetInstance()->GetMainView()->m_hWnd
	);
	CD3dClass::GetInstance()->SetMapSceneLink(m_pMapScene);

	//�̴ϸʽ� �ʱ�ȭ
	m_pMiniMapScene = new MiniMapScene;
	m_pMiniMapScene->SetCameraLink(m_pMapScene->GetCameraClass());
	m_pMiniMapScene->SetTerrainLink(m_pMapScene->GetTerrainClass());
	m_pMiniMapScene->Initialize(AfxGetInstanceHandle(),
		CD3dClass::GetInstance()->GetMiniView()->m_hWnd);
	CD3dClass::GetInstance()->SetMiniMapSceneLink(m_pMiniMapScene);

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG