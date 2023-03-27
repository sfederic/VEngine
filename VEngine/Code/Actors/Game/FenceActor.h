#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;

//Level fence obstacle that player and units can't move through
struct FenceActor : Actor
{
	ACTOR_SYSTEM(FenceActor);

	MeshComponent* mesh = nullptr;

	FenceActor();
	virtual Properties GetProps() override;
};
