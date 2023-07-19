#include "vpch.h"
#include "CrystalPickup.h"
#include "Gameplay/GameInstance.h"

void CrystalPickup::Create()
{
	SetMeshFilename("crystal.vmesh");
}

void CrystalPickup::Interact()
{
	int* crystalCount = GameInstance::GetGlobalProp<int>("HeldCrystalCount");
	*crystalCount = *crystalCount + 1;
}
