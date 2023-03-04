#include "vpch.h"
#include "CrashLandEnemy.h"
#include "Actors/Game/Player.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Physics/Raycast.h"
#include "Particle/ParticleEmitter.h"
#include "Gameplay/GameUtils.h"

CrashLandEnemy::CrashLandEnemy()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	rootComponent = mesh;

	smokeTrailEmitter = CreateComponent("SmokeTrail", ParticleEmitter("Particle/smoke.png"));
	rootComponent->AddChild(smokeTrailEmitter);

	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent->AddChild(boxTrigger);
}

void CrashLandEnemy::Create()
{
	__super::Create();

	mesh->SetMeshFilename("ico_sphere.vmesh");
}

void CrashLandEnemy::Start()
{
	crashLandDirection = GetForwardVectorV();
}

void CrashLandEnemy::Tick(float deltaTime)
{
	HitResult hitResult(this);
	if (!boxTrigger->QuickInPlaceBoxCast(hitResult, false))
	{
		constexpr float moveSpeed = 15.f;
		const XMVECTOR moveOffset = crashLandDirection * moveSpeed * deltaTime;
		AddPositionV(moveOffset);
	}
	else
	{
		smokeTrailEmitter->SetVisibility(false);
		smokeTrailEmitter->SetActive(false);
	
		GameUtils::CameraShake(0.5f);

		SetTickEnabled(false);
	}
}
