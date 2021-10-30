#pragma once
#include "System.h"

struct AssetSystem : System
{
	void WriteAllMeshDataToMeshAssetFiles();
};

extern AssetSystem assetSystem;
