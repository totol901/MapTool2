#pragma once

class TextureManager
{
public:
	TextureManager();
	TextureManager(const TextureManager&) = delete;
	TextureManager(const TextureManager&&) = delete;
	TextureManager& operator= (const TextureManager &) = delete;
	TextureManager& operator= (const TextureManager &&) = delete;
	~TextureManager();

public:
	static TextureManager* GetInstance();

	bool AddTexture(wstring filename, wstring mapName);
	void EraseTexture(wstring mapName);

public:
	ID3D11ShaderResourceView* GetTexture(wstring s);
	void Destroy();

private:
	static TextureManager* m_CInstance;

	map<wstring, ID3D11ShaderResourceView*> m_mapTexture;
};
