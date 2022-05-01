#pragma once
#include "System.h"

struct ThreadSystem : System
{
	ThreadSystem();
	uint32_t GetNumHardwareThreads();
};

extern ThreadSystem threadSystem;
