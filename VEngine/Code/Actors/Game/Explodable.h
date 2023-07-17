#pragma once

#include "GridActor.h"

class Explodable : public GridActor
{
public:
	ACTOR_SYSTEM(Explodable);

	Properties GetProps() override;

	void HitByExplosive();
};
