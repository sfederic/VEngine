#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;

//Base struct for every interactive actor that is placed in the level
struct GridActor : Actor
{
	ACTOR_SYSTEM(GridActor);

	MeshComponent* mesh = nullptr;

	GridActor();
	virtual Properties GetProps() override;
};
