#pragma once

class MtlTex
{
private:

public:
	MtlTex();
	~MtlTex();
	vector<ST_PNT_VERTEX> vertexs;
	TAG_MATERIAL m_mat;
	ID3D11ShaderResourceView* m_texture;
};

