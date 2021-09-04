#pragma once
#include <vector>

struct Material;

struct MaterialSystem
{
	std::vector<Material*> materials;

	void AddMaterial(Material* material);
	void CreateAllMaterials();
};

extern MaterialSystem materialSystem;
