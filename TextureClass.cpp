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
	// �ؽ�ó�� ���Ϸκ��� �о�´�
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
	//�ؽ�ó �� ���ҽ��� �����Ѵ�.
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