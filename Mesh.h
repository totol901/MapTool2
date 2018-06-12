#pragma once
#include "Object.h"

class Bone
{
public:
	Bone() {}
	~Bone()
	{
		UINT n = GetChildNum();
		for (UINT i = 0; i < n; i++)
		{
			SAFE_DELETE(mChilds[i]);
		}
	}

	VOID Initialize(string name, int index, D3DXMATRIX& mat, Bone* parent = nullptr)
	{
		mName = name;
		mIndex = index;
		memcpy(&mMat, &mat, sizeof(D3DMATRIX));
		mParent = parent;
	}

	Bone* GetChild(UINT index) const
	{
		if (mChilds.size() <= index) return nullptr;

		return mChilds[index];
	}

	void AddChild(Bone* bone)
	{
		mChilds.push_back(bone);
	}

	bool FindStringOfBone(const string& str)
	{
		if (strcmp(mName.c_str(), str.c_str()) == 0)
			return true;
		//if (mName.compare(str) == 0)
		//	return true;
		return false;
	}

	UINT GetChildNum() const
	{
		return mChilds.size();
	}

	void SetMatrix(D3DXMATRIX mat)
	{
		mMat = mat;
	}
	Bone* GetParent() { return mParent; };
	const D3DXMATRIX& GetMatrix() { return mMat; }
	INT GetIndex() { return mIndex; }
	string GetName() { return mName; }
private:
	string mName;
	INT mIndex;
	D3DXMATRIX mMat;

	Bone* mParent;
	vector<Bone*> mChilds;
};

class Subset : public Object
{
public:
	Subset() {}
	~Subset()
	{
		RELEASE_COM(mVB);
		RELEASE_COM(mIB);
		RELEASE_COM(mDiffuseMapSRV);
		RELEASE_COM(mSpecularMapSRV);
		RELEASE_COM(mNormalMapSRV);
	}
	vector<Vertex>& GetVertices() { return mVertices; }
	vector<UINT>& GetIndices() { return mIndices; }

	void SetMaterial(Material mat) { mMaterial = mat; }

	void SetTextureName(const int n, const string str)
	{
		switch (n)
		{
		case 0:
			mDiffuseTextureName = str;
			break;
		case 1:
			mSpecularTextureName = str;
			break;
		case 2:
			mNormalTextureName = str;
			break;
		}
	}

	ID3D11ShaderResourceView* GetMapSRV(int n)
	{
		switch (n)
		{
		case 0:
			return mDiffuseMapSRV;
			break;
		case 1:
			return mSpecularMapSRV;
			break;
		case 2:
			return mNormalMapSRV;
			break;
		}
		return nullptr;
	}
	Material GetMaterial() { return mMaterial; }
	void operVertexShader(vector<D3DXMATRIX> mat);
	void Initialize(ID3D11Device * pDevice);
	virtual void Update(const float deltaTime);
	void Draw(ID3D11DeviceContext * dc);

private:

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	ID3D11ShaderResourceView* mDiffuseMapSRV;
	ID3D11ShaderResourceView* mSpecularMapSRV;
	ID3D11ShaderResourceView* mNormalMapSRV;

	vector<Vertex> mVertices;
	vector<UINT> mIndices;
	string mDiffuseTextureName;
	string mSpecularTextureName;
	string mNormalTextureName;

	Material mMaterial;
};

class Mesh : public Object
{
public:
	Mesh();
	~Mesh();

	void Initialize(ID3D11Device * pDevice);
	virtual void Update(const float deltaTime);
	void Draw(ID3D11DeviceContext * dc, D3DXMATRIX& viewproj);

	Bone* GetBone() { return mBone; }
	void SetBone(Bone* bone) { mBone = bone; }

	vector<Subset*>& GetSubset() { return mSubset; }
	void out();
private:
	vector<Subset*> mSubset;
	Bone* mBone;
	vector<D3DXMATRIX> mTransform;
};

class StaticMesh : public Object
{
public:
	StaticMesh();
	~StaticMesh();

private:
	vector<Subset*> mSubsets;
};
