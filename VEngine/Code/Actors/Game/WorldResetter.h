#pragma once

#include "GridActor.h"

//Resets the current map but keeps the player position next to this actor.
class WorldResetter : public GridActor
{
public:
	ACTOR_SYSTEM(WorldResetter);

	void Create() override;
	void Interact() override;
};
