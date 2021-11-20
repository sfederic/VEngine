#include "Pickup.h"
#include "Components/MeshComponent.h"
#include "GameUtils.h"

Pickup::Pickup()
{
	mesh = MeshComponent::system.Add(this, MeshComponent("small_ico_sphere.fbx", "test.png"));
	rootComponent = mesh;
}

void Pickup::AddToPlayerInventory()
{
	Destroy();
}

Properties Pickup::GetProps()
{
	return Actor::GetProps();
}
