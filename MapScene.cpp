#include "stdafx.h"
#include "MapScene.h"
#include "InputClass.h"
#include "ModelClass.h"

MapScene::MapScene()
{
}

MapScene::~MapScene()
{
}

bool MapScene::Initialize(HINSTANCE hinstance, HWND hwnd)
{
	// 카메라 객체를 생성합니다.
	m_Camera = new CameraClass();
	if (!m_Camera)
	{
		return false;
	}

	// 2D 사용자 인터페이스 렌더링을 위해 카메라로 기본 뷰 행렬을 초기화합니다.
	D3DXMATRIX baseViewMatrix;
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);
	m_Camera->SetBaseMat(baseViewMatrix);

	// 카메라의 초기 위치를 설정합니다.
	m_Camera->SetPosition(50.0f, 10.0f, -7.0f);

	// 지형 객체를 생성합니다.
	m_Terrain = new TerrainClass;
	if (!m_Terrain)
	{
		return false;
	}

	// 지형 객체를 초기화 합니다.
	
	bool result = m_Terrain->Initialize(CD3dClass::GetInstance()->GetDevice(),
		"../MapTool2/data/heightmap01.bmp", L"../MapTool2/texture/dirt01.dds",
		"../MapTool2/data/colorm01.bmp");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// 색상 쉐이더 객체를 생성합니다.
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	// 색상 쉐이더 객체를 초기화합니다.
	result = m_ColorShader->Initialize(CD3dClass::GetInstance()->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	// 타이머 객체를 생성합니다.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	// 타이머 객체를 초기화 합니다.
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// 위치 개체를 생성합니다.
	m_Position = new PositionClass;
	if (!m_Position)
	{
		return false;
	}

	// 뷰어의 초기 위치를 초기 카메라 위치와 동일하게 설정합니다.
	m_Position->SetPosition(50.0f, 10.0f, -7.0f);
	m_Position->SetRotation(45.0f, 0.0f, 0.0f);
	m_Camera->SetRotation(m_Position->GetRotation());
	// fps 객체를 생성합니다.
	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}

	// fps 객체를 초기화합니다.
	m_Fps->Initialize();

	// cpu 객체를 생성합니다.
	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}

	// cpu 객체를 초기화 합니다.
	m_Cpu->Initialize();

	// 폰트 셰이더 객체를 생성합니다.
	m_FontShader = new FontShaderClass;
	if (!m_FontShader)
	{
		return false;
	}

	// 폰트 셰이더 객체를 초기화합니다.
	result = m_FontShader->Initialize(CD3dClass::GetInstance()->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// 텍스트 객체를 생성합니다.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// 텍스트 객체를 초기화합니다.
	CRect rect;
	GetWindowRect(CD3dClass::GetInstance()->GetMainView()->m_hWnd, &rect);
	result = m_Text->Initialize(CD3dClass::GetInstance()->GetDevice(), 
		CD3dClass::GetInstance()->GetDeviceContext(), hwnd, 
		rect.Width(), rect.Height(), baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// 비디오 카드 정보를 가져옵니다.
	char videoCard[128] = { 0, };
	int videoMemory = 0;
	CD3dClass::GetInstance()->GetVideoCardInfo(videoCard, videoMemory);

	// 텍스트 객체에 비디오 카드 정보를 설정합니다.
	result = m_Text->SetVideoCardInfo(videoCard, videoMemory, 
		CD3dClass::GetInstance()->GetDeviceContext());
	if (!result)
	{
		MessageBox(hwnd, L"Could not set video card info in the text object.", L"Error", MB_OK);
		return false;
	}

	//지형 셰이더 초기화
	m_TerrainShader = new  TerrainShaderClass;
	if (!m_TerrainShader)
	{
		return false;
	}

	result = m_TerrainShader->Initialize(CD3dClass::GetInstance()->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize m_TerrainShader.", L"Error", MB_OK);
		return false;
	}

	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	m_Light->SetAmbientColor(D3DXVECTOR4(0.5, 0.5, 0.5, 0.5));
	m_Light->SetDiffuseColor(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	m_Light->SetDirection(D3DXVECTOR3(0.0f, 0.0f, 0.75f));

	// Create the frustum object.
	m_Frustum = new FrustumClass;
	if (!m_Frustum)
	{
		return false;
	}

	// Create the quad tree object.
	m_QuadTree = new QuadTreeClass;
	if (!m_QuadTree)
	{
		return false;
	}

	// Initialize the quad tree object.
	result = m_QuadTree->Initialize(m_Terrain, CD3dClass::GetInstance()->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the quad tree object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky dome object.
	m_SkyDome = new SkyDomeClass;
	if (!m_SkyDome)
	{
		return false;
	}

	// Initialize the sky dome object.
	result = m_SkyDome->Initialize(CD3dClass::GetInstance()->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky dome shader object.
	m_SkyDomeShader = new SkyDomeShaderClass;
	if (!m_SkyDomeShader)
	{
		return false;
	}

	// Initialize the sky dome shader object.
	result = m_SkyDomeShader->Initialize(CD3dClass::GetInstance()->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky plane object.
	m_SkyPlane = new SkyPlaneClass;
	if (!m_SkyPlane)
	{
		return false;
	}

	// Initialize the sky plane object.
	result = m_SkyPlane->Initialize(CD3dClass::GetInstance()->GetDevice(),
		L"../MapTool2/data/cloud001.dds", L"../MapTool2/data/perturb001.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky plane object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky plane shader object.
	m_SkyPlaneShader = new SkyPlaneShaderClass;
	if (!m_SkyPlaneShader)
	{
		return false;
	}

	// Initialize the sky plane shader object.
	result = m_SkyPlaneShader->Initialize(CD3dClass::GetInstance()->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky plane shader object.", L"Error", MB_OK);
		return false;
	}

	//result = m_TestModel->Obj_Initialize(CD3dClass::GetInstance()->GetDevice(),
	//	"Building_A01.obj","../MapTool2/obj/");
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize Building_A01 object.", L"Error", MB_OK);
	//	return false;
	//}

	return true;
}

void MapScene::Shutdown()
{
	// Release the sky plane shader object.
	if (m_SkyPlaneShader)
	{
		m_SkyPlaneShader->Shutdown();
		delete m_SkyPlaneShader;
		m_SkyPlaneShader = 0;
	}

	// Release the sky plane object.
	if (m_SkyPlane)
	{
		m_SkyPlane->Shutdown();
		delete m_SkyPlane;
		m_SkyPlane = 0;
	}
	// Release the sky dome shader object.
	if (m_SkyDomeShader)
	{
		m_SkyDomeShader->Shutdown();
		delete m_SkyDomeShader;
		m_SkyDomeShader = 0;
	}

	// Release the sky dome object.
	if (m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}
	// Release the quad tree object.
	if (m_QuadTree)
	{
		m_QuadTree->Shutdown();
		delete m_QuadTree;
		m_QuadTree = 0;
	}

	// Release the frustum object.
	if (m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}

	SAFE_DELETE(m_Light);

	if (m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		SAFE_DELETE(m_TerrainShader);
	}

	// 텍스트 객체를 해제합니다.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// 폰트 쉐이더 객체를 해제합니다..
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// cpu 객체를 해제합니다.
	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// fps 객체를 해제합니다.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// 위치 객체를 해제합니다.
	if (m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// 타이머 객체를 해제합니다.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// 색상 셰이더 객체를 해제합니다.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// 지형 객체를 해제합니다.
	if (m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// 카메라 객체를 해제합니다.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
}

bool MapScene::Frame()
{
	bool foundHeight;
	D3DXVECTOR3 position;
	float height;

	InputClass::GetInstance()->Frame();

	// 사용자가 ESC를 눌렀을 때 응용 프로그램을 종료 할 것인지 확인합니다.
	if (InputClass::GetInstance()->IsEscapePressed() == true)
	{
		return false;
	}
	
	// 시스템 통계를 업데이트 합니다.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	// 텍스트 개체에서 FPS 값을 업데이트 합니다.
	bool result = m_Text->SetFps(m_Fps->GetFps(), CD3dClass::GetInstance()->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// 텍스트 개체의 CPU 사용값을 업데이트 합니다.
	result = m_Text->SetCpu(m_Cpu->GetCpuPercentage(), CD3dClass::GetInstance()->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// 프레임 입력 처리를 수행합니다.
	result = HandleInput(m_Timer->GetTime());
	if (!result)
	{
		return false;
	}

	// Get the current position of the camera.
	position = m_Camera->GetPosition();

	// Get the height of the triangle that is directly underneath the given camera position.
	foundHeight = m_QuadTree->GetHeightAtPosition(position.x, position.z, height);
	if (foundHeight)
	{
		// If there was a triangle under the camera then position the camera just above it by two units.
		m_Camera->SetPosition(position.x, height + position.y, position.z);
	}

	// Do the sky plane frame processing.
	m_SkyPlane->Frame();


	// 그래픽을 렌더링 합니다.
	result = RenderGraphics();
	if (!result)
	{
		return false;
	}

	return result;
}

bool MapScene::InputFrame()
{
	m_Timer->Frame();

	// 프레임 입력 처리를 수행합니다.
	bool result = HandleInput(m_Timer->GetTime());
	if (!result)
	{
		return false;
	}

	return true;
}



bool MapScene::HandleInput(float frameTime)
{
	bool keyDown, result;
	float posX, posY, posZ, rotX, rotY, rotZ;

	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);	

	// Handle the input.
	keyDown = InputClass::GetInstance()->IsLeftPressed();
	m_Position->Moveleft(keyDown);

	keyDown = InputClass::GetInstance()->IsRightPressed();
	m_Position->MoveRifht(keyDown);

	keyDown = InputClass::GetInstance()->IsUpPressed();
	m_Position->MoveForward(keyDown);

	keyDown = InputClass::GetInstance()->IsDownPressed();
	m_Position->MoveBackward(keyDown);

	keyDown = InputClass::GetInstance()->isQPressed();
	m_Position->TurnLeft(keyDown);

	keyDown = InputClass::GetInstance()->isEPressed();
	m_Position->TurnRight(keyDown);

	keyDown = InputClass::GetInstance()->isRPressed();
	m_Position->LookUpward(keyDown);

	keyDown = InputClass::GetInstance()->isFPressed();
	m_Position->LookDownward(keyDown);

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Set the position of the camera.
	//카매라 휠에 따라 카매라 y축 변환
	if (InputClass::GetInstance()->isMouseWheelDown())
	{
		m_Position->SetPosition(posX, posY + 0.25, posZ);
		m_Camera->SetPosition(posX, posY + 0.25, posZ);
	}
	else if (InputClass::GetInstance()->isMouseWheelUp())
	{
		m_Position->SetPosition(posX, posY - 0.25, posZ);
		m_Camera->SetPosition(posX, posY - 0.25, posZ);
		if (m_Position->GetPosition().y < 2)
		{
			m_Position->SetPosition(posX, 2, posZ);
			m_Camera->SetPosition(posX, 2, posZ);
		}
	}
	else
	{
		m_Camera->SetPosition(posX, posY, posZ);
	}
	m_Camera->SetRotation(rotX, rotY, rotZ);

	// Update the position values in the text object.
	D3DXVECTOR3 pos(posX, posY, posZ);
	result = m_Text->SetCameraPosition(pos, CD3dClass::GetInstance()->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Update the rotation values in the text object.
	D3DXVECTOR3 rot(rotX, rotY, rotZ);
	result = m_Text->SetCameraRotation(rot, CD3dClass::GetInstance()->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	return true;
}

bool MapScene::RenderGraphics()
{
	D3DXVECTOR3 cameraPosition;
	bool result;

	// 카메라의 위치에 따라 뷰 행렬을 생성합니다.
	m_Camera->Render();

	// 카메라 및 Direct3D 객체에서 월드, 뷰, 투영 및 ortho 행렬을 가져옵니다.
	D3DXMatrixIdentity(&m_worldMatrix);
	m_Camera->GetViewMatrix(m_viewMatrix);

	// Setup the projection matrix.  
	CRect rect;
	GetWindowRect(CD3dClass::GetInstance()->GetMainView()->m_hWnd, &rect);
	float fieldOfView = (float)D3DX_PI / 4.0f;
	float screenAspect = (float)rect.Width() / (float)rect.Height();
	// Create the projection matrix for 3D rendering.  
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);

	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)800, (float)600, SCREEN_NEAR, SCREEN_DEPTH);

	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();
	// Translate the sky dome to be centered around the camera position.
	D3DXMatrixTranslation(&m_worldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Turn off back face culling.
	CD3dClass::GetInstance()->TurnOffCulling();
	// Turn off the Z buffer.
	CD3dClass::GetInstance()->TurnZBufferOff();
	// Render the sky dome using the sky dome shader.
	m_SkyDome->Render(CD3dClass::GetInstance()->GetDeviceContext());
	m_SkyDomeShader->Render(CD3dClass::GetInstance()->GetDeviceContext(), m_SkyDome->GetIndexCount(),
		m_worldMatrix, m_viewMatrix, m_projectionMatrix,
		m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());
	// Turn back face culling back on.
	CD3dClass::GetInstance()->TurnOnCulling();

	// Enable additive blending so the clouds blend with the sky dome color.
	CD3dClass::GetInstance()->EnableSecondBlendState();

	// Render the sky plane using the sky plane shader.
	m_SkyPlane->Render(CD3dClass::GetInstance()->GetDeviceContext());
	m_SkyPlaneShader->Render(CD3dClass::GetInstance()->GetDeviceContext(), m_SkyPlane->GetIndexCount(),
		m_worldMatrix, m_viewMatrix, m_projectionMatrix,
		m_SkyPlane->GetCloudTexture(), m_SkyPlane->GetPerturbTexture(), m_SkyPlane->GetTranslation(), m_SkyPlane->GetScale(),
		m_SkyPlane->GetBrightness());

	// Turn the Z buffer back on.
	CD3dClass::GetInstance()->TurnZBufferOn();
	// Reset the world matrix.
	D3DXMatrixIdentity(&m_worldMatrix);

	// 알파블랜딩 온
	CD3dClass::GetInstance()->TurnOffAlphaBlending();

	// Construct the frustum.
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, m_projectionMatrix, m_viewMatrix);
	// Set the terrain shader parameters that it will use for rendering.
	result = m_TerrainShader->SetShaderParameters(CD3dClass::GetInstance()->GetDeviceContext(), m_worldMatrix, 
		m_viewMatrix, m_projectionMatrix, m_Light->GetAmbientColor(),
		m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Terrain->GetTexture());
	if (!result)
	{
		return false;
	}
	// Render the terrain using the quad tree and terrain shader.
	m_QuadTree->Render(m_Frustum, CD3dClass::GetInstance()->GetDeviceContext(), m_TerrainShader);
	// Set the number of rendered terrain triangles since some were culled.
	result = m_Text->SetRenderCount(m_QuadTree->GetDrawCount(), CD3dClass::GetInstance()->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// 모든 2D 렌더링을 시작하려면 Z 버퍼를 끕니다.
	CD3dClass::GetInstance()->TurnZBufferOff();

	// Turn back face culling back on.
	CD3dClass::GetInstance()->TurnOnCulling();

	// 텍스트를 렌더링하기 전에 알파 블렌딩을 켭니다.
	CD3dClass::GetInstance()->TurnOnAlphaBlending();

	// 텍스트 사용자 인터페이스 요소를 렌더링 합니다.
	result = m_Text->Render(CD3dClass::GetInstance()->GetDeviceContext(),
		m_FontShader, m_worldMatrix, m_orthoMatrix);
	if (!result)
	{
		return false;
	}

	// 렌더링 된 장면을 화면에 표시합니다.
	CD3dClass::GetInstance()->TurnZBufferOn();

	return true;
}