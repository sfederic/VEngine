#include "vpch.h"
#include "Pickup.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/GameUtils.h"
#include "Core/Log.h"
#include "Actors/Game/Player.h"

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

	if (!pickupAudioFilename.empty())
	{
		GameUtils::PlayAudioOneShot(pickupAudioFilename);
	}

	auto player = Player::system.GetOnlyActor();
	player->SetPickupWidgetText(VString::stows(itemName));
	player->AddPickupWidgetToViewport();

	GameInstance::SetHeldPlayerItem(itemName);
	RecalcCurrentNodePosition();
	Destroy();
}

Properties Pickup::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Item Name", &itemName);
	props.Add("Pickup Audio", &pickupAudioFilename);
	return props;
}
