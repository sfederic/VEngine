#pragma once

#include "GridActor.h"

class Explosive : public GridActor
{
public:
	ACTOR_SYSTEM(Explosive);

	void Interact() override;
};
