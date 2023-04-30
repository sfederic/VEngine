#pragma once

#include "GridActor.h"

class Valve : public GridActor
{
public:
	ACTOR_SYSTEM(Valve);

	void Create() override;
};
