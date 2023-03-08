#pragma once

#include "Render/MeshDataProxy.h"
#include "Animation/Animation.h"

namespace AssetSystem
{
	void BuildAllVMeshDataFromFBXImport();
	void BuildAllAnimationFilesFromFBXImport();

	void BuildSingleVMeshFromFBX(const std::string filename);
	void BuildSingleVAnimFromFBX(const std::string filename);

	MeshDataProxy ReadVMeshAssetFromFile(const std::string filename);
	Animation ReadVAnimAssetFromFile(const std::string filename);

	void BuildAllGameplayMapFiles();
};
