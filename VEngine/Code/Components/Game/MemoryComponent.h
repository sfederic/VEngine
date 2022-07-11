#pragma once
#include "../Component.h"
#include "../ComponentSystem.h"

struct Actor;

struct MemoryComponent : Component
{
	COMPONENT_SYSTEM(MemoryComponent);

	std::wstring memoryName;
	std::wstring memoryDescription;

	//The ConditionSystem function to search for when adding to player.
	std::string condition;

	//the argument to pass into the above condition function.
	std::string conditionArg;

	//Image to show related to this memory
	std::string imageFile;

	bool addOnInteract = false;
	
	MemoryComponent();
	virtual Properties GetProps() override;
	bool CreateMemory(std::string actorAquiredFromName);
};
