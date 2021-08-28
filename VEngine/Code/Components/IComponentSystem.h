#pragma once
#include "SystemStates.h"
#include <string>

struct IComponentSystem
{
	SystemStates systemState = SystemStates::Unloaded;

	std::string name;

	virtual void Tick(double deltaTime) = 0;
	virtual void Cleanup() = 0;
};
