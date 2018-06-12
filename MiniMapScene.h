#pragma once
#include "TextureShaderClass.h"
#include "MiniMapClass.h"
#include "TerrainClass.h"

class MiniMapScene
{
public:
	MiniMapScene();
	~MiniMapScene();
	bool Initialize(HINSTANCE, HWND);
	void Shutdown();
	bool HandleInput();
	bool RenderGraphics();
	bool Frame();

	void SetCameraLink(CameraClass* camera){m_Camera = camera;}
	void SetTerrainLink(TerrainClass* terrain){m_Terrain = terrain;}

private:
	TextureShaderClass* m_TextureShader;
	MiniMapClass* m_MiniMap;

private:
	TerrainClass* m_Terrain;
	CameraClass* m_Camera;

private:
	D3DXMATRIX m_worldMatrix;
	//D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_orthoMatrix;
};

