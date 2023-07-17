#pragma once

#include "GridActor.h"

class Explosive : public GridActor
{
public:
	ACTOR_SYSTEM(Explosive);

	void Burn() override;
	void Interact() override { Burn(); }//@Todo: think about deleting this. here for testing.

private:
	void HitNearbyExplodables();
};
