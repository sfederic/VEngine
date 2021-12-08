#include "Pickup.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameUtils.h"

Pickup::Pickup()
{

}

void Pickup::AddToPlayerInventory()
{
	Destroy();
}

Properties Pickup::GetProps()
{
	return Actor::GetProps();
}
