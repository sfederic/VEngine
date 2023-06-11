#pragma once

#include "GridActor.h"

class Explosive : public GridActor
{
public:
	ACTOR_SYSTEM(Explosive);

	void Burn() override;

private:
	void HitNearbyExplodables();
};
