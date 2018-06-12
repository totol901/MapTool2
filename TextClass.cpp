#include "stdafx.h"
#include "TextClass.h"

TextClass::TextClass()
{
	m_Font = 0;
	m_FontShader = 0;
	m_sentence1 = 0;
	m_sentence2 = 0;
	m_sentence3 = 0;
	m_sentence4 = 0;
	m_sentence5 = 0;
}

TextClass::TextClass(const TextClass& other)
{
}

TextClass::~TextClass()
{
}

bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int
	screenWidth, int screenHeight,
	D3DXMATRIX baseViewMatrix)
{
	bool result;
	// Store the screen width and height.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	// Store the base view matrix.
	m_baseViewMatrix = baseViewMatrix;
	// Create the font object.
	m_Font = new FontClass;
	if (!m_Font)
	{
		return false;
	}
	// Initialize the font object.
	result = m_Font->Initialize(device, "../MapTool2/font/fontdata.txt", L"../MapTool2/font/font.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if (!m_FontShader)
	{
		return false;
	}
	// Initialize the font shader object.
	result = m_FontShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}
	// 첫 번째 문장을 초기화합니다
	result = InitializeSentence(&m_sentence1, 150, device);
	if (!result)
	{
		return false;
	}

	// 두 번째 문장을 초기화합니다
	result = InitializeSentence(&m_sentence2, 32, device);
	if (!result)
	{
		return false;
	}

	// 세 번째 문장을 초기화합니다
	result = InitializeSentence(&m_sentence3, 16, device);
	if (!result)
	{
		return false;
	}

	// 네 번째 문장을 초기화합니다
	result = InitializeSentence(&m_sentence4, 16, device);
	if (!result)
	{
		return false;
	}

	// 다섯 번째 문장을 초기화합니다
	result = InitializeSentence(&m_sentence5, 16, device);
	if (!result)
	{
		return false;
	}

	// 여섯 번째 문장을 초기화합니다
	result = InitializeSentence(&m_sentence6, 16, device);
	if (!result)
	{
		return false;
	}

	// 일곱 번째 문장을 초기화합니다
	result = InitializeSentence(&m_sentence7, 16, device);
	if (!result)
	{
		return false;
	}

	// 여덟 번째 문장을 초기화합니다
	result = InitializeSentence(&m_sentence8, 16, device);
	if (!result)
	{
		return false;
	}

	// 아홉 번째 문장을 초기화합니다
	result = InitializeSentence(&m_sentence9, 16, device);
	if (!result)
	{
		return false;
	}

	// 열 번째 문장을 초기화합니다
	result = InitializeSentence(&m_sentence10, 16, device);
	if (!result)
	{
		return false;
	}

	// Initialize the eleventh sentence.
	result = InitializeSentence(&m_sentence11, 32, device);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetVideoCardInfo(const char* videoCardName, int videoCardMemory, ID3D11DeviceContext* deviceContext)
{
	char dataString[150] = { 0, };
	char tempString[16] = { 0, };
	char memoryString[32] = { 0, };

	// 비디오 카드 이름 문자열을 설정합니다.
	strcpy_s(dataString, "Video Card: ");
	strcat_s(dataString, videoCardName);

	// 문장 정점 버퍼를 새 문자열 정보로 업데이트합니다.
	if (!UpdateSentence(m_sentence1, dataString, 10, 10, 1.0f, 1.0f, 1.0f, deviceContext))
	{
		return false;
	}

	// 버퍼 오버플로우를 막기 위해 메모리 값을 자릅니다.
	if (videoCardMemory > 9999999)
	{
		videoCardMemory = 9999999;
	}

	// 비디오 메모리 정수 값을 문자열 형식으로 변환합니다.
	_itoa_s(videoCardMemory, tempString, 10);

	// 비디오 메모리 문자열을 설정합니다.
	strcpy_s(memoryString, "Video Memory: ");
	strcat_s(memoryString, tempString);
	strcat_s(memoryString, " MB");

	// 문장 정점 버퍼를 새 문자열 정보로 업데이트합니다.
	if (!UpdateSentence(m_sentence2, memoryString, 10, 30, 1.0f, 1.0f, 1.0f, deviceContext))
	{
		return false;
	}

	return true;
}

bool TextClass::SetCameraPosition(D3DXVECTOR3 pos, ID3D11DeviceContext* deviceContext)
{
	char tempString[16] = { 0, };
	char dataString[16] = { 0, };

	// 부동 소수점에서 정수로 위치를 변환합니다.
	int positionX = (int)pos.x;
	int positionY = (int)pos.y;
	int positionZ = (int)pos.z;

	// 9999 또는 -9999를 초과하면 위치를 자릅니다.
	if (positionX > 9999) { positionX = 9999; }
	if (positionY > 9999) { positionY = 9999; }
	if (positionZ > 9999) { positionZ = 9999; }

	if (positionX < -9999) { positionX = -9999; }
	if (positionY < -9999) { positionY = -9999; }
	if (positionZ < -9999) { positionZ = -9999; }

	// X 위치 문자열을 설정합니다.
	_itoa_s(positionX, tempString, 10);
	strcpy_s(dataString, "X: ");
	strcat_s(dataString, tempString);

	if (!UpdateSentence(m_sentence5, dataString, 10, 130, 0.0f, 1.0f, 0.0f, deviceContext))
	{
		return false;
	}

	// Y 위치 문자열을 설정합니다.
	_itoa_s(positionY, tempString, 10);
	strcpy_s(dataString, "Y: ");
	strcat_s(dataString, tempString);

	if (!UpdateSentence(m_sentence6, dataString, 10, 150, 0.0f, 1.0f, 0.0f, deviceContext))
	{
		return false;
	}

	// Z 위치 문자열을 설정합니다.
	_itoa_s(positionZ, tempString, 10);
	strcpy_s(dataString, "Z: ");
	strcat_s(dataString, tempString);

	if (!UpdateSentence(m_sentence7, dataString, 10, 170, 0.0f, 1.0f, 0.0f, deviceContext))
	{
		return false;
	}

	return true;
}

bool TextClass::SetCameraRotation(D3DXVECTOR3 rot, ID3D11DeviceContext* deviceContext)
{
	char tempString[16] = { 0, };
	char dataString[16] = { 0, };

	// 회전값을 정수로 변환합니다.
	int rotationX = (int)rot.x;
	int rotationY = (int)rot.y;
	int rotationZ = (int)rot.z;

	// X 회전 문자열을 설정합니다.
	_itoa_s(rotationX, tempString, 10);
	strcpy_s(dataString, "rX: ");
	strcat_s(dataString, tempString);

	if (!UpdateSentence(m_sentence8, dataString, 10, 210, 0.0f, 1.0f, 0.0f, deviceContext))
	{
		return false;
	}

	// Y 회전 문자열을 설정합니다.
	_itoa_s(rotationY, tempString, 10);
	strcpy_s(dataString, "rY: ");
	strcat_s(dataString, tempString);

	if (!UpdateSentence(m_sentence9, dataString, 10, 230, 0.0f, 1.0f, 0.0f, deviceContext))
	{
		return false;
	}

	// Z 회전 문자열을 설정합니다.
	_itoa_s(rotationZ, tempString, 10);
	strcpy_s(dataString, "rZ: ");
	strcat_s(dataString, tempString);

	if (!UpdateSentence(m_sentence10, dataString, 10, 250, 0.0f, 1.0f, 0.0f, deviceContext))
	{
		return false;
	}

	return true;
}

void TextClass::Shutdown()
{
	// Release the first sentence.
	ReleaseSentence(&m_sentence1);
	// Release the second sentence.
	ReleaseSentence(&m_sentence2);
	ReleaseSentence(&m_sentence3);
	ReleaseSentence(&m_sentence4);
	ReleaseSentence(&m_sentence5);
	ReleaseSentence(&m_sentence6);
	ReleaseSentence(&m_sentence7);
	ReleaseSentence(&m_sentence8);
	ReleaseSentence(&m_sentence9);
	ReleaseSentence(&m_sentence10);
	ReleaseSentence(&m_sentence11);

	// Release the font shader object.
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}
	// Release the font object.
	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}
	return;
}

bool TextClass::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char mouseString[16];
	bool result;
	// Convert the mouseX integer to string format.
	_itoa_s(mouseX, tempString, 10);
	// Setup the mouseX string.
	strcpy_s(mouseString, "Mouse X: ");
	strcat_s(mouseString, tempString);
	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence1, mouseString, 20, 20, 0.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}
	// Convert the mouseY integer to string format.
	_itoa_s(mouseY, tempString, 10);
	// Setup the mouseY string.
	strcpy_s(mouseString, "Mouse Y: ");
	strcat_s(mouseString, tempString);
	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence2, mouseString, 20, 40, 0.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}
	return true;
}

bool TextClass::SetFps(int fps, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char fpsString[16];
	float red, green, blue;
	bool result;
	// Truncate the fps to below 10,000.
	if (fps > 9999)
	{
		fps = 9999;
	}
	// Convert the fps integer to string format.
	_itoa_s(fps, tempString, 10);
	// Setup the fps string.
	strcpy_s(fpsString, "Fps: ");
	strcat_s(fpsString, tempString);
	// If fps is 60 or above set the fps color to green.
	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}
	// If fps is below 60 set the fps color to yellow.
	if (fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}
	// If fps is below 30 set the fps color to red.
	if (fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}
	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence3, fpsString, 10, 70,0.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}
	return true;
}

bool TextClass::SetCpu(int cpu, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char cpuString[16];
	bool result;
	// Convert the cpu integer to string format.
	_itoa_s(cpu, tempString, 10);
	// Setup the cpu string.
	strcpy_s(cpuString, "Cpu: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");
	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence4, cpuString, 10, 90, 0.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}
	return true;
}

bool TextClass::SetObjects(int objects, ID3D11DeviceContext *deviceContext)
{
	char tempString[16];
	char cpuString[16];
	bool result;
	// Convert the cpu integer to string format.
	_itoa_s(objects, tempString, 10);
	// Setup the cpu string.
	strcpy_s(cpuString, "Objects: ");
	strcat_s(cpuString, tempString);
	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence3, cpuString, 20, 60, 0.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}
	return true;
}

bool TextClass::SetPolygons(int Polygons, ID3D11DeviceContext *deviceContext)
{
	char tempString[16];
	char cpuString[16];
	bool result;
	// Convert the cpu integer to string format.
	_itoa_s(Polygons, tempString, 10);
	// Setup the cpu string.
	strcpy_s(cpuString, "Polygons: ");
	strcat_s(cpuString, tempString);
	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence4, cpuString, 20, 80, 0.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}
	return true;
}

bool TextClass::SetScreenSize(int ScreenX, int ScreenY, ID3D11DeviceContext *deviceContext)
{
	char tempString[32];
	char cpuString[32];
	bool result;
	// Convert the cpu integer to string format.
	_itoa_s(ScreenX, tempString, 10);
	// Setup the cpu string.
	strcpy_s(cpuString, "ScreenSize: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "*");
	_itoa_s(ScreenY, tempString, 10);
	strcat_s(cpuString, tempString);
	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence5, cpuString, 20, 100, 0.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}
	return true;
}


bool TextClass::Render(ID3D11DeviceContext* deviceContext, FontShaderClass* FontShader, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
{
	bool result;
	// Draw the first sentence.
	result = RenderSentence(m_sentence1, deviceContext, FontShader, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}
	// Draw the second sentence.
	result = RenderSentence(m_sentence2,deviceContext, FontShader, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(m_sentence3,deviceContext, FontShader, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}
	result = RenderSentence(m_sentence4,deviceContext, FontShader, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}
	result = RenderSentence(m_sentence5,deviceContext, FontShader, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}
	result = RenderSentence(m_sentence6,deviceContext, FontShader, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}
	result = RenderSentence(m_sentence7,deviceContext, FontShader, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}
	result = RenderSentence(m_sentence8,deviceContext, FontShader, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}
	result = RenderSentence(m_sentence9,deviceContext, FontShader, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}
	result = RenderSentence(m_sentence10,deviceContext, FontShader, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	result = RenderSentence(m_sentence11, deviceContext, FontShader, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetRenderCount(int count, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char renderString[32];
	bool result;


	// Truncate the render count if it gets to large to prevent a buffer overflow.
	if (count > 999999999)
	{
		count = 999999999;
	}

	// Convert the cpu integer to string format.
	_itoa_s(count, tempString, 10);

	// Setup the cpu string.
	strcpy_s(renderString, "Render Count: ");
	strcat_s(renderString, tempString);

	// Update the sentence vertex buffer with the new string information.
	result = UpdateSentence(m_sentence11, renderString, 10, 290, 0.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}


bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;
	// Create a new sentence object.
	*sentence = new SentenceType;
	if (!*sentence)
	{
		return false;
	}
	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;
	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;
	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;
	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;
	// Create the vertex array.
	vertices = new VertexType[(*sentence)->vertexCount];
	if (!vertices)
	{
		return false;
	}
	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if (!indices)
	{
		return false;
	}
	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));
	// Initialize the index array.
	for (i = 0; i<(*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;
	// Release the index array as it is no longer needed.
	delete[] indices;
	indices = 0;
	return true;
}

bool TextClass::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float
	green, float blue, ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;
	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);
	// Check for possible buffer overflow.
	if (numLetters > sentence->maxLength)
	{
		return false;
	}
	// Create the vertex array.
	vertices = new VertexType[sentence->vertexCount];
	if (!vertices)
	{
		return false;
	}
	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY, 1.0);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0,
		&mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;
	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));
	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);
	// Release the vertex array as it is no longer needed.
	delete[] vertices;
	vertices = 0;
	return true;
}

void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		// Release the sentence vertex buffer.
		if ((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}
		// Release the sentence index buffer.
		if ((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}
		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}
	return;
}

bool TextClass::RenderSentence(SentenceType* sentence, ID3D11DeviceContext* deviceContext, FontShaderClass* FontShader, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
{
	// 정점 버퍼 간격 및 오프셋을 설정합니다.
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// 렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정합니다.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	// 렌더링 할 수 있도록 입력 어셈블러에서 인덱스 버퍼를 활성으로 설정합니다.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 이 정점 버퍼에서 렌더링 되어야 하는 프리미티브 유형을 설정합니다.이 경우에는 삼각형입니다.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 입력 된 문장 색상으로 픽셀 색상 벡터를 만듭니다.
	D3DXVECTOR4 pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// 폰트 셰이더를 사용하여 텍스트를 렌더링합니다.
	return FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix, orthoMatrix, m_Font->GetTexture(), pixelColor);
}