#include "stdafx.h"
#include "TextureClass.h"
#include "D3dClass.h"

TextureClass::TextureClass()
{
}


TextureClass::~TextureClass()
{
}


bool TextureClass::Initialize(ID3D11Device* device, const WCHAR* filename)
{
	// 텍스처를 파일로부터 읽어온다
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(
		CD3dClass::GetInstance()->GetDevice(),
		filename, NULL, NULL, &m_texture, NULL)))
	{
		return false;
	}

	return true;
}

void TextureClass::Shutdown()
{
	//텍스처 뷰 리소스를 해제한다.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}