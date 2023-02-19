#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class BoxTriggerComponent;

//BoxTrigger to spawn enemies within it based on an interval.
class EnemySpawner : public Actor
{
public:
	ACTOR_SYSTEM(EnemySpawner);

	EnemySpawner();
	virtual void Start() override;
	virtual Properties GetProps() override;

	void SpawnEnemy();

private:
	BoxTriggerComponent* boxTrigger = nullptr;
	float spawnInterval = 3.0f;
	int numOfEnemiesToSpawn = 25;
};
