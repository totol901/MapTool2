#pragma once


#include "PositionClass.h"
#include "LightClass.h"
#include "DirectLightShader.h"

class ObjGroup;
class ObjLoader;
class MtlTex;

class ModelClass
{
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	~ModelClass();

	bool Obj_Initialize(ID3D11Device*, char*, WCHAR*);
	bool Obj_Initialize(ID3D11Device*, char*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	void Render(LightClass* dirlight, DirectLightShader* ls, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
		D3DXMATRIX projectionMatrix, D3DXVECTOR3 camPos);
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	static int GetOjbectsCount();
	static int GetTotalPolygonsCount();

	PositionClass postion;
	vector<ObjGroup> vec_Group;

private:
	
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

	bool LoadObjModel(char*filename, vector<VertexType>& Model_VTN_Container);
	bool InitializeObjBuffers(ID3D11Device*device, vector<VertexType>& Model_VTN_Container);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;

	ModelType* m_model;
	static int Objects;
	static int TotalPolygons;
};

