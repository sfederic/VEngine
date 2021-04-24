#pragma once

#include "Actor.h"
#include "Input.h"

//Just for testing

class TestActor : public Actor
{
public:
	TestActor() 
	{
	}

	virtual void Tick(float deltaTime)
	{

	}
};

class TestActorSystem : public ActorSystem
{

public:
	TestActorSystem();
	virtual void Tick(float deltaTime) override;
} testActorSystem;
