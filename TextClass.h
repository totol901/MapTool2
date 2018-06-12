#pragma once
#include "fontclass.h"
#include "FontShaderClass.h"

class TextClass
{
private:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();
	bool SetCameraPosition(D3DXVECTOR3 pos, ID3D11DeviceContext* deviceContext);
	bool SetCameraRotation(D3DXVECTOR3, ID3D11DeviceContext*);
	bool SetVideoCardInfo(const char* videoCardName, int videoCardMemory, ID3D11DeviceContext* deviceContext);
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, D3DXMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, FontShaderClass* FontShader, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix);
	bool SetMousePosition(int, int, ID3D11DeviceContext*);
	bool SetFps(int, ID3D11DeviceContext*);
	bool SetCpu(int, ID3D11DeviceContext*);
	bool SetObjects(int, ID3D11DeviceContext*);
	bool SetPolygons(int, ID3D11DeviceContext*);
	bool SetScreenSize(int, int, ID3D11DeviceContext*);
	bool SetRenderCount(int, ID3D11DeviceContext*);

private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(SentenceType* sentence, ID3D11DeviceContext* deviceContext, FontShaderClass* FontShader, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix);

private:
	FontClass* m_Font;
	FontShaderClass* m_FontShader;
	int m_screenWidth, m_screenHeight;
	D3DXMATRIX m_baseViewMatrix;

	SentenceType* m_sentence1 = nullptr;
	SentenceType* m_sentence2 = nullptr;
	SentenceType* m_sentence3 = nullptr;
	SentenceType* m_sentence4 = nullptr;
	SentenceType* m_sentence5 = nullptr;
	SentenceType* m_sentence6 = nullptr;
	SentenceType* m_sentence7 = nullptr;
	SentenceType* m_sentence8 = nullptr;
	SentenceType* m_sentence9 = nullptr;
	SentenceType* m_sentence10 = nullptr;
	SentenceType* m_sentence11 = nullptr;
};

