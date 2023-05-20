#include "vpch.h"
#include "Burnable.h"
#include "Gameplay/GameUtils.h"

void Burnable::Burn()
{
	GameUtils::SpawnSpriteSheet("Sprites/fire_sheet.png", GetPositionV(), false, 4, 4);
	Destroy();
}
