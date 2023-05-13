#include "vpch.h"
#include "Explosive.h"
#include "Gameplay/GameUtils.h"

void Explosive::Create()
{
	interactText = L"Ignite";
}

void Explosive::Interact()
{
	GameUtils::SpawnSpriteSheet("Sprites/blue_explosion.png", GetPositionV(), false, 5, 5);
	Destroy();
}
