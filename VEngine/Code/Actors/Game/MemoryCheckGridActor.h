#pragma once

#include "GridActor.h"

//Checks for a memory and activates an effect in-game based on an interaction.
class MemoryCheckGridActor : public GridActor
{
public:
	ACTOR_SYSTEM(MemoryCheckGridActor);

	MemoryCheckGridActor() {}
	virtual Properties GetProps() override;
	bool CheckMemory();
	virtual void ExecuteOnMemoryCheckSuccess() {}

private:
	std::string memoryToCheck;
};
