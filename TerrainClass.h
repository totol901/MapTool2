#pragma once
#include "TextureClass.h"
#include "TerrainShaderClass.h"

const int TEXTURE_REPEAT = 1;

////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
class TerrainClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR4 color;
	};

	struct HeightMapType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		float r, g, b;
		int rIndex, gIndex, bIndex;
	};

	struct VectorType
	{
		float x, y, z;
	};

	struct MaterialGroupType
	{
		int textureIndex1, textureIndex2, alphaIndex;
		int red, green, blue;
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount;
		VertexType* vertices;
		unsigned long* indices;
	};

public:
	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();

	bool Initialize(ID3D11Device*, const char*, WCHAR* textureFilename, char* colorMapFilename);
	bool InitializeBlend(ID3D11Device* device, char* heightMapFilename, char* materialsFilename, char* materialMapFilename, char* colorMapFilename);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	bool Blend_Render(ID3D11DeviceContext* deviceContext, TerrainShaderClass* shader, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
		D3DXMATRIX projectionMatrix, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 lightDirection);
	
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture()const { return m_texture->GetTexture(); }

	int GetVertexCount();
	void CopyVertexArray(void*);

	void GetTerrainSize(int&, int&);

private:
	bool LoadHeightMap(const char*);
	void NormalizeHeightMap();
	bool CalculateNormals();
	void ShutdownHeightMap();

	void CalculateTextureCoodinates();
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadColorMap(char*);

	bool LoadMaterialFile(char*, char*, ID3D11Device*);
	bool LoadMaterialMap(char*);
	bool LoadMaterialBuffers(ID3D11Device*);
	void ReleaseMaterials();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	int m_terrainWidth, m_terrainHeight;
	int m_vertexCount, m_indexCount;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	HeightMapType* m_heightMap;
	TextureClass* m_texture;
	VertexType* m_vertices;

	int m_textureCount, m_materialCount;
	TextureClass* m_Textures;
	MaterialGroupType* m_Materials;
};
