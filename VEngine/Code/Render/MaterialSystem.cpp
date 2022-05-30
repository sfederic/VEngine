#include "vpch.h"
#include "MaterialSystem.h"
#include "Material.h"
#include <cassert>

MaterialSystem materialSystem;

MaterialSystem::MaterialSystem() : System("MaterialSystem")
{
}

Material* MaterialSystem::CreateMaterial()
{
	auto material = new Material();
	materials.emplace(material->GetUID(), material);
	return material;
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
