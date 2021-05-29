#pragma once

#include "Actor.h"

class TestActor2 : public Actor
{
public:
	virtual void Tick(float deltaTime);
};

class TestActor2System : ActorSystem
{
public:
	TestActor2System();
	virtual void Tick(float deltaTime) override;
	virtual void SpawnActors(int numToSpawn) override;
	virtual Actor* SpawnActor(Transform transform) override;
};

extern TestActor2System testActor2System;
