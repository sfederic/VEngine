#pragma once

#include "GridActor.h"

class MemoryActor : public GridActor
{
public:
	ACTOR_SYSTEM(MemoryActor);

	void Create() override;
	Properties GetProps() override;

	void Interact() override {}

private:
	std::string memoryName;
};
