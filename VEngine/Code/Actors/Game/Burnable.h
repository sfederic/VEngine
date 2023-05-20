#pragma once

#include "GridActor.h"

class Burnable : public GridActor
{
public:
	ACTOR_SYSTEM(Burnable);

	void Burn() override;
};
