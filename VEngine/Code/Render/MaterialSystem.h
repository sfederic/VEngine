#pragma once
#include <unordered_map>
#include <UID.h>
#include "System.h"

struct Material;

struct MaterialSystem : System
{
	std::unordered_map<UID, Material*> materials;

	MaterialSystem();
	void AddMaterial(Material* material);
	Material* FindMaterial(UID uid);
	void CreateAllMaterials();
	void Cleanup();
};

extern MaterialSystem materialSystem;
