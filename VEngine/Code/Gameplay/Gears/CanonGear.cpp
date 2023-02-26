#include "vpch.h"
#include "CanonGear.h"
#include "Physics/Raycast.h"
#include "Actors/Game/Player.h"
#include "Actors/Game/Enemy.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/AttackTypes.h"
#include "Gameplay/GameplayTags.h"
#include "Components/MeshComponent.h"

CanonGear instance;

void CanonGear::Use()
{
	const auto player = Player::system.GetFirstActor();
	const auto damage = player->GetDamage();

	HitResult hitResult(player);
	if (Raycast(hitResult, player->GetPositionV(), player->GetForwardVectorV(), 1000.f))
	{
		GameUtils::SpawnSpriteSheet("Sprites/explosion.png", hitResult.GetHitPosV(), false, 4, 4);

		auto enemy = dynamic_cast<Enemy*>(hitResult.hitActor);
		if (enemy)
		{
			if (enemy->CanBeHit(AttackTypes::Shoot))
			{
				auto mesh = dynamic_cast<MeshComponent*>(hitResult.hitComponent);
				if (mesh)
				{
					if (!mesh->HasTag(GameplayTags::InvincibleMeshPiece))
					{
						//Only inflict damage if mesh can be hit be player attacks
						enemy->InflictDamage(damage);

						mesh->Remove();
					}
				}

				if (enemy->CheckIfAllTaggedMeshesAreDestroyed() || enemy->HasHealthDepleted())
				{
					enemy->OnDestroyed();
				}
			}
		}
	}
}
