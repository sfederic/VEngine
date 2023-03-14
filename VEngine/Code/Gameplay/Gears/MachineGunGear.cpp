#include "vpch.h"
#include "MachineGunGear.h"
#include "Physics/Raycast.h"
#include "Actors/Game/Player.h"
#include "Gameplay/GameUtils.h"
#include "Particle/SpriteSheet.h"
#include "Core/VMath.h"

MachineGunGear instance;

void MachineGunGear::Use()
{
	auto player = Player::system.GetOnlyActor();

	HitResult hit(player);
	if (Raycast(hit, player->GetPositionV(), player->GetAimDirection(), 100.f))
	{
		auto bulletSprite = GameUtils::SpawnSpriteSheet("Sprites/gunshot.png", hit.GetHitPosV(), false, 4, 5);
		bulletSprite->sprite.angle = VMath::RandomRange(0.f, 360.f);

		GameUtils::CameraShake(0.1f);
	}
}
