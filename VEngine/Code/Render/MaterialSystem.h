#pragma once

#include <unordered_map>
#include <memory>
#include "System.h"
#include "UID.h"
#include "Render/ShaderPair.h"

class Material;

class MaterialSystem : public System
{
private:
	std::unordered_map<UID, std::unique_ptr<Material>> materials;

public:
	std::string selectedMaterialInEditor;

	MaterialSystem() : System("MaterialSystem") {}
	Material* CreateMaterial(std::string textureFilename, ShaderPairNames shaderPair);
	void DestroyMaterial(UID materialUID);
	Material* FindMaterial(UID uid);
	Material LoadMaterialFromFile(const std::string filename);
	void CreateAllMaterials();
	void Cleanup();
};

extern MaterialSystem materialSystem;
