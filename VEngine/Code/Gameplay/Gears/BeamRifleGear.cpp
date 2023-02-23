#include "vpch.h"
#include "BeamRifleGear.h"
#include "Physics/Raycast.h"
#include "Actors/Game/Player.h"
#include "Actors/Game/Enemy.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/AttackTypes.h"
#include "Gameplay/GameplayTags.h"
#include "Components/MeshComponent.h"

void BeamRifleGear::Use()
{
	const auto player = Player::system.GetFirstActor();
	const auto damage = player->GetDamage();

	HitResult hitResult(player);
	if (Raycast(hitResult, player->GetPositionV(), player->GetForwardVectorV(), 1000.f))
	{
		const XMVECTOR beamEnd = player->GetPositionV() + (player->GetForwardVectorV() * 50.f);
		GameUtils::SpawnPolyboard("Particle/beam_blue.png", player->GetPositionV(), beamEnd);

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
					enemy->Destroy();
				}
			}
		}
	}
}
