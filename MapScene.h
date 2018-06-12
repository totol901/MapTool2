#pragma once
#include "CameraClass.h"
#include "TerrainClass.h"
#include "ColorShaderClass.h"
#include "PositionClass.h"
#include "InputClass.h"
#include "FpsClass.h"
#include "CpuClass.h"
#include "FontShaderClass.h"
#include "TextClass.h"
#include "TimerClass.h"
#include "TerrainShaderClass.h"
#include "LightClass.h"
#include "frustumclass.h"
#include "quadtreeclass.h"
#include "SkyDomeClass.h"
#include "SkyDomeShaderClass.h"
#include "skyplaneclass.h"
#include "skyplaneshaderclass.h"

class ModelClass;

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class MapScene
{
public:
	MapScene();
	~MapScene();

	bool Initialize(HINSTANCE, HWND);
	void Shutdown();
	bool Frame();
	bool InputFrame();

	CameraClass* GetCameraClass()const {return m_Camera;}
	TerrainClass* GetTerrainClass()const {return m_Terrain;}

private:
	bool HandleInput(float);
	bool RenderGraphics();

private:
	CameraClass* m_Camera = nullptr;
	TerrainClass* m_Terrain = nullptr;
	ColorShaderClass* m_ColorShader = nullptr;
	TimerClass* m_Timer = nullptr;
	PositionClass* m_Position = nullptr;
	FpsClass* m_Fps = nullptr;
	CpuClass* m_Cpu = nullptr;
	FontShaderClass* m_FontShader = nullptr;
	TextClass* m_Text = nullptr;
	TerrainShaderClass* m_TerrainShader = nullptr;
	LightClass* m_Light=nullptr;
	FrustumClass* m_Frustum=nullptr;
	QuadTreeClass* m_QuadTree=nullptr;
	SkyDomeClass* m_SkyDome = nullptr;
	SkyDomeShaderClass* m_SkyDomeShader = nullptr;
	SkyPlaneClass *m_SkyPlane = nullptr;
	SkyPlaneShaderClass* m_SkyPlaneShader = nullptr;

	ModelClass* m_TestModel;
private:
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_orthoMatrix;

};
