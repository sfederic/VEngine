#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;

class CrashLandEnemySpawner : public Actor
{
public:
	ACTOR_SYSTEM(CrashLandEnemySpawner);

	CrashLandEnemySpawner();
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;

	void SpawnCrashLandEnemy();

private:
	BoxTriggerComponent* boxTrigger = nullptr;

	float spawnTimer = 0.f;
	float spawnInterval = 3.0f;

	int numOfEnemiesToSpawn = 10;
};
