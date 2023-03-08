#include "vpch.h"
#include "TeleportingEnemy.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Gameplay/GameUtils.h"
#include "Core/Timer.h"

TeleportingEnemy::TeleportingEnemy()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	rootComponent->AddChild(mesh);

	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent->AddChild(boxTrigger);
}

void TeleportingEnemy::Tick(float deltaTime)
{
	teleportTimer += deltaTime;
	if (teleportTimer > 2.f)
	{
		Teleport();
		teleportTimer = 0.f;
	}
}

void TeleportingEnemy::Create()
{
	__super::Create();
	mesh->SetMeshFilename("cube.vmesh");
	boxTrigger->SetExtents(2.f, 2.f, 2.f);
}

void TeleportingEnemy::Teleport()
{
	auto teleportPoint = boxTrigger->GetRandomPointInTrigger();
	auto TeleportSpriteSpawn = [](XMVECTOR pos) {
		GameUtils::SpawnSpriteSheet("Sprites/blue_explosion.png", pos, false, 5, 5); 
	};
	TeleportSpriteSpawn(GetPositionV());
	TeleportSpriteSpawn(teleportPoint);
	mesh->SetWorldPosition(teleportPoint);
}
