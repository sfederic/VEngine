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
	auto props = __super::GetProps();
	props.AddProp(pickupName);
	return props;
}
