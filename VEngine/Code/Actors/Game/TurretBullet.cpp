#include "vpch.h"
#include "TurretBullet.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Actors/Game/Player.h"
#include "Actors/Game/TurretEnemy.h"
#include "Physics/Raycast.h"
#include "Gameplay/GameUtils.h"

TurretBullet::TurretBullet()
{
	auto mesh = CreateComponent("Mesh", MeshComponent("small_ico_sphere.vmesh", "test.png"));
	rootComponent = mesh;

	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	boxTrigger->SetBoundsExtents(XMFLOAT3(0.2f, 0.2f, 0.2f));
	rootComponent->AddChild(boxTrigger);
}

void TurretBullet::Tick(float deltaTime)
{
	auto pos = GetPositionV();
	pos += movementDirection * moveSpeed * deltaTime;
	SetPosition(pos);

	HitResult hitResult(this);
	auto turretEnemies = TurretEnemy::system.GetActorsAsBaseClass();
	hitResult.AddActorsToIgnore(turretEnemies);
	if (SimpleBoxCast(GetPositionV(), boxTrigger->GetBoundingBox().Extents, hitResult, false))
	{
		auto player = dynamic_cast<Player*>(hitResult.hitActors.front());
		if (player)
		{
			player->InflictDamage(10.f);
		}

		GameUtils::SpawnSpriteSheet("Sprites/explosion.png", GetPositionV(), false, 4, 4);

		Destroy();
	}

	DestroyIfAliveForTooLong(deltaTime);
}

void TurretBullet::DestroyIfAliveForTooLong(float deltaTime)
{
	destroyTimer += deltaTime;
	if (destroyTimer > 5.0f)
	{
		Destroy();
	}
}
