#pragma once

#include "GridActorOffsetMesh.h"

class ClockHand : public GridActorOffsetMesh
{
public:
	ACTOR_SYSTEM(ClockHand);

	Properties GetProps() override;
};
