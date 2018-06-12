#include "stdafx.h"
#include "cTile.h"
#include"TextureManager.h"

cTile::cTile()
{
}

cTile::~cTile()
{
}

bool cTile::Init(E_TILE_TEXTURE_TYPE tileType, D3DXVECTOR3 position, size_t size)
{
	//바닥 텍스처 설정
	m_eTileTexture = tileType;

	if (m_eTileTexture == E_NONE)
	{
		m_texture = TextureManager::GetInstance()->GetTexture(TEXT("tile1"));
	}

	//버텍스 설정
	D3DXVECTOR3 v[4];
	D3DXVECTOR2 t[4];
	D3DXVECTOR3 n(0, 1, 0);

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			v[i * 2 + j] = D3DXVECTOR3(position.x + i * size, position.y, position.z + j* size);
			t[i * 2 + j] = D3DXVECTOR2((float)i, (float)j);

			ST_PNT_VERTEX temp(v[i * 2 + j], n, t[i * 2 + j]);
			m_vec_Vertexs.push_back(temp);
		}
	}

	//인덱스 버퍼 설정
	m_vec_Indexs.push_back(0);
	m_vec_Indexs.push_back(1);
	m_vec_Indexs.push_back(4);
	m_vec_Indexs.push_back(0);
	m_vec_Indexs.push_back(4);
	m_vec_Indexs.push_back(3);

	//바운딩 박스
	m_boundingBox.vMin = v[0];
	m_boundingBox.vMax = v[4];
	
	//클릭 바운딩 구
	m_ClickSphere.SetUp(v[0], size * 0.5f);

	//버퍼 초기화
	InitializeBuffers();

	return true;
}

void cTile::Render()
{
	RenderBuffers();
	RenderShader();
}

void cTile::RenderShader()
{

}

void cTile::Release()
{
	RELEASE_COM(m_vertexBuffer);
	RELEASE_COM(m_indexBuffer);
}

void cTile::RenderBuffers()
{
	unsigned int stride;
	unsigned int offset;
	// Set vertex buffer stride and offset.
	stride = sizeof(ST_PNT_VERTEX);
	offset = 0;
	
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	CD3dClass::GetInstance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	// Set the index buffer to active in the input assembler so it can be rendered.
	CD3dClass::GetInstance()->GetDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	CD3dClass::GetInstance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void cTile::InitializeBuffers()
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = sizeof(ST_PNT_VERTEX) * m_vec_Vertexs.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = &m_vec_Vertexs[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	
	HR(CD3dClass::GetInstance()->GetDevice()->CreateBuffer(
		&vertexBufferDesc, &vertexData, &m_vertexBuffer)
	);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * m_vec_Indexs.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = &m_vec_Indexs[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	HR(CD3dClass::GetInstance()->GetDevice()->CreateBuffer(
		&indexBufferDesc, &indexData, &m_indexBuffer)
	);
}
