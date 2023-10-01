#pragma once

#include "GridActor.h"

//Resets the current map.
class WorldResetter : public GridActor
{
public:
	ACTOR_SYSTEM(WorldResetter);

	void Create() override;
	Properties GetProps() override;

	void Interact() override;
};
