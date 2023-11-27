#include "vpch.h"
#include "Burnable.h"
#include "Gameplay/GameUtils.h"

Properties Burnable::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}

void Burnable::Burn()
{
	GameUtils::SpawnSpriteSheet("Sprites/fire_sheet.png", GetPositionV(), false, 4, 4);
	Remove();
}
