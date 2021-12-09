#pragma once
#include "GridActor.h"
#include "../ActorSystem.h"

//Actor that can be picked up by the player.
struct Pickup : GridActor
{
	ACTOR_SYSTEM(Pickup);

	std::string pickupName;

	Pickup();
	void AddToPlayerInventory();
	virtual Properties GetProps() override;
};
