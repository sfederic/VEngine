#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;

//Actor that can be picked up by the player.
struct Pickup : Actor
{
	ACTOR_SYSTEM(Pickup);

	MeshComponent* mesh = nullptr;

	Pickup();
	void AddToPlayerInventory();
	virtual Properties GetProps() override;
};
