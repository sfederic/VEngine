#include "vpch.h"
#include "MeshActor.h"
#include "Components/MeshComponent.h"
#include "Render/Material.h"

MeshActor::MeshActor()
{
	//Don't call Create() on these spawns as the actor spawn code in WorldEditor already calls it.
	if (!spawnMeshFilename.empty())
	{
		mesh = MeshComponent::system.Add("Mesh", this, MeshComponent(spawnMeshFilename.c_str(), "test.png"), false);
		rootComponent = mesh;
	}
	else
	{
		mesh = MeshComponent::system.Add("Mesh", this, MeshComponent("cube.vmesh", "test.png"), false);
		rootComponent = mesh;
	}
}

void MeshActor::Create()
{
	if (!spawnMeshFilename.empty())
	{
		std::string meshName = spawnMeshFilename.substr(0, spawnMeshFilename.find("."));
		SetName(meshName + std::to_string(GetSystemIndex()));
	}
}

Properties MeshActor::GetProps()
{
	Properties props = Actor::GetProps();
	props.title = GetName();
	return props;
}
