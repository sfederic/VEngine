#include "vpch.h"
#include "CrystalPlacement.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameInstance.h"

void CrystalPlacement::Create()
{
	__super::Create();

	canBeRotatedInLink = false;
	canBeMovedInLink = false;
}

Properties CrystalPlacement::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Crystal Placed", &hasCrystalBeenPlaced);
	return props;
}

void CrystalPlacement::Interact()
{
	__super::Interact();

	if (hasCrystalBeenPlaced) return;

	const std::string heldItem = GameInstance::GetHeldPlayerItem();
	if (heldItem == "Crystal")
	{
		GameInstance::ClearHeldPlayerItem();
		hasCrystalBeenPlaced = true;

		crystalMesh = CreateComponent<MeshComponent>("CrystalMesh");
		crystalMesh->SetMeshFilename("crystal.vmesh");
		crystalMesh->Create();
		crystalMesh->SetLocalPosition(0.f, 1.f, 0.f);
		AddChildToRoot(crystalMesh);
	}
}
