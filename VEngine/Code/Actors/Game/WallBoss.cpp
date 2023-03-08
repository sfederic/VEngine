#include "vpch.h"
#include "WallBoss.h"
#include "Components/MeshComponent.h"
#include "Core/VMath.h"
#include "Render/RastStates.h"

WallBoss::WallBoss()
{
	baseMesh = CreateComponent<MeshComponent>("BaseMesh");
	rootComponent = baseMesh;
}

void WallBoss::Create()
{
	baseMesh->SetMeshFilename("plane.vmesh");
	baseMesh->SetTexture("demons_gate.png");
}

void WallBoss::Tick(float deltaTime)
{
	MoveWallProjectile(deltaTime);

	//Make sure the spawn and destroy timers are at the tail end of one another.
	//i.e. spawnTimer = 0 when destroyTimer = 5

	wallProjectileDestroyTimer += deltaTime;
	if (wallProjectileDestroyTimer > wallTimersMax)
	{
		wallProjectileDestroyTimer = 0.f;
		RemoveAllWallProjectileParts();
	}

	wallSpawnTimer += deltaTime;
	if (wallSpawnTimer > wallTimersMax)
	{
		wallSpawnTimer = 0.f;
		SpawnWallProjectile();
	}
}

//Spawn a wall that will move over time in the forward direction of this actor, with holes
//in the 'wall' for the player to fit through to dodge.
void WallBoss::SpawnWallProjectile()
{
	const auto wallProjectileSpawnPointCenter = GetPositionV() + GetForwardVectorV();

	//Keep an eye on these values. They're being used as negative values to make the setpositions easier.
	constexpr int wallProjectileWidth = 5;
	constexpr int wallProjectileHeight = 3;

	for (int h = -wallProjectileHeight; h < wallProjectileHeight / 2; h++)
	{
		//Skip creating a mesh on a random value in the row
		//(make sure the random value isn't on the bounds/edge of the projectile).
		const int widthValueToSkipMesh = VMath::RandomRange(-wallProjectileWidth, wallProjectileWidth);

		for (int w = -wallProjectileHeight; w < wallProjectileWidth / 2; w++)
		{
			if (w == widthValueToSkipMesh)
			{
				continue;
			}

			auto wallProjectilePartMesh = MeshComponent::system.Add(
				"WallProjectileMesh", nullptr, MeshComponent("plane.vmesh", "test.png"), true);
			const auto spawnPosition = wallProjectileSpawnPointCenter + XMVectorSet((float)w, (float)h, 0.f, 1.f);
			wallProjectilePartMesh->SetWorldPosition(spawnPosition);
			wallProjectilePartMesh->SetWorldScale(0.5f);
			wallProjectilePartMesh->SetRastState(RastStates::noBackCull);

			wallProjectileParts.push_back(wallProjectilePartMesh);
		}
	}
}

void WallBoss::MoveWallProjectile(float deltaTime)
{
	for (auto part : wallProjectileParts)
	{
		constexpr float moveSpeed = 5.f;
		const auto offset = -part->GetForwardVectorV() * moveSpeed * deltaTime;
		part->AddWorldPosition(offset);
	}
}

void WallBoss::RemoveAllWallProjectileParts()
{
	for (auto part : wallProjectileParts)
	{
		part->Remove();
	}

	wallProjectileParts.clear();
}
