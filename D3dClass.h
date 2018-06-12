#pragma once
#include "MapScene.h"
#include "MiniMapScene.h"

class CD3dClass
{
public:
	CD3dClass();
	CD3dClass(const CD3dClass&) = delete;
	CD3dClass(const CD3dClass&&) = delete;
	CD3dClass& operator= (const CD3dClass &) = delete;
	CD3dClass& operator= (const CD3dClass &&) = delete;
	~CD3dClass();

	static CD3dClass* GetInstance();

	bool Initialize(CMapTool2View* pMainView, CMiniMapView* pMiniMapView, CProperyView* pProperyView);
	void OnResize(CMapTool2View* pMainView, CMiniMapView* pMiniMapView, CProperyView* pProperyView);
	void Shutdown();
	void BeginMainScene(const float& red, const float& green,
		const float& blue, const float& alpha);
	void EndMainScene();
	void BeginMiniMapScene(const float& red, const float& green,
		const float& blue, const float& alpha);
	void EndMiniMapScene();
	void BeginPropertyScene(const float& red, const float& green,
		const float& blue, const float& alpha);
	void EndPropertyScene();


	void TurnZBufferOn();
	void TurnZBufferOff();
	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

	void Rasterizer_SetFillSolid();
	void Rasterizer_SetWireFrame();

	void GetVideoCardInfo(char*, int&);

	void TurnOnCulling();
	void TurnOffCulling();

	void EnableSecondBlendState();

public:
	ID3D11Device* GetDevice() const { return m_pDevice; }
	ID3D11DeviceContext* GetDeviceContext() const { return m_pDeviceContext; }
	CMapTool2View * GetMainView() const {return m_pMainView;}
	CMiniMapView * GetMiniView() const { return m_pMiniMapView; }
	CProperyView * GetPropertyView() const { return m_pProperyView; }
	void SetMapSceneLink(MapScene* pMapScene) { m_pMapScene = pMapScene; }
	MapScene* GetMapScene() { return m_pMapScene; }
	void SetMiniMapSceneLink(MiniMapScene* pMiniMapScene) { m_pMiniMapScene = pMiniMapScene; }
	MiniMapScene* GetMiniMapScene() { return m_pMiniMapScene; }


private:
	void CreateRasterizerStates();
	void CreateDepthStencilState();
	void CreateDisableDepthStencilState();
	void SetZBuffer();
	void SetRestersate();

	void CreateMainSwapChain(CMapTool2View* pMainView, IDXGIFactory* pFactory);
	void CreateMainRenderTargetView(const CRect ViewRect);
	void CreateMainDepthStencilBuffer(const CRect ViewRect);
	void SetOMMainRenderTarget();
	void CreateMainViewPort(const CRect mainViewRect);

	void CreateMiniMapSwapChain(CMiniMapView* pMiniMapView, 
		IDXGIFactory* pFactory);
	void CreateMiniRenderTargetView(const CRect ViewRect);
	void CreateMiniDepthStencilBuffer(const CRect ViewRect);
	void SetOMMiniRenderTarget();
	void CreateMiniViewPort(const CRect ViewRect);

	void CreatePropertySwapChain(CProperyView* pProperyView, IDXGIFactory* pFactory);
	void CreatePropertyRenderTargetView(const CRect ViewRect);
	void CreatePropertyDepthStencilBuffer(const CRect ViewRect);
	void SetOMPropertyRenderTarget();
	void CreatePropertyViewPort(const CRect ViewRect);

	

private:
	UINT m_numerator;
	UINT m_denominator;
	bool isInit;
	static CD3dClass* m_CInstance;

	char m_videoCardDescription[128] = { 0, };
	int m_videoCardMemory = 0;

	CMapTool2View * m_pMainView;
	CMiniMapView * m_pMiniMapView;
	CProperyView * m_pProperyView;

	MapScene* m_pMapScene;
	MiniMapScene* m_pMiniMapScene;

	const bool ENABLE_4X_MSAA = false;	//멀티 셈플링 유무
	bool m_bIsZBufferOn;
	bool m_bSolid_WireFrame;
	UINT m_4xMsaaQuality;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;

	ID3D11DepthStencilState* m_pDepthStencilState;
	ID3D11DepthStencilState* m_pDepthDisableStencilState;

	ID3D11RasterizerState* m_pRasterState_FillSolid;
	ID3D11RasterizerState* m_pRasterState_WireFrame;
	ID3D11RasterizerState* m_rasterStateNoCulling;

	IDXGISwapChain* m_pMainSwapChain;
	ID3D11RenderTargetView* m_pMainRenderTargetView;
	ID3D11Texture2D* m_pMainDepthStencilBuffer;
	ID3D11DepthStencilView* m_pMainDepthStencilView;
	D3D11_VIEWPORT m_MainScreenViewport;

	IDXGISwapChain* m_pMiniMapSwapChain;
	ID3D11RenderTargetView* m_pMiniMapRenderTargetView;
	ID3D11Texture2D* m_pMiniMapDepthStencilBuffer;
	ID3D11DepthStencilView* m_pMiniMapDepthStencilView;
	D3D11_VIEWPORT m_MiniMapScreenViewport;

	IDXGISwapChain* m_pPropertySwapChain;
	ID3D11RenderTargetView* m_pPropertyRenderTargetView;
	ID3D11Texture2D* m_pPropertyDepthStencilBuffer;
	ID3D11DepthStencilView* m_pPropertyDepthStencilView;
	D3D11_VIEWPORT m_PropertyScreenViewport;
	
	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;

	ID3D11BlendState* m_alphaBlendState2;
};
