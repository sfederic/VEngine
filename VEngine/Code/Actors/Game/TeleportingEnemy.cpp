#include "vpch.h"
#include "TeleportingEnemy.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Gameplay/GameUtils.h"

TeleportingEnemy::TeleportingEnemy()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	rootComponent->AddChild(mesh);

	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	rootComponent->AddChild(boxTrigger);
}

void TeleportingEnemy::Teleport()
{
	XMVECTOR teleportPoint = GameUtils::RandomPointInTriggerNotContainedByMeshBounds(boxTrigger);
	SetPosition(teleportPoint);
}
