#pragma once
#include "GridActor.h"
#include "../ActorSystem.h"

struct MeshComponent;

//Actor that can be picked up by the player.
struct Pickup : GridActor
{
	ACTOR_SYSTEM(Pickup);

	Pickup();
	void AddToPlayerInventory();
	virtual Properties GetProps() override;
};
