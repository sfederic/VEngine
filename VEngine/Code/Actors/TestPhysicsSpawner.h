#pragma once

#include "Actors/Game/GridActor.h"

class TestPhysicsSpawner : public GridActor
{
public:
	ACTOR_SYSTEM(TestPhysicsSpawner);

	TestPhysicsSpawner();
	void Tick(float deltaTime) override;
	void End() override;
	DEFAULT_PROPS;

private:
	class BoxTriggerComponent* meshSpawnTrigger = nullptr;

	bool hasBeenEmptied = false;
};
