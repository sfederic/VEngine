#include "vpch.h"
#include "CrashLandEnemy.h"
#include "Actors/Game/Player.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Physics/Raycast.h"

CrashLandEnemy::CrashLandEnemy()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	rootComponent = mesh;

	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent->AddChild(boxTrigger);
}

void CrashLandEnemy::Create()
{
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
		constexpr float moveSpeed = 5.f;
		const XMVECTOR moveOffset = crashLandDirection * moveSpeed * deltaTime;
		AddPositionV(moveOffset);
	}
}
