#pragma once
#include "GridActor.h"

struct MeshComponent;

//Actor that is transparent in the world, giving a hint to the player to instantiate it
//by interacting with it with the appropriate memory held by the player.
struct MemoryActor : GridActor
{
	ACTOR_SYSTEM(MemoryActor);

	MemoryActor();
	virtual void Start() override;
	virtual void Tick(float deltaTime) override;
	virtual Properties GetProps() override;
};
