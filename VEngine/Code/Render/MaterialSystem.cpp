#include "vpch.h"
#include "MaterialSystem.h"
#include "Material.h"
#include <cassert>

MaterialSystem materialSystem;

Material* MaterialSystem::CreateMaterial(std::string textureFilename, std::string shaderFilename)
{
	auto uid = GenerateUID();
	materials.emplace(uid, std::make_unique<Material>(textureFilename, shaderFilename));
	auto material = materials[uid].get();
	material->SetUID(uid);
	return material;
}

void MaterialSystem::DestroyMaterial(UID materialUID)
{
	materials.erase(materialUID);
}

Material* MaterialSystem::FindMaterial(UID uid)
{
	auto materialIt = materials.find(uid);
	if (materialIt == materials.end())
	{
		return nullptr;
	}

	return materialIt->second.get();
}

void MaterialSystem::CreateAllMaterials()
{
	for (auto& materialIt : materials)
	{
		materialIt.second->Create();
	}

	systemState = SystemStates::Loaded;
}

void MaterialSystem::Cleanup()
{
	materials.clear();

	systemState = SystemStates::Unloaded;
}
