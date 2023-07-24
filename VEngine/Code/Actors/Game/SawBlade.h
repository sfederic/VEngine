#pragma once

#include "GridActor.h"

//Spinning saw blade that is powered by a crystal
class SawBlade : public GridActor
{
public:
	ACTOR_SYSTEM(SawBlade);

	void PowerUp() override;
};
