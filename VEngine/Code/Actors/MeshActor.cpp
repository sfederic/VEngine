#include "vpch.h"
#include "MeshActor.h"
#include "Components/MeshComponent.h"
#include "Render/Material.h"

MeshActor::MeshActor()
{
	if (!spawnMeshFilename.empty())
	{
		mesh = MeshComponent::system.Add("Mesh", this, MeshComponent(spawnMeshFilename.c_str(), "test.png"));
		rootComponent = mesh;
	}
	else
	{
		mesh = MeshComponent::system.Add("Mesh", this, MeshComponent("cube.fbx", "test.png"));
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
