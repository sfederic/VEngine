#include "MeshActor.h"
#include "Components/MeshComponent.h"

MeshActor::MeshActor(std::string meshFilename_)
{
	meshFilename = meshFilename_;

	if (!spawnMeshFilename.empty())
	{
		meshFilename = spawnMeshFilename;
	}

	mesh = MeshComponent::system.Add(this, MeshComponent(meshFilename.c_str(), L"test.png"));
	rootComponent = mesh;
}

Properties MeshActor::GetProps()
{
	return Actor::GetProps();
}
