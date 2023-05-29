#pragma once

#include "GridActor.h"

class Canon : public GridActor
{
public:
	ACTOR_SYSTEM(Canon);

	void Create() override;

	void Interact() override;
};
