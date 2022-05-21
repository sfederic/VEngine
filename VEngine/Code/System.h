#pragma once
#include "vpch.h"
#include <string>
#include "SystemStates.h"

struct System
{
	SystemStates systemState = SystemStates::Unloaded;

	std::string name;

	System(std::string name_);
};
