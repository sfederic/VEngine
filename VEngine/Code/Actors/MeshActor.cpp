#include "MeshActor.h"
#include "Components/MeshComponent.h"

MeshActor::MeshActor(std::string meshFilename_)
{
	meshFilename = meshFilename_;

	mesh = MeshComponent::system.Add(this, MeshComponent(meshFilename.c_str()));
	rootComponent = mesh;
}

Properties MeshActor::GetProps()
{
	return Actor::GetProps();
}
