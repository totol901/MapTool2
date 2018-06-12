#include "stdafx.h"
#include "Mesh.h"

void Subset::operVertexShader(vector<D3DXMATRIX> mat)
{
	//float w[4] = { 0, };
	//XMMATRIX m;
	//int n = mVertices.size();
	//for (int i = 0; i < n; i++)
	//{
	//	w[0] = mVertices[i].mWeight[0];
	//	w[1] = mVertices[i].mWeight[1];
	//	w[2] = mVertices[i].mWeight[2];
	//	w[3] = 1.0f - mVertices[i].mWeight[0] - mVertices[i].mWeight[1] - mVertices[i].mWeight[2];
	//
	//	XMVECTOR v = XMVectorZero();
	//	XMVECTOR p = XMLoadFloat3(&mVertices[i].mPos);
	//	for (int j = 0; j < 4; j++)
	//	{
	//		m = XMLoadFloat4x4(&mat[mVertices[i].mIndices[j]]);
	//		v += w[j] * XMVector3TransformCoord(p, m);
	//		XMStoreFloat3(&mVertices[i].mPos, v);
	//	}
	//}
}

void Subset::Initialize(ID3D11Device * pDevice)
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * mVertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &mVertices[0];
	pDevice->CreateBuffer(&vbd, &vinitData, &mVB);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * mIndices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &mIndices[0];
	pDevice->CreateBuffer(&ibd, &iinitData, &mIB);

	//_bstr_t d(mDiffuseTextureName.c_str());
	//wchar_t* wd = d;
	//DirectX::TexMetadata tmdata;
	//DirectX::ScratchImage img;
	//DirectX::LoadFromTGAFile(wd, &tmdata, img);
	//DirectX::CreateShaderResourceView(pDevice, img.GetImages(), img.GetImageCount(), tmdata, &mDiffuseMapSRV);
	//
	//_bstr_t n(mNormalTextureName.c_str());
	//wchar_t* wn = n;
	//DirectX::LoadFromTGAFile(wn, &tmdata, img);
	//DirectX::CreateShaderResourceView(pDevice, img.GetImages(), img.GetImageCount(), tmdata, &mNormalMapSRV);
}

void Subset::Update(const float deltaTime)
{
	Object::Update(deltaTime);
}

void Subset::Draw(ID3D11DeviceContext * dc)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	dc->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	dc->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	dc->DrawIndexed(mIndices.size(), 0, 0);
}

Mesh::Mesh()
{
	//XMFLOAT4X4 m;
	//XMStoreFloat4x4(&m, XMMatrixIdentity());
	//for (int i = 0; i < 148; i++)
	//{
	//	mTransform.push_back(m);
	//}
}

Mesh::~Mesh()
{
	SAFE_DELETE(mBone);
	UINT n = mSubset.size();
	for (UINT i = 0; i < n; i++)
	{
		SAFE_DELETE(mSubset[i]);
	}
}
void Mesh::Initialize(ID3D11Device * pDevice)
{
	for (UINT i = 0; i < mSubset.size(); i++)
	{
		//mSubset[i]->operVertexShader(mTransform);
		mSubset[i]->Initialize(pDevice);
	}

	//std::copy(mTransform.begin(), mTransform.end(), mAnimation[0].begin());

	//vector<string> m;
	//JB::AddVectorTransform(mBone, mTransform);
	//ddVectorTransform(mBone, m);
	out();
}

void Mesh::Update(const float deltaTime)
{
	Object::Update(deltaTime);
	Object::Moving(deltaTime);
	for (UINT i = 0; i < mSubset.size(); i++)
	{
		mSubset[i]->Update(deltaTime);
	}
}

void Mesh::Draw(ID3D11DeviceContext * dc, D3DXMATRIX& viewproj)
{
	//dc->IASetInputLayout(Layout::mPNTS);
	//dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//
	//LPD3D11EFFECTTECHNIQUE activeTech = Effects::MeshEffectFX->mTech;
	//
	//D3DX11_TECHNIQUE_DESC techDesc;
	//activeTech->GetDesc(&techDesc);
	//
	//XMMATRIX mat;
	//mat = XMLoadFloat4x4(&mWorld);
	////mat = XMMatrixIdentity();
	//
	////XMMATRIX worldInvTranspose = InverseTranspose(mat);
	//XMMATRIX worldViewProj = mat * viewproj;
	//
	////Effects::MeshEffectFX->SetWorld(mat);
	////Effects::MeshEffectFX->SetWorldInvTranspose(worldInvTranspose);
	////Effects::MeshEffectFX->SetWorldViewProj(worldViewProj);
	////Effects::MeshEffectFX->SetBoneTransforms(&mTransform[0], mTransform.size());
	//
	//for (UINT p = 0; p < techDesc.Passes; ++p)
	//{
	//	for (UINT i = 0; i < mSubset.size(); ++i)
	//	{
	//		//Effects::MeshEffectFX->SetDiffuseMap(mSubset[i]->GetMapSRV(0));
	//		//Effects::MeshEffectFX->SetMaterial(mSubset[i]->GetMaterial());
	//		//Effects::MeshEffectFX->SetNormalMap(mSubset[i]->GetMapSRV(2));
	//
	//		activeTech->GetPassByIndex(p)->Apply(0, dc);
	//		mSubset[i]->Draw(dc);
	//	}
	//}
}

void Mesh::out()
{
	FILE* fp;

	fopen_s(&fp, "./ww.txt", "w");
	fprintf(fp, "%d\n", mSubset.size());
	UINT n = mSubset.size();
	for (UINT i = 0; i < n; ++i)
	{
		//fprintf(fp, "%s %s %s\n", mSubset[i]->.c_str(), mSubset[i]->specular.c_str(), mSubset[i]->noraml.c_str());
		fprintf(fp, "%f %f %f %f\n", mSubset[i]->GetMaterial().mDiffuse.x, mSubset[i]->GetMaterial().mDiffuse.y, mSubset[i]->GetMaterial().mDiffuse.z, mSubset[i]->GetMaterial().mDiffuse.w);
		fprintf(fp, "%f %f %f %f\n", mSubset[i]->GetMaterial().mAmbient.x, mSubset[i]->GetMaterial().mAmbient.y, mSubset[i]->GetMaterial().mAmbient.z, mSubset[i]->GetMaterial().mAmbient.w);
		fprintf(fp, "%f %f %f %f\n", mSubset[i]->GetMaterial().mSpecular.x, mSubset[i]->GetMaterial().mSpecular.y, mSubset[i]->GetMaterial().mSpecular.z, mSubset[i]->GetMaterial().mSpecular.w);
		fprintf(fp, "%f %f %f %f\n", mSubset[i]->GetMaterial().mReflect.x, mSubset[i]->GetMaterial().mReflect.y, mSubset[i]->GetMaterial().mReflect.z, mSubset[i]->GetMaterial().mReflect.w);
		fprintf(fp, "%d\n", mSubset[i]->GetVertices().size());
		UINT m = mSubset[i]->GetVertices().size();
		for (UINT j = 0; j < m; j++)
		{
			fprintf(fp, "%f %f %f\n", mSubset[i]->GetVertices()[j].mPos.x,
				mSubset[i]->GetVertices()[j].mPos.y, mSubset[i]->GetVertices()[j].mPos.z);
			fprintf(fp, "%f %f %f\n", mSubset[i]->GetVertices()[j].mNormal.x,
				mSubset[i]->GetVertices()[j].mNormal.y, mSubset[i]->GetVertices()[j].mNormal.z);
			fprintf(fp, "%f %f\n", mSubset[i]->GetVertices()[j].mUV.x,
				mSubset[i]->GetVertices()[j].mUV.y);
			fprintf(fp, "%f %f %f %f\n", mSubset[i]->GetVertices()[j].mTangentU.x,
				mSubset[i]->GetVertices()[j].mTangentU.y, mSubset[i]->GetVertices()[j].mTangentU.z,
				mSubset[i]->GetVertices()[j].mTangentU.w);
			fprintf(fp, "%f %f %f\n", mSubset[i]->GetVertices()[j].mWeight[0],
				mSubset[i]->GetVertices()[j].mWeight[1], mSubset[i]->GetVertices()[j].mWeight[2]);
			fprintf(fp, "%d %d %d %d\n", mSubset[i]->GetVertices()[j].mIndices[0],
				mSubset[i]->GetVertices()[j].mIndices[1], mSubset[i]->GetVertices()[j].mIndices[2],
				mSubset[i]->GetVertices()[j].mIndices[3]);
		}
	}
	fprintf(fp, "%d\n", mTransform.size());
	//JB::OutBoneName(&fp, mBone);
	fclose(fp);
}

StaticMesh::StaticMesh()
{
}

StaticMesh::~StaticMesh()
{
}