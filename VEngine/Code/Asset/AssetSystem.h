#pragma once

#include "Render/RenderTypes.h"

namespace AssetSystem
{
	void BuildAllVMeshDataFromFBXImport();
	void BuildAllVSkeletalMeshesFromFBXImport();
	MeshDataProxy ReadVMeshAssetFromFile(const std::string filename);
	void BuildAllGameplayMapFiles();
};
