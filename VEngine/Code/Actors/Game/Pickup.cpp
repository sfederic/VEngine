#include "vpch.h"
#include "Pickup.h"
#include "Gameplay/GameInstance.h"
#include "Core/Log.h"

void Pickup::Create()
{
	__super::Create();

	canBeMovedInLink = false;
	canBeRotatedInLink = false;
}

void Pickup::Interact()
{
	__super::Interact();

	if (itemName.empty())
	{
		Log("[%s]'s item name is empty.", GetName().c_str());
		return;
	}

	GameInstance::SetHeldPlayerItem(itemName);

	Destroy();
}
