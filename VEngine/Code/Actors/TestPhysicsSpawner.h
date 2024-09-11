#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class TestPhysicsSpawner : public Actor
{
public:
	ACTOR_SYSTEM(TestPhysicsSpawner);

	TestPhysicsSpawner();
	void Start() override;
	DEFAULT_PROPS;

private:
	class BoxTriggerComponent* meshSpawnTrigger = nullptr;
};
