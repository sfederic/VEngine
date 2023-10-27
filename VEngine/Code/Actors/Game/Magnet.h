#pragma once

#include "GridActor.h"

class Magnet : public GridActor
{
public:
	ACTOR_SYSTEM(Magnet);

	void Tick(float deltaTime) override;
};
