#pragma once

#include "GridActor.h"

//Checks for a memory and activates an effect in-game based on an interaction.
class MemoryCheckActor : public GridActor
{
public:
	ACTOR_SYSTEM(MemoryCheckActor);

	MemoryCheckActor() {}
	virtual Properties GetProps() override;
	bool CheckMemory();
	virtual void ExecuteOnMemoryCheckSuccess() {}

protected:
	std::string memoryToCheck;
};
