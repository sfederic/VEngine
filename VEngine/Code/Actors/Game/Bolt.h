#pragma once

#include "GridActor.h"

//Metallic bolt to work with Nut grid actor.
class Bolt : public GridActor
{
public:
	ACTOR_SYSTEM(Bolt);

	void Create() override;
	Properties GetProps() override;
};
