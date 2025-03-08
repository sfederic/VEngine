#pragma once

#define FBXSDK_SHARED //Needs to be defined for static linking
#include <fbxsdk.h>
#include <string>
#include <map>
#include <vector>
#include "Animation/Animation.h"
#include "Animation/BoneWeights.h"
#include "Render/Vertex.h"

//The oficial docs on the FBX SDK (the current version is hard to find, google brings you to 2014 docs)
//Ref: https://help.autodesk.com/view/FBX/2020/ENU/

//Great GameDev.net reference on using FBX SDK and animation.
//Ref: https://www.gamedev.net/tutorials/_/technical/graphics-programming-and-theory/how-to-work-with-fbx-sdk-r3582/

class Skeleton;
struct MeshData;

class FBXLoader
{
public:
	static FBXLoader Get()
	{
		static FBXLoader instance;
		return instance;
	}

	void Init();

	//For importing generic fbx assets
	void ImportAsMesh(std::string filepath, MeshData& meshData);

	std::map<std::string, Animation> ImportAsAnimation(const std::string filepath, const std::string filename);

	//For importing cell fractured meshes
	void ImportFracturedMesh(std::string filepath, std::vector<MeshData>& meshDatas);

private:
	void ProcessAllChildNodes(std::string fbxFilename, FbxNode* node, MeshData& meshData);
	void ProcessSkeletonNodes(FbxNode* node, Skeleton& skeleton, int parentIndex);

	void ReadNormal(fbxsdk::FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, DirectX::XMFLOAT3& outNormal);
	void ReadUVs(fbxsdk::FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, DirectX::XMFLOAT2& outUVs);
	void ReadVertexColours(fbxsdk::FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, DirectX::XMFLOAT4& outColour);

	DirectX::XMFLOAT4 FBXDouble3ToXMFloat3(const fbxsdk::FbxDouble3& fbxDouble);

	std::vector<DirectX::XMFLOAT3> ProcessControlPoints(fbxsdk::FbxMesh* currMesh);
	void ProcessTriangle(fbxsdk::FbxMesh* mesh, int triangleIndex, int& polyIndexCounter, MeshData& meshData, const std::unordered_map<int, BoneWeights>& boneWeightsMap);
	void ProcessMaterials(fbxsdk::FbxNode* node, const std::string& fbxFilename);
	void ProcessMaterial(fbxsdk::FbxSurfacePhong* fbxMaterial, const std::string& fbxFilename);
	void SetTangents(Vertex* verts[3]);
	void FlipVertexFaceOrder(MeshData& meshData);
	void ProcessChildNodes(std::string fbxFilename, fbxsdk::FbxNode* node, MeshData& meshData);
	void ProcessVertexWeights(fbxsdk::FbxCluster* cluster, int i, int currentJointIndex, std::unordered_map<int, BoneWeights>& boneWeightsMap);
	void SetInverseBindPose(MeshData& meshData, int currentJointIndex, const fbxsdk::FbxAMatrix& linkMatrix, const FbxAMatrix& clusterMatrix);
	void ProcessMesh(fbxsdk::FbxNode* node, MeshData& meshData, const std::string& fbxFilename);
	void ProcessBoneWeights(fbxsdk::FbxMesh* mesh, MeshData& meshData, std::unordered_map<int, BoneWeights>& boneWeightsMap);
	void ProcessCluster(fbxsdk::FbxCluster* cluster, fbxsdk::FbxMesh* mesh, MeshData& meshData, std::unordered_map<int, BoneWeights>& boneWeightsMap);
	void ProcessVertices(fbxsdk::FbxMesh* mesh, MeshData& meshData, const std::unordered_map<int, BoneWeights>& boneWeightsMap);

	fbxsdk::FbxManager* manager = nullptr;
	fbxsdk::FbxIOSettings* ioSetting = nullptr;
	fbxsdk::FbxImporter* importer = nullptr;
};
