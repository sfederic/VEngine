#include "vpch.h"
#include "CrashLandEnemySpawner.h"
#include "Components/BoxTriggerComponent.h"
#include "Actors/Game/CrashLandEnemy.h"
#include "Core/VMath.h"

CrashLandEnemySpawner::CrashLandEnemySpawner()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	boxTrigger->renderWireframeColour = XMFLOAT4(0.75f, 0.75f, 0.9f, 1.f);
	rootComponent = boxTrigger;
}

void CrashLandEnemySpawner::Tick(float deltaTime)
{
	spawnTimer += deltaTime;
	if (spawnTimer > spawnInterval)
	{
		spawnTimer = 0.f;
		SpawnCrashLandEnemy();
	}
}

Properties CrashLandEnemySpawner::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Spawn Interval", &spawnInterval);
	props.Add("Spawn Count", &numOfEnemiesToSpawn);
	return props;
}

void CrashLandEnemySpawner::SpawnCrashLandEnemy()
{
	if (numOfEnemiesToSpawn <= 0)
	{
		return;
	}

	Transform transform;
	XMStoreFloat3(&transform.position, boxTrigger->GetRandomPointInTrigger());
	auto crashLandEnemy = CrashLandEnemy::system.Add(transform);

	crashLandEnemy->Create();
	crashLandEnemy->CreateAllComponents();
	crashLandEnemy->SetCrashLandDirection(-VMath::GlobalUpVector());

	numOfEnemiesToSpawn--;
}
