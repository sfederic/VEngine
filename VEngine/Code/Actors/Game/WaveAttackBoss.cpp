#include "vpch.h"
#include "WaveAttackBoss.h"
#include "Components/MeshComponent.h"
#include "Actors/Game/LevelInstance.h"
#include "Physics/Raycast.h"
#include "Core/VMath.h"
#include "Render/RastStates.h"
#include "Render/BlendStates.h"

WaveAttackBoss::WaveAttackBoss()
{
	baseMesh = CreateComponent<MeshComponent>("BaseMesh");
	rootComponent = baseMesh;
}

void WaveAttackBoss::Create()
{
	baseMesh->SetMeshFilename("plane.vmesh");
	baseMesh->SetRastState(RastStates::noBackCull);
}

void WaveAttackBoss::Tick(float deltaTime)
{
	shootTimer += deltaTime;
	if (shootTimer > 4.f)
	{
		shootTimer = 0.f;
		ShootAreaAttack();
		RaycastCheckOnAreaAttackDimensions();
	}

	warpTimer += deltaTime;
	if (warpTimer > 7.f)
	{
		warpTimer = 0.f;
		shootTimer = 0.f; //Make sure there's a nice pause between warping and shooting.
		SecondPhaseWarpAroundLevel();
	}

	if (areaAttackMesh)
	{
		constexpr float areaAttackMoveSpeed = 15.f;
		areaAttackMesh->AddWorldPosition(nextAreaAttackDirection * areaAttackMoveSpeed * deltaTime);

		areaAttackMeshLifetimeTimer += deltaTime;
		if (areaAttackMeshLifetimeTimer > 3.f)
		{
			areaAttackMeshLifetimeTimer = 0.f;
			areaAttackMesh->Remove();
			areaAttackMesh = nullptr;
		}
	}
}

void WaveAttackBoss::ShootAreaAttack()
{
	areaAttackMesh = MeshComponent::system.Add("AreaAttack", nullptr, MeshComponent("plane.vmesh", "test.png"), true);
	areaAttackMesh->SetRastState(RastStates::noBackCull);
	areaAttackMesh->SetWorldPosition(GetPositionV());
	areaAttackMesh->SetWorldScale(5.f);
	areaAttackMesh->SetBlendState(BlendStates::Default);
	areaAttackMesh->SetAmbientColour(XMFLOAT4(1.f, 1.f, 1.f, 0.3f));
	areaAttackMesh->SetCollisionLayer(CollisionLayers::None);

	auto levelInstance = LevelInstance::system.GetFirstActor();
	const auto rot = VMath::LookAtRotation(GetPositionV() + nextAreaAttackDirection, areaAttackMesh->GetWorldPositionV());
	areaAttackMesh->SetWorldRotation(rot);
}

//Think of the moving areaAttackMesh more as an animation. For the actual attack hit detection,
//take the grid based points of the plane and fire raycasts from them, seeing if any hit the player.
void WaveAttackBoss::RaycastCheckOnAreaAttackDimensions()
{
	const auto center = GetPositionV();

	for (int w = -5; w < 5; w++)
	{
		for (int h = -5; h < 5; h++)
		{
			HitResult hit(this);
			auto origin = center + (GetRightVectorV() * w);
			origin += GetUpVectorV() * h;
			if (Raycast(hit, origin, GetForwardVectorV(), 50.f))
			{
				InflictDamageToActor(hit.hitActor);
			}
		}
	}
}

//Having the boss attack in one direction initially is a good test of player skills.
//For second phase, have the boss move to the faces of the level bounds and attack in the direction of
//its positon to the center of the map, that way the player will have to orient from all directions to dodge.
void WaveAttackBoss::SecondPhaseWarpAroundLevel()
{
	//@Todo: come back here and redo levelinstance code. Player was changed to use CharacterController,
	//see if it can work with BoundingBox around level.
	/*if (isInSecondPhase)
	{
		const auto levelInstance = LevelInstance::system.GetFirstActor();
		const auto randomCardinalDirection = VMath::RandomCardinalDirectionVector();
		const auto boundsFacePos = levelInstance->GetPositonOfFaceFromBounds(randomCardinalDirection);
		SetPosition(boundsFacePos);

		nextAreaAttackDirection = -randomCardinalDirection;
		const auto rot = VMath::LookAtRotation(GetPositionV() - randomCardinalDirection, GetPositionV());
		SetRotation(rot);
	}*/
}
