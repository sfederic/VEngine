#include "vpch.h"
#include "EnemySpawner.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/MeshComponent.h"
#include "Actors/Game/Enemy.h"
#include "Core/Timer.h"
#include "Gameplay/GameUtils.h"

EnemySpawner::EnemySpawner()
{
	boxTrigger = CreateComponent<BoxTriggerComponent>("BoxTrigger");
	boxTrigger->renderWireframeColour = XMFLOAT4(0.75f, 0.75f, 0.9f, 1.f);
	rootComponent = boxTrigger;
}

void EnemySpawner::Tick(float deltaTime)
{
	spawnTimer += deltaTime;
	if (spawnTimer > spawnInterval)
	{
		SpawnEnemy();
	}
}

Properties EnemySpawner::GetProps()
{
	auto props = __super::GetProps();
	props.title = "EnemySpawner";
	props.Add("Spawn Count", &numOfEnemiesToSpawn);
	props.Add("Spawn Interval", &spawnInterval);
	return props;
}

void EnemySpawner::SpawnEnemy()
{
	if (numOfEnemiesToSpawn <= 0) return;
	auto enemy = Enemy::system.Add();
	XMVECTOR spawnPoint = GameUtils::RandomPointInTriggerNotContainedByMeshBounds(boxTrigger);
	enemy->SetPosition(spawnPoint);
	numOfEnemiesToSpawn--;
}
