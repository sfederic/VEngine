#include "ThreadSystem.h"
#include <thread>

ThreadSystem threadSystem;

ThreadSystem::ThreadSystem(): System("ThreadSystem")
{
}

uint32_t ThreadSystem::GetNunHardwareThreads()
{
	return std::thread::hardware_concurrency();
}
