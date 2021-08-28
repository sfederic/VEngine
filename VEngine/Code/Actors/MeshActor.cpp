#include "MeshActor.h"
#include "Components/MeshComponent.h"

MeshActor::MeshActor()
{
	mesh = MeshComponent::system.Add(this, MeshComponent(meshFilename.c_str()));
}
