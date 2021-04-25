#pragma once

#include "Actor.h"

class TestSpawnActor : public Actor
{
public:

};

class TestSpawnActorSystem : public ActorSystem
{
public:
	TestSpawnActorSystem();
	virtual void Tick(float deltaTime);
	virtual void SpawnActors(int numToSpawn);
	virtual void SpawnActor(Transform transform);
} static testSpawnActorSystem;
