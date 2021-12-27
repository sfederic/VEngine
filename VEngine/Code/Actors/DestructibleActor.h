#pragma once
#include "Actor.h"
#include "ActorSystem.h"

struct DestructibleMeshComponent;

struct DestructibleActor : Actor
{
	ACTOR_SYSTEM(DestructibleActor);

	DestructibleMeshComponent* destructibleMesh = nullptr;

	DestructibleActor();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
};
