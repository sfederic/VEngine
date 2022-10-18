#pragma once
#include "../Component.h"
#include "../ComponentSystem.h"

class Actor;

struct MemoryComponent : Component
{
	COMPONENT_SYSTEM(MemoryComponent);

	std::wstring memoryName;

	bool addOnInteract = false;

	MemoryComponent();
	virtual Properties GetProps() override;
	bool CreateMemory(std::string actorAquiredFromName);
};
