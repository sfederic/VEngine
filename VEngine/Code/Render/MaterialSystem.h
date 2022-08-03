#pragma once

#include <unordered_map>
#include <memory>
#include "System.h"
#include "UID.h"

class Material;

class MaterialSystem : public System
{
private:
	std::unordered_map<UID, std::unique_ptr<Material>> materials;

public:
	MaterialSystem() : System("MaterialSystem") {}
	Material* CreateMaterial(std::string textureFilename, std::string shaderFilename);
	void DestroyMaterial(UID materialUID);
	Material* FindMaterial(UID uid);
	void CreateAllMaterials();
	void Cleanup();
};

extern MaterialSystem materialSystem;
