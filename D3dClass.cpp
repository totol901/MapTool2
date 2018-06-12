#include "stdafx.h"
#include "D3dClass.h"

CD3dClass* CD3dClass::m_CInstance = nullptr;

//------------------------------------------------------------------------------------
CD3dClass::CD3dClass()
	:m_pDevice(nullptr),
	m_pDeviceContext(nullptr),
	m_pMainSwapChain(nullptr),
	m_pMiniMapSwapChain(nullptr),
	m_pPropertySwapChain(nullptr),
	m_pMainRenderTargetView(nullptr),
	m_pMiniMapRenderTargetView(nullptr),
	m_pMainDepthStencilBuffer(nullptr),
	m_pMiniMapDepthStencilBuffer(nullptr),
	m_pDepthStencilState(nullptr),
	m_pDepthDisableStencilState(nullptr),
	m_pMainDepthStencilView(nullptr),
	m_pMiniMapDepthStencilView(nullptr),
	m_pRasterState_FillSolid(nullptr),
	m_pRasterState_WireFrame(nullptr),
	m_pPropertyRenderTargetView(nullptr),
	m_pPropertyDepthStencilBuffer(nullptr),
	m_pPropertyDepthStencilView(nullptr),
	m_alphaEnableBlendingState(nullptr),
	m_alphaDisableBlendingState(nullptr),
	m_bIsZBufferOn(true),
	m_bSolid_WireFrame(true),
	m_pMainView(nullptr),
	m_pMiniMapView(nullptr),
	m_pProperyView(nullptr),
	isInit(false),
	m_pMapScene(nullptr),
	m_pMiniMapScene(nullptr),
	m_rasterStateNoCulling(nullptr)
{}

//------------------------------------------------------------------------------------
CD3dClass::~CD3dClass()
{}

//------------------------------------------------------------------------------------
CD3dClass * CD3dClass::GetInstance()
{
	if (m_CInstance == nullptr)
	{
		m_CInstance = new CD3dClass();
	}

	return m_CInstance;
}

//------------------------------------------------------------------------------------
bool CD3dClass::Initialize(CMapTool2View* pMainView, CMiniMapView* pMiniMapView, CProperyView* pProperyView)
{
	IDXGIFactory* pFactory = nullptr;
	D3D_FEATURE_LEVEL featureLevel;//FeatureLevel 선언(최고 수준의 Level 받기위해)
	UINT createDeviceFlags = 0; // 디바이스 플레그 설정(Default)
	
	m_pMainView = pMainView;
	m_pMiniMapView = pMiniMapView;
	m_pProperyView = pProperyView;

	// DirectX 그래픽 인터페이스 팩토리를 만듬
	HR(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory));

	// 팩토리 객체를 사용하여 첫번째 그래픽 카드 인터페이스 어뎁터를 생성합니다
	IDXGIAdapter* adapter = nullptr;
	if (FAILED(pFactory->EnumAdapters(0, &adapter)))
	{
		return false;
	}

	// 출력(모니터)에 대한 첫번째 어뎁터를 지정합니다.
	IDXGIOutput* adapterOutput = nullptr;
	if (FAILED(adapter->EnumOutputs(0, &adapterOutput)))
	{
		return false;
	}

	// 출력 (모니터)에 대한 DXGI_FORMAT_R8G8B8A8_UNORM 표시 형식에 맞는 모드 수를 가져옵니다
	unsigned int numModes = 0;
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL)))
	{
		return false;
	}

	// 가능한 모든 모니터와 그래픽카드 조합을 저장할 리스트를 생성합니다
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// 이제 디스플레이 모드에 대한 리스트를 채웁니다
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList)))
	{
		return false;
	}

	// 이제 모든 디스플레이 모드에 대해 화면 너비/높이에 맞는 디스플레이 모드를 찾습니다.
	// 적합한 것을 찾으면 모니터의 새로고침 비율의 분모와 분자 값을 저장합니다.
	unsigned int numerator = 0;
	unsigned int denominator = 0;
	CRect rect;
	GetWindowRect(pMainView->m_hWnd, &rect);
	for (unsigned int i = 0; i<numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)rect.Width())
		{
			if (displayModeList[i].Height == (unsigned int)rect.Height())
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}
	m_numerator = numerator;
	m_denominator = denominator;

	// 비디오카드의 구조체를 얻습니다
	DXGI_ADAPTER_DESC adapterDesc;
	if (FAILED(adapter->GetDesc(&adapterDesc)))
	{
		return false;
	}

	// 비디오카드 메모리 용량 단위를 메가바이트 단위로 저장합니다
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// 비디오카드의 이름을 저장합니다
	size_t stringLength = 0;
	if (wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128) != 0)
	{
		return false;
	}

	// 디스플레이 모드 리스트를 해제합니다
	delete[] displayModeList;
	displayModeList = 0;

	// 출력 어뎁터를 해제합니다
	adapterOutput->Release();
	adapterOutput = 0;

	// 어뎁터를 해제합니다
	adapter->Release();
	adapter = 0;
	
#if defined(DEBUG) || defined(_DEBUG)  
								//에러시 디버그 출력해줌
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//디바이스, 디바이스 컨택스트 생성
	HR(D3D11CreateDevice(
		0,                 // default adapter(기본 디스플레이 어댑터 설정)
		D3D_DRIVER_TYPE_HARDWARE,	   // 3차원 그래픽 가속 적용
		0,                 // no software device
		createDeviceFlags, // 디바이스 플레그 설정
		0, 0,              // default feature level array
		D3D11_SDK_VERSION, // 항상 D3D11_SDK_VERSION 지정
		&m_pDevice,	   // d3d장치 넣어줌 (Out 값)
		&featureLevel,	   // featureLevel 넣어줌[현 사용가능 최고 수준의 Level 나옴] (OUt 값)
		&m_pDeviceContext)); // 장치 문맥(Context) 넣어줌 (Out 값)

	//FeatureLevel의 수준 검사
	if (featureLevel < D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 지원 안함.", 0, 0);
		return false;
	}

	//알맞은 멀티셈플링 퀄리티 세팅
	HR(m_pDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality));
	//올바른 세팅인지 채크
	assert(m_4xMsaaQuality > 0);

	//스왑체인 설정
	CreateMainSwapChain(pMainView, pFactory);
	CreateMiniMapSwapChain(pMiniMapView, pFactory);
	CreatePropertySwapChain(pProperyView, pFactory);

	//펙토리 할당 해제
	RELEASE_COM(pFactory);

	//레스터라이저 스테이트 생성
	CreateRasterizerStates();

	// 블렌드 상태 구조체를 초기화 합니다.
	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// 알파 지원 블렌드 상태 구조체를 설정합니다.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// 설명을 사용하여 혼합 상태를 만듭니다.
	if (FAILED(m_pDevice->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState)))
	{
		return false;
	}

	// 알파블렌드 설정을 해제합니다.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// 알파블렌드 상태값을 생성합니다.
	if (FAILED(m_pDevice->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState)))
	{
		return false;
	}

	// Create a secondary alpha blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	if (FAILED(m_pDevice->CreateBlendState(&blendStateDescription, &m_alphaBlendState2)))
	{
		return false;
	}

	isInit = true;

	//창 조절시 재설정
	OnResize(pMainView, pMiniMapView, pProperyView);

	return true;
}

//------------------------------------------------------------------------------------
void CD3dClass::Shutdown()
{
	if (m_alphaBlendState2)
	{
		m_alphaBlendState2->Release();
		m_alphaBlendState2 = 0;
	}
	RELEASE_COM(m_rasterStateNoCulling);
	RELEASE_COM(m_pRasterState_FillSolid);
	RELEASE_COM(m_pRasterState_WireFrame);
	RELEASE_COM(m_pMainDepthStencilView);
	RELEASE_COM(m_pMiniMapDepthStencilView);
	RELEASE_COM(m_pDepthDisableStencilState);
	RELEASE_COM(m_pDepthStencilState);
	RELEASE_COM(m_pMainDepthStencilBuffer);
	RELEASE_COM(m_pMiniMapDepthStencilBuffer);
	RELEASE_COM(m_pMainRenderTargetView);
	RELEASE_COM(m_pMiniMapRenderTargetView);
	RELEASE_COM(m_pPropertySwapChain);
	RELEASE_COM(m_pMiniMapSwapChain);
	RELEASE_COM(m_pMainSwapChain);
	RELEASE_COM(m_pDeviceContext);
	RELEASE_COM(m_pPropertyRenderTargetView);
	RELEASE_COM(m_pDevice);
	RELEASE_COM(m_pPropertyDepthStencilBuffer);
	RELEASE_COM(m_pPropertyDepthStencilView);
	RELEASE_COM(m_alphaEnableBlendingState);
	RELEASE_COM(m_alphaDisableBlendingState);

	SAFE_DELETE(m_CInstance);
}

//------------------------------------------------------------------------------------
void CD3dClass::BeginMainScene(const float & red, const float & green, const float & blue, const float & alpha)
{
	D3DXCOLOR color = { red, green, blue, alpha };

	m_pDeviceContext->OMSetRenderTargets(1, &m_pMainRenderTargetView, m_pMainDepthStencilView);
	m_pDeviceContext->RSSetViewports(1, &m_MainScreenViewport);

	// 벡버퍼 color로 지워줌
	m_pDeviceContext->ClearRenderTargetView(m_pMainRenderTargetView, color);
	// 깊이버퍼도 지워줌 
	m_pDeviceContext->ClearDepthStencilView(m_pMainDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

//------------------------------------------------------------------------------------
void CD3dClass::EndMainScene()
{
	// 벡버퍼에 올린 그림 화면으로 보냄
	// 가능한 빨리 그려줌
	HR(m_pMainSwapChain->Present(0, 0));
}

//------------------------------------------------------------------------------------
void CD3dClass::BeginMiniMapScene(const float & red, const float & green, const float & blue, const float & alpha)
{
	D3DXCOLOR color = { red, green, blue, alpha };

	m_pDeviceContext->OMSetRenderTargets(1, &m_pMiniMapRenderTargetView, m_pMiniMapDepthStencilView);
	m_pDeviceContext->RSSetViewports(1, &m_MiniMapScreenViewport);

	// 벡버퍼 color로 지워줌
	m_pDeviceContext->ClearRenderTargetView(m_pMiniMapRenderTargetView, color);
	// 깊이버퍼도 지워줌 
	m_pDeviceContext->ClearDepthStencilView(m_pMiniMapDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

//------------------------------------------------------------------------------------
void CD3dClass::EndMiniMapScene()
{	
	// 벡버퍼에 올린 그림 화면으로 보냄
	// 미니맵은 60FPS로 그려줌
	HR(m_pMiniMapSwapChain->Present(0, 0));
}

//------------------------------------------------------------------------------------
void CD3dClass::BeginPropertyScene(const float & red, const float & green, const float & blue, const float & alpha)
{
	D3DXCOLOR color = { red, green, blue, alpha };

	m_pDeviceContext->OMSetRenderTargets(1, &m_pPropertyRenderTargetView, m_pPropertyDepthStencilView);
	m_pDeviceContext->RSSetViewports(1, &m_PropertyScreenViewport);

	// 벡버퍼 color로 지워줌
	m_pDeviceContext->ClearRenderTargetView(m_pPropertyRenderTargetView, color);
	// 깊이버퍼도 지워줌 
	m_pDeviceContext->ClearDepthStencilView(m_pPropertyDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

//------------------------------------------------------------------------------------
void CD3dClass::EndPropertyScene()
{
	// 벡버퍼에 올린 그림 화면으로 보냄
	// 속성뷰는 60FPS로 그려줌
	HR(m_pPropertySwapChain->Present(0, 0));
}

//------------------------------------------------------------------------------------
void CD3dClass::TurnZBufferOn()
{
	m_bIsZBufferOn = true;
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
}

//------------------------------------------------------------------------------------
void CD3dClass::TurnZBufferOff()
{
	m_bIsZBufferOn = false;
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthDisableStencilState, 1);
}

void CD3dClass::TurnOnAlphaBlending()
{
	// 혼합 요소를 설정합니다.
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// 알파 블렌딩을 켭니다.
	m_pDeviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);
}

void CD3dClass::TurnOffAlphaBlending()
{
	// 혼합 요소를 설정합니다.
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// 알파 블렌딩을 켭니다.
	m_pDeviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
}

//------------------------------------------------------------------------------------
void CD3dClass::Rasterizer_SetFillSolid()
{
	m_bSolid_WireFrame = true;
	m_pDeviceContext->RSSetState(m_pRasterState_FillSolid);
}

//------------------------------------------------------------------------------------
void CD3dClass::Rasterizer_SetWireFrame()
{
	m_bSolid_WireFrame = false;
	m_pDeviceContext->RSSetState(m_pRasterState_WireFrame);
}

void CD3dClass::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
}

//------------------------------------------------------------------------------------
void CD3dClass::CreateRasterizerStates()
{
	//레스터라이저 Desc 선언
	D3D11_RASTERIZER_DESC rasterDesc;

	//솔리드 상태 생성
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// 작성한 description으로부터 래스터화기 상태를 생성
	HR(m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState_FillSolid));


	//와이어 프래임 상태 생성
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	//초기화
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// 작성한 description으로부터 래스터화기 상태를 생성
	HR(m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState_WireFrame));

	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the no culling rasterizer state.
	HR(m_pDevice->CreateRasterizerState(&rasterDesc, &m_rasterStateNoCulling));
	
}

void CD3dClass::TurnOnCulling()
{
	// Set the culling rasterizer state.
	if(m_bSolid_WireFrame)
		m_pDeviceContext->RSSetState(m_pRasterState_FillSolid);
	else
		m_pDeviceContext->RSSetState(m_pRasterState_WireFrame);

	return;
}

void CD3dClass::TurnOffCulling()
{
	// Set the no back face culling rasterizer state.
	m_pDeviceContext->RSSetState(m_rasterStateNoCulling);

	return;
}

//------------------------------------------------------------------------------------
void CD3dClass::CreateMainSwapChain(CMapTool2View * pMainView, IDXGIFactory* pFactory)
{
	DXGI_SWAP_CHAIN_DESC MainSwapChainDesc;
	CRect mainViewRect;

	//교환 사슬 description을 초기화합니다.
	//메인뷰의 벡버퍼 설정
	ZeroMemory(&MainSwapChainDesc, sizeof(MainSwapChainDesc));

	//하나의 백버퍼만을 사용하도록 합니다.
	MainSwapChainDesc.BufferCount = 1;

	//백버퍼의 너비와 높이를 설정합니다.
	GetWindowRect(pMainView->m_hWnd, mainViewRect);
	MainSwapChainDesc.BufferDesc.Width = mainViewRect.right - mainViewRect.left;
	MainSwapChainDesc.BufferDesc.Height = mainViewRect.bottom - mainViewRect.top;

	//백버퍼로 일반적인 32bit의 서페이스를 지정합니다.
	MainSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//후면 버퍼 픽셀 형식
																	 // 백버퍼의 새로고침 비율을 설정합니다. 
	MainSwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	MainSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	// 백버퍼의 용도를 설정합니다. 
	MainSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// 렌더링이 이루어질 윈도우의 핸들을 설정합니다. 
	MainSwapChainDesc.OutputWindow = pMainView->m_hWnd;

	// 멀티셈플링 유무 확인
	if (ENABLE_4X_MSAA)
	{
		MainSwapChainDesc.SampleDesc.Count = 4;
		MainSwapChainDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// 멀티셈플링 안씀
	else
	{
		MainSwapChainDesc.SampleDesc.Count = 1;
		MainSwapChainDesc.SampleDesc.Quality = 0;
	}

	MainSwapChainDesc.Windowed = true;

	// 스캔라인의 정렬과 스캔라이닝을 지정되지 않음으로(unspecified) 설정합니다. 
	MainSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //디스플레이 스캔라인 모드
	MainSwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; //디스플레이 비례 모드
																		  // 출력된 이후의 백버퍼의 내용을 버리도록 합니다. 
	MainSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	// 전체화면 전환시 후면버퍼 설정에 잘부합하는 디스플레이 모드 설정됨 
	MainSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//스왑체인 생성
	HR(pFactory->CreateSwapChain(m_pDevice, &MainSwapChainDesc, &m_pMainSwapChain));

	//알트 + 앤더 먹지 않게 해줌
	HR(pFactory->MakeWindowAssociation(pMainView->m_hWnd, DXGI_MWA_NO_WINDOW_CHANGES));
}

//------------------------------------------------------------------------------------
void CD3dClass::CreateMiniMapSwapChain(CMiniMapView * pMiniMapView, IDXGIFactory * pFactory)
{
	DXGI_SWAP_CHAIN_DESC MiniMapSwapChainDesc;
	CRect MiniMapViewRect;

	//교환 사슬 description을 초기화합니다.
	//메인뷰의 벡버퍼 설정
	ZeroMemory(&MiniMapSwapChainDesc, sizeof(MiniMapSwapChainDesc));

	//하나의 백버퍼만을 사용하도록 합니다.
	MiniMapSwapChainDesc.BufferCount = 1;

	//백버퍼의 너비와 높이를 설정합니다.
	GetWindowRect(pMiniMapView->m_hWnd, MiniMapViewRect);
	MiniMapSwapChainDesc.BufferDesc.Width = MiniMapViewRect.right - MiniMapViewRect.left;
	MiniMapSwapChainDesc.BufferDesc.Height = MiniMapViewRect.bottom - MiniMapViewRect.top;

	//백버퍼로 일반적인 32bit의 서페이스를 지정합니다.
	MiniMapSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//후면 버퍼 픽셀 형식
																	 // 백버퍼의 새로고침 비율을 설정합니다. 
	MiniMapSwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	MiniMapSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	// 백버퍼의 용도를 설정합니다. 
	MiniMapSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// 렌더링이 이루어질 윈도우의 핸들을 설정합니다. 
	MiniMapSwapChainDesc.OutputWindow = pMiniMapView->m_hWnd;

	// 멀티셈플링 유무 확인
	if (ENABLE_4X_MSAA)
	{
		MiniMapSwapChainDesc.SampleDesc.Count = 4;
		MiniMapSwapChainDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// 멀티셈플링 안씀
	else
	{
		MiniMapSwapChainDesc.SampleDesc.Count = 1;
		MiniMapSwapChainDesc.SampleDesc.Quality = 0;
	}

	MiniMapSwapChainDesc.Windowed = true;

	// 스캔라인의 정렬과 스캔라이닝을 지정되지 않음으로(unspecified) 설정합니다. 
	MiniMapSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //디스플레이 스캔라인 모드
	MiniMapSwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; //디스플레이 비례 모드
																		  // 출력된 이후의 백버퍼의 내용을 버리도록 합니다. 
	MiniMapSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	// 전체화면 전환시 후면버퍼 설정에 잘부합하는 디스플레이 모드 설정됨 
	MiniMapSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//스왑체인 생성
	HR(pFactory->CreateSwapChain(m_pDevice, &MiniMapSwapChainDesc, &m_pMiniMapSwapChain));

	//알트 + 앤더 먹지 않게 해줌
	HR(pFactory->MakeWindowAssociation(pMiniMapView->m_hWnd, DXGI_MWA_NO_WINDOW_CHANGES));
}

//------------------------------------------------------------------------------------
void CD3dClass::CreateMiniRenderTargetView(const CRect  ViewRect)
{
	ID3D11Texture2D* backBuffer;

	//버퍼 크기 재설정
	HR(m_pMiniMapSwapChain->ResizeBuffers(1, ViewRect.right - ViewRect.left,
		ViewRect.bottom - ViewRect.top, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	HR(m_pMiniMapSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	//랜더 타겟 재설정
	HR(m_pDevice->CreateRenderTargetView(backBuffer, 0, &m_pMiniMapRenderTargetView));
	RELEASE_COM(backBuffer);
}

//------------------------------------------------------------------------------------
void CD3dClass::CreateMiniDepthStencilBuffer(const CRect  ViewRect)
{
	D3D11_TEXTURE2D_DESC miniDepthBufferDesc;

	//깊이 버퍼 설정
	ZeroMemory(&miniDepthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	miniDepthBufferDesc.Width = ViewRect.right - ViewRect.left;
	miniDepthBufferDesc.Height = ViewRect.bottom - ViewRect.top;
	miniDepthBufferDesc.MipLevels = 1;
	miniDepthBufferDesc.ArraySize = 1;
	miniDepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 멀티셈플링 유무
	if (ENABLE_4X_MSAA)
	{
		miniDepthBufferDesc.SampleDesc.Count = 4;
		miniDepthBufferDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// 멀티셈플링 안함
	else
	{
		miniDepthBufferDesc.SampleDesc.Count = 1;
		miniDepthBufferDesc.SampleDesc.Quality = 0;
	}

	miniDepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	miniDepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	miniDepthBufferDesc.CPUAccessFlags = 0;
	miniDepthBufferDesc.MiscFlags = 0;

	//깊이 버퍼 텍스처 생성
	HR(m_pDevice->CreateTexture2D(&miniDepthBufferDesc, 0, &m_pMiniMapDepthStencilBuffer));
}

//------------------------------------------------------------------------------------
void CD3dClass::SetOMMiniRenderTarget()
{
	// 깊이-스텐실 뷰의 description을 초기화
	D3D11_DEPTH_STENCIL_VIEW_DESC miniDepthStencilViewDesc;
	ZeroMemory(&miniDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	// 깊이-스텐실 뷰의 description을 작성합니다.
	miniDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	miniDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	miniDepthStencilViewDesc.Texture2D.MipSlice = 0;

	// 깊이-스텐실 뷰를 생성
	HR(m_pDevice->CreateDepthStencilView(m_pMiniMapDepthStencilBuffer, &miniDepthStencilViewDesc, &m_pMiniMapDepthStencilView));

	//Output Merger에 랜더타갯 설정(깊이/스텐실 뷰 포함)
	m_pDeviceContext->OMSetRenderTargets(1, &m_pMiniMapRenderTargetView, m_pMiniMapDepthStencilView);
}

//------------------------------------------------------------------------------------
void CD3dClass::CreateMiniViewPort(const CRect  ViewRect)
{
	//뷰포트 재설정
	m_MiniMapScreenViewport.TopLeftX = 0.0f;
	m_MiniMapScreenViewport.TopLeftY = 0.0f;
	m_MiniMapScreenViewport.Width = static_cast<float>(ViewRect.right - ViewRect.left);
	m_MiniMapScreenViewport.Height = static_cast<float>(ViewRect.bottom - ViewRect.top);
	m_MiniMapScreenViewport.MinDepth = 0.0f;
	m_MiniMapScreenViewport.MaxDepth = 1.0f;

	m_pDeviceContext->RSSetViewports(1, &m_MiniMapScreenViewport);
}

//------------------------------------------------------------------------------------
void CD3dClass::CreatePropertySwapChain(CProperyView * pProperyView, IDXGIFactory * pFactory)
{
	DXGI_SWAP_CHAIN_DESC ProperypSwapChainDesc;
	CRect ProperyViewRect;

	//교환 사슬 description을 초기화합니다.
	//메인뷰의 벡버퍼 설정
	ZeroMemory(&ProperypSwapChainDesc, sizeof(ProperypSwapChainDesc));

	//하나의 백버퍼만을 사용하도록 합니다.
	ProperypSwapChainDesc.BufferCount = 1;

	//백버퍼의 너비와 높이를 설정합니다.
	GetWindowRect(pProperyView->m_hWnd, ProperyViewRect);
	ProperypSwapChainDesc.BufferDesc.Width = ProperyViewRect.right - ProperyViewRect.left;
	ProperypSwapChainDesc.BufferDesc.Height = ProperyViewRect.bottom - ProperyViewRect.top;

	//백버퍼로 일반적인 32bit의 서페이스를 지정합니다.
	ProperypSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//후면 버퍼 픽셀 형식
																		// 백버퍼의 새로고침 비율을 설정합니다. 
	ProperypSwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	ProperypSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	// 백버퍼의 용도를 설정합니다. 
	ProperypSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// 렌더링이 이루어질 윈도우의 핸들을 설정합니다. 
	ProperypSwapChainDesc.OutputWindow = pProperyView->m_hWnd;

	// 멀티셈플링 유무 확인
	if (ENABLE_4X_MSAA)
	{
		ProperypSwapChainDesc.SampleDesc.Count = 4;
		ProperypSwapChainDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// 멀티셈플링 안씀
	else
	{
		ProperypSwapChainDesc.SampleDesc.Count = 1;
		ProperypSwapChainDesc.SampleDesc.Quality = 0;
	}

	ProperypSwapChainDesc.Windowed = true;

	// 스캔라인의 정렬과 스캔라이닝을 지정되지 않음으로(unspecified) 설정합니다. 
	ProperypSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //디스플레이 스캔라인 모드
	ProperypSwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; //디스플레이 비례 모드
																			 // 출력된 이후의 백버퍼의 내용을 버리도록 합니다. 
	ProperypSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	// 전체화면 전환시 후면버퍼 설정에 잘부합하는 디스플레이 모드 설정됨 
	ProperypSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//스왑체인 생성
	HR(pFactory->CreateSwapChain(m_pDevice, &ProperypSwapChainDesc, &m_pPropertySwapChain));

	//알트 + 앤더 먹지 않게 해줌
	HR(pFactory->MakeWindowAssociation(pProperyView->m_hWnd, DXGI_MWA_NO_WINDOW_CHANGES));
}

//------------------------------------------------------------------------------------
void CD3dClass::CreatePropertyRenderTargetView(const CRect  ViewRect)
{
	ID3D11Texture2D* backBuffer;

	//버퍼 크기 재설정
	HR(m_pPropertySwapChain->ResizeBuffers(1, ViewRect.right - ViewRect.left,
		ViewRect.bottom - ViewRect.top, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	HR(m_pPropertySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	//랜더 타겟 재설정
	HR(m_pDevice->CreateRenderTargetView(backBuffer, 0, &m_pPropertyRenderTargetView));
	RELEASE_COM(backBuffer);
}

//------------------------------------------------------------------------------------
void CD3dClass::CreatePropertyDepthStencilBuffer(const CRect  ViewRect)
{
	D3D11_TEXTURE2D_DESC PropertyDepthBufferDesc;

	//깊이 버퍼 설정
	ZeroMemory(&PropertyDepthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	PropertyDepthBufferDesc.Width = ViewRect.right - ViewRect.left;
	PropertyDepthBufferDesc.Height = ViewRect.bottom - ViewRect.top;
	PropertyDepthBufferDesc.MipLevels = 1;
	PropertyDepthBufferDesc.ArraySize = 1;
	PropertyDepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 멀티셈플링 유무
	if (ENABLE_4X_MSAA)
	{
		PropertyDepthBufferDesc.SampleDesc.Count = 4;
		PropertyDepthBufferDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// 멀티셈플링 안함
	else
	{
		PropertyDepthBufferDesc.SampleDesc.Count = 1;
		PropertyDepthBufferDesc.SampleDesc.Quality = 0;
	}

	PropertyDepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	PropertyDepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	PropertyDepthBufferDesc.CPUAccessFlags = 0;
	PropertyDepthBufferDesc.MiscFlags = 0;

	//깊이 버퍼 텍스처 생성
	HR(m_pDevice->CreateTexture2D(&PropertyDepthBufferDesc, 0, &m_pPropertyDepthStencilBuffer));
}

//------------------------------------------------------------------------------------
void CD3dClass::SetOMPropertyRenderTarget()
{
	// 깊이-스텐실 뷰의 description을 초기화
	D3D11_DEPTH_STENCIL_VIEW_DESC PropertyDepthStencilViewDesc;
	ZeroMemory(&PropertyDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	// 깊이-스텐실 뷰의 description을 작성합니다.
	PropertyDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	PropertyDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	PropertyDepthStencilViewDesc.Texture2D.MipSlice = 0;

	// 깊이-스텐실 뷰를 생성
	HR(m_pDevice->CreateDepthStencilView(m_pPropertyDepthStencilBuffer, &PropertyDepthStencilViewDesc, &m_pPropertyDepthStencilView));

	//Output Merger에 랜더타갯 설정(깊이/스텐실 뷰 포함)
	m_pDeviceContext->OMSetRenderTargets(1, &m_pPropertyRenderTargetView, m_pPropertyDepthStencilView);
}

//-----------------------------------------------------------------------------------
void CD3dClass::CreatePropertyViewPort(const CRect  ViewRect)
{
	//뷰포트 재설정
	m_PropertyScreenViewport.TopLeftX = 0.0f;
	m_PropertyScreenViewport.TopLeftY = 0.0f;
	m_PropertyScreenViewport.Width = static_cast<float>(ViewRect.right - ViewRect.left);
	m_PropertyScreenViewport.Height = static_cast<float>(ViewRect.bottom - ViewRect.top);
	m_PropertyScreenViewport.MinDepth = 0.0f;
	m_PropertyScreenViewport.MaxDepth = 1.0f;

	m_pDeviceContext->RSSetViewports(1, &m_PropertyScreenViewport);
}

//------------------------------------------------------------------------------------
void CD3dClass::CreateDepthStencilState()
{
	// 스텐실 상태의 description을 초기화합니다.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	// 스텐실 상태의 description을 작성합니다.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 깊이-스텐실 상태를 생성
	HR(m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState));
}

//------------------------------------------------------------------------------------
void CD3dClass::CreateDisableDepthStencilState()
{
	// 스텐실 상태의 description을 초기화합니다.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	// 스텐실 상태의 description을 작성합니다.
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 깊이-스텐실 상태를 생성
	HR(m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthDisableStencilState));
}

//------------------------------------------------------------------------------------
void CD3dClass::SetZBuffer()
{
	if (m_bIsZBufferOn)
	{
		// 깊이-스텐실 상태를 설정
		m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
	}
	else
	{
		// 깊이-스텐실 상태를 설정
		m_pDeviceContext->OMSetDepthStencilState(m_pDepthDisableStencilState, 1);
	}
}

//------------------------------------------------------------------------------------
void CD3dClass::SetRestersate()
{
	if (m_bSolid_WireFrame)
	{
		m_pDeviceContext->RSSetState(m_pRasterState_FillSolid);
	}
	else
	{
		m_pDeviceContext->RSSetState(m_pRasterState_WireFrame);
	}
}

//------------------------------------------------------------------------------------
void CD3dClass::CreateMainRenderTargetView(const CRect ViewRect)
{
	ID3D11Texture2D* backBuffer;

	//버퍼 크기 재설정
	HR(m_pMainSwapChain->ResizeBuffers(1, ViewRect.right - ViewRect.left,
		ViewRect.bottom - ViewRect.top, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	HR(m_pMainSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	//랜더 타겟 재설정
	HR(m_pDevice->CreateRenderTargetView(backBuffer, 0, &m_pMainRenderTargetView));
	RELEASE_COM(backBuffer);
}

//------------------------------------------------------------------------------------
void CD3dClass::CreateMainDepthStencilBuffer(const CRect mainViewRect)
{
	D3D11_TEXTURE2D_DESC mainDepthBufferDesc;

	//깊이 버퍼 설정
	ZeroMemory(&mainDepthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	mainDepthBufferDesc.Width = mainViewRect.right - mainViewRect.left;
	mainDepthBufferDesc.Height = mainViewRect.bottom - mainViewRect.top;
	mainDepthBufferDesc.MipLevels = 1;
	mainDepthBufferDesc.ArraySize = 1;
	mainDepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 멀티셈플링 유무
	if (ENABLE_4X_MSAA)
	{
		mainDepthBufferDesc.SampleDesc.Count = 4;
		mainDepthBufferDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// 멀티셈플링 안함
	else
	{
		mainDepthBufferDesc.SampleDesc.Count = 1;
		mainDepthBufferDesc.SampleDesc.Quality = 0;
	}

	mainDepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	mainDepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	mainDepthBufferDesc.CPUAccessFlags = 0;
	mainDepthBufferDesc.MiscFlags = 0;

	//깊이 버퍼 텍스처 생성
	HR(m_pDevice->CreateTexture2D(&mainDepthBufferDesc, 0, &m_pMainDepthStencilBuffer));
}

//------------------------------------------------------------------------------------
void CD3dClass::CreateMainViewPort(const CRect  mainViewRect)
{
	//뷰포트 재설정
	m_MainScreenViewport.TopLeftX = 0.0f;
	m_MainScreenViewport.TopLeftY = 0.0f;
	m_MainScreenViewport.Width = static_cast<float>(mainViewRect.right - mainViewRect.left);
	m_MainScreenViewport.Height = static_cast<float>(mainViewRect.bottom - mainViewRect.top);
	m_MainScreenViewport.MinDepth = 0.0f;
	m_MainScreenViewport.MaxDepth = 1.0f;

	m_pDeviceContext->RSSetViewports(1, &m_MainScreenViewport);
}

//------------------------------------------------------------------------------------
void CD3dClass::SetOMMainRenderTarget()
{
	// 깊이-스텐실 뷰의 description을 초기화
	D3D11_DEPTH_STENCIL_VIEW_DESC mainDepthStencilViewDesc;
	ZeroMemory(&mainDepthStencilViewDesc, sizeof(mainDepthStencilViewDesc));

	// 깊이-스텐실 뷰의 description을 작성합니다.
	mainDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	mainDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	mainDepthStencilViewDesc.Texture2D.MipSlice = 0;

	// 깊이-스텐실 뷰를 생성
	HR(m_pDevice->CreateDepthStencilView(m_pMainDepthStencilBuffer, &mainDepthStencilViewDesc, &m_pMainDepthStencilView));

	//Output Merger에 랜더타갯 설정(깊이/스텐실 뷰 포함)
	m_pDeviceContext->OMSetRenderTargets(1, &m_pMainRenderTargetView, m_pMainDepthStencilView);
}

//------------------------------------------------------------------------------------
void CD3dClass::OnResize(CMapTool2View * pMainView, CMiniMapView * pMiniMapView, CProperyView * pProperyView)
{
	if (!isInit)
		return;

	//이 함수에 쓰일 변수들 채크
	assert(m_pDeviceContext);
	assert(m_pDevice);
	assert(m_pMainSwapChain);
	assert(m_pMiniMapSwapChain);
	assert(m_pPropertySwapChain);

	//사용한 COM객체들 릴리즈
	RELEASE_COM(m_pDepthStencilState);
	RELEASE_COM(m_pDepthDisableStencilState);

	RELEASE_COM(m_pMainRenderTargetView);
	RELEASE_COM(m_pMainDepthStencilBuffer);
	RELEASE_COM(m_pMainDepthStencilView);
	
	RELEASE_COM(m_pMiniMapRenderTargetView);
	RELEASE_COM(m_pMiniMapDepthStencilBuffer);
	RELEASE_COM(m_pMiniMapDepthStencilView);

	RELEASE_COM(m_pPropertyRenderTargetView);
	RELEASE_COM(m_pPropertyDepthStencilBuffer);
	RELEASE_COM(m_pPropertyDepthStencilView);

	CRect mainViewRect;
	CRect miniMapViewRect;
	CRect PropertyViewRect;

	//----공통----
	//뎁스 스텐실 상태를 만듬
	CreateDepthStencilState();
	CreateDisableDepthStencilState();
	//Z버퍼 On/Off
	SetZBuffer();
	// 래스터화기 상태를 설정
	SetRestersate();
	
	//-----메인 뷰----
	//메인 뷰 창의 크기 받아옴
	GetWindowRect(pMainView->m_hWnd, mainViewRect);
	//메인 랜더 타겟 뷰 설정
	CreateMainRenderTargetView(mainViewRect);
	CreateMainDepthStencilBuffer(mainViewRect);
	SetOMMainRenderTarget();
	//메인 뷰포트 생성
	CreateMainViewPort(mainViewRect);

	//-----미니맵 뷰----
	//미니맵 뷰 창의 크기 받아옴
	GetWindowRect(pMiniMapView->m_hWnd, miniMapViewRect);
	//미니맵 랜더 타겟 뷰 설정
	CreateMiniRenderTargetView(miniMapViewRect);
	CreateMiniDepthStencilBuffer(miniMapViewRect);
	SetOMMiniRenderTarget();
	//미니맵 뷰포트 생성
	CreateMiniViewPort(miniMapViewRect);

	//-----속성 뷰----
	//속성 뷰 창의 크기 받아옴
	GetWindowRect(pProperyView->m_hWnd, PropertyViewRect);
	//속성 랜더 타겟 뷰 설정
	CreatePropertyRenderTargetView(PropertyViewRect);
	CreatePropertyDepthStencilBuffer(PropertyViewRect);
	SetOMPropertyRenderTarget();
	//속성 뷰포트 생성
	CreatePropertyViewPort(PropertyViewRect);
}

void CD3dClass::EnableSecondBlendState()
{
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	m_pDeviceContext->OMSetBlendState(m_alphaBlendState2, blendFactor, 0xffffffff);

	return;
}