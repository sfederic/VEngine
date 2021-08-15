#pragma once
#include "SystemStates.h"

struct IComponentSystem
{
	SystemStates systemState = SystemStates::Unloaded;
};
