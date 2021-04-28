#pragma once

#include "Actor.h"

//Just for testing

class TestActor : public Actor
{
public:
	TestActor();
	virtual void Tick(float deltaTime);

	XMVECTOR currentPos;
	XMVECTOR nextPos;
};

class TestActorSystem : public ActorSystem
{
public:
	TestActorSystem();
	virtual void Tick(float deltaTime) override;
	virtual void SpawnActors(int numToSpawn);
	virtual void SpawnActor(Transform transform);
};

extern TestActorSystem testActorSystem;
