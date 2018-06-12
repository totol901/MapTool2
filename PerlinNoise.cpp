#include "stdafx.h"
#include "PerlinNoise.h"
#include "MathHelper.h"

PerlinNoise::PerlinNoise()
{
}

PerlinNoise::~PerlinNoise()
{
}

void PerlinNoise::MakeRendemVector(VertexType* vertices, int ColSize, int RowSize)
{
	int vertexCount = 0;
	int index = 0;
	D3DXVECTOR3* RendVecs;
	D3DXVECTOR3* RendCenterPoses;

	vertexCount = (ColSize + 1) * (RowSize + 1);

	RendVecs = new D3DXVECTOR3[vertexCount];
	RendCenterPoses = new D3DXVECTOR3[vertexCount / 4];

	//·£´ýÇÑ º¤ÅÍ ¼³Á¤
	for (int j = 0; j < (ColSize + 1); j++)
	{
		for (int i = 0; i < (RowSize + 1); i++)
		{
			RendVecs[j * RowSize + i] = D3DXVECTOR3(
				MathHelper::RandF(-1, 1),
				MathHelper::RandF(-1, 1),
				MathHelper::RandF(-1, 1));
		}
	}

	//·£´ýÇÑ Áß°£ Á¡ ¼³Á¤
	index = 0;
	for (int j = 0; j < (ColSize + 1); j * 2)
	{
		for (int i = 0; i < (RowSize); i++)
		{
			float maxX = RendVecs[j * RowSize + i].x;
			float minX = RendVecs[j * RowSize + i].x;
			float maxY = RendVecs[j * RowSize + i].y;
			float minY = RendVecs[j * RowSize + i].y;
			float maxZ = RendVecs[j * RowSize + i].z;
			float minZ = RendVecs[j * RowSize + i].z;

			if (RendVecs[j * RowSize + i + 1].x > maxX)
			{
				maxX = RendVecs[j * RowSize + i + 1].x;
			}
			if (RendVecs[j * RowSize + i + 1].x < minX)
			{
				minX = RendVecs[j * RowSize + i + 1].x;
			}
			if (RendVecs[(j + 1) * RowSize + i].x > maxX)
			{
				maxX = RendVecs[(j + 1) * RowSize + i].x;
			}
			if (RendVecs[(j + 1) * RowSize + i].x < minX)
			{
				minX = RendVecs[(j + 1) * RowSize + i].x;
			}
			if (RendVecs[(j + 1) * RowSize + i + 1].x > maxX)
			{
				maxX = RendVecs[(j + 1) * RowSize + i + 1].x;
			}
			if (RendVecs[(j + 1) * RowSize + i + 1].x < minX)
			{
				minX = RendVecs[(j + 1) * RowSize + i + 1].x;
			}

			if (RendVecs[j * RowSize + i + 1].y > maxY)
			{
				maxY = RendVecs[j * RowSize + i + 1].y;
			}
			if (RendVecs[j * RowSize + i + 1].y < minY)
			{
				minY = RendVecs[j * RowSize + i + 1].y;
			}
			if (RendVecs[(j + 1) * RowSize + i].y > maxY)
			{
				maxY = RendVecs[(j + 1) * RowSize + i].y;
			}
			if (RendVecs[(j + 1) * RowSize + i].y < minY)
			{
				minY = RendVecs[(j + 1) * RowSize + i].y;
			}
			if (RendVecs[(j + 1) * RowSize + i + 1].y > maxY)
			{
				maxY = RendVecs[(j + 1) * RowSize + i + 1].y;
			}
			if (RendVecs[(j + 1) * RowSize + i + 1].y < minY)
			{
				minY = RendVecs[(j + 1) * RowSize + i + 1].y;
			}

			if (RendVecs[j * RowSize + i + 1].z > maxZ)
			{
				maxZ = RendVecs[j * RowSize + i + 1].z;
			}
			if (RendVecs[j * RowSize + i + 1].z < minZ)
			{
				minZ = RendVecs[j * RowSize + i + 1].z;
			}
			if (RendVecs[(j + 1) * RowSize + i].z > maxZ)
			{
				maxZ = RendVecs[(j + 1) * RowSize + i].z;
			}
			if (RendVecs[(j + 1) * RowSize + i].z < minZ)
			{
				minZ = RendVecs[(j + 1) * RowSize + i].z;
			}
			if (RendVecs[(j + 1) * RowSize + i + 1].z > maxZ)
			{
				maxZ = RendVecs[(j + 1) * RowSize + i + 1].z;
			}
			if (RendVecs[(j + 1) * RowSize + i + 1].z < minZ)
			{
				minZ = RendVecs[(j + 1) * RowSize + i + 1].z;
			}

			RendCenterPoses[index].x = MathHelper::RandF(minX, maxX);
			RendCenterPoses[index].y = MathHelper::RandF(minY, maxY);
			RendCenterPoses[index].z = MathHelper::RandF(minZ, maxZ);

			index++;
		}
	}


	//for (int j = 0; j < (ColSize - 1); j++)
	//{
	//	for (int i = 0; i < (RowSize - 1); i++)
	//	{
	//		//1
	//		RendVecs[index] = ;
	//		//8
	//		RendVecs[index + 7] = RendVecs[index];
	//
	//		//2
	//		RendVecs[index + 1] = D3DXVECTOR3(
	//			MathHelper::RandF(),
	//			MathHelper::RandF(),
	//			MathHelper::RandF());
	//		//3
	//		RendVecs[index + 2] = RendVecs[index + 1];
	//		
	//		//4
	//		RendVecs[index + 3] = D3DXVECTOR3(
	//			MathHelper::RandF(),
	//			MathHelper::RandF(),
	//			MathHelper::RandF());
	//		//5
	//		RendVecs[index + 4] = RendVecs[index + 3];
	//
	//		//6
	//		RendVecs[index + 5] = D3DXVECTOR3(
	//			MathHelper::RandF(),
	//			MathHelper::RandF(),
	//			MathHelper::RandF());
	//		//7
	//		RendVecs[index + 6] = RendVecs[index + 5];
	//
	//		index += 8;
	//	}
	//}


}
