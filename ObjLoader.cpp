#include "stdafx.h"
#include "ObjLoader.h"
#include "ObjGroup.h"
#include "MtlTex.h"

ObjLoader::ObjLoader()
{
}


ObjLoader::~ObjLoader()
{
}

void ObjLoader::Load(IN char * szFilename, IN char* FilePath, OUT vector<ObjGroup>& vecGroup)
{
	//버텍스
	//텍스쳐
	//노말

	vector<D3DXVECTOR3> vecV;
	vector<D3DXVECTOR2> vecVT;
	vector<D3DXVECTOR3> vecVN;

	vector<ST_PNT_VERTEX> vecVertex;
	string sMtlName;

	FILE* fp;
	char str[1024];
	strcpy(str, FilePath);
	strcat(str, szFilename);
	fopen_s(&fp, str, "r");

	while (!feof(fp))
	{
		char szBuf[1024];
		fgets(szBuf, 1024, fp);

		if (szBuf[0] == '#')
			continue;
		else if (szBuf[0] == 'm')
		{
			char szMtlLib[1024];
			sscanf_s(szBuf, "%*s %s", szMtlLib, 1024);

			LoadMtlLib(szMtlLib, FilePath);

		}
		else if (szBuf[0] == 'g')
		{
			if (vecVertex.empty())
				continue;
			//ObjGroup* pGroup = new ObjGroup;
			//pGroup->m_vecVertex = vecVertex;
			//pGroup->m_pMtlTex = (m_mapMtlTex[sMtlName]);
			//vecVertex.clear();
			assert(0 && "그룹 쓰는 모델임..");
			//vecGroup.push_back(pGroup);
		}
		else if (szBuf[0] == 'v')
		{
			if (szBuf[1] == 't')
			{
				float u, v;
				sscanf_s(szBuf, "%*s %f %f %*f", &u, &v);
				vecVT.push_back(D3DXVECTOR2(u, v));
			}
			else if (szBuf[1] == 'n')
			{
				float x, y, z;
				sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z);
				vecVN.push_back(D3DXVECTOR3(x, y, z));
			}
			else
			{
				float x, y, z;
				sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
		}
		else if (szBuf[0] == 'u')
		{
			char szMtlName[1024];
			sscanf_s(szBuf, "%*s %s", szMtlName, 1024);
			sMtlName = szMtlName;
			
		}
		else if (szBuf[0] == 'f')
		{
			int nIndex[3][3];
			sscanf_s(szBuf, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
				&nIndex[0][0], &nIndex[0][1], &nIndex[0][2],
				&nIndex[1][0], &nIndex[1][1], &nIndex[1][2],
				&nIndex[2][0], &nIndex[2][1], &nIndex[2][2]);

			for (int i = 0; i < 3; ++i)
			{
				ST_PNT_VERTEX v;
				v.p = vecV[nIndex[i][0] - 1];
				v.t = vecVT[nIndex[i][1] - 1];
				v.n = vecVN[nIndex[i][2] - 1];
				m_mapMtlTex[sMtlName]->vertexs.push_back(v);
			}
		}
	}
	fclose(fp);

	for (auto iter = m_mapMtlTex.begin(); iter != m_mapMtlTex.end(); iter++)
	{
		ObjGroup Group;
		Group.m_pMtlTex = iter->second;
		vecGroup.push_back(Group);
	}
}

void ObjLoader::LoadMtlLib(char * szFilename, char* FilePath)
{
	FILE* fp;
	char str[1024];
	strcpy(str, FilePath);
	strcat(str, szFilename);
	fopen_s(&fp, str, "r");

	string sMtlName;
	TAG_MATERIAL stMtl;

	while (!feof(fp))
	{
		char szBuf[1024];
		fgets(szBuf, 1024, fp);

		int CharIndex = 0;
		while ((szBuf[CharIndex] == ' ' ||
			szBuf[CharIndex] == '\t'))
		{
			++CharIndex;
			if (CharIndex >= 1024)
			{
				CharIndex = 0;
				break;
			}
		}

		if (szBuf[0 + CharIndex] == '#')
			continue;
		else if (szBuf[0 + CharIndex] == 'n')
		{
			char szMtlName[1024];
			sscanf_s(szBuf, "%*s %s", szMtlName, 1024);
			sMtlName = szMtlName;
			if (m_mapMtlTex.find(sMtlName) == m_mapMtlTex.end())
			{
				m_mapMtlTex[sMtlName] = new MtlTex;
				ZeroMemory(&stMtl, sizeof(TAG_MATERIAL));
			}

		}
		else if (szBuf[0 + CharIndex] == 'K')
		{
			if (szBuf[1 + CharIndex] == 'a')
			{
				float r, g, b;
				sscanf_s(szBuf, "%*s %f %f %f", &r, &g, &b);
				stMtl.Ambient.x = r;
				stMtl.Ambient.y = g;
				stMtl.Ambient.z = b;
				stMtl.Ambient.w = 1.0f;
			}
			if (szBuf[1 + CharIndex] == 'd')
			{
				float r, g, b;
				sscanf_s(szBuf, "%*s %f %f %f", &r, &g, &b);
				stMtl.Diffuse.x = r;
				stMtl.Diffuse.y = g;
				stMtl.Diffuse.z = b;
				stMtl.Diffuse.w = 1.0f;
			}
			if (szBuf[1 + CharIndex] == 's')
			{
				float r, g, b;
				sscanf_s(szBuf, "%*s %f %f %f", &r, &g, &b);
				stMtl.Specular.x = r;
				stMtl.Specular.y = g;
				stMtl.Specular.z = b;
				stMtl.Specular.w = 1.0f;
			}
		}

		else if (szBuf[0 + CharIndex] == 'm')
		{
			char szTexFile[1024];
			sscanf_s(szBuf, "%*s %s", szTexFile, 1024);
			HR(D3DX11CreateShaderResourceViewFromFileA(
				CD3dClass::GetInstance()->GetDevice(),
				szTexFile, NULL, NULL, &m_mapMtlTex[sMtlName]->m_texture, NULL));
			m_mapMtlTex[sMtlName]->m_mat = (stMtl);
		}
	}
	fclose(fp);
}
