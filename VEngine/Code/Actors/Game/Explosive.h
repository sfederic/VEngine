#pragma once

#include "GridActor.h"

class Explosive : public GridActor
{
public:
	ACTOR_SYSTEM(Explosive);

	Properties GetProps() override;

	void Burn() override;

private:
	void HitNearbyExplodables();
};
