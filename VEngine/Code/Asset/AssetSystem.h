#pragma once

#include "Render/RenderTypes.h"

namespace AssetSystem
{
	void BuildAllVMeshDataFromFBXImport();
	void BuildAllAnimationFilesFromFBXImport();

	MeshDataProxy ReadVMeshAssetFromFile(const std::string filename);
	Animation ReadVAnimAssetFromFile(const std::string filename);

	void BuildAllGameplayMapFiles();
};
