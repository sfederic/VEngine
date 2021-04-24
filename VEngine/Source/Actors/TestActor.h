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
};

class TestActorSystem : public ActorSystem
{

public:
	TestActorSystem();
	virtual void Tick(float deltaTime) override;
} static testActorSystem;

