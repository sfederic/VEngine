#pragma once
#include "SystemStates.h"

struct IComponentSystem
{
	SystemStates systemState = SystemStates::Unloaded;

	virtual void Tick(double deltaTime) = 0;
	virtual void Cleanup() = 0;
};
