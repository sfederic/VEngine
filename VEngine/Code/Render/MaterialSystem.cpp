#include "MaterialSystem.h"
#include "Material.h"
#include <cassert>

MaterialSystem materialSystem;

MaterialSystem::MaterialSystem() : System("MaterialSystem")
{
}

void MaterialSystem::AddMaterial(Material* material)
{
	assert(material);
	material->uid = GenerateUID();
	materials.emplace(material->uid, material);
}

Material* MaterialSystem::FindMaterial(UID uid)
{
	auto materialIt = materials.find(uid);
	if (materialIt == materials.end())
	{
		return nullptr;
	}

	return materialIt->second;
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
	for (auto& materialIt : materials)
	{
		//@Todo: there's an occasianal crash here I don't know why. Probably the range for loop.
		delete materialIt.second;
	}

	materials.clear();
}
