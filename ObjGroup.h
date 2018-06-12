#pragma once

class MtlTex;

class ObjGroup
{
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};
public:
	ObjGroup();
	~ObjGroup();

	bool InitializeObjBuffers();
	
	int m_vertexCount;
	int m_indexCount;
	MtlTex*	m_pMtlTex;
	//vector<ST_PNT_VERTEX> m_vecVertex;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;

};

