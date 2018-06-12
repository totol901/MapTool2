#include "stdafx.h"
#include "ObjGroup.h"
#include "MtlTex.h"

ObjGroup::ObjGroup()
	:m_vertexCount(0),
	m_indexCount(0)
{
}


ObjGroup::~ObjGroup()
{
}

bool ObjGroup::InitializeObjBuffers()
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	m_vertexCount = m_pMtlTex->vertexs.size();
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_pMtlTex->vertexs.size()];
	if (!vertices)
	{
		return false;
	}
	// Create the index array.
	indices = new unsigned long[m_pMtlTex->vertexs.size()];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i<m_pMtlTex->vertexs.size(); i++)
	{
		vertices[i].position = m_pMtlTex->vertexs[i].p;
		vertices[i].texture = m_pMtlTex->vertexs[i].t;
		vertices[i].normal = m_pMtlTex->vertexs[i].n;
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	result = CD3dClass::GetInstance()->GetDevice()->
		CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_pMtlTex->vertexs.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	result = CD3dClass::GetInstance()->GetDevice()->
		CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;

	return true;
}
