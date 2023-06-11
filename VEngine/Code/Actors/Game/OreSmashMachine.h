#pragma once

#include "GridActor.h"

class BoxTriggerComponent;

//Takes an ore object and smashes it down to a Crystal.
class OreSmashMachine : public GridActor
{
public:
	ACTOR_SYSTEM(OreSmashMachine);

	OreSmashMachine();

	void Interact() override;

private:
	BoxTriggerComponent* oreToCrystalTrigger = nullptr;
};
