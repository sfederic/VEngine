
#include "PlayerUpgrade.h"
#include "Gameplay/GameInstance.h"
#include "Gameplay/GameUtils.h"
#include "Core/Log.h"

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

	GameUtils::PlayAudioOneShot("upgrade_gained.wav");

	RecalcCurrentNodePosition();

	DeferDestroy();
}
