#include "vpch.h"
#include "Crushable.h"
#include "Gameplay/GameUtils.h"

void Crushable::Crushed()
{
	GameUtils::SpawnSpriteSheet("Sprites/dust.png", GetPositionV(), false, 3, 2, 3.f);

	Destroy();
}

void Crushable::Attacked()
{
	__super::Attacked();

	Destroy();
}
