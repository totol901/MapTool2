#include "stdafx.h"
#include "FBXLoader.h"
#include "Mesh.h"

FBXLoader::FBXLoader()
{
}

FBXLoader::~FBXLoader()
{
}

Bone* FBXLoader::ParseSkeletonHierarchy(FbxNode * pNode)
{
	Bone* bone = nullptr;
	int n = pNode->GetChildCount();
	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		auto currNode = pNode->GetChild(i);
		if (currNode->GetNodeAttribute()->GetAttributeType() != FbxNodeAttribute::eSkeleton) continue;
		bone = new Bone();
		ParseSkeletonHierarchyRecursively(currNode, 0, 0, -1, bone);
	}
	return bone;
}

VOID FBXLoader::ParseSkeletonHierarchyRecursively(FbxNode * pNode, int inDepth, int myIndex, int inParentIndex, Bone* bone, Bone* p)
{
	static int size = 0;
	if (pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		bone->Initialize(pNode->GetName(), inParentIndex, *D3DXMatrixIdentity(nullptr), p);
		size++;
	}
	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		Bone* b = new Bone();
		ParseSkeletonHierarchyRecursively(pNode->GetChild(i), inDepth + 1, size, myIndex, b, bone);
		bone->AddChild(b);
	}
}

VOID FBXLoader::ParseNode(FbxNode * pNode, vector<Subset*>& subsets, Bone* bone)
{
	if (pNode->GetMesh())
	{
		vector<Vertex> vecPNT;
		ParseControlPoint(pNode, vecPNT);
		ParseBone(pNode, bone, vecPNT);
		ParseVertexInfo(pNode, vecPNT, subsets);
		ParseMaterial(pNode, subsets);
	}
	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		ParseNode(pNode->GetChild(i), subsets, bone);
	}
}

VOID FBXLoader::ParseControlPoint(FbxNode * pNode, vector<Vertex>& ver)
{
	auto pMesh = pNode->GetMesh();

	for (int i = 0; i < pMesh->GetControlPointsCount(); ++i)
	{
		Vertex pnt;
		pnt.mPos.x = static_cast<float>(pMesh->GetControlPointAt(i).mData[0]);
		pnt.mPos.y = static_cast<float>(pMesh->GetControlPointAt(i).mData[1]);
		pnt.mPos.z = -static_cast<float>(pMesh->GetControlPointAt(i).mData[2]);
		//XMStoreFloat3(&pnt.mPos, XMVector3TransformCoord(XMLoadFloat3(&pnt.mPos), XMMatrixIdentity()));
		ZeroMemory(pnt.mIndices, sizeof(pnt.mIndices));
		ZeroMemory(pnt.mWeight, sizeof(pnt.mWeight));
		ver.push_back(pnt);
	}
}

VOID FBXLoader::ParseVertexInfo(FbxNode * pNode, vector<Vertex>& ver, vector<Subset*>& subsets)
{
	auto pMesh = pNode->GetMesh();
	auto SubsetNum = pMesh->GetElementMaterial()->GetIndexArray().GetLast() + 1;
	auto Subindex = new UINT[SubsetNum];
	UINT TotalIndex = 0;

	map<int, Subset*> mapSubset;
	ZeroMemory(Subindex, sizeof(UINT) * SubsetNum);
	for (int i = 0; i < SubsetNum; i++)
	{
		mapSubset[i] = new Subset();
		subsets.push_back(mapSubset[i]);
	}
	UINT SubsetIndexNum;
	for (int i = 0; i < pMesh->GetPolygonCount(); ++i)
	{
		SubsetIndexNum = pMesh->GetElementMaterial()->GetIndexArray().GetAt(i);
		UINT indexArr[3];
		for (int j = 0; j < pMesh->GetPolygonSize(i); ++j)
		{
			auto index = pMesh->GetPolygonVertex(i, j);
			ReadNormal(pMesh, index, TotalIndex, ver[index]);
			ReadUV(pMesh, index, pMesh->GetTextureUVIndex(i, j), ver[index]);
			ReadTangent(pMesh, index, TotalIndex, ver[index]);
			++TotalIndex;
			indexArr[j] = index;
		}
		mapSubset[SubsetIndexNum]->GetVertices().push_back(ver[indexArr[0]]);
		mapSubset[SubsetIndexNum]->GetVertices().push_back(ver[indexArr[2]]);
		mapSubset[SubsetIndexNum]->GetVertices().push_back(ver[indexArr[1]]);
		mapSubset[SubsetIndexNum]->GetIndices().push_back(Subindex[SubsetIndexNum]++);
		mapSubset[SubsetIndexNum]->GetIndices().push_back(Subindex[SubsetIndexNum]++);
		mapSubset[SubsetIndexNum]->GetIndices().push_back(Subindex[SubsetIndexNum]++);
	}
	SAFE_DELETE(Subindex);

	ver.clear();
}

VOID FBXLoader::ParseMaterial(FbxNode * pNode, vector<Subset*>& subsets)
{
	auto materialCount = pNode->GetMaterialCount();
	for (int i = 0; i < materialCount; ++i)
	{
		auto surfaceMaterial = pNode->GetMaterial(i);
		ParseMaterialAttribute(surfaceMaterial, i, subsets[subsets.size() - materialCount + i]);
		ParseMaterialTexture(surfaceMaterial, i, subsets[subsets.size() - materialCount + i]);
	}
}

VOID FBXLoader::ParseBone(FbxNode * pNode, Bone* bone, vector<Vertex>& ver)
{
	FbxMesh* fbxMesh = pNode->GetMesh();

	string name = pNode->GetName();
	UINT numOfDeformers = fbxMesh->GetDeformerCount();

	FbxAMatrix geometryTransform = GetGeometryTransformation(pNode);

	for (UINT deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
	{
		FbxSkin* pSkin = reinterpret_cast<FbxSkin*>(fbxMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!pSkin) continue;

		UINT numOfClusters = pSkin->GetClusterCount();
		for (UINT clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
		{
			FbxCluster* pCluster = pSkin->GetCluster(clusterIndex);
			string sJointName = pCluster->GetLink()->GetName();
			//UINT JointIndex = FindJointIndexUsingName(sJointName);
			Bone* b = nullptr;
			//FindStringToBone(sJointName, bone, &b);
			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix globalBindposeInversMatrix;

			pCluster->GetTransformMatrix(transformMatrix);
			pCluster->GetTransformLinkMatrix(transformLinkMatrix);
			globalBindposeInversMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometryTransform;

			D3DXMATRIX m4x4;
			ConvertFbxMatrix(globalBindposeInversMatrix, m4x4);

			b->SetMatrix(m4x4);

			UINT numofIndices = pCluster->GetControlPointIndicesCount();
			for (UINT i = 0; i < numofIndices; ++i)
			{
				int a = pCluster->GetControlPointIndices()[i];
				for (UINT j = 0; j < 4; j++)
				{
					if (ver[a].mIndices[j] == 0)
					{
						ver[a].mIndices[j] = b->GetIndex() + 1;
						if (j < 3)
							ver[a].mWeight[j] = static_cast<float>(pCluster->GetControlPointWeights()[i]);
						break;
					}
				}
			}

			//m_sSkeleton.mJoint[JointIndex].mat = globalBindposeInversMatrix;
		}
	}
}

FbxAMatrix FBXLoader::GetGeometryTransformation(FbxNode * pNode)
{
	const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
}

void FBXLoader::ConvertFbxMatrix(FbxAMatrix mat, OUT D3DXMATRIX& m4x4)
{
	//FbxVector4 t = mat.GetT();
	//FbxVector4 r = mat.GetR();
	//
	//t.Set(t.mData[0], t.mData[1], -t.mData[2]);
	//r.Set(-r.mData[0], -r.mData[1], r.mData[2]);
	//
	//mat.SetT(t);
	//mat.SetR(r);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m4x4.m[i][j] = static_cast<float>(mat.Get(i, j));
		}
	}
}

void FBXLoader::BoneMatrixPath(Bone * bone)
{
	FbxAMatrix mat;
	D3DXMATRIX m4x4 = bone->GetMatrix();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mat.mData[i][j] = m4x4.m[i][j];
		}
	}

	FbxVector4 t = mat.GetT();
	FbxVector4 r = mat.GetR();

	t.Set(t.mData[0], t.mData[1], -t.mData[2]);
	r.Set(-r.mData[0], -r.mData[1], r.mData[2]);

	mat.SetT(t);
	mat.SetR(r);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m4x4.m[i][j] = static_cast<float>(mat.Get(i, j));
		}
	}

	bone->SetMatrix(m4x4);
	UINT n = bone->GetChildNum();
	for (UINT i = 0; i < n; i++)
	{
		BoneMatrixPath(bone->GetChild(i));
	}
}

void FBXLoader::ParseMaterialAttribute(FbxSurfaceMaterial * inMaterial, UINT index, Subset* inObj)
{
	FbxDouble3 double3;

	Material matl;

	double3 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->Ambient;
	matl.mAmbient.x = static_cast<float>(double3.mData[0]);
	matl.mAmbient.y = static_cast<float>(double3.mData[1]);
	matl.mAmbient.z = static_cast<float>(double3.mData[2]);
	matl.mAmbient.w = 1.0f;

	double3 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->Diffuse;
	matl.mDiffuse.x = static_cast<float>(double3.mData[0]);
	matl.mDiffuse.y = static_cast<float>(double3.mData[1]);
	matl.mDiffuse.z = static_cast<float>(double3.mData[2]);
	matl.mDiffuse.w = 1.0f;

	double3 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->Specular;
	matl.mSpecular.x = static_cast<float>(double3.mData[0]);
	matl.mSpecular.y = static_cast<float>(double3.mData[1]);
	matl.mSpecular.z = static_cast<float>(double3.mData[2]);
	matl.mSpecular.w = 1.0f;

	double3 = reinterpret_cast<FbxSurfacePhong*>(inMaterial)->Reflection;
	matl.mReflect.x = static_cast<float>(double3.mData[0]);
	matl.mReflect.y = static_cast<float>(double3.mData[1]);
	matl.mReflect.z = static_cast<float>(double3.mData[2]);
	matl.mReflect.w = 1.0f;

	inObj->SetMaterial(matl);
}

void FBXLoader::ParseMaterialTexture(FbxSurfaceMaterial * inMaterial, UINT index, Subset * inObj)
{
	int textureIndex = 0;
	FbxProperty property;

	FBXSDK_FOR_EACH_TEXTURE(textureIndex)
	{
		property = inMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[textureIndex]);
		if (property.IsValid())
		{
			UINT textureCount = property.GetSrcObjectCount<FbxTexture>();
			for (UINT i = 0; i < textureCount; i++)
			{
				FbxLayeredTexture* pLayeredTexture = property.GetSrcObject<FbxLayeredTexture>(i);
				if (!pLayeredTexture)
				{
					FbxTexture* texture = property.GetSrcObject<FbxTexture>(i);
					if (texture)
					{
						string textureType = property.GetNameAsCStr();
						FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);

						if (fileTexture)
						{
							string name = fileTexture->GetFileName();
							int index = 0;
							index = name.find_last_of('/');

							if (index == -1)
								index = name.find_last_of('\\');

							if (index != -1)
								name = name.substr(index + 1, strlen(name.c_str()));

							//name = sPath + name;
							if (textureType == "DiffuseColor")
							{
								inObj->SetTextureName(0, fileTexture->GetFileName());
							}
							else if (textureType == "SpecularColor")
							{
								inObj->SetTextureName(1, fileTexture->GetFileName());
							}
							else if (textureType == "NormalMap")
							{
								inObj->SetTextureName(2, fileTexture->GetFileName());
							}
						}
					}
				}
			}
		}
	}
}

VOID FBXLoader::ReadNormal(FbxMesh * pMesh, UINT mPolygonIndex, UINT IndexNum, Vertex & pnt)
{
	FbxGeometryElementNormal* pVertexNormal = pMesh->GetElementNormal(0);
	switch (pVertexNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (pVertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pnt.mNormal.x = static_cast<float>(pVertexNormal->GetDirectArray().GetAt(mPolygonIndex).mData[0]);
			pnt.mNormal.y = static_cast<float>(pVertexNormal->GetDirectArray().GetAt(mPolygonIndex).mData[1]);
			pnt.mNormal.z = -static_cast<float>(pVertexNormal->GetDirectArray().GetAt(mPolygonIndex).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = pVertexNormal->GetIndexArray().GetAt(mPolygonIndex);
			pnt.mNormal.x = static_cast<float>(pVertexNormal->GetDirectArray().GetAt(index).mData[0]);
			pnt.mNormal.y = static_cast<float>(pVertexNormal->GetDirectArray().GetAt(index).mData[1]);
			pnt.mNormal.z = -static_cast<float>(pVertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		}
		break;
	case FbxGeometryElement::eByPolygonVertex:
		switch (pVertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pnt.mNormal.x = static_cast<float>(pVertexNormal->GetDirectArray().GetAt(IndexNum).mData[0]);
			pnt.mNormal.y = static_cast<float>(pVertexNormal->GetDirectArray().GetAt(IndexNum).mData[1]);
			pnt.mNormal.z = -static_cast<float>(pVertexNormal->GetDirectArray().GetAt(IndexNum).mData[2]);
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = pVertexNormal->GetIndexArray().GetAt(IndexNum);
			pnt.mNormal.x = static_cast<float>(pVertexNormal->GetDirectArray().GetAt(index).mData[0]);
			pnt.mNormal.y = static_cast<float>(pVertexNormal->GetDirectArray().GetAt(index).mData[1]);
			pnt.mNormal.z = -static_cast<float>(pVertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		}
		break;
	}
}

VOID FBXLoader::ReadUV(FbxMesh * pMesh, UINT mPolygonIndex, UINT UVIndex, Vertex & pnt)
{
	FbxGeometryElementUV* pVertexUV = pMesh->GetElementUV(0);

	switch (pVertexUV->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (pVertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pnt.mUV.x = static_cast<float>(pVertexUV->GetDirectArray().GetAt(mPolygonIndex).mData[0]);
			pnt.mUV.y = 1.0f - static_cast<float>(pVertexUV->GetDirectArray().GetAt(mPolygonIndex).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = pVertexUV->GetIndexArray().GetAt(mPolygonIndex);
			pnt.mUV.x = static_cast<float>(pVertexUV->GetDirectArray().GetAt(index).mData[0]);
			pnt.mUV.y = 1.0f - static_cast<float>(pVertexUV->GetDirectArray().GetAt(index).mData[1]);
		}
		break;
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (pVertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			pnt.mUV.x = static_cast<float>(pVertexUV->GetDirectArray().GetAt(UVIndex).mData[0]);
			pnt.mUV.y = 1.0f - static_cast<float>(pVertexUV->GetDirectArray().GetAt(UVIndex).mData[1]);
		}
		break;
		}
	}
}

VOID FBXLoader::ReadTangent(FbxMesh * pMesh, UINT mPolygonIndex, UINT IndexNum, Vertex & pnt)
{
	FbxGeometryElementTangent* vertexTangent = pMesh->GetElementTangent(0);
	if (!vertexTangent) return;

	switch (vertexTangent->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pnt.mTangentU.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(mPolygonIndex).mData[0]);
			pnt.mTangentU.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(mPolygonIndex).mData[1]);
			pnt.mTangentU.z = -static_cast<float>(vertexTangent->GetDirectArray().GetAt(mPolygonIndex).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexTangent->GetIndexArray().GetAt(mPolygonIndex);
			pnt.mTangentU.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			pnt.mTangentU.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			pnt.mTangentU.z = -static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pnt.mTangentU.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(IndexNum).mData[0]);
			pnt.mTangentU.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(IndexNum).mData[1]);
			pnt.mTangentU.z = -static_cast<float>(vertexTangent->GetDirectArray().GetAt(IndexNum).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexTangent->GetIndexArray().GetAt(IndexNum);
			pnt.mTangentU.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			pnt.mTangentU.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			pnt.mTangentU.z = -static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		}
	}
}

Mesh* FBXLoader::LoadFBX(const string fileName)
{
	FILE* fp;

	fopen_s(&fp, fileName.c_str(), "r");

	FbxManager* mFbxSdkManager = nullptr;
	FbxImporter* pImport;
	FbxScene *fbxScene;

	//if (!mFbxSdkManager)
	{
		mFbxSdkManager = FbxManager::Create();
		FbxIOSettings* pIOsettings = FbxIOSettings::Create(mFbxSdkManager, IOSROOT);
		mFbxSdkManager->SetIOSettings(pIOsettings);
	}

	pImport = FbxImporter::Create(mFbxSdkManager, "");
	fbxScene = FbxScene::Create(mFbxSdkManager, "");

	bool bSuccess = pImport->Initialize(fileName.c_str(), -1, mFbxSdkManager->GetIOSettings());
	if (!bSuccess)
		return nullptr;

	bSuccess = pImport->Import(fbxScene);
	if (!bSuccess)
		return nullptr;

	pImport->Destroy();

	Mesh* pMesh = new Mesh();

	printf("%s loading...\n", fileName.c_str());
	pMesh->SetBone(ParseSkeletonHierarchy(fbxScene->GetRootNode()));

	bool IsBoneAnimation;

	if (pMesh->GetBone() == nullptr)
	{
		printf("no skeleton \n");
		IsBoneAnimation = false;
	}
	else
		IsBoneAnimation = true;

	vector<Subset*>& mSubset = pMesh->GetSubset();
	Bone* bone = pMesh->GetBone();
	ParseNode(fbxScene->GetRootNode(), mSubset, bone);

	chrono::system_clock::time_point start = chrono::system_clock::now();

	BoneMatrixPath(bone);
	chrono::duration<double> sec = chrono::system_clock::now() - start;
	cout << sec.count() << " seconds" << endl;

	return pMesh;
}

void FBXLoader::ParseFindAnimation(FbxNode * pNode, FbxScene* fs, OUT Mesh* pMesh)
{
	if (pNode->GetMesh())
	{
		ParseAnimation(pNode, fs, pMesh);
	}
	int childCount = pNode->GetChildCount();
	for (int i = 0; i < childCount; i++)
	{
		ParseFindAnimation(pNode->GetChild(i), fs, pMesh);
	}
}

void FBXLoader::ParseAnimation(FbxNode * pNode, FbxScene* fs, OUT Mesh* pMesh)
{
	FbxMesh* fbxMesh = pNode->GetMesh();

	string name = pNode->GetName();
	UINT numOfDeformers = fbxMesh->GetDeformerCount();

	FbxAMatrix geometryTransform = GetGeometryTransformation(pNode);
	for (UINT deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
	{
		FbxSkin* pSkin = reinterpret_cast<FbxSkin*>(fbxMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!pSkin) continue;

		UINT numOfClusters = pSkin->GetClusterCount();

		for (UINT clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
		{
			FbxCluster* pCluster = pSkin->GetCluster(clusterIndex);
			string sJointName = pCluster->GetLink()->GetName();

			FbxAnimStack* pAnimStack = fs->GetSrcObject<FbxAnimStack>(0);
			FbxString animStackName = pAnimStack->GetName();
			string sAnimationName = animStackName.Buffer();
			FbxTakeInfo* takeInfo = fs->GetTakeInfo(animStackName);
			FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
			FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
			FbxLongLong length = end.GetFrameCount(FbxTime::eFrames24) - start.GetFrameCount(FbxTime::eFrames24) + 1;
			for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames24); i < end.GetFrameCount(FbxTime::eFrames24); ++i)
			{
				FbxTime currTime;
				currTime.SetFrame(i, FbxTime::eFrames24);
				FbxAMatrix currMat = pNode->EvaluateGlobalTransform(currTime) * geometryTransform;
				currMat = currMat.Inverse() * pCluster->GetLink()->EvaluateGlobalTransform(currTime);
				D3DXMATRIX mat;
				ConvertFbxMatrix(currMat, mat);
			}
		}
	}
}
void FBXLoader::LoadFBXAnimation(const string fileName, OUT Mesh* pMesh)
{
	FbxManager* mFbxSdkManager = nullptr;
	FbxImporter* pImport;
	FbxScene *fbxScene;

	if (!mFbxSdkManager)
	{
		mFbxSdkManager = FbxManager::Create();
		FbxIOSettings* pIOsettings = FbxIOSettings::Create(mFbxSdkManager, IOSROOT);
		mFbxSdkManager->SetIOSettings(pIOsettings);
	}

	pImport = FbxImporter::Create(mFbxSdkManager, "");
	fbxScene = FbxScene::Create(mFbxSdkManager, "");

	pImport->Initialize(fileName.c_str(), -1, mFbxSdkManager->GetIOSettings());

	printf("%s loading...\n", fileName.c_str());
	pImport->Import(fbxScene);
	ParseFindAnimation(fbxScene->GetRootNode(), fbxScene, pMesh);
}