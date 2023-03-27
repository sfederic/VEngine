#pragma once

#include "GridActor.h"

//GridActor that detonates when attacked by unit or player.
class ExplodingObject : public GridActor
{
public:
	ACTOR_SYSTEM(ExplodingObject);

	ExplodingObject() {}
	virtual void Attacked() override;
};
