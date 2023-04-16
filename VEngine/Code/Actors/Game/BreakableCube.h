#pragma once

#include "GridActor.h"

class BreakableCube : public GridActor
{
public:
	ACTOR_SYSTEM(BreakableCube);

	void Create() override;
};
