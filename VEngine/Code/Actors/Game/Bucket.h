#pragma once

#include "GridActor.h"

class Bucket : public GridActor
{
public:
	ACTOR_SYSTEM(Bucket);

	bool isFilled = false;

	void Create() override;
	void Tick(float deltaTime) override;
};
