#include "vpch.h"
#include "CrystalPlacement.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameInstance.h"

Properties CrystalPlacement::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Crystal Placed", &hasCrystalBeenPlaced);
	return props;
}

void CrystalPlacement::Interact()
{
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
		rootComponent->AddChild(crystalMesh);
	}
}
