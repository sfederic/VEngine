#include "vpch.h"
#include "WallBoss.h"
#include "Components/MeshComponent.h"
#include "Core/VMath.h"

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

void WallBoss::Start()
{
	SpawnWallProjectile();
}

void WallBoss::Tick(float deltaTime)
{
	MoveWallProjectile(deltaTime);

	/*wallSpawnTimer += deltaTime;
	if (wallSpawnTimer > 1.0f)
	{
		wallSpawnTimer = 0.f;
		SpawnWallProjectile();
	}*/
}

//Spawn a wall that will move over time in the forward direction of this actor, with holes
//in the 'wall' for the player to fit through to dodge.
void WallBoss::SpawnWallProjectile()
{
	const auto wallProjectileSpawnPointCenter = GetPositionV() + GetForwardVectorV();

	constexpr int wallProjectileWidth = 4;
	constexpr int wallProjectileHeight = 2;

	for (int h = -wallProjectileHeight; h < wallProjectileHeight / 2; h++)
	{
		//Skip creating a mesh on a random value in the row
		//(make sure the random value isn't on the bounds/edge of the projectile).
		const int widthValueToSkipMesh = VMath::RandomRange(0, wallProjectileWidth);

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
		const auto offset = -part->GetForwardVectorV() * deltaTime;
		part->AddWorldPosition(offset);
	}
}
