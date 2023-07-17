#pragma once

#include "GridActor.h"

class Explosive : public GridActor
{
public:
	ACTOR_SYSTEM(Explosive);

	Properties GetProps() override;

	void Burn() override;
	void Interact() override { Burn(); }//@Todo: think about deleting this. here for testing.

private:
	void HitNearbyExplodables();
};
