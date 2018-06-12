#include "stdafx.h"
#include "ModelClass.h"
#include "ObjGroup.h"
#include "MtlTex.h"
#include "ObjLoader.h"


int ModelClass::Objects = 0;
int ModelClass::TotalPolygons = 0;

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_model = 0;
	++Objects;
}



ModelClass::~ModelClass()
{
	TotalPolygons -= m_indexCount;
	--Objects;
}

void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();
	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);
	return;
}

void ModelClass::Render(LightClass* dirlight, DirectLightShader* ls, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
	D3DXMATRIX projectionMatrix, D3DXVECTOR3 camPos)
{
	for (int i = 0; i < vec_Group.size(); i++)
	{
		unsigned int stride;
		unsigned int offset;
		// Set vertex buffer stride and offset.
		stride = sizeof(VertexType);
		offset = 0;

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		CD3dClass::GetInstance()->GetDeviceContext()->
			IASetVertexBuffers(0, 1, &vec_Group[i].m_vertexBuffer, 
				&stride, &offset);
		// Set the index buffer to active in the input assembler so it can be rendered.
		CD3dClass::GetInstance()->GetDeviceContext()->
			IASetIndexBuffer(vec_Group[i].m_indexBuffer, 
				DXGI_FORMAT_R32_UINT, 0);
		
		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		CD3dClass::GetInstance()->GetDeviceContext()->
			IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//셰이더 처리
		D3DXVECTOR4 am, di;
		am.x = dirlight->GetAmbientColor().x*vec_Group[i].m_pMtlTex->m_mat.Ambient.x;
		am.y = dirlight->GetAmbientColor().y*vec_Group[i].m_pMtlTex->m_mat.Ambient.y;
		am.z = dirlight->GetAmbientColor().z*vec_Group[i].m_pMtlTex->m_mat.Ambient.z;
		am.w = dirlight->GetAmbientColor().w*vec_Group[i].m_pMtlTex->m_mat.Ambient.w;

		di.x = dirlight->GetDiffuseColor().x*vec_Group[i].m_pMtlTex->m_mat.Diffuse.x;
		di.y = dirlight->GetDiffuseColor().y*vec_Group[i].m_pMtlTex->m_mat.Diffuse.y;
		di.z = dirlight->GetDiffuseColor().z*vec_Group[i].m_pMtlTex->m_mat.Diffuse.z;
		di.w = dirlight->GetDiffuseColor().w*vec_Group[i].m_pMtlTex->m_mat.Diffuse.w;
		ls->Render(vec_Group[i].m_indexCount, worldMatrix, viewMatrix, projectionMatrix,
			vec_Group[i].m_pMtlTex->m_texture,
			dirlight->GetDirection(),
			am,
			di,
			camPos,
			vec_Group[i].m_pMtlTex->m_mat.Specular,
			1.0);
	}
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}
ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}
int ModelClass::GetOjbectsCount()
{
	return Objects;
}
int ModelClass::GetTotalPolygonsCount()
{
	return TotalPolygons;
}
bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}
	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i<m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
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
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
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

void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}
	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;
	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;
	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}
	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}
	return true;
}

void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
	return;
}

bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;
	// Open the model file.
	fin.open(filename);
	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}
	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	// Read in the vertex count.
	fin >> m_vertexCount;
	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;
	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}
	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);
	// Read in the vertex data.
	for (i = 0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}
	// Close the model file.
	fin.close();

	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}
	return;
}

bool ModelClass::Obj_Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	vector<VertexType> Model_VTN_Container;
	
	bool result;
	
	// Load in the model data,
	result = LoadObjModel(modelFilename, Model_VTN_Container);
	if (!result)
	{
		return false;
	}
	// Initialize the vertex and index buffers.
	result = InitializeObjBuffers(device, Model_VTN_Container);
	if (!result)
	{
		return false;
	}
	// Load the texture for this model.
	
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ModelClass::Obj_Initialize(ID3D11Device* device, char* modelFilename, char* filePath)
{

	ObjLoader Loader;

	//ojb 파일 파싱하고 Group화 함
	Loader.Load(modelFilename, filePath, vec_Group);

	//그룹마다 버퍼 만듬
	for (int i = 0; i < vec_Group.size(); i++)
	{
		vec_Group[i].InitializeObjBuffers();
	}

	return true;
}

bool ModelClass::LoadObjModel(char*filename, vector<VertexType>& Model_VTN_Container)
{
	vector<D3DXVECTOR3> Vertex_Container;
	vector<D3DXVECTOR2> Texture_Container;
	vector<D3DXVECTOR3> Normal_Container;
	vector<UINT> VertexIndex_Container;
	vector<UINT> TextureIndex_Container;
	vector<UINT> NormalIndex_Container;

	ifstream fin;
	char str[1024];
	int i;
	// Open the model file.
	fin.open(filename);
	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}
	// Read up to the value of vertex count.
	string line;
	string key;
	while (!fin.eof())
	{
		std::getline(fin, line);
		stringstream sts(line);
		sts >> key >> std::ws;
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (key == "v")
		{
			//버텍스 담아줌
			D3DXVECTOR3 temp_Vertex;
			sts >> temp_Vertex.x >> temp_Vertex.y >> temp_Vertex.z;
			//오른손 좌표계 -> 왼손좌표계
			temp_Vertex.z = 1.0f * temp_Vertex.z;
			Vertex_Container.push_back(temp_Vertex);
		}
		if (key == "vt")
		{
			//텍스쳐(좌표) 담아줌
			D3DXVECTOR2 temp_Texture;
			sts >> temp_Texture.x >> temp_Texture.y;
			//텍스쳐 좌표도 뒤집어줌
			temp_Texture.y = 1.0f - temp_Texture.y;
			Texture_Container.push_back(temp_Texture);
		}
		if (key == "vn")
		{
			//노멀 벡터 담아줌
			D3DXVECTOR3 temp_Normal;
			sts >> temp_Normal.x >> temp_Normal.y >> temp_Normal.z;
			//오른손 좌표계 -> 왼손좌표계
			temp_Normal.z = 1.0f * temp_Normal.z;
			Normal_Container.push_back(temp_Normal);
		}


		// If the line starts with 'f' then increment the face count.
		//예) f 1/1/1 2/2/2 3/3/3
		if (key == "f")
		{
			UINT v, t, n;
			while (!sts.eof())
			{
				//선행공백 제거하며 vertex_index값 불러옴
				sts >> v >> std::ws;
				VertexIndex_Container.push_back(v - 1);
				//다음(v) char를 건너 뛴다
				//해당 char가 '/'이면
				if (sts.peek() == '/')
				{
					//다음 char로 이동
					sts.get();
					//건너 뛴 char가 '/'이거나 아닐때
					if (sts.peek() == '/')
					{
						sts.get();
						sts >> n >> std::ws;
						NormalIndex_Container.push_back(n - 1);
					}
					else
					{
						sts >> t >> std::ws;
						TextureIndex_Container.push_back(t - 1);

						if (sts.peek() == '/')
						{
							sts.get();
							sts >> n >> std::ws;
							NormalIndex_Container.push_back(n - 1);
						}
					}
				}
			}
		}
	}

	fin.close();

	//모든 파싱이 끝남

	for (int i = 0; i < VertexIndex_Container.size(); i++)
	{
		VertexType temp;

		temp.position = Vertex_Container[VertexIndex_Container[i]];

		temp.texture = Texture_Container[TextureIndex_Container[i]];

		temp.normal = Normal_Container[NormalIndex_Container[i]];

		Model_VTN_Container.push_back(temp);
	}

	return true;
}

bool ModelClass::InitializeObjBuffers(ID3D11Device *device, vector<VertexType>& Model_VTN_Container)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	m_vertexCount = Model_VTN_Container.size();
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[Model_VTN_Container.size()];
	if (!vertices)
	{
		return false;
	}
	// Create the index array.
	indices = new unsigned long[Model_VTN_Container.size()];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i<Model_VTN_Container.size(); i++)
	{
		vertices[i].position = Model_VTN_Container[i].position;
		vertices[i].texture = Model_VTN_Container[i].texture;
		vertices[i].normal = Model_VTN_Container[i].normal;
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
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * Model_VTN_Container.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;

	TotalPolygons += m_indexCount;

	return true;
}
