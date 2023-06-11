#pragma once

#include "GridActor.h"

class Ore : public GridActor
{
public:
	ACTOR_SYSTEM(Ore);

	void Create() override;
};
