#include "vpch.h"
#include "Burnable.h"
#include "Gameplay/GameUtils.h"
#include "Player.h"

Properties Burnable::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void Burnable::Burn()
{
	GameUtils::SpawnSpriteSheet("Sprites/fire_sheet.png", GetPositionV(), false, 4, 4);
	Player::system.GetOnlyActor()->ResetCameraPosAndTargetToPlayerIfThisActor(this);
	Remove();
}
