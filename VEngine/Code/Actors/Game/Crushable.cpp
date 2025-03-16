
#include "Crushable.h"
#include "Gameplay/GameUtils.h"

void Crushable::Crushed()
{
	GameUtils::SpawnSpriteSheet("Sprites/dust.png", GetPositionV(), false, 3, 2, 3.f);

	Remove();
}

void Crushable::Attacked()
{
	__super::Attacked();

	Remove();
}
