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
	m_fbxLoader = new FBXLoader();
	IDXGIFactory* pFactory = nullptr;
	D3D_FEATURE_LEVEL featureLevel;//FeatureLevel ����(�ְ� ������ Level �ޱ�����)
	UINT createDeviceFlags = 0; // ����̽� �÷��� ����(Default)

	m_pMainView = pMainView;
	m_pMiniMapView = pMiniMapView;
	m_pProperyView = pProperyView;

	// DirectX �׷��� �������̽� ���丮�� ����
	HR(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory));

	// ���丮 ��ü�� ����Ͽ� ù��° �׷��� ī�� �������̽� ��͸� �����մϴ�
	IDXGIAdapter* adapter = nullptr;
	if (FAILED(pFactory->EnumAdapters(0, &adapter)))
	{
		return false;
	}

	// ���(�����)�� ���� ù��° ��͸� �����մϴ�.
	IDXGIOutput* adapterOutput = nullptr;
	if (FAILED(adapter->EnumOutputs(0, &adapterOutput)))
	{
		return false;
	}

	// ��� (�����)�� ���� DXGI_FORMAT_R8G8B8A8_UNORM ǥ�� ���Ŀ� �´� ��� ���� �����ɴϴ�
	unsigned int numModes = 0;
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL)))
	{
		return false;
	}

	// ������ ��� ����Ϳ� �׷���ī�� ������ ������ ����Ʈ�� �����մϴ�
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// ���� ���÷��� ��忡 ���� ����Ʈ�� ä��ϴ�
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList)))
	{
		return false;
	}

	// ���� ��� ���÷��� ��忡 ���� ȭ�� �ʺ�/���̿� �´� ���÷��� ��带 ã���ϴ�.
	// ������ ���� ã���� ������� ���ΰ�ħ ������ �и�� ���� ���� �����մϴ�.
	unsigned int numerator = 0;
	unsigned int denominator = 0;
	CRect rect;
	GetWindowRect(pMainView->m_hWnd, &rect);
	for (unsigned int i = 0; i < numModes; i++)
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

	// ����ī���� ����ü�� ����ϴ�
	DXGI_ADAPTER_DESC adapterDesc;
	if (FAILED(adapter->GetDesc(&adapterDesc)))
	{
		return false;
	}

	// ����ī�� �޸� �뷮 ������ �ް�����Ʈ ������ �����մϴ�
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// ����ī���� �̸��� �����մϴ�
	size_t stringLength = 0;
	if (wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128) != 0)
	{
		return false;
	}

	// ���÷��� ��� ����Ʈ�� �����մϴ�
	delete[] displayModeList;
	displayModeList = 0;

	// ��� ��͸� �����մϴ�
	adapterOutput->Release();
	adapterOutput = 0;

	// ��͸� �����մϴ�
	adapter->Release();
	adapter = 0;

#if defined(DEBUG) || defined(_DEBUG)
	//������ ����� �������
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//����̽�, ����̽� ���ý�Ʈ ����
	HR(D3D11CreateDevice(
		0,                 // default adapter(�⺻ ���÷��� ����� ����)
		D3D_DRIVER_TYPE_HARDWARE,	   // 3���� �׷��� ���� ����
		0,                 // no software device
		createDeviceFlags, // ����̽� �÷��� ����
		0, 0,              // default feature level array
		D3D11_SDK_VERSION, // �׻� D3D11_SDK_VERSION ����
		&m_pDevice,	   // d3d��ġ �־��� (Out ��)
		&featureLevel,	   // featureLevel �־���[�� ��밡�� �ְ� ������ Level ����] (OUt ��)
		&m_pDeviceContext)); // ��ġ ����(Context) �־��� (Out ��)

	//FeatureLevel�� ���� �˻�
	if (featureLevel < D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 ���� ����.", 0, 0);
		return false;
	}

	//�˸��� ��Ƽ���ø� ����Ƽ ����
	HR(m_pDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality));
	//�ùٸ� �������� äũ
	assert(m_4xMsaaQuality > 0);

	//����ü�� ����
	CreateMainSwapChain(pMainView, pFactory);
	CreateMiniMapSwapChain(pMiniMapView, pFactory);
	CreatePropertySwapChain(pProperyView, pFactory);

	//���丮 �Ҵ� ����
	RELEASE_COM(pFactory);

	//�����Ͷ����� ������Ʈ ����
	CreateRasterizerStates();

	// ����� ���� ����ü�� �ʱ�ȭ �մϴ�.
	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// ���� ���� ����� ���� ����ü�� �����մϴ�.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// ������ ����Ͽ� ȥ�� ���¸� ����ϴ�.
	if (FAILED(m_pDevice->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState)))
	{
		return false;
	}

	// ���ĺ���� ������ �����մϴ�.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// ���ĺ���� ���°��� �����մϴ�.
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

	//â ������ �缳��
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

	// ������ color�� ������
	m_pDeviceContext->ClearRenderTargetView(m_pMainRenderTargetView, color);
	// ���̹��۵� ������
	m_pDeviceContext->ClearDepthStencilView(m_pMainDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

//------------------------------------------------------------------------------------
void CD3dClass::EndMainScene()
{
	// �����ۿ� �ø� �׸� ȭ������ ����
	// ������ ���� �׷���
	HR(m_pMainSwapChain->Present(0, 0));
}

//------------------------------------------------------------------------------------
void CD3dClass::BeginMiniMapScene(const float & red, const float & green, const float & blue, const float & alpha)
{
	D3DXCOLOR color = { red, green, blue, alpha };

	m_pDeviceContext->OMSetRenderTargets(1, &m_pMiniMapRenderTargetView, m_pMiniMapDepthStencilView);
	m_pDeviceContext->RSSetViewports(1, &m_MiniMapScreenViewport);

	// ������ color�� ������
	m_pDeviceContext->ClearRenderTargetView(m_pMiniMapRenderTargetView, color);
	// ���̹��۵� ������
	m_pDeviceContext->ClearDepthStencilView(m_pMiniMapDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

//------------------------------------------------------------------------------------
void CD3dClass::EndMiniMapScene()
{
	// �����ۿ� �ø� �׸� ȭ������ ����
	// �̴ϸ��� 60FPS�� �׷���
	HR(m_pMiniMapSwapChain->Present(0, 0));
}

//------------------------------------------------------------------------------------
void CD3dClass::BeginPropertyScene(const float & red, const float & green, const float & blue, const float & alpha)
{
	D3DXCOLOR color = { red, green, blue, alpha };

	m_pDeviceContext->OMSetRenderTargets(1, &m_pPropertyRenderTargetView, m_pPropertyDepthStencilView);
	m_pDeviceContext->RSSetViewports(1, &m_PropertyScreenViewport);

	// ������ color�� ������
	m_pDeviceContext->ClearRenderTargetView(m_pPropertyRenderTargetView, color);
	// ���̹��۵� ������
	m_pDeviceContext->ClearDepthStencilView(m_pPropertyDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

//------------------------------------------------------------------------------------
void CD3dClass::EndPropertyScene()
{
	// �����ۿ� �ø� �׸� ȭ������ ����
	// �Ӽ���� 60FPS�� �׷���
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
	// ȥ�� ��Ҹ� �����մϴ�.
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// ���� ������� �մϴ�.
	m_pDeviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);
}

void CD3dClass::TurnOffAlphaBlending()
{
	// ȥ�� ��Ҹ� �����մϴ�.
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// ���� ������� �մϴ�.
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
	//�����Ͷ����� Desc ����
	D3D11_RASTERIZER_DESC rasterDesc;

	//�ָ��� ���� ����
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

	// �ۼ��� description���κ��� ������ȭ�� ���¸� ����
	HR(m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState_FillSolid));

	//���̾� ������ ���� ����
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	//�ʱ�ȭ
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

	// �ۼ��� description���κ��� ������ȭ�� ���¸� ����
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
	if (m_bSolid_WireFrame)
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

	//��ȯ �罽 description�� �ʱ�ȭ�մϴ�.
	//���κ��� ������ ����
	ZeroMemory(&MainSwapChainDesc, sizeof(MainSwapChainDesc));

	//�ϳ��� ����۸��� ����ϵ��� �մϴ�.
	MainSwapChainDesc.BufferCount = 1;

	//������� �ʺ�� ���̸� �����մϴ�.
	GetWindowRect(pMainView->m_hWnd, mainViewRect);
	MainSwapChainDesc.BufferDesc.Width = mainViewRect.right - mainViewRect.left;
	MainSwapChainDesc.BufferDesc.Height = mainViewRect.bottom - mainViewRect.top;

	//����۷� �Ϲ����� 32bit�� �����̽��� �����մϴ�.
	MainSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//�ĸ� ���� �ȼ� ����
																	 // ������� ���ΰ�ħ ������ �����մϴ�.
	MainSwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	MainSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	// ������� �뵵�� �����մϴ�.
	MainSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// �������� �̷���� �������� �ڵ��� �����մϴ�.
	MainSwapChainDesc.OutputWindow = pMainView->m_hWnd;

	// ��Ƽ���ø� ���� Ȯ��
	if (ENABLE_4X_MSAA)
	{
		MainSwapChainDesc.SampleDesc.Count = 4;
		MainSwapChainDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// ��Ƽ���ø� �Ⱦ�
	else
	{
		MainSwapChainDesc.SampleDesc.Count = 1;
		MainSwapChainDesc.SampleDesc.Quality = 0;
	}

	MainSwapChainDesc.Windowed = true;

	// ��ĵ������ ���İ� ��ĵ���̴��� �������� ��������(unspecified) �����մϴ�.
	MainSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //���÷��� ��ĵ���� ���
	MainSwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; //���÷��� ��� ���
																		  // ��µ� ������ ������� ������ �������� �մϴ�.
	MainSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	// ��üȭ�� ��ȯ�� �ĸ���� ������ �ߺ����ϴ� ���÷��� ��� ������
	MainSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//����ü�� ����
	HR(pFactory->CreateSwapChain(m_pDevice, &MainSwapChainDesc, &m_pMainSwapChain));

	//��Ʈ + �ش� ���� �ʰ� ����
	HR(pFactory->MakeWindowAssociation(pMainView->m_hWnd, DXGI_MWA_NO_WINDOW_CHANGES));
}

//------------------------------------------------------------------------------------
void CD3dClass::CreateMiniMapSwapChain(CMiniMapView * pMiniMapView, IDXGIFactory * pFactory)
{
	DXGI_SWAP_CHAIN_DESC MiniMapSwapChainDesc;
	CRect MiniMapViewRect;

	//��ȯ �罽 description�� �ʱ�ȭ�մϴ�.
	//���κ��� ������ ����
	ZeroMemory(&MiniMapSwapChainDesc, sizeof(MiniMapSwapChainDesc));

	//�ϳ��� ����۸��� ����ϵ��� �մϴ�.
	MiniMapSwapChainDesc.BufferCount = 1;

	//������� �ʺ�� ���̸� �����մϴ�.
	GetWindowRect(pMiniMapView->m_hWnd, MiniMapViewRect);
	MiniMapSwapChainDesc.BufferDesc.Width = MiniMapViewRect.right - MiniMapViewRect.left;
	MiniMapSwapChainDesc.BufferDesc.Height = MiniMapViewRect.bottom - MiniMapViewRect.top;

	//����۷� �Ϲ����� 32bit�� �����̽��� �����մϴ�.
	MiniMapSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//�ĸ� ���� �ȼ� ����
																	 // ������� ���ΰ�ħ ������ �����մϴ�.
	MiniMapSwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	MiniMapSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	// ������� �뵵�� �����մϴ�.
	MiniMapSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// �������� �̷���� �������� �ڵ��� �����մϴ�.
	MiniMapSwapChainDesc.OutputWindow = pMiniMapView->m_hWnd;

	// ��Ƽ���ø� ���� Ȯ��
	if (ENABLE_4X_MSAA)
	{
		MiniMapSwapChainDesc.SampleDesc.Count = 4;
		MiniMapSwapChainDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// ��Ƽ���ø� �Ⱦ�
	else
	{
		MiniMapSwapChainDesc.SampleDesc.Count = 1;
		MiniMapSwapChainDesc.SampleDesc.Quality = 0;
	}

	MiniMapSwapChainDesc.Windowed = true;

	// ��ĵ������ ���İ� ��ĵ���̴��� �������� ��������(unspecified) �����մϴ�.
	MiniMapSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //���÷��� ��ĵ���� ���
	MiniMapSwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; //���÷��� ��� ���
																		  // ��µ� ������ ������� ������ �������� �մϴ�.
	MiniMapSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	// ��üȭ�� ��ȯ�� �ĸ���� ������ �ߺ����ϴ� ���÷��� ��� ������
	MiniMapSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//����ü�� ����
	HR(pFactory->CreateSwapChain(m_pDevice, &MiniMapSwapChainDesc, &m_pMiniMapSwapChain));

	//��Ʈ + �ش� ���� �ʰ� ����
	HR(pFactory->MakeWindowAssociation(pMiniMapView->m_hWnd, DXGI_MWA_NO_WINDOW_CHANGES));
}

//------------------------------------------------------------------------------------
void CD3dClass::CreateMiniRenderTargetView(const CRect  ViewRect)
{
	ID3D11Texture2D* backBuffer;

	//���� ũ�� �缳��
	HR(m_pMiniMapSwapChain->ResizeBuffers(1, ViewRect.right - ViewRect.left,
		ViewRect.bottom - ViewRect.top, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	HR(m_pMiniMapSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	//���� Ÿ�� �缳��
	HR(m_pDevice->CreateRenderTargetView(backBuffer, 0, &m_pMiniMapRenderTargetView));
	RELEASE_COM(backBuffer);
}

//------------------------------------------------------------------------------------
void CD3dClass::CreateMiniDepthStencilBuffer(const CRect  ViewRect)
{
	D3D11_TEXTURE2D_DESC miniDepthBufferDesc;

	//���� ���� ����
	ZeroMemory(&miniDepthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	miniDepthBufferDesc.Width = ViewRect.right - ViewRect.left;
	miniDepthBufferDesc.Height = ViewRect.bottom - ViewRect.top;
	miniDepthBufferDesc.MipLevels = 1;
	miniDepthBufferDesc.ArraySize = 1;
	miniDepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// ��Ƽ���ø� ����
	if (ENABLE_4X_MSAA)
	{
		miniDepthBufferDesc.SampleDesc.Count = 4;
		miniDepthBufferDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// ��Ƽ���ø� ����
	else
	{
		miniDepthBufferDesc.SampleDesc.Count = 1;
		miniDepthBufferDesc.SampleDesc.Quality = 0;
	}

	miniDepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	miniDepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	miniDepthBufferDesc.CPUAccessFlags = 0;
	miniDepthBufferDesc.MiscFlags = 0;

	//���� ���� �ؽ�ó ����
	HR(m_pDevice->CreateTexture2D(&miniDepthBufferDesc, 0, &m_pMiniMapDepthStencilBuffer));
}

//------------------------------------------------------------------------------------
void CD3dClass::SetOMMiniRenderTarget()
{
	// ����-���ٽ� ���� description�� �ʱ�ȭ
	D3D11_DEPTH_STENCIL_VIEW_DESC miniDepthStencilViewDesc;
	ZeroMemory(&miniDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	// ����-���ٽ� ���� description�� �ۼ��մϴ�.
	miniDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	miniDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	miniDepthStencilViewDesc.Texture2D.MipSlice = 0;

	// ����-���ٽ� �並 ����
	HR(m_pDevice->CreateDepthStencilView(m_pMiniMapDepthStencilBuffer, &miniDepthStencilViewDesc, &m_pMiniMapDepthStencilView));

	//Output Merger�� ����Ÿ�� ����(����/���ٽ� �� ����)
	m_pDeviceContext->OMSetRenderTargets(1, &m_pMiniMapRenderTargetView, m_pMiniMapDepthStencilView);
}

//------------------------------------------------------------------------------------
void CD3dClass::CreateMiniViewPort(const CRect  ViewRect)
{
	//����Ʈ �缳��
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

	//��ȯ �罽 description�� �ʱ�ȭ�մϴ�.
	//���κ��� ������ ����
	ZeroMemory(&ProperypSwapChainDesc, sizeof(ProperypSwapChainDesc));

	//�ϳ��� ����۸��� ����ϵ��� �մϴ�.
	ProperypSwapChainDesc.BufferCount = 1;

	//������� �ʺ�� ���̸� �����մϴ�.
	GetWindowRect(pProperyView->m_hWnd, ProperyViewRect);
	ProperypSwapChainDesc.BufferDesc.Width = ProperyViewRect.right - ProperyViewRect.left;
	ProperypSwapChainDesc.BufferDesc.Height = ProperyViewRect.bottom - ProperyViewRect.top;

	//����۷� �Ϲ����� 32bit�� �����̽��� �����մϴ�.
	ProperypSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//�ĸ� ���� �ȼ� ����
																		// ������� ���ΰ�ħ ������ �����մϴ�.
	ProperypSwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	ProperypSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	// ������� �뵵�� �����մϴ�.
	ProperypSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// �������� �̷���� �������� �ڵ��� �����մϴ�.
	ProperypSwapChainDesc.OutputWindow = pProperyView->m_hWnd;

	// ��Ƽ���ø� ���� Ȯ��
	if (ENABLE_4X_MSAA)
	{
		ProperypSwapChainDesc.SampleDesc.Count = 4;
		ProperypSwapChainDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// ��Ƽ���ø� �Ⱦ�
	else
	{
		ProperypSwapChainDesc.SampleDesc.Count = 1;
		ProperypSwapChainDesc.SampleDesc.Quality = 0;
	}

	ProperypSwapChainDesc.Windowed = true;

	// ��ĵ������ ���İ� ��ĵ���̴��� �������� ��������(unspecified) �����մϴ�.
	ProperypSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //���÷��� ��ĵ���� ���
	ProperypSwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; //���÷��� ��� ���
																			 // ��µ� ������ ������� ������ �������� �մϴ�.
	ProperypSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	// ��üȭ�� ��ȯ�� �ĸ���� ������ �ߺ����ϴ� ���÷��� ��� ������
	ProperypSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//����ü�� ����
	HR(pFactory->CreateSwapChain(m_pDevice, &ProperypSwapChainDesc, &m_pPropertySwapChain));

	//��Ʈ + �ش� ���� �ʰ� ����
	HR(pFactory->MakeWindowAssociation(pProperyView->m_hWnd, DXGI_MWA_NO_WINDOW_CHANGES));
}

//------------------------------------------------------------------------------------
void CD3dClass::CreatePropertyRenderTargetView(const CRect  ViewRect)
{
	ID3D11Texture2D* backBuffer;

	//���� ũ�� �缳��
	HR(m_pPropertySwapChain->ResizeBuffers(1, ViewRect.right - ViewRect.left,
		ViewRect.bottom - ViewRect.top, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	HR(m_pPropertySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	//���� Ÿ�� �缳��
	HR(m_pDevice->CreateRenderTargetView(backBuffer, 0, &m_pPropertyRenderTargetView));
	RELEASE_COM(backBuffer);
}

//------------------------------------------------------------------------------------
void CD3dClass::CreatePropertyDepthStencilBuffer(const CRect  ViewRect)
{
	D3D11_TEXTURE2D_DESC PropertyDepthBufferDesc;

	//���� ���� ����
	ZeroMemory(&PropertyDepthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	PropertyDepthBufferDesc.Width = ViewRect.right - ViewRect.left;
	PropertyDepthBufferDesc.Height = ViewRect.bottom - ViewRect.top;
	PropertyDepthBufferDesc.MipLevels = 1;
	PropertyDepthBufferDesc.ArraySize = 1;
	PropertyDepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// ��Ƽ���ø� ����
	if (ENABLE_4X_MSAA)
	{
		PropertyDepthBufferDesc.SampleDesc.Count = 4;
		PropertyDepthBufferDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// ��Ƽ���ø� ����
	else
	{
		PropertyDepthBufferDesc.SampleDesc.Count = 1;
		PropertyDepthBufferDesc.SampleDesc.Quality = 0;
	}

	PropertyDepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	PropertyDepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	PropertyDepthBufferDesc.CPUAccessFlags = 0;
	PropertyDepthBufferDesc.MiscFlags = 0;

	//���� ���� �ؽ�ó ����
	HR(m_pDevice->CreateTexture2D(&PropertyDepthBufferDesc, 0, &m_pPropertyDepthStencilBuffer));
}

//------------------------------------------------------------------------------------
void CD3dClass::SetOMPropertyRenderTarget()
{
	// ����-���ٽ� ���� description�� �ʱ�ȭ
	D3D11_DEPTH_STENCIL_VIEW_DESC PropertyDepthStencilViewDesc;
	ZeroMemory(&PropertyDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	// ����-���ٽ� ���� description�� �ۼ��մϴ�.
	PropertyDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	PropertyDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	PropertyDepthStencilViewDesc.Texture2D.MipSlice = 0;

	// ����-���ٽ� �並 ����
	HR(m_pDevice->CreateDepthStencilView(m_pPropertyDepthStencilBuffer, &PropertyDepthStencilViewDesc, &m_pPropertyDepthStencilView));

	//Output Merger�� ����Ÿ�� ����(����/���ٽ� �� ����)
	m_pDeviceContext->OMSetRenderTargets(1, &m_pPropertyRenderTargetView, m_pPropertyDepthStencilView);
}

//-----------------------------------------------------------------------------------
void CD3dClass::CreatePropertyViewPort(const CRect  ViewRect)
{
	//����Ʈ �缳��
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
	// ���ٽ� ������ description�� �ʱ�ȭ�մϴ�.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	// ���ٽ� ������ description�� �ۼ��մϴ�.
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

	// ����-���ٽ� ���¸� ����
	HR(m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState));
}

//------------------------------------------------------------------------------------
void CD3dClass::CreateDisableDepthStencilState()
{
	// ���ٽ� ������ description�� �ʱ�ȭ�մϴ�.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	// ���ٽ� ������ description�� �ۼ��մϴ�.
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

	// ����-���ٽ� ���¸� ����
	HR(m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthDisableStencilState));
}

//------------------------------------------------------------------------------------
void CD3dClass::SetZBuffer()
{
	if (m_bIsZBufferOn)
	{
		// ����-���ٽ� ���¸� ����
		m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
	}
	else
	{
		// ����-���ٽ� ���¸� ����
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

	//���� ũ�� �缳��
	HR(m_pMainSwapChain->ResizeBuffers(1, ViewRect.right - ViewRect.left,
		ViewRect.bottom - ViewRect.top, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	HR(m_pMainSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	//���� Ÿ�� �缳��
	HR(m_pDevice->CreateRenderTargetView(backBuffer, 0, &m_pMainRenderTargetView));
	RELEASE_COM(backBuffer);
}

//------------------------------------------------------------------------------------
void CD3dClass::CreateMainDepthStencilBuffer(const CRect mainViewRect)
{
	D3D11_TEXTURE2D_DESC mainDepthBufferDesc;

	//���� ���� ����
	ZeroMemory(&mainDepthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

	mainDepthBufferDesc.Width = mainViewRect.right - mainViewRect.left;
	mainDepthBufferDesc.Height = mainViewRect.bottom - mainViewRect.top;
	mainDepthBufferDesc.MipLevels = 1;
	mainDepthBufferDesc.ArraySize = 1;
	mainDepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// ��Ƽ���ø� ����
	if (ENABLE_4X_MSAA)
	{
		mainDepthBufferDesc.SampleDesc.Count = 4;
		mainDepthBufferDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// ��Ƽ���ø� ����
	else
	{
		mainDepthBufferDesc.SampleDesc.Count = 1;
		mainDepthBufferDesc.SampleDesc.Quality = 0;
	}

	mainDepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	mainDepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	mainDepthBufferDesc.CPUAccessFlags = 0;
	mainDepthBufferDesc.MiscFlags = 0;

	//���� ���� �ؽ�ó ����
	HR(m_pDevice->CreateTexture2D(&mainDepthBufferDesc, 0, &m_pMainDepthStencilBuffer));
}

//------------------------------------------------------------------------------------
void CD3dClass::CreateMainViewPort(const CRect  mainViewRect)
{
	//����Ʈ �缳��
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
	// ����-���ٽ� ���� description�� �ʱ�ȭ
	D3D11_DEPTH_STENCIL_VIEW_DESC mainDepthStencilViewDesc;
	ZeroMemory(&mainDepthStencilViewDesc, sizeof(mainDepthStencilViewDesc));

	// ����-���ٽ� ���� description�� �ۼ��մϴ�.
	mainDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	mainDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	mainDepthStencilViewDesc.Texture2D.MipSlice = 0;

	// ����-���ٽ� �並 ����
	HR(m_pDevice->CreateDepthStencilView(m_pMainDepthStencilBuffer, &mainDepthStencilViewDesc, &m_pMainDepthStencilView));

	//Output Merger�� ����Ÿ�� ����(����/���ٽ� �� ����)
	m_pDeviceContext->OMSetRenderTargets(1, &m_pMainRenderTargetView, m_pMainDepthStencilView);
}

//------------------------------------------------------------------------------------
void CD3dClass::OnResize(CMapTool2View * pMainView, CMiniMapView * pMiniMapView, CProperyView * pProperyView)
{
	if (!isInit)
		return;

	//�� �Լ��� ���� ������ äũ
	assert(m_pDeviceContext);
	assert(m_pDevice);
	assert(m_pMainSwapChain);
	assert(m_pMiniMapSwapChain);
	assert(m_pPropertySwapChain);

	//����� COM��ü�� ������
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

	//----����----
	//���� ���ٽ� ���¸� ����
	CreateDepthStencilState();
	CreateDisableDepthStencilState();
	//Z���� On/Off
	SetZBuffer();
	// ������ȭ�� ���¸� ����
	SetRestersate();

	//-----���� ��----
	//���� �� â�� ũ�� �޾ƿ�
	GetWindowRect(pMainView->m_hWnd, mainViewRect);
	//���� ���� Ÿ�� �� ����
	CreateMainRenderTargetView(mainViewRect);
	CreateMainDepthStencilBuffer(mainViewRect);
	SetOMMainRenderTarget();
	//���� ����Ʈ ����
	CreateMainViewPort(mainViewRect);

	//-----�̴ϸ� ��----
	//�̴ϸ� �� â�� ũ�� �޾ƿ�
	GetWindowRect(pMiniMapView->m_hWnd, miniMapViewRect);
	//�̴ϸ� ���� Ÿ�� �� ����
	CreateMiniRenderTargetView(miniMapViewRect);
	CreateMiniDepthStencilBuffer(miniMapViewRect);
	SetOMMiniRenderTarget();
	//�̴ϸ� ����Ʈ ����
	CreateMiniViewPort(miniMapViewRect);

	//-----�Ӽ� ��----
	//�Ӽ� �� â�� ũ�� �޾ƿ�
	GetWindowRect(pProperyView->m_hWnd, PropertyViewRect);
	//�Ӽ� ���� Ÿ�� �� ����
	CreatePropertyRenderTargetView(PropertyViewRect);
	CreatePropertyDepthStencilBuffer(PropertyViewRect);
	SetOMPropertyRenderTarget();
	//�Ӽ� ����Ʈ ����
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