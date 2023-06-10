#pragma once

#include "GridActor.h"

class Mineable : public GridActor
{
public:
	ACTOR_SYSTEM(Mineable);

	void Mine();
};
