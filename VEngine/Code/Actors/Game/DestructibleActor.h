#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;

//Static Actor that can be destroyed by characters in-world.
struct DestructibleActor : Actor
{
	ACTOR_SYSTEM(DestructibleActor);

	MeshComponent* mesh = nullptr;

	DestructibleActor();
	virtual Properties GetProps() override;
};
