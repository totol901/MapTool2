#include "stdafx.h"
#include "TextureManager.h"

TextureManager* TextureManager::m_CInstance = nullptr;

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

ID3D11ShaderResourceView* TextureManager::GetTexture(wstring s)
{
	if (m_mapTexture.find(s) == m_mapTexture.end())
	{
		assert(0 && TEXT("텍스쳐 찾을 수 없음"));
	}

	return m_mapTexture[s];
}

void TextureManager::Destroy()
{
	for each(auto it in m_mapTexture)
	{
		RELEASE_COM(it.second);
	}

	SAFE_DELETE(m_CInstance);
}

TextureManager * TextureManager::GetInstance()
{
	if (m_CInstance == nullptr)
	{
		m_CInstance = new TextureManager();
	}

	return m_CInstance;
}

bool TextureManager::AddTexture(wstring filename, wstring mapName)
{
	ID3D11ShaderResourceView* texture;

	if (m_mapTexture.find(mapName) == m_mapTexture.end())
	{
		HR(D3DX11CreateShaderResourceViewFromFile(
			CD3dClass::GetInstance()->GetDevice(),
			filename.c_str(), NULL, NULL, &texture, NULL)
		);

		m_mapTexture.insert(make_pair(mapName, texture));

		return true;
	}

	return false;
}

void TextureManager::EraseTexture(wstring mapName)
{
	auto at = m_mapTexture.find(mapName);
	if (at == m_mapTexture.end())
	{
		assert(0 && "Erase할 텍스쳐 찾지못함");
	}

	RELEASE_COM(at->second);
	m_mapTexture.erase(mapName);
}

