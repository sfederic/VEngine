#pragma once
#include "SystemStates.h"

struct IComponentSystem
{
	SystemStates systemState = SystemStates::Unloaded;

	virtual void Cleanup() = 0;
};
