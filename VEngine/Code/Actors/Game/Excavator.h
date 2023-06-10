#pragma once

#include "GridActor.h"

class Excavator : public GridActor
{
public:
	ACTOR_SYSTEM(Excavator);

	void Create() override;
	void Interact() override;

private:
	void Dig();
};
