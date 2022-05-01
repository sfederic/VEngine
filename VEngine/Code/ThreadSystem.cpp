#include "ThreadSystem.h"
#include <thread>

ThreadSystem threadSystem;

ThreadSystem::ThreadSystem(): System("ThreadSystem")
{
}

uint32_t ThreadSystem::GetNumHardwareThreads()
{
	return std::thread::hardware_concurrency();
}
