#include "GridActor.h"
#include "Components/MeshComponent.h"

GridActor::GridActor()
{
	mesh = MeshComponent::system.Add(this);
	rootComponent = mesh;
}

Properties GridActor::GetProps()
{
	return Actor::GetProps();
}
