#pragma once

#include "GridActor.h"

class Burnable : public GridActor
{
public:
	ACTOR_SYSTEM(Burnable);

	Properties GetProps() override;
	void Burn() override;
};
