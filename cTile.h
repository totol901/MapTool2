#pragma once

enum E_TILE_TEXTURE_TYPE
{
	E_NONE
};

class cTile
{
public:
	cTile();
	~cTile();

public:
	bool Init(E_TILE_TEXTURE_TYPE tileType, D3DXVECTOR3 position, size_t size);
	void Release();
	void Render();
	

private:
	void InitializeBuffers();
	void RenderBuffers();
	void RenderShader();
	
private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;

	E_TILE_TEXTURE_TYPE m_eTileTexture;

	ID3D11ShaderResourceView* m_texture;

	ST_BOUNDING_BOX m_boundingBox;
	ST_SPHERE m_ClickSphere;
	vector<ST_PNT_VERTEX> m_vec_Vertexs;
	vector<UINT> m_vec_Indexs;
};
