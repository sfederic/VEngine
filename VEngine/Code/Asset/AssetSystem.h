#pragma once
#include "System.h"

struct AssetSystem : System
{
	AssetSystem();
	void WriteAllMeshDataToMeshAssetFiles();
	void ReadAllMeshAssetsFromFile();
};

extern AssetSystem assetSystem;
