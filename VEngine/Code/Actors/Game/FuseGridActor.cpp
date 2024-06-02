#include "vpch.h"
#include "FuseGridActor.h"
#include "Components/MeshComponent.h"
#include "Core/VMath.h"
#include "Gameplay/GameUtils.h"
#include "Particle/SpriteSheet.h"
#include "Physics/Raycast.h"
#include "Grid.h"

void FuseGridActor::OnLinkMove()
{
	__super::OnLinkMove();

	HitResult hit(this);
	if (Physics::Raycast(hit, GetPositionV(), nextMoveCardinalDirection, 1.f))
	{
		auto hitFusedActor = hit.GetHitActorAs<FuseGridActor>();
		if (hitFusedActor)
		{
			//Because the fused grid actor is now larger, set it as a big grid actor so that full grid
			//recalcs are happening on move/rotate.
			bigGridActor = true;

			GameUtils::PlayAudioOneShot("equip.wav");

			auto& fuseSprite = GameUtils::SpawnSpriteSheet("Sprites/blue_explosion.png", hit.GetHitPosV(), false, 5, 5, 25.f);
			fuseSprite.SetUseOwnRotation(true);
			const auto fuseSpriteRotation = VMath::LookAtRotation(hit.GetHitPosV(), GetPositionV());
			fuseSprite.SetWorldRotation(fuseSpriteRotation);
			fuseSprite.SetWorldScale(1.5f);

			for (auto hitFusedMesh : hitFusedActor->GetComponents<MeshComponent>())
			{
				auto newMesh = CreateComponent<MeshComponent>(std::to_string((unsigned int)GenerateUID()));
				newMesh->SetMeshFilename(hitFusedMesh->GetMeshFilename());

				GameUtils::SetLinkedMeshEffect(newMesh);

				newMesh->Create();

				rootComponent->AddChild(newMesh);
				newMesh->SetWorldPosition(hitFusedMesh->GetWorldPositionV());
			}

			hitFusedActor->Remove();

			Grid::system.GetOnlyActor()->RecalcNodesToIgnoreLinkedGridActor(this);

			//Reset this actor's next position so that moving into the fuse actor creates a LEGO-like 'click'.
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
