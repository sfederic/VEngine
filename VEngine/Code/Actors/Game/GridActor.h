#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;

//Base struct for every interactive actor that is placed in the level
struct GridActor : Actor
{
	ACTOR_SYSTEM(GridActor);

	MeshComponent* mesh = nullptr;

	//Actor's positions on the level grid
	int xIndex = -1;
	int yIndex = -1;

	int health = 0;

	bool isDestructible = false;

	GridActor();
	virtual Properties GetProps() override;
};
