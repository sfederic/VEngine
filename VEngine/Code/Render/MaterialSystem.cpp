#include "MaterialSystem.h"
#include "Material.h"
#include <cassert>

MaterialSystem materialSystem;

MaterialSystem::MaterialSystem() : System("MaterialSystem")
{
	//Don't add editor instance material to the system
	currentMaterialEditorInstance = new Material();
}

void MaterialSystem::AddMaterial(Material* material)
{
	assert(material);
	material->uid = GenerateUID();
	materials.emplace(material->uid, material);

	if (systemState == SystemStates::Loaded)
	{
		material->Create();
	}
}

Material* MaterialSystem::FindMaterial(UID uid)
{
	auto materialIt = materials.find(uid);
	return materialIt->second;
}

void MaterialSystem::CreateAllMaterials()
{
	for (auto materialIt : materials)
	{
		materialIt.second->Create();
	}

	systemState = SystemStates::Loaded;
}

void MaterialSystem::Cleanup()
{
	for (auto materialIt : materials)
	{
		delete materialIt.second;
	}

	materials.clear();
}
