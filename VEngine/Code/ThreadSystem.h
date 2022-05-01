#pragma once
#include "System.h"
#include <functional>

struct ThreadSystem : System
{
	ThreadSystem();
	static uint32_t GetNumHardwareThreads();
	static void CreateThread(std::function<void()> function);
};

extern ThreadSystem threadSystem;
