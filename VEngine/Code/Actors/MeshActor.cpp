#include "MeshActor.h"
#include "Components/MeshComponent.h"
#include "Render/Material.h"

MeshActor::MeshActor()
{
	if (!spawnMeshFilename.empty())
	{
		mesh = MeshComponent::system.Add(this, MeshComponent(spawnMeshFilename.c_str(), "test.png"));
		rootComponent = mesh;
	}
	else
	{
		mesh = MeshComponent::system.Add(this, MeshComponent("cube.fbx", "test.png"));
		rootComponent = mesh;
	}
}

Properties MeshActor::GetProps()
{
	Properties props = Actor::GetProps();
	props.title = name;
	return props;
}
