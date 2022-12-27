#pragma once

#include "InteractActor.h"
#include "../ActorSystem.h"

//And object in-world the player can interact with to repair.
class RepairObject : public InteractActor
{
public:
	ACTOR_SYSTEM(RepairObject);

	RepairObject();
	Properties GetProps() override;

	void Interact() override;
};
