#pragma once

#include "GridActor.h"

//Shows invisible actors when a ray is cast from that actor, through this actor and to the camera.
class LookingGlass : public GridActor
{
public:
	ACTOR_SYSTEM(LookingGlass);

	void Create() override;
	Properties GetProps() override;
};
