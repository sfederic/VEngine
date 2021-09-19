#include "MaterialSystem.h"
#include "Material.h"
#include <cassert>

MaterialSystem materialSystem;

MaterialSystem::MaterialSystem()
{
	//Don't add editor instance material to the system
	currentMaterialEditorInstance = new Material();
}

void MaterialSystem::AddMaterial(Material* material)
{
	assert(material);
	materials.push_back(material);

	if (systemState == SystemStates::Loaded)
	{
		material->Create();
	}
}

void MaterialSystem::CreateAllMaterials()
{
	for (Material* material : materials)
	{
		material->Create();
	}

	systemState = SystemStates::Loaded;
}

void MaterialSystem::Cleanup()
{
	for (Material* material : materials)
	{
		delete material;
	}

	materials.clear();
}
