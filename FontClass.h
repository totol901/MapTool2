#pragma once
#include "TextureManager.h"

class FontClass
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();
	bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();
	ID3D11ShaderResourceView* GetTexture();
	void BuildVertexArray(void*, char*, float, float, float);
private:
	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();
private:
	FontType* m_Font;
	wstring textureName;
	//TextureClass* m_Texture;
};

