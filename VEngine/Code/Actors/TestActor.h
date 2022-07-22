#pragma once

#include "Actor.h"
#include "ActorSystem.h"

struct MeshComponent;

struct TestActor : Actor
{
	ACTOR_SYSTEM(TestActor);

	MeshComponent* mesh1 = nullptr;
	MeshComponent* mesh2 = nullptr;

	TestActor();
	virtual void Start() override;
	virtual Properties GetProps() override;
};
