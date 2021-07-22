#pragma once

#include "Actor.h"

//Just for testing

class TestActor : public Actor
{
public:
	TestActor();
	virtual Properties GetProps() override;
	virtual void Tick(float deltaTime);

	XMVECTOR currentPos;
	XMVECTOR nextPos;

	XMVECTOR currentRot;
	XMVECTOR nextRot;

	int testint;
	float testfloat;
};

class TestActorSystem : public ActorSystem
{
public:
	TestActorSystem();
	virtual void Tick(float deltaTime) override;
	virtual void SpawnActors(int numToSpawn);
	virtual Actor* SpawnActor(Transform transform);
};

extern TestActorSystem testActorSystem;
