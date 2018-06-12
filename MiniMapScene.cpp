#include "stdafx.h"
#include "MiniMapScene.h"
#include "D3dClass.h"

MiniMapScene::MiniMapScene()
{
	m_TextureShader = 0;
	m_MiniMap = 0;
	m_Terrain = 0;
}

MiniMapScene::~MiniMapScene()
{
}

bool MiniMapScene::Initialize(HINSTANCE hinstance, HWND hwnd)
{
	bool result;
	int terrainWidth, terrainHeight;
	//D3DXMATRIX baseViewMatrix;

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(CD3dClass::GetInstance()->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}
	
	// Get the size of the terrain as the minimap will require this information.
	m_Terrain->GetTerrainSize(terrainWidth, terrainHeight);

	// Create the mini map object.
	m_MiniMap = new MiniMapClass;
	if (!m_MiniMap)
	{
		return false;
	}

	CRect rect;
	GetWindowRect(hwnd, &rect);
	// Initialize the mini map object.
	//m_Camera->Render();
	//m_Camera->GetViewMatrix(baseViewMatrix);
	result = m_MiniMap->Initialize(CD3dClass::GetInstance()->GetDevice(), hwnd, 
		rect.Width(), rect.Height(), m_Camera->GetBaseMat(), (float)(terrainWidth - 1),
		(float)(terrainHeight - 1));
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the mini map object.", L"Error", MB_OK);
		return false;
	}
	return true;
}

void MiniMapScene::Shutdown()
{
	// Release the mini map object.
	if (m_MiniMap)
	{
		m_MiniMap->Shutdown();
		delete m_MiniMap;
		m_MiniMap = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
}

bool MiniMapScene::HandleInput()
{
	D3DXVECTOR3 pos;

	// Get the view point position/rotation.
	pos = m_Camera->GetPosition();

	// Update the location of the camera on the mini map.
	m_MiniMap->PositionUpdate(pos.x, pos.z);

	return true;
}

bool MiniMapScene::RenderGraphics()
{
	bool result;

	//m_Camera->Render();
	// 카메라 및 Direct3D 객체에서 월드, 뷰, 투영 및 ortho 행렬을 가져옵니다.
	D3DXMatrixIdentity(&m_worldMatrix);
	//m_Camera->GetViewMatrix(m_viewMatrix);

	// Setup the projection matrix.  
	CRect rect;
	GetWindowRect(CD3dClass::GetInstance()->GetMiniView()->m_hWnd, &rect);
	float fieldOfView = (float)D3DX_PI / 4.0f;
	float screenAspect = (float)rect.Width() / (float)rect.Height();
	// Create the projection matrix for 3D rendering.  
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);

	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)rect.Width(), (float)rect.Height(), SCREEN_NEAR, SCREEN_DEPTH);

	// Render the mini map.
	result = m_MiniMap->Render(CD3dClass::GetInstance()->GetDeviceContext(), 
		m_worldMatrix, m_orthoMatrix, m_TextureShader);
	if (!result)
	{
		return false;
	}
}

bool MiniMapScene::Frame()
{
	// 프레임 입력 처리를 수행합니다.
	bool result = HandleInput();
	if (!result)
	{
		return false;
	}

	// 그래픽을 렌더링 합니다.
	result = RenderGraphics();
	if (!result)
	{
		return false;
	}

	return result;
}
