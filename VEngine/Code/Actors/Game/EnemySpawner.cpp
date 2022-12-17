#include "vpch.h"
#include "EnemySpawner.h"
#include "Components/BoxTriggerComponent.h"
#include "Actors/Game/Enemy.h"
#include "Timer.h"

EnemySpawner::EnemySpawner()
{
	boxTrigger = CreateComponent(BoxTriggerComponent(), "BoxTrigger");
	rootComponent = boxTrigger;
}

void EnemySpawner::Start()
{
	Timer::SetTimer(spawnInterval, std::bind(&EnemySpawner::SpawnEnemy, this), true);
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
	Enemy::system.Add();
	numOfEnemiesToSpawn--;
}
