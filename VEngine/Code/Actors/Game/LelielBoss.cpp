#include "vpch.h"
#include "LelielBoss.h"
#include "Actors/Game/LevelInstance.h"
#include "Components/MeshComponent.h"
#include "Core/VMath.h"

LelielBoss::LelielBoss()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	rootComponent = mesh;
}

void LelielBoss::Create()
{
	mesh->SetMeshFilename("sphere.vmesh");
	mesh->SetVisibility(false);
}

void LelielBoss::Start()
{
	nextPosToMoveTo = GetPositionV();
}

void LelielBoss::Tick(float deltaTime)
{
	//Grab the level bounds and move this actor towards a random point inside of it, 
	//switchting to another after it's reached it.
	if (XMVector4Equal(nextPosToMoveTo, GetPositionV()))
	{
		nextPosToMoveTo = LevelInstance::system.GetFirstActor()->GetRandomRoundedPointInsideLevelBounds();
	}

	SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextPosToMoveTo, deltaTime, moveSpeed));
}
