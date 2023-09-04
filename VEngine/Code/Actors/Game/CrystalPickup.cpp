#include "vpch.h"
#include "CrystalPickup.h"
#include "Gameplay/GameInstance.h"
#include "Core/Log.h"

void CrystalPickup::Create()
{
	__super::Create();

	SetMeshFilename("crystal.vmesh");

	itemIconFilename = "crystal_pickup_icon.png";
	itemName = "Crystal";
}

void CrystalPickup::Interact()
{
	GameInstance::SetHeldPlayerItem(itemName);
	__super::Interact();
}
