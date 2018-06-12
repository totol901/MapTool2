#pragma once
#include <fbxsdk.h>

class Mesh;
class Bone;
class Subset;

class FBXLoader
{
public:
	FBXLoader();
	~FBXLoader();
private:
	Bone* ParseSkeletonHierarchy(FbxNode* pNode);
	VOID ParseSkeletonHierarchyRecursively(FbxNode * pNode, int inDepth, int myIndex, int inParentIndex, Bone* bone, Bone* p = nullptr);

	VOID ParseNode(FbxNode* pNode, vector<Subset*>& subsets, Bone* bone);
	VOID ParseControlPoint(FbxNode* pNode, vector<Vertex>& ver);
	VOID ParseVertexInfo(FbxNode* pNode, vector<Vertex>& ver, vector<Subset*>& subsets);
	VOID ParseMaterial(FbxNode* pNode, vector<Subset*>& subsets);
	VOID ParseBone(FbxNode* pNode, Bone* bone, vector<Vertex>& ver);

	void ParseMaterialAttribute(FbxSurfaceMaterial* inMaterial, UINT index, Subset* inObj);
	void ParseMaterialTexture(FbxSurfaceMaterial* inMaterial, UINT index, Subset* inObj);

	VOID ReadNormal(FbxMesh* pMesh, UINT mPolygonIndex, UINT IndexNum, Vertex& pnt);
	VOID ReadUV(FbxMesh* pMesh, UINT mPolygonIndex, UINT UVIndex, Vertex& pnt);
	VOID ReadTangent(FbxMesh* pMesh, UINT mPolygonIndex, UINT IndexNum, Vertex& pnt);
	FbxAMatrix GetGeometryTransformation(FbxNode * pNode);

	void ConvertFbxMatrix(FbxAMatrix mat, OUT D3DXMATRIX& m4x4);

	void BoneMatrixPath(Bone* bone);

	void ParseFindAnimation(FbxNode* pNode, FbxScene* fs, OUT Mesh* pMesh);
	void ParseAnimation(FbxNode* pNode, FbxScene* fs, OUT Mesh* pMesh);

private:
public:
	Mesh* LoadFBX(const string fileName);

	void LoadFBXAnimation(const string fileName, OUT Mesh* pMesh);
};
