#pragma once

#include <vector>
#include "Render/MeshDataProxy.h"
#include "Animation/Animation.h"

namespace AssetSystem
{
	void ResetMeshData();

	void BuildAllVMeshDataFromFBXImport();
	void CreateVMeshFromInWorldMesh();
	void BuildAllAnimationFilesFromFBXImport();

	void BuildSingleVMeshFromFBX(const std::string fbxFilePath, const std::string fbxFilename);
	void BuildSingleVAnimFromFBX(const std::string fbxAnimFilePath, const std::string fbxAnimFilename);

	MeshDataProxy ReadVMeshAssetFromFile(const std::string filename);
	std::vector<Animation> ReadVAnimAssetFromFile(const std::string filename);

	void BuildAllGameplayMapFiles();

	void WriteOutAllVertexColourData();
	void LoadVertexColourDataFromFile();
	void LoadVertexColourDataFromFilename(const std::string filename);
};
