#pragma once

const int MAX_TRIANGLES = 10000;

class TerrainClass;
class FrustumClass;
class TerrainShaderClass;

class QuadTreeClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR4 color;
	};

	struct VectorType
	{
		float x, y, z;
	};

	struct NodeType
	{
		float positionX, positionZ, width;
		int triangleCount;
		ID3D11Buffer* vertexBuffer, *indexBuffer;
		VectorType* vertexArray;
		NodeType* nodes[4];
	};

public:
	QuadTreeClass();
	~QuadTreeClass();

	bool Initialize(TerrainClass*, ID3D11Device*);
	void Shutdown();
	void Render(FrustumClass*, ID3D11DeviceContext*, TerrainShaderClass*);
	void RenderTRBlend(FrustumClass*, ID3D11DeviceContext*, TerrainShaderClass*);

	int GetDrawCount();

	bool GetHeightAtPosition(float, float, float&);

private:
	void CalculateMeshDimensions(int, float&, float&, float&);
	void CreateTreeNode(NodeType*, float, float, float, ID3D11Device*);
	int CountTriangles(float, float, float);
	bool IsTriangleContained(int, float, float, float);
	void ReleaseNode(NodeType*);
	void RenderNode(NodeType*, FrustumClass*, ID3D11DeviceContext*, TerrainShaderClass*);
	void BlendRenderNode(NodeType*, FrustumClass*, ID3D11DeviceContext*, TerrainShaderClass*);


	void FindNode(NodeType*, float, float, float&);
	bool CheckHeightOfTriangle(float, float, float&, float[3], float[3], float[3]);

private:
	int m_triangleCount, m_drawCount;
	VertexType* m_vertexList = nullptr;
	NodeType* m_parentNode = nullptr;

};

