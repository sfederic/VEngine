#pragma once

#include "GridActor.h"

class LockedDoor : public GridActor
{
public:
	ACTOR_SYSTEM(LockedDoor);

	void Activate() override;
};

