#pragma once

class PerlinNoise
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

public:
	PerlinNoise();
	~PerlinNoise();

public:
	void init();
	void update();
	void render();
	void release();

private:
	void MakeRendemVector(VertexType* vertices, int ColSize,int RowSize);

};

