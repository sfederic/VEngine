#include "vpch.h"
#include "WaveAttackBoss.h"
#include "Components/MeshComponent.h"
#include "Physics/Raycast.h"

WaveAttackBoss::WaveAttackBoss()
{
	baseMesh = CreateComponent<MeshComponent>("BaseMesh");
	rootComponent = baseMesh;
}

void WaveAttackBoss::Create()
{
	baseMesh->SetMeshFilename("plane.vmesh");
}

void WaveAttackBoss::Tick(float deltaTime)
{
	shootTimer += deltaTime;
	if (shootTimer > 5.f)
	{
		shootTimer = 0.f;
		ShootAreaAttack();
		RaycastCheckOnAreaAttackDimensions();
	}

	if (areaAttackMesh)
	{
		constexpr float areaAttackMoveSpeed = 5.f;
		areaAttackMesh->AddWorldPosition(-GetForwardVectorV() * areaAttackMoveSpeed * deltaTime);

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
			if (Raycast(hit, GetPositionV(), -GetForwardVectorV(), 50.f))
			{
				InflictDamageToActor(hit.hitActor);
			}
		}
	}
}
