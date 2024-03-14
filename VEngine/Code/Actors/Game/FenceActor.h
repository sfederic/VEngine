#pragma once

#include "GridActor.h"

//Level fence obstacle that player and units can't move through
class FenceActor : public GridActor
{
public:
	ACTOR_SYSTEM(FenceActor);

	FenceActor();
	void Create() override;
	Properties GetProps() override;
};
