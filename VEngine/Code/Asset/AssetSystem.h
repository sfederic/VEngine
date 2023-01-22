#pragma once

#include "Render/RenderTypes.h"

namespace AssetSystem
{
	void WriteAllMeshDataToMeshAssetFiles();
	MeshDataProxy ReadVMeshAssetFromFile(const std::string filename);
	void BuildAllGameplayMapFiles();
};
