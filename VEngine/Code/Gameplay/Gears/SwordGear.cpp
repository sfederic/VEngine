#include "vpch.h"
#include "SwordGear.h"
#include "Physics/Raycast.h"
#include "Actors/Game/Player.h"
#include "Actors/Game/Enemy.h"
#include "Actors/MeshSliceActor.h"
#include "Gameplay/GameUtils.h"
#include "Gameplay/AttackTypes.h"
#include "Gameplay/GameplayTags.h"
#include "Components/MeshComponent.h"
#include "Components/SliceableMeshComponent.h"

SwordGear instance;

void SwordGear::Use()
{
	const auto player = Player::system.GetFirstActor();

	auto slicePlaneNormal = [&]() -> XMVECTOR { return player->GetUpVectorV(); };
	//Give plane center a small offset as it's messing up right now
	auto slicePlaneCenter = [&]() -> XMVECTOR { return player->GetPositionV() + player->GetForwardVectorV() + XMVectorSet(0.1f, 0.1f, 0.1f, 1.f); };

	HitResult hit(player);
	XMVECTOR origin = player->GetPositionV() + player->GetForwardVectorV();

	if (SimpleBoxCast(origin, XMFLOAT3(0.5f, 0.5f, 0.5f), hit, true))
	{
		GameUtils::SpawnSpriteSheet("Sprites/v_slice.png", origin, false, 4, 5);

		for (auto hitComponent : hit.hitComponents)
		{
			auto mesh = dynamic_cast<MeshComponent*>(hit.hitComponent);

			//Handle mesh slicing on attack
			auto sliceMeshComponent = dynamic_cast<SliceableMeshComponent*>(hit.hitComponent);
			if (sliceMeshComponent)
			{
				sliceMeshComponent->SliceMesh(slicePlaneCenter(), slicePlaneNormal());
			}
			else if (mesh)
			{
				if (!mesh->HasTag(GameplayTags::InvincibleMeshPiece))
				{
					mesh->Remove();
				}
			}
		}

		for (auto hitActor : hit.hitActors)
		{
			auto meshSplitActor = dynamic_cast<MeshSliceActor*>(hitActor);
			if (meshSplitActor)
			{
				meshSplitActor->SliceMesh(slicePlaneCenter(), slicePlaneNormal());
				continue;
			}

			auto enemy = dynamic_cast<Enemy*>(hitActor);
			if (enemy)
			{
				if (enemy->CanBeHit(AttackTypes::Melee))
				{
					if (enemy->CheckIfAllTaggedMeshesAreDestroyed() || enemy->HasHealthDepleted())
					{
						enemy->OnDestroyed();
					}
				}
			}
		}
	}
}
