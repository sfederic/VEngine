#include "vpch.h"
#include "FuseGridActor.h"
#include "Components/MeshComponent.h"
#include "Core/VMath.h"
#include "Gameplay/GameUtils.h"
#include "Particle/SpriteSheet.h"
#include "Physics/Raycast.h"

void FuseGridActor::OnLinkMove()
{
	__super::OnLinkMove();

	HitResult hit(this);
	if (Raycast(hit, GetPositionV(), nextMoveCardinalDirection, 1.f))
	{
		auto hitFusedActor = hit.GetHitActorAs<FuseGridActor>();
		if (hitFusedActor)
		{
			auto& fuseSprite = GameUtils::SpawnSpriteSheet("Sprites/blue_explosion.png", hit.GetHitPosV(), false, 5, 5, 25.f);
			fuseSprite.SetUseOwnRotation(true);
			const auto fuseSpriteRotation = VMath::LookAtRotation(hit.GetHitPosV(), GetPositionV());
			fuseSprite.SetWorldRotation(fuseSpriteRotation);

			for (auto hitFusedMesh : hitFusedActor->GetComponents<MeshComponent>())
			{
				auto newMesh = CreateComponent<MeshComponent>(std::to_string((unsigned int)GenerateUID()));
				newMesh->SetMeshFilename(hitFusedMesh->GetMeshFilename());
				newMesh->Create();

				rootComponent->AddChild(newMesh);
				newMesh->SetWorldPosition(hitFusedMesh->GetWorldPositionV());
			}

			hitFusedActor->Remove();

			SetNextPos(GetPositionV());
		}
	}
}

Properties FuseGridActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
