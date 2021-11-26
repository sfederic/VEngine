#pragma once
#include "GridActor.h"
#include "../ActorSystem.h"

struct MeshComponent;

//Static Actor that can be destroyed by characters in-world.
struct DestructibleActor : GridActor
{
	ACTOR_SYSTEM(DestructibleActor);

	//MeshComponent* mesh = nullptr;

	DestructibleActor();
	virtual Properties GetProps() override;
};
