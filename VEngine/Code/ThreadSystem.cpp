#include "ThreadSystem.h"
#include <thread>

ThreadSystem threadSystem;

//@Todo: this needs a lot more work. Make a 'good enough' thread pool for the editor side of the engine.
std::vector<std::thread> threadPool;

ThreadSystem::ThreadSystem(): System("ThreadSystem")
{
}

uint32_t ThreadSystem::GetNumHardwareThreads()
{
	return std::thread::hardware_concurrency();
}

void ThreadSystem::CreateThread(std::function<void()> function)
{
	threadPool.push_back(std::thread(function));
}
