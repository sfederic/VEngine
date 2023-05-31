#include "vpch.h"
#include "Canon.h"
#include "Components/MeshComponent.h"
#include "Physics/Raycast.h"
#include "Gameplay/GameUtils.h"

void Canon::Create()
{
	canBeRotatedPitchXAxis = false;
	ignoreRotationValidCheck = true;

	mesh->SetMeshFilename("turret.vmesh");
}

void Canon::Interact()
{
	HitResult hit(this);
	if (Raycast(hit, GetPositionV(), GetForwardVectorV(), 100.f))
	{
		auto gridActor = hit.GetHitActorAs<GridActor>();
		if (gridActor)
		{
			gridActor->Attacked();

			GameUtils::SpawnParticleEmitter("Particle/smoke.png", GetPositionV(), 1.5f);
		}
	}
}