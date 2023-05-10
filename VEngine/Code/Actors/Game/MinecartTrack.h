#pragma once

#include "GridActor.h"

class MinecartTrack : public GridActor
{
public:
	ACTOR_SYSTEM(MinecartTrack);

	void Create() override;
};
