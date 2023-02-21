#include "vpch.h"
#include "WaveAttackBoss.h"
#include "Actors/Game/LevelInstance.h"
#include "Components/MeshComponent.h"
#include "Physics/Raycast.h"
#include "Core/VMath.h"

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
		constexpr float areaAttackMoveSpeed = 5.f;
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
	areaAttackMesh = MeshComponent::system.Add("AreaAttack", this, MeshComponent("plane.vmesh", "test.png"), true);
	areaAttackMesh->SetRastState(RastStates::noBackCull);
	areaAttackMesh->SetWorldPosition(GetPositionV());
	areaAttackMesh->SetWorldScale(5.f);
	areaAttackMesh->SetBlendState(BlendStates::Default);
	areaAttackMesh->SetAmbientColour(XMFLOAT4(1.f, 1.f, 1.f, 0.3f));

	const auto rot = DirectX::XMQuaternionRotationAxis(nextAreaAttackDirection, XM_PI);
	areaAttackMesh->SetWorldRotation(rot);
}

//Think of the moving areaAttackMesh more as an animation. For the actual attack hit detection,
//take the grid based points of the plane and fire raycasts from them, seeing if any hit the player.
void WaveAttackBoss::RaycastCheckOnAreaAttackDimensions()
{
	constexpr int waveAttackWidth = 3;
	constexpr int waveAttackHeight = 3;

	const auto waveAttackCenter = GetPosition();

	const auto waveAttackMinX = (int)(waveAttackCenter.x - waveAttackWidth);
	const auto waveAttackMinY = (int)(waveAttackCenter.y - waveAttackHeight);

	const auto waveAttackMaxX = (int)(waveAttackCenter.x + waveAttackWidth);
	const auto waveAttackMaxY = (int)(waveAttackCenter.x + waveAttackHeight);

	for (int w = waveAttackMinX; w < waveAttackMaxX; w++)
	{
		for (int h = waveAttackMinY; h < waveAttackMaxY; h++)
		{
			HitResult hit(this);
			const auto origin = XMVectorSet((float)w, (float)h, 0.f, 1.f);
			if (Raycast(hit, origin, -GetForwardVectorV(), 50.f))
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
	if (isInSecondPhase)
	{
		const auto levelInstance = LevelInstance::system.GetFirstActor();
		const auto randomCardinalDirection = VMath::RandomCardinalDirectionVector();
		const auto boundsFacePos = levelInstance->GetPositonOfFaceFromBounds(randomCardinalDirection);
		SetPosition(boundsFacePos);

		nextAreaAttackDirection = -randomCardinalDirection;
		const auto rot = DirectX::XMQuaternionRotationAxis(nextAreaAttackDirection, XM_PI);
		SetRotation(rot);
	}
}
