#include "vpch.h"
#include "PlayerUpgrade.h"
#include "Components/MeshComponent.h"
#include "Gameplay/GameInstance.h"
#include "Core/Log.h"

PlayerUpgrade::PlayerUpgrade()
{
	_upgradeMesh = CreateComponent<MeshComponent>("Mesh");
	_upgradeMesh->SetMeshFilename("cube.vmesh");
	SetRootComponent(_upgradeMesh);
}

void PlayerUpgrade::Create()
{
	__super::Create();

	canBeMovedInLink = false;
	canBeRotatedInLink = false;
}

Properties PlayerUpgrade::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Upgrade Name", &_upgradeGlobalMappingName);
	return props;
}

void PlayerUpgrade::Interact()
{
	__super::Interact();

	bool* upgradeProperty = GameInstance::GetGlobalProp<bool>(_upgradeGlobalMappingName);
	if (upgradeProperty)
	{
		*upgradeProperty = true;
		Log("[%s] added as player upgrade.", _upgradeGlobalMappingName.c_str());
	}
}
