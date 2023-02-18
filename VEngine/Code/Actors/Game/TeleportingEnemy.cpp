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

void TeleportingEnemy::Start()
{
	Timer::SetTimer(4.f, std::bind(&TeleportingEnemy::Teleport, this), true);
}

void TeleportingEnemy::Create()
{
	mesh->SetMeshFilename("cube.vmesh");

	boxTrigger->SetExtents(2.f, 2.f, 2.f);
}

void TeleportingEnemy::Teleport()
{
	XMVECTOR teleportPoint = GameUtils::RandomPointInTriggerNotContainedByMeshBounds(boxTrigger);
	GameUtils::SpawnSpriteSheet("Sprites/blue_explosion.png", GetPositionV(), false, 5, 5);
	SetPosition(teleportPoint);
}
