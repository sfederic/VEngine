#pragma once

#include "GridActor.h"

class Bucket : public GridActor
{
public:
	ACTOR_SYSTEM(Bucket);

	void Create() override;

	bool isFilled = false;
};
