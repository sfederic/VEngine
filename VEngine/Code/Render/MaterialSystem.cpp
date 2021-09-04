#include "MaterialSystem.h"
#include "Material.h"
#include <cassert>

MaterialSystem materialSystem;

void MaterialSystem::AddMaterial(Material* material)
{
	assert(material);
	materials.push_back(material);
}

void MaterialSystem::CreateAllMaterials()
{
	for (Material* material : materials)
	{
		material->Create();
	}
}
